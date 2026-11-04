"""Validate CMainGameComponent's resolution/event-package members and static data against retail."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
M='@CMainGameComponent@@'
SPECS=[dict(file='CMainGameComponent_PostChangeResolution_00416365.cpp',address='00416365',symbol='?PostChangeResolution'+M+'QAEXKKK@Z'),
 dict(file='CMainGameComponent_InitEventPackageLoading_00416056.cpp',address='00416056',symbol='?InitEventPackageLoading'+M+'IAE_NPBG@Z'),
 dict(file='CMainGameComponent_InitEventPackageSaving_004160cc.cpp',address='004160cc',symbol='?InitEventPackageSaving'+M+'IAE_NPBG@Z'),
 dict(file='CMainGameComponent_ProcessEventPackage_00416670.cpp',address='00416670',symbol='?ProcessEventPackage'+M+'IAEXPBVCGameEventPackage@@@Z')]
EXTERNAL={'?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z':0x434a20,'?ChangeResolution@CWorld@@QAEXKKK@Z':0x49d820,'??0CWideString@@QAE@PBG@Z':0x99b6b0,'??1CWideString@@QAE@XZ':0x99b510,
 '?GetNoEvents@CGameEventPackage@@QBEKXZ':0x9f16e0,'?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z':0x9f16d0,'?ProcessEvent'+M+'IAEXPBVCGameEvent@@@Z':0x415fe0}
DATA=[('?ConstantFPS'+M+'0JA',0x1375550,4),('?AlwaysUpdateGameFrame'+M+'2_NA',0x13b8688,1),('?ProfileUpdateTime'+M+'2NA',0x13b8690,8),('?ProfileRenderTime'+M+'2NA',0x13b8698,8),
 ('?PMainGameComponent'+M+'1PAV1@A',0x13b86a0,4),('?UseOldInterpolationTiming'+M+'0_NA',0x13b86a4,1)]
COMP=0x2000000;FILETABLE=0x2100000;NAME=0x2100100;PACKAGE=0x2100200;STACK=0x3000000;STOP=0x4000000;STUBS=0x5000000
def run(codes,entry,args,noEvents=0):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x415000,0x2000),(0x434000,0x1000),(0x49d000,0x1000),(0x99b000,0x1000),(0x9f1000,0x1000),(COMP,0x20000),(FILETABLE,0x1000),(STACK,0x10000),(STOP,0x1000),(STUBS,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in EXTERNAL.values():u.mem_write(a,b'\xc3')
 u.mem_write(COMP,b'\x4d'*0x16200);put(COMP+0x24,0x2018000);put(COMP+0x28,0x2019000);put(COMP+0x16154,FILETABLE)
 for k in range(8):put(FILETABLE+4*k,STUBS+16*k)
 u.mem_write(STUBS,b'\xc3'*0x1000);trace=[];counter=[0]
 pops={0x434a20:12,0x49d820:12,0x99b6b0:4,0x99b510:0,0x9f16e0:0,0x9f16d0:4,0x415fe0:4}
 def hook(uc,a,s,user):
  sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0
  if a in pops:
   pop=pops[a];args=[get(sp+4+4*i) for i in range(pop//4)]
   if a==0x9f16e0:counter[0]+=1;result=noEvents
   if a==0x9f16d0:result=0x7000+args[0]*16
   if a==0x99b6b0:put(this,0xabc)
   trace.append((hex(a),this,tuple(args)))
  elif STUBS<=a<STUBS+0x1000:
   slot=(a-STUBS)//16;pop={1:12,3:12,4:12}.get(slot,0);args=[get(sp+4+4*i) for i in range(pop//4)]
   if slot==4:args.append(get(args[0]))
   trace.append(('slot',slot,this,tuple(args)))
  else:return
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,COMP);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(entry,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 return u.reg_read(UC_X86_REG_EAX)&0xff,trace,bytes(u.mem_read(COMP+0x16150,0x30)),counter[0]
def compare(candidate,native):
 cases=0
 for w,h,d in [(640,480,16),(1600,1200,32),(0,0,0),(0xffffffff,1,2)]:
  c=run(candidate,0x416365,[w,h,d]);assert c==run(native,0x416365,[w,h,d])
  assert [t[0] for t in c[1]]==['0x434a20','0x49d820'] and c[1][0][1]==0x2019000 and c[1][1][1]==0x2018000 and c[1][0][2]==c[1][1][2]==(w,h,d);cases+=1
 for entry,access,slot in [(0x416056,1,3),(0x4160cc,4,4)]:
  c=run(candidate,entry,[NAME]);assert c==run(native,entry,[NAME])
  kinds=[t[0] if t[0]!='slot' else ('slot',t[1]) for t in c[1]]
  assert kinds==['0x99b6b0',('slot',1),'0x99b510',('slot',slot)] and c[0]==1 and c[1][1][3][1:]==(access,2) and c[1][3][3][:3]==(COMP+0x16174,4,0)
  if slot==4:assert c[1][3][3][3]==1
  cases+=1
 for n in range(7):
  c=run(candidate,0x416670,[PACKAGE],n);assert c==run(native,0x416670,[PACKAGE],n)
  assert c[3]==n+1 and [t[2][0] for t in c[1] if t[0]=='0x415fe0']==[0x7000+16*i for i in range(n)];cases+=1
 return cases
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/MainGame_test.cpp');ap.add_argument('--data',type=Path,default=ROOT/'rebuild/src/compiled/01/37/CMainGameComponent_StaticData_01375550.cpp')
 ap.add_argument('--data-test',type=Path,default=ROOT/'rebuild/tests/integration/MainGameStaticData_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/main-game.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='main-game-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(SPECS,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0;patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);struct.pack_into('<I',patched,pos,(targets[name]-addr-pos-4 if kind==20 else targets[name])&0xffffffff)
   raw=pe.get_data(addr-0x400000,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
   entries.append(dict(s,bytes=len(b),relocations=sorted([pos,list(v)] for pos,v in rel.items()),grade='RELOCATION_MATCH' if rel else 'MATCH'))
  data=Coff(compile(a.data,'data.obj'));checked=[]
  for symbol,address,size in DATA:
   rva=address-0x400000;section=next(s for s in pe.sections if s.VirtualAddress<=rva<s.VirtualAddress+s.Misc_VirtualSize);assert section.Name.rstrip(b'\0')==b'.data'
   value=pe.get_data(rva,size) if rva+size<=section.VirtualAddress+section.SizeOfRawData else bytes(size)
   assert coff_data(data,symbol,size)==value,symbol;checked.append(dict(symbol=symbol,address='%08x'%address,value=value.hex()))
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');assert all(s['symbol'] not in Coff(test).symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'MAIN_GAME_BEHAVIOR_PASS scenarios=19' in r.stdout,(r.returncode,r.stdout,r.stderr)
 dtest=compile(a.data_test,'dtest.obj');assert not any(s in Coff(dtest).symbols for s,_,_ in DATA);dexe=w/'data.exe'
 r2=subprocess.run([str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(dexe),str(dtest)],env=env,capture_output=True,text=True);assert r2.returncode,'data test must require the provider'
 r2=subprocess.run([str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(dexe),str(dtest),str(w/'data.obj')],env=env,capture_output=True,text=True);assert not r2.returncode,r2.stdout
 r2=subprocess.run([str(dexe)],capture_output=True,text=True,timeout=30);assert not r2.returncode and 'MAIN_GAME_STATIC_DATA_PASS' in r2.stdout,r2.stdout
 report=dict(entries=entries,data_symbols=checked,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),
  data_sha256=hashlib.sha256(a.data.read_bytes()).hexdigest(),data_test_sha256=hashlib.sha256(a.data_test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS)+1)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('MAIN_GAME_PASS compiled=19 comparative=%d providers=%d data=%d'%(comparisons,len(SPECS),len(DATA)))
if __name__=='__main__':main()
