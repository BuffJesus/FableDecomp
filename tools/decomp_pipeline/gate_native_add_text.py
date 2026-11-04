"""Native text queue submission with actual coordinate conversion and viewport lookup."""
import json,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_add_text as oracle
from variable_allocator_parity import check
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-add-text-'));env=V.env();objects=[]
sources=['rebuild/src/compiled/00/9d/CRenderManager2D_AddText_009db700.cpp',
         'rebuild/src/compiled/00/9e/Global_GFVirtualToActualScalars_009e1e10.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_GetCurrentAWindow_00a0aaa0.cpp',
         'rebuild/src/compiled/00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp',
         'rebuild/src/compiled/00/9d/CQuickDrawTriInfo_CopyConstructor_009de890.cpp',
         'rebuild/src/compiled/00/9f/CTexture_CopyConstructor_009f9d00.cpp',
         'rebuild/src/compiled/00/9e/STLQuickDrawInfo_InsertOverflow_009e1750.cpp',
         'rebuild/tests/integration/NativeAddText_test.cpp']
for i,source in enumerate(sources):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'_malloc':0xbfea0e,'_free':0xbfea14,
         '??$__uninitialized_copy@PAVCQuickDrawTriInfo@@PAV1@@_STL@@YIPAVCQuickDrawTriInfo@@PAV1@00ABU__false_type@0@@Z':0x9e11a0,
         '?_M_clear@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXXZ':0x9e1650,
         '??0CWideString@@QAE@ABV0@@Z':0x99b720,'??0CTexture@@QAE@ABV0@@Z':0x9f9d00,
         '??0CWideString@@QAE@XZ':0x99aed0,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,
         '??1CWideString@@QAE@XZ':0x99b510,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,
         '??0CQuickDrawTriInfo@@QAE@ABV0@@Z':0x9de890,
         '?_M_insert_overflow@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXPAVCQuickDrawTriInfo@@ABV3@ABU__false_type@2@I_N@Z':0x9e1750,
         '?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ':0xa0aaa0,
         '?GFVXToAX@@YIMM@Z':0x9e1e10,'?GFVYToAY@@YIMM@Z':0x9e1e40,
         '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
         '?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ':0x9bedc0,
         '?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,'?g_FableVirtualHeight_013961F0@@3NA':0x13961f0}
reports=[check(objects[0],'?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z',0x9db700,266,9,targets),
         check(objects[1],'?GFVXToAX@@YIMM@Z',0x9e1e10,40,3,targets),
         check(objects[1],'?GFVYToAY@@YIMM@Z',0x9e1e40,41,3,targets),
         check(objects[2],'?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ',0xa0aaa0,12,0,targets),
         check(objects[4],'??0CQuickDrawTriInfo@@QAE@ABV0@@Z',0x9de890,164,2,targets),
         check(objects[5],'??0CTexture@@QAE@ABV0@@Z',0x9f9d00,68,0,targets),
         check(objects[6],'?_M_insert_overflow@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXPAVCQuickDrawTriInfo@@ABV3@ABU__false_type@2@I_N@Z',0x9e1750,243,6,targets),
         check(objects[6],'??$__uninitialized_copy@PAVCQuickDrawTriInfo@@PAV1@@_STL@@YIPAVCQuickDrawTriInfo@@PAV1@00ABU__false_type@0@@Z',0x9e11a0,46,1,targets),
         check(objects[6],'?_M_clear@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXXZ',0x9e1650,58,3,targets)]
# Rename only the tested allocator boundary; verify every emitted code section
# and relocation against the unmodified production object before replay.
replay=w/'overflow-replay.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Dmalloc=FableQueueTestMalloc','/Dfree=FableQueueTestFree','/Fo'+str(replay),str(ROOT/sources[6])],env,w)
assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[6]),Coff(replay)
for symbol,(section,offset) in original.symbols.items():
    if symbol not in renamed.symbols or section not in original.sections:continue
    body,rel=original.sections[section]
    if not rel:continue
    other,otherOffset=renamed.symbols[symbol];otherBody,otherRel=renamed.sections[other]
    assert body==otherBody and offset==otherOffset
    assert {o:({'_malloc':'_FableQueueTestMalloc','_free':'_FableQueueTestFree'}.get(t,t),k)for o,(t,k)in rel.items()}==otherRel
objects[6]=replay
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(7):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert not r.returncode,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//(oracle.WIDTH+8),difference%(oracle.WIDTH+8),len(actual),len(expected)))
report.update(providers=reports,omission_checks=7,boundary='Actual AddText, viewport lookup, scalar conversion, queue record and texture copy constructors. String, texture destruction, COM AddRef and malloc/free boundaries recorded. Vector growth/range copy/clear execute native production code; allocator symbol substitution is byte-checked. Full arena preserves destination padding and unused bitfields; uninitialized local padding is not inspected. Offscreen, available/full/null queue slots, callback mutations and four x87 modes. Copy constructors preserve padding and bitfields; string/COM callback mutations checked. No rasterization; allocation storage supplied by the fixture.')
(ROOT/'rebuild/compile-gate/native-add-text.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots')
