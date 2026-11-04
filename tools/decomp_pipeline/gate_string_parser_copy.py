"""Compile and validate retail CStringParser quote-position behavior."""
import json, subprocess, tempfile
from pathlib import Path
import check_one
import oracle_string_parser_copy
from gate_game_component_abi import Coff
ROOT = Path(__file__).resolve().parents[2]
def main():
    retail_cases = oracle_string_parser_copy.collect()
    work = Path(tempfile.mkdtemp(prefix='parser-position-gate-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    source = ROOT / 'rebuild/src/compiled/00/46/CStringParser_CopyConstructor_0046381b.cpp'
    test = ROOT / 'rebuild/tests/00/46/CStringParser_CopyConstructor_0046381b_test.cpp'
    obj = work / 'position.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    coff = Coff(obj)
    section, offset = coff.symbols['??0CStringParser@@QAE@ABV0@@Z']
    body, relocations = coff.sections[section]
    assert offset == 0
    result = check_one.cl(['/nologo', '/O2', '/MT', '/Fe' + str(work / 'test.exe'), str(source), str(test)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'test.exe')], capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'PARSER_COPY_OK cases=3' in result.stdout
    (ROOT / 'rebuild/compile-gate/string-parser-copy.json').write_text(json.dumps(dict(address='0046381b', grade='BEHAVIORAL', compiled_bytes=len(body), retail_bytes=86, linked_cases=3, retail_cases=retail_cases), indent=2) + '\n')
    print('PARSER_COPY_PASS cases=3')
if __name__ == '__main__': main()
