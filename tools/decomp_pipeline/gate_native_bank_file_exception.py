"""Native bank exception constructor, original bytes, mutation model and host ABI."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess,struct
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
from NativeBankFileException_model import compare,SYMBOL
SOURCE=R/'rebuild/src/compiled/00/9d/CBankFileException_Constructor_009d1e70.cpp'
FIXTURE=R/'rebuild/tests/integration/NativeBankFileException_test.cpp'
MODEL=R/'rebuild/tests/integration/NativeBankFileException_model.py'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inputs={Path(__file__),EXE,MODEL};visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in (SOURCE,FIXTURE):closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for directory in (V.VC/'lib',V.VC/'bin') for p in directory.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-file-exception/00/9d/009d1e70'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(n,data):
  p=b/n;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in visited:
  target=b/('root.cpp' if p==SOURCE else 'fixture.cpp') if p in (SOURCE,FIXTURE) else inc/p.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 def compile_source(src,obj,label):
  result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(inc),'/Fo'+str(obj),str(src)],env,b);save(label+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 obj=b/'root.obj';fo=b/'fixture.obj';compile_source(b/'root.cpp',obj,'root');compile_source(b/'fixture.cpp',fo,'fixture');report=compare(obj);assert report['cases']==120 and len(report['controls'])==3
 def link(root,label):
  exe=b/(label+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'/map:'+str(b/(label+'.map')),str(fo),*([str(root)] if root else []),'kernel32.lib'];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True,timeout=30);save(label+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link(obj,'probe');assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=15);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=20' in result.stdout,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 aliases={'??BCCharString@@QBEPBDXZ':'@Convert@8','??1CCharString@@QAE@XZ':'@Destroy@8'};c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];patched=bytearray(body);entry=mapping[SYMBOL]
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(mapping[aliases[name]]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
 with pefile.PE(str(exe)) as pe:assert bytes(patched)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body))
 _,result=link(None,'missing-root');assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 native=[]
 for name,old,new in [('skip-conversion','static_cast<const char*>(errortext);',';'),('wrong-type','Type = type;','Type = BANK_IS_CORRUPT;')]:
  text=SOURCE.read_text();assert text.count(old)==1;src=b/(name+'.cpp');src.write_text(text.replace(old,new));mut=b/(name+'.obj');compile_source(src,mut,name);out,result=link(mut,name);assert not result.returncode,result.stdout+result.stderr
  result=subprocess.run([str(out)],cwd=b,capture_output=True,text=True,timeout=15);save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'BANK_EXCEPTION_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr);native.append(name)
 for name,h in pins.items():assert sha(Path(name))==h,('input drift',name)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report.update(build=str(b),host_cases=20,native_controls=native,final_bytes=33,final_fixups=2,missing_root_rejected=True,input_sha256=pins,object_sha256={str(p):sha(p) for p in (obj,fo)},executable_sha256=sha(exe))
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-file-exception.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_FILE_EXCEPTION_PASS bytes=33 model=120 host=20 controls=5 grade=MATCH')

if __name__=='__main__':main()
