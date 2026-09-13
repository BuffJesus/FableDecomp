#!/usr/bin/env python3
"""Reproduce complete retail NOVI_AffairWoman Init/Main byte snapshots."""
from __future__ import annotations
import argparse, hashlib, json
from pathlib import Path
import pefile
from capstone import CS_ARCH_X86, CS_MODE_32, Cs

REGIONS=(("Init",0x00DB1E80,0x00DB1EF4),("Main",0x00DB1F00,0x00DB299A))
def export(exe: Path)->dict:
 raw=exe.read_bytes(); pe=pefile.PE(data=raw,fast_load=True); dec=Cs(CS_ARCH_X86,CS_MODE_32); rows=[]
 for name,start,end in REGIONS:
  off=pe.get_offset_from_rva(start-pe.OPTIONAL_HEADER.ImageBase); code=raw[off:off+end-start]; ins=list(dec.disasm(code,start))
  if not ins or ins[-1].address+ins[-1].size!=end or ins[-1].mnemonic!='ret': raise RuntimeError(f'{name} boundary is not an exact terminal return')
  rows.append({'name':name,'address':f'0x{start:08X}','endExclusive':f'0x{end:08X}','rangeSize':len(code),
               'bytesSha256':hashlib.sha256(code).hexdigest().upper(),'instructionCount':len(ins),'terminal':'ret'})
 return {'schema':'affair-woman-full-retail-bytes/0.1','source':'installed retail Fable.exe PE image',
         'sourceSha256':hashlib.sha256(raw).hexdigest().upper(),'nativeClass':'CNOVI_AffairWoman','regions':rows,
         'coverage':{'operations':'Init and complete Main dispatcher','liveRelationships':['NOVI_AffairMan','NOVI_AffairWife'],
                     'runOffMarker':'AffairWomanRunOffPoint','terminalCleanup':'0x00DB296B-0x00DB2999'}}
def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('--exe',type=Path,required=True);ap.add_argument('--output',type=Path);a=ap.parse_args();s=json.dumps(export(a.exe),indent=2)+'\n'
 if a.output:a.output.write_text(s,encoding='utf-8')
 else:print(s,end='')
 return 0
if __name__=='__main__':raise SystemExit(main())
