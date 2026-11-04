"""Validate CParentDefClassBase's accessors, sub-def lookups, AddSubDef and InternalSaveBinary."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from parent_def_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol,size):return dict(file=file,address=address,symbol=symbol,native_bytes=size)
SPECS=[
 S('CParentDefClassBase_GetInstantiationName_004303b0.cpp','004303b0','?GetInstantiationName@CParentDefClassBase@@UBE?AVCDefString@@XZ',12),
 S('CParentDefClassBase_SetInstantiationName_004303c0.cpp','004303c0','?SetInstantiationName@CParentDefClassBase@@UAEXVCDefString@@@Z',10),
 S('CParentDefClassBase_IsParentDef_004303d0.cpp','004303d0','?IsParentDef@CParentDefClassBase@@UBE_NXZ',3),
 S('CParentDefClassBase_GetPSubDefInfoMap_004303e0.cpp','004303e0','?GetPSubDefInfoMap@CParentDefClassBase@@UAEPAXXZ',4),
 S('CParentDefClassBase_GetPSubDefInfoConst_009ad4a0.cpp','009ad4a0','?GetPSubDefInfo@CParentDefClassBase@@UBEPBVCSubDefInfo@CDefClassBase@@ABVCCharString@@@Z',98),
 S('CParentDefClassBase_GetPSubDefInfo_009ad510.cpp','009ad510','?GetPSubDefInfo@CParentDefClassBase@@UAEPAVCSubDefInfo@CDefClassBase@@ABVCCharString@@@Z',98),
 S('CParentDefClassBase_AddSubDef_009b00d0.cpp','009b00d0','?AddSubDef@CParentDefClassBase@@UAEXABVCCharString@@JJ@Z',155),
 S('CParentDefClassBase_InternalSaveBinary_009b0200.cpp','009b0200','?InternalSaveBinary@CParentDefClassBase@@UAEXAAVCDataOutputStream@@@Z',209)]
PAIR='U?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@';MAP='?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@'
EXTERNAL={'?FableCRC_Calc_004014A0@@YIIIPBXI@Z':0x4014a0,'?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,
 '?LowerBound@'+MAP+'QBEPBU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@ABK@Z':0x9b2130,
 '?LowerBound@'+MAP+'QAEPAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@ABK@Z':0x9b20d0,
 '?_M_insert_overflow@?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$allocator@U?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@@2@@_STL@@QAEXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@2@ABU32@ABU__false_type@2@I_N@Z':0x9b60d0,
 '??$sort@PAU?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@_STL@@YIXPAU?$pair@KVCSubDefInfo@CDefClassBase@@@0@0V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@Z':0x9b84c0,
 '?WriteUBYTE@CDataOutputStream@@QAEXE@Z':0x993e50,'?WriteUWORD@CDataOutputStream@@QAEXG@Z':0x993e80,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,'?WriteSLONG@CDataOutputStream@@QAEXJ@Z':0x993eb0}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ParentDef_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/parent-def.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='parent-def-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(SPECS,coffs):
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
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'PARENT_DEF_BEHAVIOR_PASS scenarios=31' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=['FableCRC_Calc','CVectorMap::LowerBound (both)','vector::_M_insert_overflow','_STL::sort','CDataOutputStream writes'],
  deferred=['CParentDefClassBase::RemoveSubDef 0x009B0170 (0.836: retail loads end() before the UpperBound call)'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PARENT_DEF_PASS compiled=31 comparative=%d providers=%d'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
