"""Validate the CMemoryFile family: relocation/NEAR proof, vtable ABI, native oracle and behavior fixture."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from near_match import compare as similarity,retail_relocations
from memory_file_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
def S(file,address,symbol,size,**extra):return dict(file=file,address=address,symbol=symbol,native_bytes=size,**extra)
SPECS=[
 S('CMemoryFile_Open_009e90a0.cpp','009e90a0','?Open@CMemoryFile@@QAEXXZ',0x1e),
 S('CMemoryFile_SetPosition_009e90c0.cpp','009e90c0','?SetPosition@CMemoryFile@@UAEXJ@Z',0x76),
 S('CMemoryFile_MovePosition_009e9140.cpp','009e9140','?MovePosition@CMemoryFile@@UAEXJ@Z',0xdb),
 S('CMemoryFile_Read_009e9220.cpp','009e9220','?Read@CMemoryFile@@UAEXPAXJPAVCASuspendableProcess@@@Z',0x102),
 S('CMemoryFile_DumpToFile_009e9330.cpp','009e9330','?DumpToFile@CMemoryFile@@QAEXXZ',0x58),
 S('CMemoryFile_Close_009e9390.cpp','009e9390','?Close@CMemoryFile@@UAEXXZ',0x2e),
 S('CMemoryFile_Open_009e93c0.cpp','009e93c0','?Open@CMemoryFile@@QAEXPAXJ@Z',0x5d),
 S('CMemoryFile_OpenFromFile_009e9420.cpp','009e9420','?OpenFromFile@CMemoryFile@@QAEXAAVCAFile@@JPAVCASuspendableProcess@@@Z',0x78),
 S('CMemoryFile_Open_009e94a0.cpp','009e94a0','?Open@CMemoryFile@@UAEXABVCWideString@@W4FileAccessType@@K@Z',0x146),
 S('CMemoryFile_Write_009e95f0.cpp','009e95f0','?Write@CMemoryFile@@UAEXPBXJPAVCASuspendableProcess@@@Z',0x13c,near=True),
 S('CMemoryFile_SetLength_009e9730.cpp','009e9730','?SetLength@CMemoryFile@@UAEXJ@Z',0x22),
 S('CMemoryFile_Copy_009e9760.cpp','009e9760','?Copy@CMemoryFile@@AAEXABV1@@Z',0x4c),
 S('CMemoryFile_CMemoryFile_009e97b0.cpp','009e97b0','??0CMemoryFile@@QAE@ABVCWideString@@W4FileAccessType@@K@Z',0x3d),
 S('CMemoryFile_Clone_009e97f0.cpp','009e97f0','?Clone@CMemoryFile@@UBEPAVCAFile@@XZ',0x1f),
 S('CMemoryFile_CopyConstructor_009e99c0.cpp','009e99c0','??0CMemoryFile@@QAE@ABV0@@Z',0x70),
 S('CMemoryFile_GetPosition_00412050.cpp','00412050','?GetPosition@CMemoryFile@@UBEJXZ',4),
 S('CMemoryFile_GetLength_00412060.cpp','00412060','?GetLength@CMemoryFile@@UBEJXZ',4),
 S('CMemoryFile_IsOpen_00405d30.cpp','00405d30','?IsOpen@CMemoryFile@@UBE_NXZ',4)]
COPY_HELPER='??$__copy@PBEV?$back_insert_iterator@V?$CArrayVector@E@@@_STL@@H@_STL@@YI?AV?$back_insert_iterator@V?$CArrayVector@E@@@0@PBE0V10@ABUrandom_access_iterator_tag@0@PAH@Z'
EXTERNAL={COPY_HELPER:0x9e9900,'??4CWideString@@QAEABV0@PBG@Z':0x99b800,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'??1CWideString@@QAE@XZ':0x99b510,'??0CWideString@@QAE@XZ':0x99aed0,
 '??0CAFile@@QAE@XZ':0x99a2f0,'??_7CMemoryFile@@6B@':0x122e0fc,'??_7CDiskFileWin32@@6B@':0x122d06c,'??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z':0x99ad80,
 '?Write@CDiskFileWin32@@UAEXPBXJPAVCASuspendableProcess@@@Z':0x99aa90,'?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z':0x99a960,'?Close@CDiskFileWin32@@UAEXXZ':0x99a920,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'?resize@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXIABE@Z':0x486189,'?resize@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXI@Z':0x4861df,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,'??1CCharString@@QAE@XZ':0x99eae0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??BCCharString@@QBEPBDXZ':0x99e4c0,
 '??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,'__TI2?AVCBBBFileException@@':0x1369380,'__TI1?AVCBBBFileException@@':0x1369380,'__CxxThrowException@8':0xbfeb84,'??0CBBBFileException@@QAE@VCCharString@@W4EFileExceptionType@0@@Z':0x99ae50,
 '?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z':0x999110,'?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,'?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??2@YAPAXI@Z':0xbfea1a,'_memmove':0xbfeae6,'?_M_insert_overflow@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXPAEABEABU__true_type@2@I_N@Z':0x485b6c,
 '??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,'??_C@_00CNPNBAHC@?$AA@':0x122d70e}
# Retail CMemoryFile vtable 0122E0FC: slot -> expected compiled symbol.
VTABLE_EXTRA={'??_ECMemoryFile@@UAEPAXI@Z':0x4120c0,'?GetPathname@CMemoryFile@@UBE?AVCWideString@@XZ':0x412070,'?IsIOFinished@CAFile@@UBE_NXZ':0x401650,
 '?GetAlignment@CAFile@@UBEJXZ':0x401660,'?GetFileInformation@CAFile@@UAE?AVCFileInformation@@XZ':0x412090}
def literal(c,name,image):
 sec,off=c.symbols[name];data=c.sections[sec][0][off:];wide=name.startswith('??_C@_1');term=b'\0\0' if wide else b'\0';k=0
 while True:
  j=data.find(term,k)
  if not wide or j%2==0:break
  k=j+1
 text=data[:j]+term;hit=image.find(text,0x122b000-0x400000)
 while hit>=0 and wide and hit%2:hit=image.find(text,hit+1)
 assert hit>=0,name;return 0x400000+hit
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/MemoryFile_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/memory-file.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='memory-file-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];coffs=[];providers=[]
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 for i,s in enumerate(SPECS):
  source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,'source%d.obj'%i)
  objects.append(obj);coffs.append(Coff(obj));providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 targets={s['symbol']:int(s['address'],16) for s in SPECS};targets.update(EXTERNAL);candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  image=pe.get_memory_mapped_image()
  for s,c in zip(SPECS,coffs):
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0,(s['file'],off);patched=bytearray(b)
   for pos,(name,kind) in rel.items():
    if name not in targets and name.startswith('??_C@'):targets[name]=literal(c,name,image)
    assert kind in (6,20);target=targets[name];struct.pack_into('<I',patched,pos,(target-addr-pos-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['native_bytes']);candidate[addr]=patched;native[addr]=raw;entry=dict(s,bytes=len(b),relocations=sorted([pos,list(v)] for pos,v in rel.items()))
   if s.get('near'):
    score=similarity(bytes(patched),raw,tuple(rel),retail_relocations(s['address'],len(raw)));assert score.near_match and score.built_instructions==score.retail_instructions and score.opcode_similarity>=0.91,score
    entry.update(grade='NEAR_MATCH',similarity=dict(retail_instructions=score.retail_instructions,built_instructions=score.built_instructions,opcode_similarity=score.opcode_similarity))
   else:
    assert len(b)==len(raw) and patched==raw,s['file'];entry.update(grade='RELOCATION_MATCH' if rel else 'MATCH')
   entries.append(entry)
  # The byte back-inserter copy is emitted naturally by Copy's unit (retail
  # COMDAT 0x009E9900); verified exactly, not claimed as separate coverage.
  c=coffs[[s['address'] for s in SPECS].index('009e9760')];sec,off=c.symbols[COPY_HELPER];b,rel=c.sections[sec];patched=bytearray(b)
  for pos,(name,kind) in rel.items():struct.pack_into('<I',patched,pos,(targets[name]-0x9e9900-pos-4 if kind==20 else targets[name])&0xffffffff)
  assert off==0 and bytes(patched)==pe.get_data(0x9e9900-0x400000,len(b)),'copy helper'
  # Sixteen-slot vtable emitted by the constructor's unit against retail 0122E0FC.
  c=coffs[[s['address'] for s in SPECS].index('009e97b0')];sec,off=c.symbols['??_7CMemoryFile@@6B@'];_,rel=c.sections[sec];slots=[]
  for k in range(16):
   name=rel[off+4*k][0];want=targets.get(name,VTABLE_EXTRA.get(name));assert want==pe.get_dword_at_rva(0x122e0fc-0x400000+4*k),(k,name);slots.append(name)
  assert off+64 not in rel
  native_extra={0x99ae50:pe.get_data(0x99ae50-0x400000,0x1f)}
 comparisons=compare({**candidate,**native_extra},{**native,**native_extra})
 test=compile(a.test,'test.obj');tc=Coff(test);assert all(s['symbol'] not in tc.symbols for s in SPECS);objects.append(test)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in range(len(SPECS)):
  r=subprocess.run(cmd+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',SPECS[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode and 'MEMORY_FILE_BEHAVIOR_PASS scenarios=11031' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,vtable_slots=slots,comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=len(SPECS),
  boundaries=['CCharString/CWideString','CAFile statics','CDiskFileWin32','vector<unsigned char>::resize','_CxxThrowException'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('MEMORY_FILE_PASS compiled=11031 comparative=%d providers=%d slots=16'%(comparisons,len(SPECS)))
if __name__=='__main__':main()
