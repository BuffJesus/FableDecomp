"""Per-map static opening with actual counted-pointer lifetime against retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-texture-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineMap_OpenDevelopmentStaticMap_00b42530.cpp';owner=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_CountedLifetime_009d69a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMapTexture_test.cpp'
symbol='?OpenDevelopmentStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@W4STATIC_MAP_OPEN_STATE@1@@Z'
copy='??0?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@ABV0@@Z';destroy='??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ';assign='??4?$CCountedPointer@VCBankFileAsyncEntry@@@@QAEAAV0@ABV0@@Z'
targets={'?_M_fill_insert@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXPADIABD@Z':0x4110d0,'??_7CMemoryDataInputStream@@6B@':0x123675c,'??0?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@ABV0@@Z':0x9d69a0,'_free':0xbfea14,'??1CMemoryDataInputStream@@UAE@XZ':0x993b10,'?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z':0x9d58d0,'??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ':0x9c9fc0,'?OpenStaticMap@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z':0xbdf010,'??0CDataInputStream@@QAE@XZ':0x993b20,'?OpenMemorySource@CMemoryDataInputStream@@QAEXPBXK@Z':0x994570,'?CloseStaticMap@CEngineMap@@QAEXXZ':0xb3ef40,'?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z':0x9cbe90,'??4?$CCountedPointer@VCBankFileAsyncEntry@@@@QAEAAV0@ABV0@@Z':0xa5f6d0,'?OpenStaticMap@CEngineLandscapeMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z':0xbdd0e0,'?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z':0xb3efa0,'?SetBankFileEntry@CEngineStaticMapFileBlockManager@@QAEXV?$CCountedPointer@VCBankFileAsyncEntry@@@@@Z':0xbe03a0,'?SetPosition@CDataInputStream@@UAEXK@Z':0x993bc0,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0}
quadsource=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_QuadLoadHeader_00bde290.cpp'
quadsymbol='?LoadHeader@CQuadTreeElement@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z'
landsource=ROOT/'rebuild/src/compiled/00/bd/CEngineLandscapeMap_OpenStaticMap_00bdd0e0.cpp'
detailsource=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_OpenStaticMap_00bdf010.cpp'
landsymbol='?OpenStaticMap@CEngineLandscapeMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z'
detailsymbol='?OpenStaticMap@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z'
targets.update({'_malloc':0xbfea0e,'?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z':0xbf9290,'?LoadHeader@CQuadTreeElement@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':0xbde290,'?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':0xbdedd0,'___security_cookie':0x139c8a8,'@__security_check_cookie@4':0xbfe9f9})
parsersource=ROOT/'rebuild/src/compiled/00/b3/CEngineMap_ReadMapInfoBlock_00b3efa0.cpp'
parsersymbol='?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z'
targets['?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z']=0x993ca0
readsource=ROOT/'rebuild/src/compiled/00/9c/CBankFileEntryUpdateData_ReadInfo_009cbe90.cpp'
readsymbol='?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z'
blocksource=ROOT/'rebuild/src/compiled/00/be/CEngineStaticMapFileBlockManager_SetBankFileEntry_00be03a0.cpp'
blocksymbol='?SetBankFileEntry@CEngineStaticMapFileBlockManager@@QAEXV?$CCountedPointer@VCBankFileAsyncEntry@@@@@Z'
targets['??3@YAXPAX@Z']=0xbfe9bc
texture_source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_Load_00bf9290.cpp'
vector_source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_VectorResize_00bf97a0.cpp'
string_source=ROOT/'rebuild/src/compiled/00/99/CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp'
texture_symbol='?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z'
string_symbol='?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ'
vo=w/'vector-original.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(vo),str(vector_source)],env,w);assert not r.returncode,r.stdout+r.stderr;vc=Coff(vo)
specs=[(next(s for s in vc.symbols if s.startswith(pre) and (not suffix or s.endswith(suffix))),a,n,r) for pre,suffix,a,n,r in [('?resize@','QAEXI@Z',0xbf97a0,108,5),('?erase@','',0xbf9420,77,2),('?_M_fill_insert@','',0xbf95d0,280,8),('??$__copy@','',0xbf8e50,61,1),('??$__copy_backward@','',0xbf8e90,64,1),('??$fill@','',0xbf8e20,46,1),('??$uninitialized_fill_n@','',0xbf8ff0,50,1),('?_M_insert_overflow@','',0xbf94d0,249,7),('??$__uninitialized_copy@','',0xbf8f30,54,1),('??$__uninitialized_fill_n@','',0xbf8f70,50,1)]]
targets.update({s:a for s,a,n,r in specs});targets.update({'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0})
boundary={0xb3ef40:'@Close@4',0x9ccdc0:'@Find@12',0x993b20:'@StreamCtor@4',0x993b10:'@StreamDtor@4',0x9cbe90:'@Read@20',0x994570:'@MemoryOpen@16',0xb3efa0:'@ReadMapInfo@8',0x9d58d0:'@GetEntry@16',0xbe03a0:'@SetEntry@16',0x993bc0:'@Position@12',0xbdd0e0:'@Landscape@16',0xbdf010:'@Detail@16'}
vt='??_7CMemoryDataInputStream@@6B@';fill='?_M_fill_insert@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXPADIABD@Z'
aliases={'@OpenMap@16':symbol,'@Copy@12':copy,'@Destroy@4':destroy,'@Assign@12':assign,'@Fill@20':fill,'_StreamTable':vt,'_Delete':'??3@YAXPAX@Z'};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
del aliases[landsymbol];del aliases[detailsymbol];aliases.update({'@Landscape@16':landsymbol,'@Detail@16':detailsymbol,'?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z':'@Palette@12','?LoadHeader@CQuadTreeElement@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':'@Quad@12','?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':'@Types@12'})
del aliases[quadsymbol];aliases['@Quad@12']=quadsymbol
del aliases[parsersymbol];aliases.update({'@ReadMapInfo@8':parsersymbol,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':'@Overflow@16'})
del aliases[readsymbol];aliases['@Read@20']=readsymbol
del aliases[blocksymbol];aliases['@SetEntry@16']=blocksymbol
slots={'??_ECMemoryDataInputStream@@UAEPAXI@Z':0x485420,'?SetPosition@CDataInputStream@@UAEXK@Z':0x993bc0,'?GetPosition@CDataInputStream@@UBEKXZ':0x405fa0,'?Close@CDataInputStream@@UAEXXZ':0x993c80,'?ReadCompressed@CMemoryDataInputStream@@UAEXPAXJ@Z':0x995b10,'?ReadZlibCompressed@CDataInputStream@@UAEXPAXJ@Z':0x996230,'?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ':0x995d30,'?ReadNullTerminatedWString@CMemoryDataInputStream@@UAE?AVCWideString@@XZ':0x994960,'?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z':0x993b90,'?NeedsBufferedRead@CDataInputStream@@EAE_NJ@Z':0x993ba0,'?InternalReadUnbuffered@CDataInputStream@@EAEXPAEJ@Z':0x993bb0}
for s in list(slots)[3:]:aliases[s]='_Trap'
aliases['??1CBaseClassNonCopyable@@UAE@XZ']='_Trap'
del aliases[texture_symbol];del aliases[string_symbol]
aliases.update({'??0CCharString@@QAE@XZ':'@DefaultString@4','??0CCharString@@QAE@PBDJ@Z':'@ConstructString@16','??0CCharString@@QAE@ABV0@@Z':'@CopyString@12','??4CCharString@@QAEAAV0@ABV0@@Z':'@AssignString@12','??1CCharString@@QAE@XZ':'@DestroyString@4'})
(w/'map_texture_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(5514);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in [(0xb42530,538,0),(0x9d69a0,25,576),(0x9c9fc0,53,640),(0xa5f6d0,72,704),(0xbe03a0,90,800),(0x9cbe90,67,896),(0xb3efa0,1597,1024),(0xbdd0e0,292,2688),(0xbdf010,186,3008),(0xbde290,249,3264),(0xbf9290,362,3584),(0x995d30,138,5376)]+[(a,n,4096+off) for (sym,a,n,r),off in zip(specs,[0,128,224,512,576,640,704,768,1024,1088])]:
 span=read_virtual_data(raw,address,length);body[offset:offset+length]=span
 for i in md.disasm(span,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   a=i.operands[0].imm;assert a in targets.values();fixups.append((offset+i.address-address+1,a,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp==0x139c8a8:fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
    elif op.type==2 and op.imm==0x123675c:fixups.append((offset+i.address-address+i.imm_offset,op.imm,0))
assert len(fixups)==94
(w/'map_texture_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,owner,fixture,source,blocksource,readsource,parsersource,landsource,detailsource,landsource,quadsource)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/GS','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dfree=MapFree'] if n==3 else ['/Dfree=MapFree','/Dmalloc=ChildAllocate'] if n==9 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=[check(objects[0],symbol,0xb42530,538,20,targets)]+[check(objects[1],s,a,n,r,targets) for s,a,n,r in [(copy,0x9d69a0,25,0),(destroy,0x9c9fc0,53,1),(assign,0xa5f6d0,72,1)]]
proof.append(check(objects[4],blocksymbol,0xbe03a0,90,2,targets))
proof.append(check(objects[5],readsymbol,0x9cbe90,67,0,targets))
proof.append(check(objects[6],parsersymbol,0xb3efa0,1597,18,targets))
proof.append(check(objects[10],quadsymbol,0xbde290,249,1,targets))
proof.extend([check(objects[7],landsymbol,0xbdd0e0,292,6,targets),check(objects[8],detailsymbol,0xbdf010,186,5,targets)])
ownership_emissions=[check(objects[7],s,a,n,r,targets) for s,a,n,r in [(copy,0x9d69a0,25,0),(destroy,0x9c9fc0,53,1),(assign,0xa5f6d0,72,1)]]
c=Coff(objects[7]);d=Coff(objects[9]);sec,off=c.symbols[landsymbol];sec1,off1=d.symbols[landsymbol];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_ChildAllocate','_free':'_MapFree'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
c=Coff(objects[0]);d=Coff(objects[3]);sec,off=c.symbols[symbol];sec1,off1=d.symbols[symbol];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:('_MapFree' if t=='_free' else t,k) for p,(t,k) in rel.items()}==rel1
sec,off=c.symbols[vt];b,rel=c.sections[sec];assert off==0 and b==bytes(44) and set(rel)==set(range(0,44,4));patched=bytearray(b)
for p,(t,k) in rel.items():assert k==6;struct.pack_into('<I',patched,p,slots[t])
assert patched==read_virtual_data(raw,0x123675c,44)
extra_objects=[]
for i,path in enumerate((texture_source,vector_source,string_source)):
 obj=w/('texture-extra'+str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj)]+(['/Dmalloc=ChildAllocate','/Dfree=MapFree'] if i==1 else [])+[str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;extra_objects.append(obj)
texture_proof=check(extra_objects[0],texture_symbol,0xbf9290,362,9,targets)
string_proof=check(extra_objects[2],string_symbol,0x995d30,138,3,targets)
vector_proof=[check(vo,s,a,n,r,targets) for s,a,n,r in specs]
vd=Coff(extra_objects[1])
for sym,a,n,r in specs:
 sec,off=vc.symbols[sym];sec1,off1=vd.symbols[sym];b,rel=vc.sections[sec];b1,rel1=vd.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_ChildAllocate','_free':'_MapFree'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[objects[2],objects[3],objects[4],objects[5],objects[6],objects[9],objects[8],objects[10]]+extra_objects
for omit in range(1,len(linked)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,(w,omit)
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,state,old,index,length,mismatch,children,count in itertools.product((0,0x95),range(7),(0,1,2),(0,1),(0,1),(0,512),(0,1,2,3,4),(0,1),(1,2)):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0xc00);put(4,base+0xc80 if children else 0);put(0x20,old);put(0x1c,0x1111);put(0x3c,base+0x700);put(0x40,base+0x840);struct.pack_into('<4H',a,0x5c,7,9,11,13)
  put(0x22c,base+0x400);put(0x404,base+0x500);put(0x504,length);put(0x508,base+0x1a00);put(0x800,base+0x780);put(0x804,base+0x860);put(0x44,base+0x720);put(0x48,base+0x880);put(0x840,count);put(0x860,2);put(0x880,count)
  for j in range(23):put(0x900+j*4,0x20+j)
  for j,v in enumerate((7,9,11,13)):put(0x920+j*4,v+(1 if mismatch==j+1 else 0))
  put(0x910,128);put(0x914,160);put(0x958,192);put(0xc18,base+0x740);put(0xc1c,base+0x8a0);put(0x8a0,count)
  for j,v in enumerate((256,7,55,66,77)):put(0x1a80+j*4,v)
  put(0x1aa0,48);put(0x1aa4,0);put(0x1aa8,1)
  order=[0,4,12,32,36,40,44,8,16,20,48,52,56,60,24,28]+list(range(64,88,4))+[88]
  for j,off in enumerate(order):a[0x1a00+j*4:0x1a04+j*4]=a[0x900+off:0x904+off]
  put(0xc40,base+0x1300);put(0xc44,base+0x1000);put(0xc48,base+0x1008);put(0xc4c,base+0x1000+(8 if count==1 else 32));put(0xc50,0x7777)
  a[0x1b00]=1 if fill else 0;put(0x1b01,1 if count==1 else 3);a[0x1b05:0x1b13]=b'one\0two\0three\0'
  f.write(struct.pack('<3I',mode,state,index));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%8204==0;snapshots=size//8204
report=dict(cases=cases,snapshots=snapshots,omission_checks=10,providers=proof,texture_provider=texture_proof,string_provider=string_proof,vector_providers=vector_proof,linked_landscape_ownership=ownership_emissions,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,owner,blocksource,readsource,parsersource,landsource,detailsource,quadsource,texture_source,vector_source,string_source)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Ten mandatory providers,twenty-two original spans/94 audited fixups. Actual map opening,metadata parsing,child openers,quad header,ownership,texture loading,all palette-vector helpers and memory-string reader compose. Full8KiB snapshots. Actual eleven-slot stream table audited;native local stream uses a cloned table with only the string slot redirected to native reader. Allocation renames audited. Existing state/mismatch/ownership cases retained,serialized texture flags/counts/names,spare/growing vector. String lifecycle,stream setup/position/overflow,bank lookup/entry,scenery palette,Close,char-vector fill and allocation remain boundaries. Default string callback initializes companion temporary Index.')
(ROOT/'rebuild/compile-gate/native-map-texture.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_TEXTURE_GATE_PASS',snapshots,'snapshots')
