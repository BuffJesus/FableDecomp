#!/usr/bin/env python3
"""Explicit live D3D9 test of recovered texture code using an owned hidden window.
Not an offline bootstrap gate: requires a working Windows D3D9 HAL device.
"""
import json
from check_cgame_play import ROOT, parity, run
from check_ui_texture_surfaces import SOURCES

def main():
    directory=ROOT/'work/ui_texture_device_check'; directory.mkdir(parents=True,exist_ok=True)
    env=parity.env(); objects=[]
    sources=SOURCES+[
        'rebuild/src/compiled/00/9e/CPixelFormat_SetD3DFormat_009e3830.cpp',
        'rebuild/src/compiled/00/9a/CSystemManager_GetGlobal_009a4ec0.cpp',
        'rebuild/tests/integration/UiTextureDevice_test.cpp']
    for i,source in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects,'kernel32.lib'],env)
    try:
        output=run([exe],env)
        accepted='UI_TEXTURE_DEVICE PASS cases=9 failures=0' in output
    except Exception as error:
        output=str(error); accepted=False
    (directory/'report.json').write_text(json.dumps(dict(accepted=accepted,output=output,scope='real D3D9 HAL device; recovered creation/mip accounting/surface ownership/locking/clear/release; hidden owned window, no game/presenter launch'),indent=2)+'\n')
    print(output)
    return 0 if accepted else 1
if __name__=='__main__': raise SystemExit(main())
