"""Validate CDefClassBase's own members (defaults, InternalSaveBinary, UnloadResource) and its 25-slot vtable."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from def_class_base_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol,size):return dict(file=file,address=address,symbol=symbol,native_bytes=size)
SPECS=[
 S('CDefClassBase_GetInstantiationName_0042da10.cpp','0042da10','?GetInstantiationName@CDefClassBase@@UBE?AVCDefString@@XZ',13),
 S('CDefClassBase_SetInstantiationName_0042da20.cpp','0042da20','?SetInstantiationName@CDefClassBase@@UAEXVCDefString@@@Z',3),
 S('CDefClassBase_AddSubDef_0042da30.cpp','0042da30','?AddSubDef@CDefClassBase@@UAEXABVCCharString@@JJ@Z',3),
 S('CDefClassBase_RemoveSubDef_0042da40.cpp','0042da40','?RemoveSubDef@CDefClassBase@@UAEXABVCCharString@@@Z',3),
 S('CDefClassBase_GetPSubDefInfo_0042da50.cpp','0042da50','?GetPSubDefInfo@CDefClassBase@@UAEPAVCSubDefInfo@1@ABVCCharString@@@Z',5),
 S('CDefClassBase_GetPSubDefInfoConst_0042da60.cpp','0042da60','?GetPSubDefInfo@CDefClassBase@@UBEPBVCSubDefInfo@1@ABVCCharString@@@Z',5),
 S('CDefClassBase_IsParentDef_0042da70.cpp','0042da70','?IsParentDef@CDefClassBase@@UBE_NXZ',3),
 S('CDefClassBase_PostTransfer_0042da80.cpp','0042da80','?PostTransfer@CDefClassBase@@UAEXXZ',1),
 S('CDefClassBase_Validate_0042da90.cpp','0042da90','?Validate@CDefClassBase@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z',3),
 S('CDefClassBase_GetPSubDefInfoMap_0042daa0.cpp','0042daa0','?GetPSubDefInfoMap@CDefClassBase@@UAEPAXXZ',3),
 S('CDefClassBase_InternalSaveBinary_009acab0.cpp','009acab0','?InternalSaveBinary@CDefClassBase@@UAEXAAVCDataOutputStream@@@Z',0x4a),
 S('CDefClassBase_UnloadResource_009ace90.cpp','009ace90','?UnloadResource@CDefClassBase@@UAE_NXZ',0x44)]
EXTERNAL={'?WriteUBYTE@CDataOutputStream@@QAEXE@Z':0x993e50}
# Retail 0x01230BCC slot owners that are not providers of this batch.
VTABLE_EXTRA={'??_ECDefClassBase@@UAEPAXI@Z':0x42dab0,'?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ':0x42d930,
 '?GetUnloadableMemory@CResource@@UBEKXZ':0x9fbef0,'?IsResourceLoaded@CResource@@UBE_NXZ':0x42d950,'?OnResourceLoaded@CResource@@UAEXXZ':0x42d960,
 '?CanAbortResourceLoad@CResource@@UBE_NJ@Z':0x42d970,'?OnResourceLoadAborted@CResource@@UAEXXZ':0x42d980,'?GetDebugMemoryInUse@CResource@@UBEKXZ':0x9fbf00,
 '?ResetDebugMemoryInUse@CResource@@UAEXXZ':0x9fbf10,'__purecall':0xbfea08,'?InternalLoadBinary@CDefClassBase@@UAEXAAVCDataInputStream@@@Z':0x9acb20}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/DefClassBase_test.cpp');ap.add_argument('--abi-test',type=Path,default=ROOT/'rebuild/tests/integration/DefClassBaseVtable_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/def-class-base.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='def-class-base-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name,opt='/O2'):
  obj=w/name;r=check_one.cl(['/nologo','/c',opt,'/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
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
  abi=Coff(compile(a.abi_test,'abi.obj','/Od'));sec,off=abi.symbols['??_7CDefClassBase@@6B@'];_,rel=abi.sections[sec];slots=[]
  for k in range(25):
   name=rel[off+4*k][0];want=targets.get(name,VTABLE_EXTRA.get(name));assert want==pe.get_dword_at_rva(0x1230bcc-0x400000+4*k),(k,name);slots.append(name)
  assert off+100 not in rel
  # Oracle: the defaults are one contiguous block in retail.
  block=bytearray(pe.get_data(0x42da10-0x400000,0x95));nblock=bytes(block)
  for s in SPECS[:10]:o=int(s['address'],16)-0x42da10;block[o:o+len(candidate[int(s['address'],16)])]=candidate[int(s['address'],16)]
 comparisons=compare({0x42da10:block,0x9acab0:candidate[0x9acab0],0x9ace90:candidate[0x9ace90]},{0x42da10:nblock,0x9acab0:native[0x9acab0],0x9ace90:native[0x9ace90]})
 test=compile(a.test,'test.obj');tc=Coff(test);assert all(s['symbol'] not in tc.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'DEF_CLASS_BASE_BEHAVIOR_PASS scenarios=136' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,vtable_slots=slots,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),abi_fixture_sha256=hashlib.sha256(a.abi_test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=['CDataOutputStream::WriteUBYTE','CIVCountedPointeeBase::DeleteCIVCountedPointeeBase','CResource defaults','CDefClassBase::InternalLoadBinary'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('DEF_CLASS_BASE_PASS compiled=136 comparative=%d providers=%d slots=25'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
