"""Validate display initialization, owning destruction and callback-list cleanup."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from collections import Counter
from pathlib import Path
import capstone,pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE
from gate_display_reset import exception_metadata
import display_init_oracle as I
import display_destructor_oracle as D
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(I.ENTRY,'?Init@CDisplayManager@@QAEXABVCDisplayManagerInit@@_N@Z',500),
       (D.ENTRY,'??1CDisplayManager@@UAE@XZ',268),(D.LIST,D.LIST_SYMBOL,62)]

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--init-source',type=Path,default=ROOT/'rebuild/src/compiled/00/9c/CDisplayManager_Init_009c0e50.cpp')
 ap.add_argument('--init-test',type=Path,default=ROOT/'rebuild/tests/00/9c/CDisplayManager_Init_009c0e50_test.cpp')
 ap.add_argument('--destructor-source',type=Path,default=ROOT/'rebuild/src/compiled/00/9c/CDisplayManager_Destructor_009c01c0.cpp')
 ap.add_argument('--destructor-test',type=Path,default=ROOT/'rebuild/tests/00/9c/CDisplayManager_Destructor_009c01c0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-lifetime.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-lifetime-'))
 proofs=[];coffs={};objects={};behaviors={};hashes={}
 for name,source,test in [('init',args.init_source,args.init_test),('destructor',args.destructor_source,args.destructor_test)]:
  obj=work/(name+'.obj');fixture=work/(name+'-test.obj')
  for src,out,flags in [(source,obj,[]),(test,fixture,['/EHsc'])]:
   r=check_one.cl(['/nologo','/c','/O2','/MT',*flags,'/Fo'+str(out),str(src.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
  coffs[name]=Coff(obj);objects[name]=(obj,fixture)
  hashes[name]=dict(source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(test.read_bytes()).hexdigest())
 pe=pefile.PE(str(EXE));codes={};natives={}
 for entry,symbol,length in SPECS:
  name='init' if entry==I.ENTRY else 'destructor';targets=I.TARGETS if name=='init' else D.TARGETS
  coff=coffs[name];sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);score=compare(built,native,relocs);assert score.near_match,score
  patched=bytearray(built);checked=[]
  for site,(target_name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=targets[target_name]
   struct.pack_into('<I',patched,site,(target if kind==6 else target-entry-site-4)&0xffffffff)
   checked.append(dict(offset=site,symbol=target_name,target=target,kind=kind))
  decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;actual=[]
  for ins in decoder.disasm(native,entry):
   for op in ins.operands:
    if op.type==capstone.x86.X86_OP_IMM and (ins.mnemonic=='call' or (ins.mnemonic=='jmp' and not entry<=op.imm<entry+length) or 0x1000000<=op.imm<0x1500000):actual.append(op.imm)
    elif op.type==capstone.x86.X86_OP_MEM and op.mem.disp>=0x1000000:actual.append(op.mem.disp)
  assert Counter(actual)==Counter(c['target'] for c in checked),(hex(entry),actual,checked)
  if entry==D.LIST:assert patched==native
  codes[entry]=patched;natives[entry]=native
  proofs.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH' if entry==D.LIST else 'NEAR_MATCH',built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked))
 for name,(obj,test) in objects.items():
  exe=work/(name+'-fixture.exe');cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
  r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
  r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
  r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
  expected='DISPLAY_INIT_BEHAVIOR_PASS scenarios=2304' if name=='init' else 'DISPLAY_DESTRUCTOR_BEHAVIOR_PASS scenarios=6144'
  assert r.returncode==0 and expected in r.stdout,r.stdout+r.stderr;behaviors[name]=r.stdout.strip()
 metadata=exception_metadata(coffs['init'],pe)
 init_cases=0
 for dims,flags,exclusive,caps,result,old,mutation,nulltimer in itertools.product([(-1,0),(31,32),(33,720),(1920,1080)],range(4),range(2),range(2),range(3),range(2),range(6),range(2)):
  case=(*dims,flags,exclusive,caps,result,old,mutation,nulltimer)
  assert I.execute(codes[I.ENTRY],*case)==I.execute(natives[I.ENTRY],*case),case;init_cases+=1
 destructor_cases=0
 for case in itertools.product(range(64),[0,1,3,15],range(6)):
  assert D.execute(codes[D.ENTRY],codes[D.LIST],*case)==D.execute(natives[D.ENTRY],natives[D.LIST],*case),case;destructor_cases+=1

 result=dict(entries=proofs,behavior=behaviors,hashes=hashes,exception_types=metadata,comparative_scenarios=dict(init=init_cases,destructor=destructor_cases))
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n')
 print('DISPLAY_LIFETIME_PASS compiled=8448 comparative=6144')
if __name__=='__main__':main()
