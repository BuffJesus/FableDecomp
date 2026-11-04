"""Real D3D9 raster/readback smoke check; device setup is a harness boundary."""
import hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
work=ROOT/'work/codex-native-startup/native-indexed-raster-20261009'
work.mkdir(parents=True,exist_ok=True)
sources=['rebuild/src/compiled/00/a0/CRenderManagerCore_DrawAsIndexedTris_00a0af40.cpp',
         'rebuild/src/compiled/00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp',
         'rebuild/src/compiled/00/9b/CDisplayManager_NativeRender_009bef20.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_Frame_00a0b480.cpp',
         'rebuild/src/compiled/00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_SetVertexFormat_00a0aa30.cpp',
         'rebuild/src/compiled/00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawAsIndexedTrisTextures_00a0aea0.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsTris_00a0ae10.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTris_00a0ac60.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp',
         'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTrisTextures_00a0ab50.cpp',
         'rebuild/src/compiled/00/9f/CTexture_GetDimensions_009f9db0.cpp',
         'rebuild/src/compiled/00/9f/CTexture_GetLevelByteLength_009f9e80.cpp',
         'rebuild/src/compiled/00/9e/CPixelFormat_Initialise_009e3830.cpp',
         'rebuild/src/compiled/00/9e/CPixelFormat_GetColourDepth_009e3820.cpp',
         'rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp',
         'rebuild/tests/integration/NativeIndexedRaster_test.cpp']
env=V.env();objects=[]
for i,source in enumerate(sources):
    obj=work/f'{i}.obj'
    result=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,work)
    assert not result.returncode,result.stdout+result.stderr
    objects.append(obj)
proofs=[check(objects[0],'?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z',0xa0af40,58,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}),
        check(objects[1],'?RealiseRenderState@CRenderStateManager@@QAEXXZ',0xa058c0,167,0,{})]
targets={'?BeginRender@CRenderManagerCore@@QAE_NXZ':0xa0b480,'?EndRender@CRenderManagerCore@@QAEXXZ':0xa0b520,'?GFGetTime@@YINXZ':0x9e1bc0,'__ftol2':0xbfea70}
for index,symbol,address,size,fixups in [(2,'?BeginRender@CDisplayManager@@QAE_NXZ',0x9bef20,41,1),(2,'?EndRender@CDisplayManager@@QAEXXZ',0x9bef50,43,1),(3,'?BeginRender@CRenderManagerCore@@QAE_NXZ',0xa0b480,146,1),(3,'?EndRender@CRenderManagerCore@@QAEXXZ',0xa0b520,63,2)]:
    proofs.append(check(objects[index],symbol,address,size,fixups,targets))
proofs.append(check(objects[4],'?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ',0xa05840,121,0,{}))
proofs.append(check(objects[5],'?SetVertexFormat@CRenderManagerCore@@QAEXK@Z',0xa0aa30,23,0,{}))
proofs.append(check(objects[6],'?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z',0x9a0cf0,79,0,{}))
proofs.append(check(objects[7],'?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z',0xa0aea0,146,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
proofs.append(check(objects[8],'?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXKK@Z',0xa0ae10,39,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
proofs.append(check(objects[9],'?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXKKKK@Z',0xa0ac60,51,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
proofs.append(check(objects[10],'?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z',0xa0abe0,127,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
proofs.append(check(objects[11],'?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKKK@Z',0xa0ab50,139,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
proofs.append(check(objects[12],'?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z',0x9f9db0,43,0,{}))
proofs.append(check(objects[13],'?GetLevelByteLength@CTexture@@QBEJJ@Z',0x9f9e80,82,2,{'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820}))
proofs.append(check(objects[14],'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830,50,1,{'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40}))
proofs.append(check(objects[15],'?GetColourDepth@CPixelFormat@@QBEKXZ',0x9e3820,13,1,{'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40}))
user32=Path('C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x86/user32.lib')
assert user32.is_file(),user32
exe=work/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib',str(user32)]
for omit in range(17):
    result=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True)
    assert result.returncode,omit
result=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True)
assert not result.returncode,result.stdout+result.stderr
frame=work/'frame.ppm'
wire=work/'restored-wire.ppm'
textured=work/'textured.ppm'
buffer_indexed=work/'buffer-indexed.ppm';buffer_tris=work/'buffer-tris.ppm'
tex_indexed=work/'textured-buffer-indexed.ppm';tex_tris=work/'textured-buffer-tris.ppm'
buffer_frames=[buffer_indexed,buffer_tris,tex_indexed,tex_tris]
result=subprocess.run([str(exe),str(frame),str(wire),str(textured)]+list(map(str,buffer_frames)),capture_output=True,text=True,timeout=45)
assert not result.returncode,(result.returncode,result.stdout,result.stderr)
assert 'NATIVE_INDEXED_RASTER_PASS' in result.stdout and 'texture_level_bytes=16' in result.stdout
assert all(p.read_bytes()==textured.read_bytes() for p in buffer_frames),'UP and bound-buffer raster images differ'
report=dict(providers=proofs,omission_checks=17,pixels=114688,texture_dimensions=[2,2],texture_level_bytes=16,output=result.stdout.strip(),frame=str(frame),frame_sha256=hashlib.sha256(frame.read_bytes()).hexdigest(),restored_frame=str(wire),restored_frame_sha256=hashlib.sha256(wire.read_bytes()).hexdigest(),textured_frame=str(textured),textured_frame_sha256=hashlib.sha256(textured.read_bytes()).hexdigest(),buffer_frames=list(map(str,buffer_frames)),buffer_frames_sha256=[hashlib.sha256(p.read_bytes()).hexdigest() for p in buffer_frames],
            source_sha256={s:hashlib.sha256((ROOT/s).read_bytes()).hexdigest() for s in sources},
            boundary='Actual texture dimensions andmip byte-length viaoriginal pixel-format helpers/table verified againstreal2x2texture. Actual compiled recovered display/core begin/end,vertex-format selection,texture attach/unbind,indexed-UP and bound-buffer indexed/nonindexed drawing with/without texture-list overloads,dirty render/texture-stage/sampler realization and capture restoration on a real Direct3D9 device. Seven full readbacks classified;UV probes,solid/wireframe,and exact textured image equality across five UP/buffer paths. Harness supplies hidden window,device,geometry,texture pixels,buffer creation/upload/binding,clock,initial states/capture record and readback. Bounded primitive raster smoke check,not a reconstructed game/frontend frame.')
(ROOT/'rebuild/compile-gate/native-indexed-raster.json').write_text(json.dumps(report,indent=2)+'\n')
print(result.stdout.strip(),flush=True)
