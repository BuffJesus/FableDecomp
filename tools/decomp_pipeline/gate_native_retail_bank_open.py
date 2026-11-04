"""Genuine C++ retail BIGB open: exact root, bounded recorder behavior and faults.

Dependencies are controlled in this gate; it does not claim a full retail-bank
runtime closure. Template helper identities follow the recovered retail-bank
header/direct-callee ledger, rather than bootstrap owner-address aliases.
"""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,re,struct,subprocess
import shutil
import check_one as V
import native_exact_roots
from gate_game_component_abi import Coff
import pefile
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp'
FIXTURE=R/'rebuild/tests/00/9a/CBankFileManager_OpenRetailBank_009a8840_test.cpp'
BYTE=R/'rebuild/src/compiled/00/41/STLByteVector_SizeConstructor_00411910.cpp'
DELETE=R/'rebuild/src/compiled/00/9a/CCountedPointer_DiskFile_RetailDeleteData_009a9030.cpp'
BYTE_FIXTURE=R/'rebuild/tests/integration/NativeByteVectorCtor_test.cpp'
SYMBOL='?OpenRetailBank@CBankFileManager@@QAEXABVCWideString@@_N@Z'
FLAGS=['/nologo','/c','/O2','/MT','/Gy']
ADDRESS={'?PathExists@CAFile@@SI_NABVCWideString@@@Z': 10064432, '??2@YAPAXI@Z': 12577306, '??0CBaseClassNonCopyable@@QAE@XZ': 10068720, '??_7CDiskFileWin32@@6B@': 19058796, '??0CWideString@@QAE@XZ': 10071760, '??0?$CCountedPointer@VCDiskFileWin32@@@@QAE@PAVCDiskFileWin32@@@Z': 10132320, '??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z': 10045184, '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z': 10042528, '??_C@_04EKGIKCLO@BIGB?$AA@': 19509372, '??1CFileDataInputStream@@UAE@XZ': 10045312, '??1?$CCountedPointer@VCDiskFileWin32@@@@QAE@XZ': 10132400, '??0CRetailBank@NBankFileManager@@QAE@XZ': 10142928, '??0?$CCountedPointer@VCRetailBank@NBankFileManager@@@@QAE@PAVCRetailBank@NBankFileManager@@@Z': 10132752, '?push_back@CRetailBankList@CBankFileManager@@QAEXABV?$CCountedPointer@VCRetailBank@NBankFileManager@@@@@Z': 10136224, '?ToCharString@CWideString@@QBE?AVCCharString@@XZ': 10072768, '??4CCharString@@QAEAAV0@ABV0@@Z': 10088368, '??1CCharString@@QAE@XZ': 10087136, '??4?$CCountedPointer@VCDiskFileWin32@@@@QAEAAV0@ABV0@@Z': 10132464, '??0?$vector@EV?$allocator@E@_STL@@@_STL@@QAE@I@Z': 4266256, '?SetPosition@CDataInputStream@@UAEXK@Z': 10042304, '??0CMemoryDataInputStream@@QAE@PBXK@Z': 10045744, '?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ': 10050864, '??ACContainedBankMap@NBankFileManager@@QAEAAUCContainedBank@1@ABVCCharString@@@Z': 10143024, '??1CMemoryDataInputStream@@UAE@XZ': 10042128, '_free': 12577300, '??0CThreadedFile@@QAE@XZ': 10018768, '?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z': 10132608, '?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z': 10019296, '??3@YAXPAX@Z': 12577212}
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 b=R/'rebuild/build/native-retail-bank-open/00/9a/009a8840'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
 inputs={Path(__file__),Path(native_exact_roots.__file__)};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 snapshots={SOURCE:'root.cpp',FIXTURE:'Fixture.cpp',BYTE:'byte.cpp',DELETE:'delete.cpp',BYTE_FIXTURE:'ByteFixture.cpp'}
 for p in snapshots:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
 inc=b/'include';inc.mkdir()
 for p in visited:
  q=b/snapshots[p] if p in snapshots else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 def compile(path,extra=()):
  obj=path.with_suffix('.obj');run=V.cl(FLAGS+list(extra)+['/I'+str(inc),'/Fo'+str(obj),str(path)],env,b);(b/(path.stem+'.compile.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr;return obj
 root,fixture=compile(b/'root.cpp'),compile(b/'Fixture.cpp')
 def execute(label,obj,test_fixture=None):
  group=[obj] if isinstance(obj,Path) else obj
  exe=b/(label+'.exe');run=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),'/map:'+str(exe.with_suffix('.map')),*map(str,group),str(fixture if test_fixture is None else test_fixture)],cwd=b,env=env,capture_output=True,text=True);(b/(label+'.link.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));assert not run.returncode,run.stdout+run.stderr
  run=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);(b/(label+'.run.json')).write_text(json.dumps(dict(code=run.returncode,stdout=run.stdout,stderr=run.stderr)));return run
 run=execute('probe',root);assert not run.returncode and 'FABLETLC_OPEN_RETAIL_BANK PASS' in run.stdout,run.stdout+run.stderr
 controls=[]
 for i,(name,old,new) in enumerate([('alignment-value','contained.alignment_ = alignment;','contained.alignment_ = alignment+1;'),('entry-count-value','contained.entryCount_ = entryCount;','contained.entryCount_ = entryCount+1;'),('missing-threaded-open','bank->threadedFile_->Open(pathname, false);',';')]):
  text=(b/'root.cpp').read_text();assert text.count(old)==1;source=b/('control%d.cpp'%i);source.write_text(text.replace(old,new));obj=compile(source);run=execute('control%d'%i,obj);assert run.returncode==1 and 'FABLETLC_OPEN_RETAIL_BANK FAIL' in run.stdout,(name,run.returncode,run.stdout,run.stderr);controls.append(name)
 byte,delete,byte_fixture=compile(b/'byte.cpp',['/Dmalloc=FixtureVectorCtorMalloc']),compile(b/'delete.cpp'),compile(b/'ByteFixture.cpp')
 run=execute('byte-probe',[byte,delete],byte_fixture);assert not run.returncode and 'NATIVE_BYTE_VECTOR_CTOR_PASS cases=256 callback=1' in run.stdout,run.stdout+run.stderr
 byte_controls=[]
 for i,(name,old,new) in enumerate([('nonzero-fill','this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, _Tp(),','this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, _Tp(1),'),('wrong-capacity',': _Vector_base<_Tp, _Alloc>(__n, _Alloc())',': _Vector_base<_Tp, _Alloc>(__n+1, _Alloc())')]):
  private=b/('byte-control-include%d'%i);shutil.copytree(inc,private);header=private/'fable_stlport_vector_impl.h';text=header.read_text();assert text.count(old)==1;header.write_text(text.replace(old,new));source=b/('byte-control%d.cpp'%i);source.write_bytes((b/'byte.cpp').read_bytes());obj=compile(source,['/Dmalloc=FixtureVectorCtorMalloc','/I'+str(private)]);run=execute('byte-control%d'%i,[obj,delete],byte_fixture);assert run.returncode==80 and 'BYTE_VECTOR_CTOR_FAILURE' in run.stderr,(name,run.returncode,run.stdout,run.stderr);byte_controls.append(name)
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 c=Coff(root);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert not off;at=mapping[SYMBOL];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(at+site+4 if kind==20 else 0))&0xffffffff)
 with pefile.PE(str(b/'probe.exe')) as pe:assert pe.get_data(at-pe.OPTIONAL_HEADER.ImageBase,len(body))==bytes(fixed)
 roots=[dict(name='retail_bank_open',source=SOURCE,symbol=SYMBOL,address=0x9a8840,size=1565,instructions=528,relocations=44,flags=FLAGS)]
 roots += [dict(name='byte_vector_constructor',source=BYTE,symbol='??0?$vector@EV?$allocator@E@_STL@@@_STL@@QAE@I@Z',address=0x411910,size=77,instructions=36,relocations=1,flags=FLAGS),dict(name='retail_disk_deleter',source=DELETE,symbol='?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z',address=0x9a9030,size=11,instructions=6,relocations=0,flags=FLAGS)]
 address=dict(ADDRESS);address['_malloc']=0xbfea0e
 exact=native_exact_roots.run(__file__,roots,address,'native-retail-bank-open.json',['Six recorder scenarios;three root faults. 256 guarded byte-vector constructions;two header faults. Public disk deleter scalar-callback ABI.'], 'Three genuine C++ original-exact bodies. Disk/stream/string/map/threaded dependencies controlled;no runtime closure or new root count claimed.')
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 receipt=dict(status='PASS',grade='MATCH',root_bytes=1565,instructions=528,relocations=44,behavior_scenarios=6,controls=controls,byte_vector_cases=256,byte_vector_controls=byte_controls,scalar_callback_cases=1,input_sha256=pins,object_sha256={str(p):sha(p) for p in b.glob('*.obj')},exe_sha256=sha(b/'probe.exe'),byte_exe_sha256=sha(b/'byte-probe.exe'),final_root_verified=True,exact=exact,boundary='Controlled dependency recorders. No installed files or saves touched;historical root credit unchanged,public disk deleter is alternate ABI provider.')
 (b/'receipt.json').write_text(json.dumps(receipt,indent=2));print('NATIVE_RETAIL_BANK_OPEN_PASS bytes=1565 instructions=528 relocations=44 cases=6 controls=3 grade=MATCH providers=2 vector_cases=256 vector_controls=2')
if __name__=='__main__':main()
