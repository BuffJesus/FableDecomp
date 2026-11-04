import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-data-create-volume-'));env=V.env()
root='?CreateVolumeTexture@CManagedTextureData@@QAE_NKKKIABVCPixelFormat@@@Z';reset='?Reset@?$CCountedPointer@VCVolumeTexture@@@@QAEXPAVCVolumeTexture@@@Z';base='??0CBaseClass@@QAE@XZ';scalar='??_GCVolumeTexture@@UAEPAXI@Z'
specs=[('00/a6/CManagedTextureData_CreateVolumeTexture_00a699d0.cpp',root,0xa699d0,169,0,6),('00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp',reset,0xa6ae60,103,192,3),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp',base,0x99a2d0,9,320,1),('00/a6/CVolumeTexture_Destructor_00a69590.cpp',scalar,0xa69590,43,None,4)]
dimensions='?GetNextValidDimensions@CTexture@@SI?AVC2DExtentsI@@ABV2@ABVCPixelFormat@@@Z';init='?Initialise@CVolumeTexture@@QAE_NABVC3DExtentsI@@IABVCPixelFormat@@KW4EVolumePool@@_N@Z';helper='?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z'
targets={sym:address for rel,sym,address,size,off,count in specs};targets.update({dimensions:0x9fa620,init:0xa68370,helper:0xa6acb0,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'??_7CVolumeTexture@@6B@':0x129dc18,'??_7CBaseClass@@6B@':0x1231710,'?Uninitialise@CVolumeTexture@@QAEXXZ':0xa68060,'??1CBaseClass@@UAE@XZ':0x99a2e0})
offsets={0xa6ae60:192,0x99a2d0:320,0x9fa620:0x80000000,0xbfea1a:0x80000001,0xbfe9bc:0x80000002,0xa6acb0:0x80000003,0x129dc18:0x80000004,0x1231710:0x80000005,0xa68370:0x80000006}
objects=[];sources=[];coffs=[];proofs=[];body=bytearray(352);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;raw=EXE.read_bytes()
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_data_create_volume_candidate2_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets))
 if offset is None:continue
 b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  elif ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==10,fixups
supporting_ctor=check(objects[0],'??0CVolumeTexture@@QAE@XZ',0xa695c0,26,2,targets) if '??0CVolumeTexture@@QAE@XZ' in coffs[0].symbols else None
supporting_helper=check(objects[1],helper,0xa6acb0,11,0,targets)
(w/'managed_texture_data_create_volume_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@ManagedVolumeCreate@28':root,'@VolumeDeleteData@4':helper,'@VolumeScalar@12':scalar,'_VolumeVTable':'??_7CVolumeTexture@@6B@','_BaseVTable':'??_7CBaseClass@@6B@',dimensions:'@DimensionsBoundary@12'}
defined={sym for c in coffs for sym in c.symbols};undefined={sym for c in coffs for data,relocs in c.sections.values() for sym,kind in relocs.values() if sym not in defined}
for sym in undefined-set(aliases)-{init,'?Uninitialise@CVolumeTexture@@QAEXXZ','??2@YAPAXI@Z','??3@YAXPAX@Z','??_7type_info@@6B@','__purecall'}:aliases[sym]='_AbortBoundary'
(w/'managed_texture_data_create_volume_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureDataCreateVolume_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];ordered=[objects[3]]+objects[:3]
for omit in objects:
 r=subprocess.run(cmd+[str(o) for o in ordered if o!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,ordered)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=27648 snapshots=' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_volume_types.h','fable_surface_types.h','fable_extents_i.h','fable_counted_pointer.h','fable_resource_list.h','fable_base_class.h','rebuild_abi.h')]
report=dict(cases=27648,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,unclaimed_constructor_proof=supporting_ctor,unclaimed_deleter_proof=supporting_helper,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact169B managed-volume creator plus actual103B counted-volume Reset and9B BaseClass constructor execute against original bytes; fourth mandatory source supplies verified43B volume scalar and runtimevtable identity, not invoked here. Inline26B volume constructor supportingproof. Next-valid-dimensions/allocator16+12/delete/olddeleter/volume-initializer functional boundaries; deferred rounding/main volume initialization not claimed recovered. Cache roundedwidth-height beforeallocationandReset, callbacks mutate stilllive returnedextent, construct16B volume preservingByteLength, Resetactualnull-old/shared/refwrap/allocationfailure/liveData reload, original16bit WidthHeightstores andfull32bitdepth, fixedlevels1/pool1/flags0/autofalse/ignoredfourthuint, Allocated storedaftercallback. Full16KiB snapshots at everycall/final,two placements-fills; no stack pointers logged/read afterreturn, no actualscalar shutdown/helper virtual deletion inArena gate; previouslycovered realheap test executes them. Only root addscoverage.')
(ROOT/'rebuild/compile-gate/native-managed-texture-data-create-volume.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_DATA_CREATE_VOLUME_GATE_PASS')
