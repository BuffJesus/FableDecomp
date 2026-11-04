"""Validate typed handle/movable memory-pool policy overrides, forwarding and virtual slot ABI."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from movable_pool_policy_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SPECS=[
 dict(file='CMovableResourceMemoryPool_FreeResource_00a62ed0.cpp',address='00a62ed0',symbol='?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z',bytes=5),
 dict(file='CMovableResourceMemoryPool_MoveResource_00a62f30.cpp',address='00a62f30',symbol='?MoveResource@CMovableResourceMemoryPool@@MAEXPAXJ00K@Z',bytes=53),
 dict(file='CMovableResourceMemoryPool_CanMoveResource_00a62f70.cpp',address='00a62f70',symbol='?CanMoveResource@CMovableResourceMemoryPool@@MAE_NPAXJ@Z',bytes=26),
 dict(file='CMovableResourceMemoryPool_ValidateResource_00a62ec0.cpp',address='00a62ec0',symbol='?ValidateResource@CMovableResourceMemoryPool@@MAEXPAX@Z',bytes=12),
 dict(file='CHandleBasedMemoryPool_MoveResource_00a02120.cpp',address='00a02120',symbol='?MoveResource@CHandleBasedMemoryPool@@MAEXPAXJ00K@Z',bytes=26),
 dict(file='CHandleBasedMemoryPool_CanMoveResource_00a03a40.cpp',address='00a03a40',symbol='?CanMoveResource@CHandleBasedMemoryPool@@MAE_NPAXJ@Z',bytes=5),
 dict(file='CHandleBasedMemoryPool_ValidateResource_00a03a50.cpp',address='00a03a50',symbol='?ValidateResource@CHandleBasedMemoryPool@@MAEXPAX@Z',bytes=3),
 dict(file='CHandleBasedMemoryPool_InternalSetResource_00a028b0.cpp',address='00a028b0',symbol='?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z',bytes=22)]
EXTERNAL={'?Free@CHandleBasedMemoryPool@@QAEXJ@Z':0xa02b30,'_memmove':0xbfeae6}
VTABLES={'??_7CHandleBasedMemoryPool@@6B@':0x129c964,'??_7CMovableResourceMemoryPool@@6B@':0x129d6a0}
DTORS={'??_7CHandleBasedMemoryPool@@6B@':('??_ECHandleBasedMemoryPool@@UAEPAXI@Z',0xa03aa0),'??_7CMovableResourceMemoryPool@@6B@':('??_ECMovableResourceMemoryPool@@UAEPAXI@Z',0xa62f90)}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/MovablePoolPolicy_test.cpp');ap.add_argument('--abi-test',type=Path,default=ROOT/'rebuild/tests/integration/PoolVirtualLayout_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/movable-pool-policy.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='movable-pool-policy-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(SPECS,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['bytes'],(s['file'],len(b));patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
   entries.append(dict(s,native_bytes=len(raw),relocations=sorted([pos,list(v)] for pos,v in rel.items()),grade='RELOCATION_MATCH' if rel else 'MATCH'))
  # Four-slot ABI: compiled vtables must name the same methods at the same slots as retail.
  abi=Coff(compile(a.abi_test,'abi.obj'));slots={}
  for vt,retail in VTABLES.items():
   sec,off=abi.symbols[vt];_,rel=abi.sections[sec];names=[rel[off+4*k][0] for k in range(4)];slots[vt]=names
   assert names[0]==DTORS[vt][0] and pe.get_dword_at_rva(retail-0x400000)==DTORS[vt][1],(vt,names)
   for k in range(1,4):assert pe.get_dword_at_rva(retail-0x400000+4*k)==targets[names[k]],(vt,k,names[k])
   assert off+16 not in rel,vt
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');c=Coff(test);assert all(s['symbol'] not in c.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'MOVABLE_POOL_POLICY_BEHAVIOR_PASS scenarios=2000' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,vtable_slots=slots,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),abi_fixture_sha256=hashlib.sha256(a.abi_test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),boundaries=['CHandleBasedMemoryPool::Free','memmove','CMovableResource virtual methods'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('MOVABLE_POOL_POLICY_PASS compiled=2000 comparative=%d providers=%d slots=8'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
