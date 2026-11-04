"""Fresh typed wide-substring ABI and independent bounded native-state parity."""
from pathlib import Path
from datetime import datetime,timezone
import sys,re,json,hashlib,subprocess
import check_one as V
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/99/CWideString_GetSubString_0099bdb0.cpp'
HELPER=R/'tools/decomp_pipeline/native_wide_substring_verify.py'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 b=R/'rebuild/build/native-wide-substring/00/99/0099bdb0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 inputs={Path(__file__),HELPER,EXE,SOURCE,Path(sys.executable)};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for p in (V.VC/'bin').iterdir() if p.suffix.lower() in ('.exe','.dll'))
 import gate_game_component_abi,near_match,unicorn,capstone
 inputs.update(p for p in Path(unicorn.__file__).parent.rglob('*.dll') if p.is_file())
 for module in tuple(sys.modules.values()):
  path=getattr(module,'__file__',None)
  if path and Path(path).is_file():inputs.add(Path(path).resolve())
 pins={str(p):sha(p) for p in sorted(inputs)}
 for p in visited:
  if p.is_relative_to(R/'rebuild/include'):
   q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 src=b/'root.cpp';src.write_bytes(SOURCE.read_bytes());obj=b/'root.obj';env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(inc),'/Fo'+str(obj),str(src)],env,b)
 (b/'compile.json').write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([sys.executable,str(HELPER),str(b),str(obj)],cwd=R,capture_output=True,text=True,timeout=120)
 (b/'verify.run.json').write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
 proof=json.loads((b/'substring-contract-verification.json').read_text());assert proof['cases']==proof['original_cases']==992 and proof['controls']==2 and proof['score']['near_match']
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',grade='NEAR_MATCH',build=str(b),cases=992,original_cases=992,controls=2,score=proof['score'],input_sha256=pins,object_sha256={str(obj):sha(obj)},new_roots=0,boundary=proof['boundary']+' Typed provider coexists with legacy CCharString-named source;historical metadata/grades unchanged.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2)+'\n');(R/'rebuild/compile-gate/native-wide-substring.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WIDE_SUBSTRING_PASS cases=992 original=992 controls=2 score='+str(proof['score']['opcode_similarity']));print(b)
if __name__=='__main__':main()
