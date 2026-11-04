"""Validate native definition symbol-pass orchestration."""
import json, subprocess, tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    work=Path(tempfile.mkdtemp(prefix='definition-symbol-gate-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source=ROOT/'rebuild/src/compiled/00/9a/CDefinitionManager_ParseStringForSymbols_009ade90.cpp';test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_ParseStringForSymbols_009ade90_test.cpp';obj=work/'parser.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);symbol=next(k for k in c.symbols if k.startswith('?ParseStringForSymbols@'));section,offset=c.symbols[symbol];body,relocs=c.sections[section]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(work/'test.exe'),str(source),str(test)],env,work);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(work/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_SYMBOL_PARSER_OK cases=2' in r.stdout
    (ROOT/'rebuild/compile-gate/definition-symbol-parser.json').write_text(json.dumps(dict(address='009ade90',grade='BEHAVIORAL',compiled_bytes=len(body),retail_bytes=701,linked_cases=2,mocked_boundaries='token reader, keyword search, instantiation, error, enum parser'),indent=2)+'\n')
    print('DEFINITION_SYMBOL_PARSER_PASS cases=2 bytes='+str(len(body)))
if __name__=='__main__':main()
