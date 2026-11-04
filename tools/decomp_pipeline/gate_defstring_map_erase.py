"""Check typed string-map erase behavior and relocated retail parity."""
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE, pe_sections, va_to_off

ROOT = Path(__file__).resolve().parents[2]

def main():
    work = Path(tempfile.mkdtemp(prefix='defstring-map-erase-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source = ROOT/'rebuild/src/compiled/00/43/STL_DefStringMapErase_0043020a.cpp'
    test = ROOT/'rebuild/tests/00/43/STL_DefStringMapErase_0043020a_test.cpp'
    obj = work/'erase.obj'
    result = check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
    assert result.returncode == 0, result.stdout+result.stderr
    coff = Coff(obj)
    key = next(k for k in coff.symbols if k.startswith('?_M_erase'))
    section, offset = coff.symbols[key]
    body, relocations = coff.sections[section]
    patched = bytearray(body)
    for position, (symbol, kind) in relocations.items():
        target = 0x43020a if symbol == key else 0xbfea14 if symbol == '_free' else 0x99eae0 if symbol.startswith('??1CCharString@@') else None
        assert target is not None and kind == 20, (symbol, kind)
        struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-0x43020a-position-4)&0xffffffff)
    raw = EXE.read_bytes()
    retail_offset = va_to_off(pe_sections(raw),0x43020a)
    assert len(body) == 53 and bytes(patched) == raw[retail_offset:retail_offset+53], len(body)
    exe = work/'test.exe'
    result = check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(source),str(test)],env,work)
    assert result.returncode == 0, result.stdout+result.stderr
    result = subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode == 0 and 'DEFSTRING_MAP_ERASE_OK cases=2' in result.stdout, result.stdout
    (ROOT/'rebuild/compile-gate/defstring-map-erase.json').write_text(json.dumps(dict(address='0043020a',grade='RELOCATION_MATCH',bytes=53,linked_cases=2,mocked_boundaries='string destruction recorded by fixture; native CRT allocation and free'),indent=2)+'\n')
    print('DEFSTRING_MAP_ERASE_PASS cases=2 bytes=53')

if __name__ == '__main__':
    main()
