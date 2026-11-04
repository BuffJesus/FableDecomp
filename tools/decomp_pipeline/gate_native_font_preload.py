"""Typed default CFontBank::Preload: real candidate/original calls and provider controls."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,struct,hashlib,subprocess
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
SYMBOL='?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def run(source,test,b):
 b.mkdir(parents=True);inputs={Path(__file__),EXE};seen=set()
 def closure(p):
  p=p.resolve()
  if p in seen:return
  seen.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file());closure(q)
 closure(source);closure(test)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(R/'rebuild/include')
 original=read_virtual_data(EXE.read_bytes(),0x9a0b20,5);assert original==bytes.fromhex('b001c20800');assert struct.unpack('<I',read_virtual_data(EXE.read_bytes(),0x129ab38+4*4,4))[0]==0x9a0b20;(b/'original.bin').write_bytes(original)
 def compile(path,label,opt='/O2'):
  obj=b/(label+'.obj');result=V.cl(['/nologo','/c',opt,'/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(path)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(source=str(path),code=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr;return obj
 def audit(obj):
  coff=Coff(obj);assert SYMBOL in coff.symbols,('missing typed provider',list(coff.symbols));sec,offset=coff.symbols[SYMBOL];body,relocations=coff.sections[sec];assert not offset and not relocations and body==original;return dict(symbol=SYMBOL,bytes=5,relocations=0,grade='MATCH',vtable='0x129ab38',slot=4,target='0x009a0b20')
 native=compile(source,'candidate');proof=audit(native);host=compile(test,'host','/Od')
 def link(label,objects):
  exe=b/(label+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,objects)],cwd=b,env=env,capture_output=True,text=True);(b/(label+'.link.json')).write_text(json.dumps(dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr)));return result,exe
 result,exe=link('probe',[native,host]);assert not result.returncode,result.stdout+result.stderr
 positive=subprocess.run([str(exe),str(b/'original.bin')],cwd=b,capture_output=True,text=True,timeout=20);record=dict(code=positive.returncode,stdout=positive.stdout,stderr=positive.stderr);(b/'run.json').write_text(json.dumps(record));assert not positive.returncode and 'NATIVE_FONT_PRELOAD_BEHAVIOR_PASS cases=256 original=256' in positive.stdout,record
 import pefile
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 pe=pefile.PE(str(exe));assert pe.get_data(mapping[SYMBOL]-pe.OPTIONAL_HEADER.ImageBase,5)==original
 text=source.read_text();assert text.count('return true;')==1;fault=b/'false.cpp';fault.write_text(text.replace('return true;','return false;'));obj=compile(fault,'false')
 rejected=False
 try:audit(obj)
 except AssertionError:rejected=True
 assert rejected
 result,fault_exe=link('false',[obj,host]);assert not result.returncode,result.stdout+result.stderr
 failed=subprocess.run([str(fault_exe),str(b/'original.bin')],cwd=b,capture_output=True,text=True,timeout=20);assert failed.returncode==80 and 'FONT_PRELOAD_FAILURE' in failed.stderr,(failed.returncode,failed.stderr)
 wrong=b/'wrong-symbol.cpp';wrong.write_text('#include "fable_font_bank.h"\nstruct CFontBank_Methods:CFontBank{bool M(int,int);};\nbool CFontBank_Methods::M(int,int){return true;}\n');obj=compile(wrong,'wrong-symbol');rejected=False
 try:audit(obj)
 except AssertionError:rejected=True
 assert rejected
 omitted,_=link('omitted',[host]);assert omitted.returncode!=0 and 'Preload' in omitted.stdout+omitted.stderr
 again=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=20);assert not again.returncode and 'NATIVE_FONT_PRELOAD_BEHAVIOR_PASS cases=256 original=0' in again.stdout
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',build=str(b),proof=proof,cases=256,original_cases=256,direct_and_virtual_calls=512,controls=dict(return_false=dict(code=failed.returncode,stderr=failed.stderr),wrong_symbol_rejected=True,missing_provider_link_rejected=True),input_sha256=pins,objects={str(p):sha(p) for p in (native,host)},run=record,boundary='Default Preload leaf returns true,ignores both arguments andpreserves receiver/string bytes andstring instance counts. Original five-byte code executes256 valid-object ABI cases; candidate called directly andvirtually512 times. Real native empty-string helpers are dependencies. Real empty font-name/base construction and destruction execute; glyph services remain controlled; no glyph acquisition/font readiness/rasterization claim.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));return report

def main():
 report=run(R/'rebuild/src/compiled/00/9a/CFontBank_Preload_009a0b20.cpp',R/'rebuild/tests/00/9a/CFontBank_Preload_009a0b20_test.cpp',R/'rebuild/build/native-font-preload/00/9a/009a0b20'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ'))
 (R/'rebuild/compile-gate/native-font-preload.json').write_text(json.dumps(report,indent=2));print('NATIVE_FONT_PRELOAD_GATE_PASS cases=256 original=256 controls=2 wrong_symbol=1 omission=1 exact=1')
if __name__=='__main__':main()
