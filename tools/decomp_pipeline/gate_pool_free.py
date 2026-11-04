"""Validate handle-pool Free, free-list insertion and neighbour coalescing against retail."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from near_match import compare as similarity,retail_relocations
from pool_free_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SPECS=[
 dict(file='CHandleBasedMemoryPool_Free_00a02b30.cpp',address='00a02b30',symbol='?Free@CHandleBasedMemoryPool@@QAEXJ@Z',bytes=201,native_bytes=201,near=True),
 dict(file='CHandleBasedMemoryPool_InsertFreeArea_00a01da0.cpp',address='00a01da0',symbol='?InsertFreeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@@Z',bytes=81,native_bytes=81),
 dict(file='CHandleBasedMemoryPool_CombineAreas_00a01e40.cpp',address='00a01e40',symbol='?CombineAreas@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z',bytes=141,native_bytes=141)]
EXTERNAL={'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PoolFree_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/pool-free.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='pool-free-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
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
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['native_bytes']);candidate[addr]=patched;native[addr]=raw;entry=dict(s,relocations=sorted([pos,list(v)] for pos,v in rel.items()))
   if s.get('near'):
    score=similarity(bytes(patched),raw,tuple(rel),retail_relocations(s['address'],len(raw)));assert score.near_match and score.built_instructions==score.retail_instructions and score.opcode_similarity>=0.98,score
    entry.update(grade='NEAR_MATCH',similarity=score._asdict() if hasattr(score,'_asdict') else score.to_dict())
   else:
    assert patched==raw,s['file'];entry.update(grade='RELOCATION_MATCH' if rel else 'MATCH')
   entries.append(entry)
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');c=Coff(test);assert all(s['symbol'] not in c.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'POOL_FREE_BEHAVIOR_PASS scenarios=2592' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),boundaries=['EnterCriticalSection','LeaveCriticalSection'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('POOL_FREE_PASS compiled=2592 comparative=%d providers=%d'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
