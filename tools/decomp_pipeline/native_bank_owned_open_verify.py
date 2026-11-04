from pathlib import Path
import sys,json,subprocess,struct
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_exact_roots import import_names
import disk_open_original_evidence as diskproof
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from native_retail_readonly_verify import verify
def run(b):
 r=json.loads((b/'receipt.json').read_text());objects=[Path(p) for p in r['object_sha256']]
 proof=verify(b,objects,{p.name for p in objects if p.name!='Fixture.obj'},b/'Fixture.cpp')
 # Independently resolve every relocation in the selected retail roots.
 def obj(source):return Path(next(o for p,o in r['source_objects'].items() if Path(p).name==source))
 targets={'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CDiskFileWin32@@6B@':0x122d06c,'??0CWideString@@QAE@XZ':0x99aed0,'?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z':0x9d1ee0,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'?ReadFileInformation@CBankFile@@AAE_N_N@Z':0x9d0880,'?CloseBankFileImpl@@YIXPAVCBankFile@@@Z':0x9cd850,'?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,'?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z':0x9a9030,'?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ':0x997f80,'?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c90,'??1CWideString@@QAE@XZ':0x99b510,'??BCWideString@@QBEPBGXZ':0x99af00,'??BCCharString@@QBEPBDXZ':0x99e4c0,'??1CCharString@@QAE@XZ':0x99eae0}
 specs=[('CAFile_ConvertPathToFullPath_00999140.cpp','?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z',0x999140,64),('CBankFile_OpenFileReadOnly_009d1090.cpp','?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z',0x9d1090,204),('CCountedPointer_DiskFile_Reset_009d1ee0.cpp','?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z',0x9d1ee0,103),('CCountedPointer_DiskFile_Reset_009d1ee0.cpp','?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z',0x9a9030,11),('CBankFile_ReadData_009cbf60.cpp','?ReadData@CBankFile@@IBEXKPAXK@Z',0x9cbf60,40),('CAFile_PathExists_00999230.cpp','?PathExists@CAFile@@SI_NABVCWideString@@@Z',0x999230,111),('CBankFile_ReadFileInformation_009d0880.cpp','??0CBankFileException@@QAE@VCCharString@@W4EBankFileExceptionType@0@@Z',0x9d1e70,33)]
 raw=EXE.read_bytes();imports={v:k for k,v in import_names(raw).items()};exact=[]
 for source,symbol,entry,size in specs:
  c=Coff(obj(source));sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert not off and len(body)==size
  fixed=bytearray(body)
  for site,(name,kind) in rels.items():
   target=imports[name[7:].split('@')[0]] if name.startswith('__imp__') else targets[name]
   assert kind in (6,20);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==read_virtual_data(raw,entry,size),symbol
  exact.append(dict(symbol=symbol,address=hex(entry),bytes=size,relocations=len(rels)))
 _,disk=diskproof.audit(obj('CDiskFileWin32_Open_0099a6a0.cpp'))
 command=[sys.executable,str(R/'tools/decomp_pipeline/native_bank_header_literals.py'),str(b),str(obj('CBankFile_ReadFileInformation_009d0880.cpp'))]
 result=subprocess.run(command,cwd=R,capture_output=True,text=True,timeout=60)
 (b/'literal-proof.run.json').write_text(json.dumps(dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2));assert result.returncode==0,(result.stdout,result.stderr)
 literal=json.loads((b/'literal-correspondence.json').read_text());assert literal['literal_verified_grade']['near_match']
 (b/'retail-body-proof.json').write_text(json.dumps(dict(status='PASS',exact=exact,disk=disk,header=literal),indent=2))
 controls=[('skip_header','CBankFile_OpenFileReadOnly_009d1090.cpp','if(!ReadFileInformation(!(flags&2)))','if(false)'),('skip_opened','CBankFile_OpenFileReadOnly_009d1090.cpp',' NotifyBankOpened();',' ;'),('skip_filename','CBankFile_OpenFileReadOnly_009d1090.cpp',' Filename=filename;',' ;'),('skip_release','CCountedPointer_DiskFile_Reset_009d1ee0.cpp','--Info->RefCount;',';'),('skip_failed_cleanup','CBankFile_OpenFileReadOnly_009d1090.cpp','  CloseBankFileImpl(this);','  ;'),('missing_returns_true','CBankFile_OpenFileReadOnly_009d1090.cpp','if(!CAFile::PathExists(filename))return false;','if(!CAFile::PathExists(filename))return true;'),('skip_os_close','CDiskFileWin32_Close_0099A920.cpp','        CloseHandle(WinFileHandle);','        ;')]
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(b/'include')
 # Snapshot uses its own copied include directory.
 inc=next(p for p in b.iterdir() if p.is_dir() and (p/'fable_game_text_bank.h').exists());env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 records=[]
 for name,source,old,new in controls:
  obj=next(Path(obj) for p,obj in r['source_objects'].items() if Path(p).name==source)
  text=obj.with_suffix('.cpp').read_text();assert text.count(old)==1
  cpp=b/(name+'.cpp');cpp.write_text(text.replace(old,new));fault=b/(name+'.obj')
  result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/Dmalloc=FixtureVectorMalloc','/Dfree=FixtureVectorFree','/I'+str(inc),'/Fo'+str(fault),str(cpp)],env,b)
  assert not result.returncode,result.stdout+result.stderr
  exe=b/(name+'.exe');command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*[str(fault if p==obj else p) for p in objects],'kernel32.lib']
  result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);assert not result.returncode,result.stdout+result.stderr
  result=subprocess.run([str(exe),str(b/'cases.bin'),str(b/'synthetic-bank.bin'),*(['failures'] if name in ('skip_failed_cleanup','missing_returns_true','skip_os_close') else [])],cwd=b,capture_output=True,text=True,timeout=30)
  record=dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr);records.append(record);print(record,flush=True)
  assert result.returncode==80,record
 (b/'ownership-controls.json').write_text(json.dumps(dict(status='PASS',controls=records),indent=2))
 return dict(status='PASS',linked_symbols=proof['linked_symbols'],unique_bodies=proof['unique_bodies'],exact_roots=len(exact)+1,controls=records,header_grade=literal['literal_verified_grade'])
