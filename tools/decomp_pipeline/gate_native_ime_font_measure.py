"""Gate composed native font selection/measurement against complete retail replay."""
import json,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_ime_font_measure as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-ime-font-measure-'));env=V.env();objects=[]
for i,source in enumerate(['rebuild/src/compiled/00/40/CIMEFont_GetFontNameAndScale_00404970.cpp',
                          'rebuild/src/compiled/00/40/CIMEFont_GetTextExtent_00404f60.cpp',
                          'rebuild/src/compiled/00/40/CIMEFont_DrawTextW_00405020.cpp',
                          'rebuild/src/compiled/00/9e/Global_GFACoordToVCoord_009e21d0.cpp',
                          'rebuild/src/compiled/00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp',
                          'rebuild/src/compiled/00/9d/CRenderManager2D_Draw3DScaledText_009dbff0.cpp',
                          'rebuild/tests/integration/NativeIMEFontMeasure_test.cpp']):
    obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/source)],env,w)
    assert not r.returncode,r.stdout+r.stderr
    objects.append(obj)
targets={'__real@4f800000':0x122dcb4,'__real@3daaaaab':0x122dca0,'__real@3d800000':0x122dc98,
         '__real@3d638e39':0x122dc90,'__real@3d2aaaab':0x122dc88,'__real@3d124925':0x122dc80,
         '??_C@_0L@BEDDKEGH@ENG_ARIAL_?$AA@':0x122dca8,'??_C@_02MIGHPNDL@12?$AA@':0x122dca4,
         '??_C@_02KMALDIDP@16?$AA@':0x122dc9c,'??_C@_02DCIIBFLB@18?$AA@':0x122dc94,
         '??_C@_02JMHLOEOE@24?$AA@':0x122dc8c,'??_C@_02DAMOKLOI@28?$AA@':0x122dc84,
         '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,'??YCCharString@@QAEABV0@PBD@Z':0x99f100,
         '??0CCharString@@QAE@XZ':0x99e4b0,'??1CCharString@@QAE@XZ':0x99eae0,
         '?GetFontNameAndScale@CIMEFont@NUISystem@@ABEXAAVCCharString@@AAM@Z':0x404970,
         '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
         '?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z':0x9e2c80,
         '__ftol2':0xbfea70,'??3@YAXPAX@Z':0xbfe9bc}
targets.update({'?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z':0x9db700,'??0CWideString@@QAE@PBG@Z':0x99b6b0,'??0CWideString@@QAE@ABV0@@Z':0x99b720,'??1CWideString@@QAE@XZ':0x99b510,
                '?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z':0x9e21d0,'?g_IMEAlphaDefault_013750B0@@3EA':0x13750b0,
                '__real@40000000':0x122dcdc,
                '?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z':0x9dbff0})
reports=[check(objects[0],'?GetFontNameAndScale@CIMEFont@NUISystem@@ABEXAAVCCharString@@AAM@Z',0x404970,226,18,targets),
         check(objects[1],'?GetTextExtent@CIMEFont@NUISystem@@UAEXPBGPAK1@Z',0x404f60,179,8,targets),
         check(objects[2],'?DrawTextW@CIMEFont@NUISystem@@UAEXPBG@Z',0x405020,410,19,targets),
         check(objects[5],'?Draw3DScaledText@CRenderManager2D@@QAE_NABVC3DVector@@MVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z',0x9dbff0,49,2,targets)]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(6):
    r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs,expected,report=oracle.collect();(w/'input.bin').write_bytes(inputs)
r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30)
assert not r.returncode,(r.returncode,r.stdout,r.stderr,w)
actual=(w/'actual.bin').read_bytes()
if actual!=expected:
    (w/'expected.bin').write_bytes(expected)
    difference=next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),min(len(actual),len(expected)))
    raise AssertionError((w,difference,difference//4104,difference%4104,len(actual),len(expected)))
report.update(providers=reports,omission_checks=6,boundary='Actual font selection/measurement/text submission and coordinate conversion and counted-pointer destructor. String/bank calls and releases recorded, callback height changes, aliased width/height outputs, signed/large extents and four x87 modes checked. Shadow thresholds, alpha refresh, renderer changes and by-value string lifetime checked; AddText queue boundary recorded, no glyph drawing.')
(ROOT/'rebuild/compile-gate/native-ime-font-measure.json').write_text(json.dumps(report,indent=2)+'\n')
print(r.stdout.strip(),report['snapshots'],'full-state snapshots')
