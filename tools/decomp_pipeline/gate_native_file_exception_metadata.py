"""Compile-only check of disk exception catch metadata against the original PE."""
from pathlib import Path
from datetime import datetime, timezone
import hashlib
import json
import re
import struct

import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data

R = Path(__file__).resolve().parents[2]
TYPE = '.?AVCBBBFileException@@'
BASE = '.?AVCExceptionBase@@'


def original_graph(raw):
    read = lambda address, count: struct.unpack('<' + 'I' * count, read_virtual_data(raw, address, count * 4))
    attributes, unwind, forward, array = read(0x1369380, 4)
    assert (attributes, unwind, forward) == (0, 0, 0)
    count, = read(array, 1)
    assert count == 2
    result = []
    for pointer in read(array + 4, count):
        properties, descriptor, member, vbtable, vdisp, size, copy = read(pointer, 7)
        name = read_virtual_data(raw, descriptor + 8, 80).split(b'\0')[0].decode()
        result.append((name, properties, member, vbtable, vdisp, size, copy))
    return result


def compiled_graph(obj):
    c = Coff(obj)
    def section(symbol):
        index, offset = c.symbols[symbol]
        assert offset == 0
        return c.sections[index]
    info, refs = section('__TI2?AVCBBBFileException@@')
    assert info == bytes(16) and refs == {12: ('__CTA2?AVCBBBFileException@@', 6)}
    array, refs = section('__CTA2?AVCBBBFileException@@')
    assert array == struct.pack('<3I', 2, 0, 0) and set(refs) == {4, 8}
    result = []
    for offset in (4, 8):
        symbol, kind = refs[offset]
        assert kind == 6
        data, targets = section(symbol)
        values = struct.unpack('<7I', data)
        assert set(targets) == {4} and targets[4][1] == 6
        descriptor, descriptor_refs = section(targets[4][0])
        assert descriptor_refs == {0: ('??_7type_info@@6B@', 6)}
        name = descriptor[8:].split(b'\0')[0].decode()
        result.append((name, values[0], *values[2:]))
    return result


def main():
    build = R / 'rebuild/build/native-file-exception-metadata' / datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
    build.mkdir(parents=True)
    inputs = {Path(__file__), EXE}
    seen = set()
    def closure(path):
        if path in seen:
            return
        seen.add(path)
        inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"\n]+)"', path.read_text(encoding='utf-8'), re.M):
            closure(next(p for p in (path.parent / name, R / 'rebuild/include' / name) if p.is_file()))
    sources = [R / 'rebuild/src/compiled/00/99' / ('CDiskFileWin32_' + suffix + '.cpp') for suffix in ('MovePosition_0099ac90', 'SetPosition_0099abf0', 'Read_0099a960', 'Open_0099a6a0', 'Write_0099aa90')]
    sources.append(R / 'rebuild/src/compiled/00/99/CMemoryDataOutputStream_SetPosition_00994b10.cpp')
    for source in sources:
        closure(source)
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    pins = {str(p): sha(p) for p in inputs}
    expected = original_graph(EXE.read_bytes())
    assert [row[0] for row in expected] == [TYPE, BASE]
    env = V.env()
    rows = []
    for source in sources:
        obj = build / (source.stem + '.obj')
        result = V.cl(['/nologo', '/c', '/O2', '/MT', '/Gy', '/I' + str(R / 'rebuild/include'), '/Fo' + str(obj), str(source)], env, build)
        (build / (source.stem + '.compile.json')).write_text(json.dumps(dict(code=result.returncode, stdout=result.stdout, stderr=result.stderr), indent=2))
        assert not result.returncode, result.stdout + result.stderr
        assert compiled_graph(obj) == expected, source
        rows.append(dict(source=str(source),object_sha256=sha(obj)))
    # Compile the former declaration and require the graph checker to reject it.
    mutated = build / 'include'
    mutated.mkdir()
    for path in seen:
        if path.is_relative_to(R / 'rebuild/include'):
            target = mutated / path.relative_to(R / 'rebuild/include')
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(path.read_bytes())
    header = mutated / 'fable_file_exception.h'
    text = header.read_text()
    assert text.count('class CBBBFileException : public CExceptionBase') == 1
    header.write_text(text.replace('class CBBBFileException : public CExceptionBase', 'class CBBBFileException'))
    stability = []
    for source in sources:
        obj = build / (source.stem + '.missing-base.obj')
        result = V.cl(['/nologo', '/c', '/O2', '/MT', '/Gy', '/I' + str(mutated), '/Fo' + str(obj), str(source)], env, build)
        (build / (source.stem + '.missing-base.compile.json')).write_text(json.dumps(dict(code=result.returncode, stdout=result.stdout, stderr=result.stderr), indent=2))
        assert not result.returncode, result.stdout + result.stderr
        try:
            compiled_graph(obj)
        except (KeyError, AssertionError):
            pass
        else:
            raise AssertionError('missing-base mutation survived')
        old, fixed = Coff(obj), Coff(build / (source.stem + '.obj'))
        owner, method = source.stem.split('_')[:2]
        symbol = next(s for s in old.symbols if s.startswith('?' + method + '@' + owner + '@@'))
        def body(coff):
            section, offset = coff.symbols[symbol]
            assert offset == 0
            data, refs = coff.sections[section]
            normalized = {}
            for site, (name, kind) in refs.items():
                if name.startswith('$L'):
                    target_section, target_offset = coff.symbols[name]
                    assert target_section == section
                    name = 'local+' + str(target_offset)
                normalized[site] = (name.replace('__TI1?AVCBBBFileException@@', '__TI2?AVCBBBFileException@@'), kind)
            return data, normalized
        assert body(old) == body(fixed), ('function body changed', source)
        stability.append(dict(source=str(source), symbol=symbol, root_bytes=len(body(fixed)[0]), normalized_relocations_unchanged=True))
    assert all(sha(Path(path)) == digest for path, digest in pins.items())
    report = dict(status='PASS', build=str(build), catchable_types=expected, sources=rows, body_stability=stability, controls=['missing-base'], input_sha256=pins, boundary='Compile-only exception metadata; runtime catch dispatch and cleanup not claimed.')
    (build / 'receipt.json').write_text(json.dumps(report, indent=2) + '\n')
    (R / 'rebuild/compile-gate/native-file-exception-metadata.json').write_text(json.dumps(report, indent=2) + '\n')
    print('NATIVE_FILE_EXCEPTION_METADATA_PASS sources=6 types=2 controls=1 runtime=unclaimed')


if __name__ == '__main__':
    main()
