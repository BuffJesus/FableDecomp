#!/usr/bin/env python3
"""Reproduce complete retail OVI_DeadFather lifecycle snapshots."""
from __future__ import annotations
import argparse,hashlib,json
from pathlib import Path
import pefile
from capstone import CS_ARCH_X86,CS_MODE_32,Cs
REGIONS=(("OnPredicateFail",0x00DB8260,0x00DB8261),("Init",0x00DB8290,0x00DB82F4),("Main",0x00DB8300,0x00DB8516))
def export(exe:Path)->dict:
 raw=exe.read_bytes();pe=pefile.PE(data=raw,fast_load=True);dec=Cs(CS_ARCH_X86,CS_MODE_32);rows=[]
 for name,start,end in REGIONS:
  off=pe.get_offset_from_rva(start-pe.OPTIONAL_HEADER.ImageBase);code=raw[off:off+end-start];ins=list(dec.disasm(code,start))
  if not ins or ins[-1].address+ins[-1].size!=end or ins[-1].mnemonic!='ret':raise RuntimeError(f'{name} boundary is not exact')
  rows.append({'name':name,'address':f'0x{start:08X}','endExclusive':f'0x{end:08X}','rangeSize':len(code),'bytesSha256':hashlib.sha256(code).hexdigest().upper(),'instructionCount':len(ins),'terminal':'ret'})
 return {'schema':'dead-father-full-retail-bytes/0.1','source':'installed retail Fable.exe PE image','sourceSha256':hashlib.sha256(raw).hexdigest().upper(),'nativeClass':'COVI_DeadFather','regions':rows,'coverage':{'operations':'empty predicate override, complete Init and Main','boundaryCorrection':'translation_unit Main size 531 truncates the final epilogue; exact terminal ret is 0x00DB8515','lifecycle':['minimap marker','dad pose','DadFound wait','marker removal','control cleanup']}}
def main()->int:
 ap=argparse.ArgumentParser();ap.add_argument('--exe',type=Path,required=True);ap.add_argument('--output',type=Path);a=ap.parse_args();s=json.dumps(export(a.exe),indent=2)+'\n'
 if a.output:a.output.write_text(s,encoding='utf-8')
 else:print(s,end='')
 return 0
if __name__=='__main__':raise SystemExit(main())
