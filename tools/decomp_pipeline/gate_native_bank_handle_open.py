"""Typed bank-handle dispatch, contained lookup and counted-owner parity.

The portable parent fixture controls its callees and checks 512 branch/state/ABI
cases. Separate saved native family runs cover the installed BIGB dependency
closure. No installed files are read or modified by this gate.
"""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,struct,subprocess
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
import native_exact_roots
import pefile,capstone
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/9d/CBankFile_OpenReadOnly_009d06f0.cpp'
FIND=R/'rebuild/src/compiled/00/9a/CBankFileManager_FindContainedBank_009a7f80.cpp'
FIXTURE=R/'rebuild/tests/integration/NativeBankHandleOpen_test.cpp'
SYMBOL='?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z'
FIND_SYMBOL='?FindContainedBank@CBankFileManager@@QAE_NABVCCharString@@AAUCContainedBank@NBankFileManager@@AAV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z'
MANAGER='?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A'
FLAGS=['/nologo','/c','/O2','/MT','/Gy']
ADDRESS={
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,FIND_SYMBOL:0x9a7f80,
 '??1?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@XZ':0x9a9d60,
 '??4?$CCountedPointer@VCDiskFileWin32@@@@QAEAAV0@ABV0@@Z':0x9a9bf0,
 '??1?$CCountedPointer@VCDiskFileWin32@@@@QAE@XZ':0x9a9bb0,
 '??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z':0x994700,
 '?SetPosition@CDataInputStream@@UAEXK@Z':0x993bc0,
 '?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z':0x9cfbc0,
 '??1CFileDataInputStream@@UAE@XZ':0x994780,
 '?GetDevBankFilePath@CBankFileManager@@QBE?AVCWideString@@ABVCCharString@@@Z':0x9a7ca0,
 '??1CWideString@@QAE@XZ':0x99b510,MANAGER:0x13ca79c,
 '?Find@CBasicRetailBankAliasMap@@QBEPAUCNode@1@ABVCCharString@@@Z':0x9ab5d0,
 '?Find@CContainedBankMap@NBankFileManager@@QBEPAUCNode@12@ABVCCharString@@@Z':0x9ab4f0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,
 '??3@YAXPAX@Z':0xbfe9bc,
}
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 b=R/'rebuild/build/native-bank-handle-open/00/9d/009d06f0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();visited=set();inputs={Path(__file__)}
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 sources={SOURCE:'root.cpp',FIND:'find.cpp',FIXTURE:'Fixture.cpp'}
 for p in sources:closure(p)
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
 for p in visited:
  q=b/sources[p] if p in sources else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 def compile(p):
  obj=p.with_suffix('.obj');run=V.cl(FLAGS+['/I'+str(inc),'/Fo'+str(obj),str(p)],env,b);(b/(p.stem+'.compile.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr;return obj
 root,fixture=compile(b/'root.cpp'),compile(b/'Fixture.cpp');compile(b/'find.cpp')
 c=Coff(root);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert not off
 original=read_virtual_data(EXE.read_bytes(),0x9d06f0,385);score=compare(body,original,tuple(rels),retail_relocations('009d06f0',385)).to_dict();assert score['near_match'] and score['complete_decode'],score
 def execute(label,obj):
  exe=b/(label+'.exe');run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),'/map:'+str(exe.with_suffix('.map')),str(obj),str(fixture)],cwd=b,env=env,capture_output=True,text=True);(b/(label+'.link.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
  run=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);(b/(label+'.run.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));return run
 expected='NATIVE_BANK_HANDLE_PASS cases=512 retail=32 missing=64 version=32 dev=384';run=execute('probe',root);assert run.returncode==0 and expected in run.stdout,run.stdout+run.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 mapping.setdefault(MANAGER,mapping['_FixtureManagerWords'])
 at=mapping[SYMBOL];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);assert pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body))==bytes(fixed)
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;align=lambda n,a:(n+a-1)//a*a;last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(original);by_address={value:name for name,value in ADDRESS.items()};calls=[];globals=[]
 for ins in decoder.disasm(original,0x9d06f0):
  pos=ins.address-0x9d06f0
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=int(ins.op_str,16);name=by_address[target];struct.pack_into('<I',payload,pos+1,(mapping[name]-base-pos-5)&0xffffffff);calls.append(dict(site=hex(ins.address),target=hex(target),symbol=name))
  for operand in ins.operands:
   if operand.type==capstone.x86.X86_OP_MEM and operand.mem.base==0 and operand.mem.index==0:address=operand.mem.disp;offset=ins.disp_offset
   elif operand.type==capstone.x86.X86_OP_IMM:address=operand.imm;offset=ins.imm_offset
   else:continue
   if not 0x1200000<=address<0x1600000:continue
   assert address in (0x13ca79c,0x13ca7b0);struct.pack_into('<I',payload,pos+offset,mapping[MANAGER]+address-0x13ca79c);globals.append(dict(site=hex(ins.address),address=hex(address)))
 header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);size=align(len(payload),pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+size-len(oracle)));oracle[start:start+len(payload)]=payload
 struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',len(payload),rva,size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(payload),pe.OPTIONAL_HEADER.SectionAlignment));offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[offset:offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);(b/'original-parent.exe').write_bytes(oracle)
 original_run=subprocess.run([str(b/'original-parent.exe')],cwd=b,capture_output=True,text=True,timeout=30);(b/'original-parent.run.json').write_text(json.dumps(dict(code=original_run.returncode,stdout=original_run.stdout,stderr=original_run.stderr)));assert original_run.returncode==0 and expected in original_run.stdout and original_run.stdout==run.stdout,original_run.stdout+original_run.stderr
 faults=[('force-dev-bit','!(flags&4)','!(flags&8)'),('wrong-version-test','info.dataVersion_!=GetDataFormatVersion()','info.dataVersion_==GetDataFormatVersion()'),('missing-spare-slot','info.entryCount_+1,info.entryCount_','info.entryCount_,info.entryCount_'),('missing-open-notify','NotifyBankOpened();',';'),('dev-result-inverted','return OpenFileReadOnly(','return !OpenFileReadOnly(')]
 for i,(name,old,new) in enumerate(faults):
  text=(b/'root.cpp').read_text();assert text.count(old)==1;p=b/('control%d.cpp'%i);p.write_text(text.replace(old,new));result=execute('control%d'%i,compile(p));assert result.returncode==80 and 'BANK_HANDLE_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr)
 exact_roots=[dict(name='find_contained_bank',source=FIND,symbol=FIND_SYMBOL,address=0x9a7f80,size=270,instructions=109,relocations=8,flags=FLAGS),dict(name='retail_owner_destructor',source=SOURCE,symbol='??1?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@XZ',address=0x9a9d60,size=53,instructions=19,relocations=1,flags=FLAGS),dict(name='disk_owner_destructor',source=SOURCE,symbol='??1?$CCountedPointer@VCDiskFileWin32@@@@QAE@XZ',address=0x9a9bb0,size=53,instructions=19,relocations=1,flags=FLAGS)]
 exact=native_exact_roots.run(__file__,exact_roots,ADDRESS,'native-bank-handle-helpers.json',['512 parent cases/original parent,shared disk/retail owner references;native family receipts cover actual FindContainedBank.'],'Find270 andtwo owner destructor53B bodies original-exact;owner operations inline inread-open sources.')
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 receipt=dict(status='PASS',grade='NEAR_MATCH',build=str(b),score=score,parent_bytes=len(body),original_parent_bytes=385,host_cases=512,original_cases=512,controls=[x[0] for x in faults],helper_exact=exact,input_sha256=pins,object_sha256={str(p):sha(p) for p in b.glob('*.obj')},exe_sha256=sha(b/'probe.exe'),original_exe_sha256=sha(b/'original-parent.exe'),original_calls=calls,original_globals=globals,final_root_verified=True,boundary='Parent callees controlled inportablefixture. Actual native all8 BIGB family covered separately. Preserve FileSize,retail spare slot,ignored reader result andfailure state;no malformed/OOM/unwind contract ornew historical root count.')
 (b/'receipt.json').write_text(json.dumps(receipt,indent=2));(R/'rebuild/compile-gate/native-bank-handle-open.json').write_text(json.dumps(receipt,indent=2));print('NATIVE_BANK_HANDLE_OPEN_PASS cases=512 original=512 controls=5 parent=near_match find=MATCH owners=2')
if __name__=='__main__':main()
