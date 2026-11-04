from pathlib import Path
import sys,json,struct,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
def audit(obj):
 c=Coff(obj);symbol='?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z';section,offset=c.symbols[symbol];body,rels=c.sections[section];assert not offset and len(body)==640 and len(rels)==51
 address={'??0CWideString@@QAE@XZ':0x99aed0,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z':0x999140,'?GetLength@CWideString@@QBEJXZ':0x99b220,'?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,'??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,'??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,'??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,'??1CCharString@@QAE@XZ':0x99eae0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??BCCharString@@QBEPBDXZ':0x99e4c0,'__TI2?AVCBBBFileException@@':0x1369380,'__TI1?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84,'??BCWideString@@QBEPBGXZ':0x99af00,'?GFGetLastWindowsError@@YI?AVCCharString@@XZ':0xa5ed60,'?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,'?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z':0x999970,'?CreateDirectoryForFile@CAFile@@SIXABVCWideString@@@Z':0x999620,'??1CWideString@@QAE@XZ':0x99b510}
 literals={'??_C@_0O@GLFMJAB@Bad?5filename?5?$AA@':(0x129a80c,b'Bad filename \0'),'??_C@_04PACPBLLI@?$CJ?5?9?5?$AA@':(0x129a804,b') - \0'),'??_C@_0BA@NLBPNJP@?8?5?$CIaccess?5type?5?$AA@':(0x129a7f4,b"' (access type \0"),'??_C@_0BD@DFOLBPKK@Cannot?5open?5file?5?8?$AA@':(0x129a7e0,b"Cannot open file '\0")}
 for name,(target,text) in literals.items():
  sec,off=c.symbols[name];data,fixups=c.sections[sec];assert not fixups and data[off:off+len(text)]==text and read_virtual_data(EXE.read_bytes(),target,len(text))==text;address[name]=target
 imports={v:k for k,v in import_names(EXE.read_bytes()).items()};patched=bytearray(body);records=[]
 for site,(name,kind) in rels.items():
  if name.startswith('$L'):
   sec,off=c.symbols[name];assert sec==section;address[name]=0x99a6a0+off
  elif name.startswith('__imp__'):address[name]=imports[name[7:].split('@')[0]]
  target=address[name];value=target+struct.unpack_from('<i',body,site)[0]-(0x99a6a0+site+4 if kind==20 else 0);struct.pack_into('<I',patched,site,value&0xffffffff);records.append(dict(site=site,symbol=name,kind=kind,target=target))
 assert bytes(patched)==read_virtual_data(EXE.read_bytes(),0x99a6a0,640)
 # Verify the original throw descriptor actually names the claimed exception type.
 raw=EXE.read_bytes();throw=struct.unpack('<4I',read_virtual_data(raw,0x1369380,16));array=throw[3];count=struct.unpack('<I',read_virtual_data(raw,array,4))[0];assert 1<=count<=4
 types=[]
 for i in range(count):
  entry=struct.unpack('<I',read_virtual_data(raw,array+4+i*4,4))[0];descriptor=struct.unpack('<I',read_virtual_data(raw,entry+4,4))[0]
  types.append(read_virtual_data(raw,descriptor+8,64).split(b'\0')[0].decode())
 assert '.?AVCBBBFileException@@' in types
 return bytes(patched),dict(grade='MATCH',bytes=640,code_bytes=624,jump_table_bytes=16,relocations=records,exception_types=types)
