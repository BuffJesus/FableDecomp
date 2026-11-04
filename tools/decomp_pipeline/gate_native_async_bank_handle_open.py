"""Native asynchronous bank-handle dispatch and shared threaded ownership."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,subprocess,re,struct
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
import pefile,capstone,native_exact_roots
R=Path(__file__).resolve().parents[2]
def main():
 b=R/'rebuild/build/native-async-bank-handle-open/00/9d/009d56c0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();seen=set();sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 def closure(p):
  if p in seen:return
  seen.add(p)
  if p.is_relative_to(R/'rebuild/include'):
   q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   q=next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file());closure(q)
 sources=[R/'rebuild/tests/integration/NativeAsyncBankHandleOpen_test.cpp',R/'rebuild/src/compiled/00/9d/CBankFileAsync_OpenReadOnly_009d56c0.cpp',R/'rebuild/src/compiled/00/9d/CCountedPointer_ThreadedFile_Assign_009d6fd0.cpp',R/'rebuild/src/compiled/00/9a/CCountedPointer_CThreadedFile_Reset_009a9c80.cpp'];objects=[];env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)+';'+str(R/'rebuild/tests/integration')
 for source in sources:closure(source)
 inputs=set(seen)|{Path(__file__),EXE}
 inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py'))
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
 for i,source in enumerate(sources):
  closure(source);p=b/('unit%d.cpp'%i);p.write_bytes(source.read_bytes());obj=p.with_suffix('.obj');run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/I'+str(R/'rebuild/tests/integration'),'/Fo'+str(obj),str(p)],env,b);assert not run.returncode,run.stdout+run.stderr;objects.append(obj)
 run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr
 run=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);(b/'run.json').write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,(run.returncode,run.stdout,run.stderr);print(b);print(run.stdout);(b/'composition.json').write_text(json.dumps(dict(objects={str(p):sha(p) for p in objects},sources={str(s):str(o) for s,o in zip(sources,objects)}),indent=2))

 r=json.loads((b/'composition.json').read_text());
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 parent='?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z';c=Coff(b/'unit1.obj');symbol='?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z';sec,off=c.symbols[symbol];body,rels=c.sections[sec];image=EXE.read_bytes();original=read_virtual_data(image,0x9d56c0,348);score=compare(body,original,tuple(rels),retail_relocations('009d56c0',348)).to_dict();assert score['near_match']
 mapping.setdefault('?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A',mapping['_FixtureManagerWords'])
 linked=pefile.PE(str(b/'probe.exe'));root_at=mapping[symbol];fixed=bytearray(body)
 for site,(dep,kind) in rels.items():
  target=root_at+c.symbols[dep][1] if dep.startswith('$L') and c.symbols[dep][0]==sec else mapping[dep]
  assert kind in (6,20);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(root_at+site+4 if kind==20 else 0))&0xffffffff)
 assert linked.get_data(root_at-linked.OPTIONAL_HEADER.ImageBase,len(body))==bytes(fixed)
 targets={0x9d06f0:parent,0x99ebf0:'??0CCharString@@QAE@PBDJ@Z',0x9e9f40:'?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z',0x99eae0:'??1CCharString@@QAE@XZ',0x9a7f80:next(n for n in mapping if n.startswith('?FindContainedBank@')),0x9a9d60:'??1?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@XZ',0x9d6fd0:'??4?$CCountedPointer@VCThreadedFile@@@@QAEAAV0@ABV0@@Z',0x9a9c40:'??1?$CCountedPointer@VCThreadedFile@@@@QAE@XZ',0xbfea1a:'??2@YAPAXI@Z',0x98dfd0:'??0CThreadedFile@@QAE@XZ',0x9a9c80:'?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z',0x9cbf10:'?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ',0x98e1e0:'?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z',0x99b510:'??1CWideString@@QAE@XZ'}
 literal=next(n for n in c.indices.values() if n.startswith('??_C@'));raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);last=pe.sections[-1];align=lambda n,a:(n+a-1)//a*a;rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(original);decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;calls=[]
 for ins in decoder.disasm(original,0x9d56c0):
  pos=ins.address-0x9d56c0
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=int(ins.op_str,16);name=targets[target];struct.pack_into('<I',payload,pos+1,(mapping[name]-base-pos-5)&0xffffffff);calls.append(dict(site=hex(ins.address),symbol=name))
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_IMM and op.imm in (0x129b884,0x13ca79c):struct.pack_into('<I',payload,pos+ins.imm_offset,mapping[literal] if op.imm==0x129b884 else mapping['_FixtureManagerWords'])
 header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(payload),pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+size-len(oracle)));oracle[start:start+len(payload)]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',len(payload),rva,size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(payload),pe.OPTIONAL_HEADER.SectionAlignment));at=mapping[symbol];offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[offset:offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);(b/'original.exe').write_bytes(oracle)
 run=subprocess.run([str(b/'original.exe')],cwd=b,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==json.loads((b/'run.json').read_text())['stdout'],(run.returncode,run.stdout,run.stderr)
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include');faults=[('missing-progress',1,'NProgressDisplay::DisplayProgress(CCharString("Open Bank File Async"),-1.0f,false,false);',';'),('wrong-retail-cache',1,'NonCached=retail->useDma_;','NonCached=!retail->useDma_;'),('async-disabled',1,'DisableAsync=false;','DisableAsync=true;'),('dev-open-result-used',1,'ThreadedFile->Open(GetBankPathname(),NonCached);','if(!ThreadedFile->Open(GetBankPathname(),NonCached))return false;'),('base-failure-accepted',1,'if(!CBankFile::OpenReadOnly(handle,flags))return false;','if(!CBankFile::OpenReadOnly(handle,flags))return true;'),('missing-shared-reference',2,'if(control_)++control_->RefCount;',';')]
 controls=[]
 for label,unit,old,new in faults:
  text=(b/('unit%d.cpp'%unit)).read_text();assert old in text;p=b/(label+'.cpp');p.write_text(text.replace(old,new));obj=p.with_suffix('.obj');run=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(b/'include'),'/Fo'+str(obj),str(p)],env,b);assert not run.returncode,run.stdout+run.stderr;objects=[str(obj) if Path(x).name=='unit%d.obj'%unit else x for x in r['objects']];exe=b/(label+'.exe');run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*objects],cwd=b,env=env,capture_output=True,text=True);assert not run.returncode,run.stdout+run.stderr;run=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);assert run.returncode==80 and 'ASYNC_HANDLE_FAILURE' in run.stderr,(label,run.returncode,run.stderr);controls.append(dict(name=label,code=run.returncode));print('DETECTED',label)

 exact=native_exact_roots.run(__file__,[
 dict(name='threaded_owner_assignment',source=sources[2],symbol='??4?$CCountedPointer@VCThreadedFile@@@@QAEAAV0@ABV0@@Z',address=0x9d6fd0,size=72,instructions=32,relocations=1,flags=['/nologo','/c','/O2','/MT','/Gy']),
 dict(name='threaded_owner_destructor',source=sources[1],symbol='??1?$CCountedPointer@VCThreadedFile@@@@QAE@XZ',address=0x9a9c40,size=53,instructions=19,relocations=1,flags=['/nologo','/c','/O2','/MT','/Gy'])],{'??3@YAXPAX@Z':0xbfe9bc},'native-async-bank-handle-helpers.json',['512 parent cases and512 original cases;shared owner replacement andsame-owner checks.'],'Typed shared threaded ownership;actual queued BIGB family receipts saved separately.')
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 report=dict(status='PASS',grade='NEAR_MATCH',build=str(b),cases=512,original_cases=512,controls=controls,score=score,calls=calls,helpers=exact,final_root_verified=True,input_sha256=pins,object_sha256={str(p):sha(p) for p in b.glob('unit*.obj')},exe_sha256=sha(b/'probe.exe'),original_exe_sha256=sha(b/'original.exe'),boundary='Controlled parent callees;native installed BIGB family/queued worker reads independently covered bysaved work receipts. No malformed/OOM/unwind contract ornew historical count.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-async-bank-handle-open.json').write_text(json.dumps(report,indent=2));print('NATIVE_ASYNC_BANK_HANDLE_OPEN_PASS cases=512 original=512 controls=6 parent=near_match helpers=2MATCH')
if __name__=='__main__':main()
