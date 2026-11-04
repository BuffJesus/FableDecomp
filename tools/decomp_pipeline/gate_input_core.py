"""Validate the input core: CInputManager members, device defaults, vtables, oracle and fixture."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from input_core_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol):return dict(file=file,address=address,symbol=symbol)
M='@CInputManager@@'
SPECS=[
 S('CInputManager_EventScanStart_009f4ed0.cpp','009f4ed0','?EventScanStart'+M+'QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z'),
 S('CInputManager_EventScanNext_009f4f10.cpp','009f4f10','?EventScanNext'+M+'QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z'),
 S('CInputManager_EventScanEnd_009f4ac0.cpp','009f4ac0','?EventScanEnd'+M+'QBEXAAVCInputEventScanInfo@@@Z'),
 S('CInputManager_UpdateChecksum_009f4a90.cpp','009f4a90','?UpdateChecksum'+M+'QAEXJABVCChecksum@@@Z'),
 S('CInputManager_SetHasInputFocus_009f4e20.cpp','009f4e20','?SetHasInputFocus'+M+'QAEX_N@Z'),
 S('CInputManager_SetExclusiveMode_009f4e70.cpp','009f4e70','?SetExclusiveMode'+M+'UAE_N_N@Z'),
 S('CInputManager_IsJoystickDeviceNumberActive_009f5250.cpp','009f5250','?IsJoystickDeviceNumberActive'+M+'QBE_NJ@Z'),
 S('CInputManager_ClearEvents_009f5540.cpp','009f5540','?ClearEvents'+M+'QAEXXZ'),
 S('CInputManager_StreamInputsToFile_009f4cf0.cpp','009f4cf0','?StreamInputsToFile'+M+'QAEXABVCVersion@@ABVCWideString@@@Z'),
 S('CInputManager_StreamInputsFromFile_009f5e10.cpp','009f5e10','?StreamInputsFromFile'+M+'QAEXABVCVersion@@ABVCWideString@@@Z'),
 S('STLInputEventVector_erase_009f67e0.cpp','009f67e0','?erase@?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@_STL@@@_STL@@QAEPAVCInputEvent@@PAV3@0@Z'),
 S('CMouse_SetHasInputFocus_00a673a0.cpp','00a673a0','?SetHasInputFocus@CMouse@@UAEX_N@Z'),
 S('CKeyboard_SetHasInputFocus_00a66ac0.cpp','00a66ac0','?SetHasInputFocus@CKeyboard@@UAEX_N@Z'),
 S('CKeyboard_GetKeyName_00a66ad0.cpp','00a66ad0','?GetKeyName@CKeyboard@@UBE_NW4EInputKey@@AAVCWideString@@@Z'),
 S('CJoystick_SetExclusiveMode_009e41a0.cpp','009e41a0','?SetExclusiveMode@CJoystick@@UAE_N_N@Z'),
 S('CJoystick_Update_009e41b0.cpp','009e41b0','?Update@CJoystick@@UAEXXZ'),
 S('CJoystick_UpdateInsertionsAndRemovals_009e4930.cpp','009e4930','?UpdateInsertionsAndRemovals@CJoystick@@UAEXKK@Z'),
 S('CJoystick_Clear_009e4940.cpp','009e4940','?Clear@CJoystick@@UAEXXZ'),
 S('CJoystick_SetHasInputFocus_009e4950.cpp','009e4950','?SetHasInputFocus@CJoystick@@UAEX_N@Z'),
 S('CJoystick_EventScanStart_009e4230.cpp','009e4230','?EventScanStart@CJoystick@@MBE_NPAVCInputEventScanInfo@@PAVCInputEvent@@@Z'),
 S('CJoystick_EventScanNext_009e4280.cpp','009e4280','?EventScanNext@CJoystick@@MBE_NPAVCInputEventScanInfo@@PAVCInputEvent@@@Z'),
 S('CJoystick_EventScanEnd_009e41c0.cpp','009e41c0','?EventScanEnd@CJoystick@@MBEXPAVCInputEventScanInfo@@@Z'),
 S('CJoystick_ClearAllMaintainedEvents_009e42c0.cpp','009e42c0','?ClearAllMaintainedEvents@CJoystick@@QAEXXZ'),
 S('CMouse_ClearAllMaintainedEvents_00a66c00.cpp','00a66c00','?ClearAllMaintainedEvents@CMouse@@QAEXXZ'),
 S('CKeyboard_ClearAllMaintainedEvents_00a66860.cpp','00a66860','?ClearAllMaintainedEvents@CKeyboard@@QAEXXZ')]
EXTERNAL={'?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ':0xa03b40,'??1CInputEvent@@QAE@XZ':0xa03b30,
 '?clear@?$list@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@_STL@@@_STL@@QAEXXZ':0x9e4710,
 '?LoadAllEventPackages'+M+'IAEXABVCVersion@@AAVCAFile@@@Z':0x9f5d70,'??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z':0x99ad80,
 '??_7CDiskFileWin32@@6B@':0x122d06c,'?Close@CDiskFileWin32@@UAEXXZ':0x99a920,'??1CWideString@@QAE@XZ':0x99b510,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300}
VTABLES={'??_7CMouse@@6B@':(0x129dbc4,18,'??_ECMouse@@UAEPAXI@Z',0xa673b0),'??_7CKeyboard@@6B@':(0x129dba8,5,'??_ECKeyboard@@UAEPAXI@Z',0xa66ae0),
 '??_7CJoystick@@6B@':(0x129c0ec,19,'??_ECJoystick@@UAEPAXI@Z',0x9e4980)}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/InputCore_test.cpp');ap.add_argument('--abi-test',type=Path,default=ROOT/'rebuild/tests/integration/DeviceVtables_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/input-core.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='input-core-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  for s,c in zip(SPECS,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0,s['file'];patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw,s['file'];candidate[addr]=patched;native[addr]=raw
   entries.append(dict(s,bytes=len(b),relocations=sorted([pos,list(v)] for pos,v in rel.items()),grade='RELOCATION_MATCH' if rel else 'MATCH'))
  abi=Coff(compile(a.abi_test,'abi.obj'));slots={}
  for vt,(retail,n,dtor,dtor_addr) in VTABLES.items():
   sec,off=abi.symbols[vt];_,rel=abi.sections[sec];names=[]
   for k in range(n):
    name=rel[off+4*k][0];want=dtor_addr if name==dtor else 0xbfea08 if name=='__purecall' else targets[name]
    assert want==pe.get_dword_at_rva(retail-0x400000+4*k),(vt,k,name);names.append(name)
   assert off+4*n not in rel;slots[vt]=names
 comparisons=compare(candidate,native)
 test=compile(a.test,'test.obj');tc=Coff(test);assert all(s['symbol'] not in tc.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'INPUT_CORE_BEHAVIOR_PASS scenarios=560' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,vtable_slots=slots,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),abi_fixture_sha256=hashlib.sha256(a.abi_test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=['CInputEvent dtor/GetType','list<CMaintainedEvent>::clear','LoadAllEventPackages','CDiskFileWin32','CWideString'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('INPUT_CORE_PASS compiled=560 comparative=%d providers=%d vtables=3'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
