"""Original/recreated header differential models; helpers remain explicit boundaries."""
from pathlib import Path
import sys,json,struct,itertools,hashlib,pefile
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import NativeBankHeader_model as NORMAL
import NativeBankHeaderThrow_model as THROW
import NativeBankHeaderMutation_model as MUTATION
ENTRY=0x9d0880;SYMBOL='?ReadFileInformation@CBankFile@@AAE_N_N@Z'
def patched_body(obj):
 c=Coff(Path(obj));sec,off=c.symbols[SYMBOL];assert off==0;body,rels=c.sections[sec];image=EXE.read_bytes()
 addresses={'?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,'??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z':0x994700,'?SetPosition@CDataInputStream@@UAEXK@Z':0x993bc0,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'??1CFileDataInputStream@@UAE@XZ':0x994780,'??1CCharString@@QAE@XZ':0x99eae0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z':0x9cfbc0,'?ReadRecoveryFile@CBankFile@@AAE_NK@Z':0x9d0380,'@__security_check_cookie@4':0xbfe9f9,'___security_cookie':0x139c8a8,'__CxxThrowException@8':0xbfeb84,'__TI2?AVCBankFileException@@':0x1369470,'??0CBankFileException@@QAE@VCCharString@@W4TypeCode@0@@Z':0x9d1e70,'??BCCharString@@QBEPBDXZ':0x99e4c0,'?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,'??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,'??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600}
 addresses['??0CBankFileException@@QAE@VCCharString@@W4EBankFileExceptionType@0@@Z']=0x9d1e70
 pe=pefile.PE(data=image);patched=bytearray(body)
 for site,(name,kind) in rels.items():
  if name.startswith('??_C@'):
   s,o=c.symbols[name];literal=c.sections[s][0][o:];pos=image.find(literal);assert pos>=0,(name,literal);addresses[name]={b')\0':0x122e020,b' not \0':0x125c2b8,b'BBBB\0':0x129b5a4}.get(literal,pe.OPTIONAL_HEADER.ImageBase+pe.get_rva_from_offset(pos))
  target=addresses[name];struct.pack_into('<I',patched,site,(target+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 return bytes(patched)
def run(obj,build):
 b=Path(build);built=patched_body(obj);original=read_virtual_data(EXE.read_bytes(),ENTRY,1635);results={}
 normal=(base+scenario for base in itertools.product((0,1),(1,2,0xffffffff),(16,64),(0,4,32),(0,1,255),(0,1)) for scenario in ((True,7,19,32),(False,7,19,32),(True,7,19,0),(True,0,19,32),(True,8,19,32),(True,7,0,32),(True,7,20,32)))
 throws=((1,state,64,chunk,1,1,valid,version,format,size,flag) for chunk,flag in itertools.product((0,4,32),(1,255)) for valid,version,format,size,state in ((False,7,19,32,1),(True,7,19,0,1),(True,8,19,32,1),(True,7,20,32,1),(True,7,19,32,2)))
 mutations=(base+(True,7,19,32,mutation) for base in itertools.product((0,1),(1,2,0xffffffff),(16,64),(0,4,32),(0,1,255),(0,1)) for mutation in (1,2,3))
 for name,model,cases,count in [('normal',NORMAL,normal,1512),('throw',THROW,throws,30),('mutation',MUTATION,mutations,648)]:
  outputs=[]
  try:
   for args in cases:
    model.code=original;expected=model.execute(args);model.code=built;actual=model.execute(args);assert expected==actual,(name,args,expected,actual);outputs.append(actual)
  finally:model.code=original
  assert len(outputs)==count;(b/(name+'-differential.json')).write_text(json.dumps(dict(status='PASS',cases=count,results=outputs),indent=2));results[name]=count;print('BANK_HEADER_DIFFERENTIAL_PASS',name,count,flush=True)
 return dict(status='PASS',cases=results,boundary='1512 full-bank/ABI nonthrow,30 controlled throw boundary and648 helper-mutation cases; no native exception-unwinding claim.')
