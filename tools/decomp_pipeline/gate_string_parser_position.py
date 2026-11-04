"""Compile and validate retail CStringParser quote-position behavior."""
import json, subprocess, tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT = Path(__file__).resolve().parents[2]
def main():
    work = Path(tempfile.mkdtemp(prefix='parser-position-gate-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    source = ROOT / 'rebuild/src/compiled/00/9b/CStringParser_SetStringPos_009b9940.cpp'
    test = ROOT / 'rebuild/tests/00/9b/CStringParser_SetStringPos_009b9940_test.cpp'
    obj = work / 'position.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols['?SetStringPos@CStringParser@@QAEXK@Z']
    body, relocations = coff.sections[section]
    assert offset == 0 and not relocations
    result = check_one.cl(['/nologo', '/O2', '/MT', '/Fe' + str(work / 'test.exe'), str(source), str(test)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'test.exe')], capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'PARSER_POSITION_OK cases=10' in result.stdout
    (ROOT / 'rebuild/compile-gate/string-parser-position.json').write_text(json.dumps(dict(address='009b9940', grade='BEHAVIORAL', compiled_bytes=len(body), retail_bytes=62, linked_cases=10), indent=2) + '\n')
    print('PARSER_POSITION_PASS cases=10')
if __name__ == '__main__': main()
