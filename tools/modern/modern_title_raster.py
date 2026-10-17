"""Fork step 1 on real reconstructed code: the parity lane's retail-title raster, rebuilt with VS 2022.

The parity lane's gate (tools/decomp_pipeline/gate_native_retail_title_raster.py) compiles 17
reconstructed renderer functions with VC7.1, links them into NativeRetailTitleRaster_test.cpp and
draws FRONTEND_TITLE_01_SPRITE on the GPU (288x160 PPM + 7 control frames). This script compiles
the SAME source files, unmodified, with the current MSVC (x86 by default, --arch x64 to try 64-bit),
runs the fixture, and compares every output pixel with the VC7.1 run's frames. Nothing in the
parity tree is written; outputs go to work/modern_title_raster/<arch>/.

    python tools/modern/modern_title_raster.py [--arch x86|x64]
"""
import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
REFERENCE = ROOT / "work/codex-native-startup/native-retail-title-raster-20261010"
BANK = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\graphics\pc\frontend.big")
SOURCES = [
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawAsIndexedTris_00a0af40.cpp",
    "rebuild/src/compiled/00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp",
    "rebuild/src/compiled/00/9b/CDisplayManager_NativeRender_009bef20.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_Frame_00a0b480.cpp",
    "rebuild/src/compiled/00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_SetVertexFormat_00a0aa30.cpp",
    "rebuild/src/compiled/00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawAsIndexedTrisTextures_00a0aea0.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsTris_00a0ae10.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTris_00a0ac60.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp",
    "rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTrisTextures_00a0ab50.cpp",
    "rebuild/src/compiled/00/9f/CTexture_GetDimensions_009f9db0.cpp",
    "rebuild/src/compiled/00/9f/CTexture_GetLevelByteLength_009f9e80.cpp",
    "rebuild/src/compiled/00/9e/CPixelFormat_Initialise_009e3830.cpp",
    "rebuild/src/compiled/00/9e/CPixelFormat_GetColourDepth_009e3820.cpp",
    "rebuild/src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp",
    "rebuild/tests/integration/NativeRetailTitleRaster_test.cpp",
    "rebuild/integration/fable_texture_decode.c",
]
FLAGS = ["/nologo", "/c", "/O2", "/MT", "/W1", "/permissive", "/Zc:wchar_t-"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--arch", choices=("x86", "x64"), default="x86")
    args = ap.parse_args()
    env = json.load(open(ROOT / f"work/modern_census/env_{args.arch}.json"))
    env["INCLUDE"] = env["INCLUDE"] + ";" + str(ROOT / "rebuild/include")
    cl = shutil.which("cl", path=env["PATH"])
    link = shutil.which("link", path=env["PATH"])
    out = ROOT / "work/modern_title_raster" / args.arch
    out.mkdir(parents=True, exist_ok=True)
    objects, failures = [], []
    for i, src in enumerate(SOURCES):
        obj = out / f"{i}.obj"
        std = [] if src.endswith(".c") else ["/std:c++17"]
        r = subprocess.run([cl, *FLAGS, *std, f"/Fo{obj}", str(ROOT / src)], env=env, capture_output=True, text=True,
                           cwd=str(out))
        if r.returncode:
            failures.append((src, [l for l in r.stdout.splitlines() if "error" in l][:3]))
        objects.append(obj)
    if failures:
        for src, errs in failures:
            print("COMPILE FAIL", src)
            for e in errs:
                print("   ", e)
        return 1
    exe = out / "fixture.exe"
    r = subprocess.run([link, "/nologo", f"/out:{exe}", "kernel32.lib", "user32.lib", *map(str, objects)],
                       env=env, capture_output=True, text=True)
    if r.returncode:
        print("LINK FAIL\n" + r.stdout[-2000:])
        return 1
    frame = out / "retail-title.ppm"
    controls = [out / f"control-{i}.ppm" for i in range(7)]
    r = subprocess.run([str(exe), str(BANK), str(frame), *map(str, controls)], capture_output=True, text=True, timeout=60)
    print(r.stdout.strip())
    if r.returncode:
        print(f"fixture exit {r.returncode}", r.stderr[-500:])
        return 1
    same = []
    for mine, ref in [(frame, REFERENCE / "retail-title.ppm")] + [(c, REFERENCE / c.name) for c in controls]:
        same.append(mine.read_bytes() == ref.read_bytes())
    print(f"{args.arch}: {sum(same)}/{len(same)} frames byte-identical to the VC7.1 build's frames")
    return 0 if all(same) else 1


if __name__ == "__main__":
    sys.exit(main())
