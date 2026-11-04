"""Validate the pair<unsigned long, CDefClassBase::CSubDefInfo> sort, heap and insertion helpers and CVectorMap::LowerBound."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from subdef_sort_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol,size):return dict(file=file,address=address,symbol=symbol,native_bytes=size)
SPECS=[
 S('CVectorMapSubDefInfo_LowerBound_009b20d0.cpp','009b20d0','?LowerBound@?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@QAEPAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@ABK@Z',82),
 S('CVectorMapSubDefInfo_LowerBoundConst_009b2130.cpp','009b2130','?LowerBound@?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@QBEPBU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@ABK@Z',82),
 S('STL_AdjustHeap_SubDefEntry_009b2f30.cpp','009b2f30','??$__adjust_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@HU12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@HHU10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',149),
 S('STL_InsertionSort_SubDefEntry_009b5670.cpp','009b5670','??$__insertion_sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',122),
 S('STL_IntrosortLoop_SubDefEntry_009b7dd0.cpp','009b7dd0','??$__introsort_loop@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@U12@HV?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@00HV?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',242),
 S('STL_MakeHeap_SubDefEntry_009b3c00.cpp','009b3c00','??$__make_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@U12@H@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@0PAH@Z',101),
 S('STL_PartialSort_SubDefEntry_009b6ad0.cpp','009b6ad0','??$__partial_sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@000V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',160),
 S('STL_PushHeap_SubDefEntry_009b1ca0.cpp','009b1ca0','??$__push_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@HU12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@HHU10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',106),
 S('STL_Sort_SubDefEntry_009b84c0.cpp','009b84c0','??$sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',82),
 S('STL_SortHeap_SubDefEntry_009b5af0.cpp','009b5af0','??$sort_heap@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',179),
 S('STL_UnguardedInsertionSortAux_SubDefEntry_009b2cb0.cpp','009b2cb0','??$__unguarded_insertion_sort_aux@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@00V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',103),
 S('STL_UnguardedPartition_SubDefEntry_009b3800.cpp','009b3800','??$__unguarded_partition@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@PAU10@0U10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',120)]
# Helpers whose bodies are visible in a caller's unit but stay calls, as in
# retail: that unit emits them (COMDAT), so they have no unit of their own.
EXTERNAL={}
COPY_BACKWARD='??$__copy_backward@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@PAU12@H@_STL@@YIPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z'
COMDAT=[dict(file='STL_InsertionSort_SubDefEntry_009b5670.cpp',address='009b18e0',symbol=COPY_BACKWARD,native_bytes=76),
 dict(file='STL_Sort_SubDefEntry_009b84c0.cpp',address='009b6830',symbol='??$__final_insertion_sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',native_bytes=80),
 dict(file='STL_UnguardedInsertionSortAux_SubDefEntry_009b2cb0.cpp',address='009b1a30',symbol='??$__unguarded_linear_insert@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@U12@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@U10@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z',native_bytes=67)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/SubDefSort_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/subdef-sort.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='subdef-sort-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS+COMDAT};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  owners=[coffs[[x['file'] for x in SPECS].index(s['file'])] for s in COMDAT]
  for s,c in zip(SPECS+COMDAT,coffs+owners):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['native_bytes'],(s['file'],len(b));patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
   entries.append(dict(s,bytes=len(b),relocations=sorted([pos,list(v)] for pos,v in rel.items()),grade='RELOCATION_MATCH' if rel else 'MATCH'))
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');tc=Coff(test);assert all(s['symbol'] not in tc.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'SUBDEF_SORT_BEHAVIOR_PASS scenarios=50' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=[],covered=[c['symbol']+' from '+c['file'] for c in COMDAT],deferred=['_STL::__copy 0x009B1620 (exact; reached only from the deferred CParentDefClassBase::RemoveSubDef)'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('SUBDEF_SORT_PASS compiled=50 comparative=%d providers=%d'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
