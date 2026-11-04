import json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
import vector4_parity
import oracle_native_shader_lights as oracle
w=Path(tempfile.mkdtemp(prefix='native-shader-lights-'));env=V.env();objects=[]
sources=['rebuild/src/compiled/00/98/CShaderRenderManager_AllocateLights_0098b420.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_Allocate_00a5c910.cpp','rebuild/src/compiled/00/a5/CSimpleMemoryPool_BlockLifetime_00a5c890.cpp',vector4_parity.sources['Erase'],vector4_parity.sources['Insert'],'rebuild/src/compiled/00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp','rebuild/tests/integration/NativeShaderLights_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w);assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'_malloc': 12577294, '_free': 12577300, '?FreeAll@CSimpleMemoryPool@@QAEXXZ': 10864416, '?Allocate@CSimpleMemoryPool@@QAEPAXK@Z': 10864912, '?erase@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEPAVC4DVector@@PAV3@0@Z': 10014576, '?_M_fill_insert@?$vector@VC4DVector@@V?$allocator@VC4DVector@@@_STL@@@_STL@@QAEXPAVC4DVector@@IABV3@@Z': 10015584, '??0?$vector@UCLightInfo@CShaderRenderManager@@V?$allocator@UCLightInfo@CShaderRenderManager@@@_STL@@@_STL@@QAE@I@Z': 10013888, '??$__copy@PAUCLightInfo@CShaderRenderManager@@PAU12@H@_STL@@YIPAUCLightInfo@CShaderRenderManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z': 10011376, '?AllocateLights@CShaderRenderManager@@QAEXKK@Z': 10007584, '??_7CMemoryBlock@CSimpleMemoryPool@@6B@': 19518824, '??_7CIVCountedPointeeBase@@6B@': 19092688, '?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z': 10431456, '?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z': 10431264, '??2@YAPAXI@Z': 12577306, '??3@YAXPAX@Z': 12577212, '?CreateNewMemoryBlock@CSimpleMemoryPool@@AAEXK@Z': 10864784}
root_specs=[['??0?$vector@UCLightInfo@CShaderRenderManager@@V?$allocator@UCLightInfo@CShaderRenderManager@@@_STL@@@_STL@@QAE@I@Z', 10013888, 119, 1], ['??$__copy@PAUCLightInfo@CShaderRenderManager@@PAU12@H@_STL@@YIPAUCLightInfo@CShaderRenderManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z', 10011376, 72, 0], ['?AllocateLights@CShaderRenderManager@@QAEXKK@Z', 10007584, 446, 9]]
lifetime_specs=[['??0CMemoryBlock@CSimpleMemoryPool@@QAE@K@Z', 10864240, 53, 2], ['??1CMemoryBlock@CSimpleMemoryPool@@UAE@XZ', 10864352, 32, 3], ['??_GCMemoryBlock@CSimpleMemoryPool@@UAEPAXI@Z', 10865072, 52, 4], ['?CreateNewMemoryBlock@CSimpleMemoryPool@@AAEXK@Z', 10864784, 121, 3]]
reports=[check(objects[0],sym,a,n,rel,targets)for sym,a,n,rel in root_specs]
support=[check(objects[1],'?FreeAll@CSimpleMemoryPool@@QAEXXZ',0xa5c720,101,0,targets),check(objects[2],'?Allocate@CSimpleMemoryPool@@QAEPAXK@Z',0xa5c910,134,1,targets)]
support += [check(objects[3],sym,a,n,rel,targets)for sym,a,n,rel in lifetime_specs]
support += [check(objects[4 if name=='Erase' else 5],sym,a,n,rel,vector4_parity.targets)for name,sym,a,n,rel in vector4_parity.specs if name!='Resize']
for i in (0,5):
    replay=w/('replay%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=FableLightsMalloc','/Dfree=FableLightsFree','/Fo'+str(replay),str(ROOT/sources[i])],env,w);assert not r.returncode,r.stdout+r.stderr
    original,renamed=Coff(objects[i]),Coff(replay)
    for symbol,(section,offset) in original.symbols.items():
        if not symbol.startswith('?') or symbol not in renamed.symbols or section not in original.sections:continue
        body,rel=original.sections[section];other,other_offset=renamed.symbols[symbol];other_body,other_rel=renamed.sections[other]
        assert body==other_body and offset==other_offset,(i,symbol,section,other)
        assert {o:({'_malloc':'_FableLightsMalloc','_free':'_FableLightsFree'}.get(t,t),k)for o,(t,k)in rel.items()}==other_rel
    objects[i]=replay
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(7):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=60)
assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+12),difference%(oracle.WIDTH+12)))
report.update(providers=reports,support=support,omission_checks=7,boundary='Native light allocation, temporary light-info vector and copy, pool allocation/block creation and attenuation-vector erase/growth. Only heap APIs recorded; allocator-renamed code audited. Full48KiB arena; new light records mask only unspecified first48 bytes and two padding bytes, preserving all existing copied data and initialized Size/enable/update fields. Known vtable relocation normalized. Zero, shrink/grow, retained capacities, pool split/growth, allocation multiplication overflow and unsigned wrap included.')
(ROOT/'rebuild/compile-gate/native-shader-lights.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots',flush=True)
