"""Original metadata/pointer-fill roots transplanted only into build-owned probes."""
from pathlib import Path
import json,re,struct,subprocess
import pefile,capstone
from pe_oracle import EXE,read_virtual_data

def mapping(build,stem='probe'):
 result={}
 for line in (build/(stem+'.map')).read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:result[m[1]]=int(m[2],16)
 return result

def transplant(raw,address,size,symbol,targets,names):
 pe=pefile.PE(data=raw);last=pe.sections[-1];align=lambda n,a:(n+a-1)//a*a;rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(read_virtual_data(EXE.read_bytes(),address,size));calls=[]
 for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(bytes(payload),address):
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
   target=int(ins.op_str,16);name=targets[target];site=ins.address-address+1;struct.pack_into('<I',payload,site,(names[name]-base-site-4)&0xffffffff);calls.append(dict(site=hex(ins.address),target=hex(target),symbol=name))
 header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);file_size=align(size,pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+file_size-len(oracle)));oracle[start:start+size]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,b'.oracle\0',size,rva,file_size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+size,pe.OPTIONAL_HEADER.SectionAlignment));at=names[symbol];offset=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[offset:offset+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);return bytes(oracle),calls

def metadata(build,expected):
 names=mapping(build);lookup=lambda prefix:next(n for n in names if n.startswith(prefix));targets={0x9caeb0:lookup('?resize@?$vector@PAVCDataBankEntry'),0x9cb1f0:lookup('?reserve@?$vector@VCDataBankTextEntry'),0x9cb2a0:lookup('?reserve@?$vector@VCDataBankGroupEntry'),0x9ca950:lookup('?_M_insert_overflow@?$vector@PAVCDataBankEntry'),0x9cb740:lookup('?push_back@?$vector@VCDataBankTextEntry'),0x9cb790:lookup('?push_back@?$vector@VCDataBankGroupEntry'),0x994930:'??0CMemoryDataInputStream@@QAE@PBXK@Z',0x993ca0:'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0x49b760:'?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEXI@Z',0x995d30:'?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ',0x99efb0:'??4CCharString@@QAEAAV0@ABV0@@Z',0x99eae0:'??1CCharString@@QAE@XZ',0x993b10:'??1CMemoryDataInputStream@@UAE@XZ',0x9cbc20:'?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z'};roots=[(0x9c82c0,122,lookup('?NotifyBankSize@CDataBank@')),(0x9c8c00,469,lookup('?NotifyEntryAdded@CDataBank@'))];proof=[]
 for mask in range(4):
  raw=(build/'probe.exe').read_bytes();calls=[]
  for index,(address,size,symbol) in enumerate(roots):
   if mask&(1<<index):raw,part=transplant(raw,address,size,symbol,targets,names);calls+=part
  exe=build/('original-metadata-%d.exe'%mask);exe.write_bytes(raw);run=subprocess.run([str(exe)],cwd=build,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==expected,(mask,run.returncode,run.stdout,run.stderr);proof.append(dict(mask=mask,calls=calls,code=run.returncode))
 (build/'original-metadata-proof.json').write_text(json.dumps(proof,indent=2));return proof

def pointer_fill(build,expected):
 names=mapping(build,'pointer');lookup=lambda prefix:next(n for n in names if n.startswith(prefix));symbol=lookup('?_M_fill_insert@?$vector@PAVCDataBankEntry');fill=lookup('??$fill@PAPAVCDataBankEntry');overflow=lookup('?_M_insert_overflow@?$vector@PAVCDataBankEntry');raw,calls=transplant((build/'pointer.exe').read_bytes(),0x9caa30,276,symbol,{0xbfeae6:'_memmove',0x9c9c60:fill,0x9ca950:overflow},names);exe=build/'original-pointer.exe';exe.write_bytes(raw);run=subprocess.run([str(exe)],cwd=build,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==expected,(run.returncode,run.stdout,run.stderr);return dict(code=run.returncode,calls=calls)
