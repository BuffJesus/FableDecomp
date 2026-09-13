#!/usr/bin/env python3
"""Reproduce complete retail Q_NewOakValeIntro quest-level byte snapshots."""
from __future__ import annotations
import argparse,hashlib,json
from pathlib import Path
import pefile
from capstone import CS_ARCH_X86,CS_MODE_32,Cs
REGIONS=(("RegisterMain",0x00DAACE0,0x00DAAD6B,"ret"),("OnPersist",0x00DAADA0,0x00DAADC1,"ret"),("Init",0x00DAADD0,0x00DABABA,"ret"),("Main",0x00DABAC0,0x00DAC2B4,"ret"),("AddBadDeed",0x00DAEA70,0x00DAEBEF,"ret"),("AddGoodDeed",0x00DB0660,0x00DB0880,"ret"),("StartBarrelTimer",0x00DB4F70,0x00DB51A2,"ret"),("DoMission",0x00DBDE40,0x00DBE2D4,"ret"),("WatchForGotGold",0x00DBE2E0,0x00DBE3B4,"ret"),("AttackStuff",0x00DBE3C0,0x00DBE4DD,"ret"),("ManageQuestCoreMarkers",0x00DBE4E0,0x00DBE890,"ret"),("WatchBarrels",0x00DBE890,0x00DBEB17,"ret"),("PostAttackStuff",0x00DBEB20,0x00DBEF67,"ret"),("DeletingDestructor",0x00DBEFA0,0x00DBEFBE,"ret"),("DestructorImplementation",0x00DBEFC0,0x00DBF182,"jmp"))
def export(exe:Path)->dict:
 raw=exe.read_bytes();pe=pefile.PE(data=raw,fast_load=True);dec=Cs(CS_ARCH_X86,CS_MODE_32);rows=[]
 for name,start,end,terminal in REGIONS:
  off=pe.get_offset_from_rva(start-pe.OPTIONAL_HEADER.ImageBase);code=raw[off:off+end-start];ins=list(dec.disasm(code,start))
  if not ins or ins[-1].address+ins[-1].size!=end or ins[-1].mnemonic!=terminal:raise RuntimeError(f'{name} boundary is not exact')
  rows.append({'name':name,'address':f'0x{start:08X}','endExclusive':f'0x{end:08X}','rangeSize':len(code),'bytesSha256':hashlib.sha256(code).hexdigest().upper(),'instructionCount':len(ins),'terminal':terminal})
 return {'schema':'new-oakvale-quest-full-retail-bytes/0.1','source':'installed retail Fable.exe PE image','sourceSha256':hashlib.sha256(raw).hexdigest().upper(),'nativeClass':'CQ_NewOakValeIntroScript','regions':rows,'coverage':{'operations':'complete quest lifecycle and every named quest helper','mainBoundaryCorrection':'translation_unit Main size 2018 stops at the first return; complete cleanup epilogue ends at 0x00DAC2B3','destructorTailJump':'0x00DBF17D -> CScriptBase destructor 0x00CBD510'}}
def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('--exe',type=Path,required=True);ap.add_argument('--output',type=Path);a=ap.parse_args();s=json.dumps(export(a.exe),indent=2)+'\n'
 if a.output:a.output.write_text(s,encoding='utf-8')
 else:print(s,end='')
 return 0
if __name__=='__main__':raise SystemExit(main())
