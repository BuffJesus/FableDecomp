"""Verify native critical-message conversion, dialog arguments and lifetimes.

The fixture replaces MessageBoxW; this gate never opens a dialog.
"""
import argparse
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
ADDRESS = 0x009a4ed0
SYMBOL = '?DisplayCriticalMessage@CSystemManager@@QAEXPBD@Z'
TARGETS = {
    '??0CCharString@@QAE@PBDJ@Z': 0x0099ebf0,
    '?ToWideString@CCharString@@QBE?AVCWideString@@XZ': 0x0099e480,
    '?GetAsCharArray@CWideString@@QBEPBGXZ': 0x0099af10,
    '??1CWideString@@QAE@XZ': 0x0099b510,
    '??1CCharString@@QAE@XZ': 0x0099eae0,
    '__imp__MessageBoxW@16': 0x014402c8,
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_DisplayCriticalMessage_009a4ed0.cpp')
    parser.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/00/9a/CSystemManager_DisplayCriticalMessage_009a4ed0_test.cpp')
    parser.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/critical-message.json')
    args = parser.parse_args()
    work = Path(tempfile.mkdtemp(prefix='critical-message-'))
    env = check_one.env()
    obj, test_obj = work/'source.obj', work/'fixture.obj'
    for source, target, optimization in [(args.source, obj, '/O2'), (args.test, test_obj, '/Od')]:
        result = check_one.cl(['/nologo', '/c', optimization, '/Oy', '/GS', '/MT',
                              '/Fo'+str(target), str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocs = coff.sections[section]
    with pefile.PE(str(EXE), fast_load=True) as pe:
        base = pe.OPTIONAL_HEADER.ImageBase
        retail = pe.get_data(ADDRESS-base, 75)
        assert offset == 0 and len(built) == 75 and len(relocs) == 7
        assert check_one.mask(built, relocs) == check_one.mask(retail, relocs)
        checked = []
        for site, (symbol, kind) in relocs.items():
            actual = int.from_bytes(retail[site:site+4], 'little', signed=kind == 20)
            if kind == 20:
                actual += ADDRESS+site+4
                assert actual == TARGETS[symbol]
            elif symbol in TARGETS:
                assert kind == 6 and actual == TARGETS[symbol]
            else:
                assert kind == 6 and symbol.startswith('??_C@')
                assert actual == 0x0129ab70
                literal_section, literal_offset = coff.symbols[symbol]
                caption, literal_relocs = coff.sections[literal_section]
                expected = 'Application Error\0'.encode('utf-16-le')
                assert not literal_relocs and caption[literal_offset:] == expected
                assert pe.get_data(actual-base, len(expected)) == expected
            checked.append({'offset': site, 'symbol': symbol, 'target': f'{actual:08x}'})
    exe = work/'fixture.exe'
    command = [str(check_one.VC/'bin/link.exe'), '/nologo', '/out:'+str(exe)]
    standalone = subprocess.run(command+[str(test_obj)], env=env, capture_output=True, text=True)
    assert standalone.returncode != 0, 'fixture must require candidate'
    linked = subprocess.run(command+[str(obj), str(test_obj)], env=env, capture_output=True, text=True)
    assert linked.returncode == 0, linked.stdout+linked.stderr
    tested = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert tested.returncode == 0 and 'CRITICAL_MESSAGE_BEHAVIOR_PASS scenarios=45' in tested.stdout, tested.stdout+tested.stderr
    report = {'address': f'{ADDRESS:08x}', 'grade': 'RELOCATION_MATCH', 'bytes': 75,
              'relocations': checked, 'behavior': tested.stdout.strip(),
              'source_sha256': hashlib.sha256(args.source.read_bytes()).hexdigest(),
              'fixture_sha256': hashlib.sha256(args.test.read_bytes()).hexdigest()}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('CRITICAL_MESSAGE_PASS grade=RELOCATION_MATCH bytes=75 relocations=7 scenarios=45')


if __name__ == '__main__':
    main()
