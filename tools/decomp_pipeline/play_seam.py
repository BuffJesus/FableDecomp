#!/usr/bin/env python3
"""Build and run a trap-linked startup, Play or system-init probe; print the first missing callee.

1. link_closure.py from CGame::Play (plus a main that calls g_Game_013B83D0.Play())
   pulls in every landed genuine source the link needs;
2. seam_traps.py turns what is still unresolved into named traps / zeroed data;
3. the seam links (/FORCE:MULTIPLE only for per-function-file COMDAT duplicates) and runs.

The first FABLETLC_SEAM_TRAP line is the next function the real game loop needs, in
execution order. Traps are scaffolding and never land.

rebuild/seam_scaffold/*.cpp are linked too: behaviourally faithful bodies for functions that
are near misses (not byte-exact, so not landed). They let the seam run past a blocked
function; each file names the retail address and why it is not landed. Delete a scaffold
file when its function lands.

Usage: python tools/decomp_pipeline/play_seam.py [--entry startup|play|system] [--out DIR] [--reindex SUBSTRING ...]
  --entry startup enters recovered WinMain with the current module handle, an
  empty command line, normal show mode, and no authored engine boundaries or Play scaffolds. Unrecovered startup calls trap;
  successful linking or returning from WinMain does not prove a frontend boot.
  --entry system probes CSystemManager::Initialise with default flags (no display,
  input, audio or font handles), C++-constructed harness globals and no Play scaffolds.
  It does not run GFMain/GFInitialise or constitute a native frontend boot.
  Linking uses the installed Windows SDK for real COM/User32/Advapi32 imports.
  --reindex refreshes rebuild/build/symbol_index.tsv for the given sources first
  (use it for anything landed since the last full symbol_index.py run).
"""
import argparse
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import verify_and_land as V  # noqa: E402

ROOT = HERE.parents[1]
# GFMain does not run in the seam, so main applies the def switches it would
# set from the retail userst.ini ("UseCompiledDefs TRUE;", "AllowDataGeneration
# FALSE;"): compiled defs, no checksums. Left at their zero/data defaults,
# AddDefClass builds and transfers a NULLDEF_ default in the defaults mode,
# which retail never does (its binary-only vector transfer then writes to the
# context's unset save stream).
# GFMain also copies AllowDataGeneration into CreatingCompiledDefs (0x0138E188,
# initialised to 1: Compile would write the compiled file) and points
# CompiledDefDirectory at "Data\CompiledDefs\" relative to the install; the seam
# reads the install's compiled defs read-only through an absolute path instead
# (FABLE_INSTALL overrides the Steam default). With compiled defs, GFMain then
# loads names.bin read-only (0x00402D63): a compiled def file only loads over the
# string table it was built with (LoadBinaryDefinitions checks its RandomID).
INSTALL = os.environ.get("FABLE_INSTALL", r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters")


def main_source(entry="play"):
    if entry == "startup":
        return ('#include "fable_boot.h"\n#include <stdio.h>\n'
                'extern "C" __declspec(dllimport) FableInstanceHandle FABLE_STDCALL GetModuleHandleA(const char*);\n'
                '// Probe the real WinMain; missing engine functions must trap.\n'
                'int main() { char commandLine[] = "";\n'
                '  int result = WinMain(GetModuleHandleA(0), 0, commandLine, 1);\n'
                '  printf("FABLETLC_STARTUP_RETURNED code=%d (not a frontend completion check)\\n", result);\n'
                '  return result; }\n')
    if entry == "system":
        return ('#include "fable_system_manager.h"\n#include <stdio.h>\n#include <process.h>\n'
                '// Diagnostic inputs: C++ default construction and retail default flags.\n'
                '// This isolates real Initialise; it does not claim full GFMain startup.\n'
                'CSystemManager GSystemManager;\nCScratchBuffer GScratchBuffer;\n'
                'int main() { CSystemManagerInit init; init.WindowsShow=0;\n'
                '  ESystemCode code=GFGetSystemManager()->Initialise(&init);\n'
                '  printf("FABLETLC_SYSTEM_INITIALISE code=%d flags=%lu font=%p display=%p\\n",\n'
                '    (int)code,init.Flags,GFGetSystemManager()->PFontManager.Data,\n'
                '    GFGetSystemManager()->PDisplayManager.Data); fflush(stdout);\n'
                '  _exit(code==kSystemCodeSuccess?0:1); }\n')
    compiled = (INSTALL.rstrip("\\") + "\\Data\\CompiledDefs\\").replace("\\", "\\\\")
    return ('#include "fable_cgame.h"\n#include "fable_definition_manager.h"\n'
            '#include "fable_crc.h"\n'
            '// Retail constant data at 0x0129A168, generated from its polynomial.\n'
            '// A zero-data trap makes every registered class CRC zero and leaves\n'
            '// GlobalIndices empty when loading the compiled definition file.\n'
            'fable_u32 g_FableCRCTable_0129A168[256];\n'
            'static void InitialiseCRC() {\n'
            '  for (unsigned i = 0; i < 256; ++i) {\n'
            '    fable_u32 value = i;\n'
            '    for (unsigned bit = 0; bit < 8; ++bit)\n'
            '      value = (value >> 1) ^ ((value & 1) ? 0xEDB88320u : 0);\n'
            '    g_FableCRCTable_0129A168[i] = value;\n'
            '  }\n'
            '}\n'
            'struct SeamDefSwitches : CDefinitionManager\n'
            '{ static void Apply() { UsingCompiledDefs = true; CreateChecksums = false;\n'
            '  CreatingCompiledDefs = false;\n'
            '  CompiledDefDirectory = CWideString(L"' + compiled + '");\n'
            '  GDefStringTable.TableFilename = CompiledDefDirectory + L"names.bin";\n'
            '  GDefStringTable.LoadTable(true); } };\n'
            'int main() { InitialiseCRC(); SeamDefSwitches::Apply(); g_Game_013B83D0.Play(); return 0; }\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--out", default=os.path.join(os.environ.get("TEMP", "."), "fable-play-seam-run"))
    ap.add_argument("--reindex", nargs="*", default=[])
    ap.add_argument("--entry", choices=["startup", "play", "system"], default="play",
                    help="Recovered WinMain startup, Play reachability, or isolated system initialization")
    args = ap.parse_args()
    out = Path(args.out)
    # link_closure reuses an object when its source is older, which misses header edits;
    # a full seam compile takes ~15 s, so always start from clean objects.
    shutil.rmtree(out / "obj", ignore_errors=True)
    out.mkdir(parents=True, exist_ok=True)
    for sub in args.reindex:
        subprocess.run([sys.executable, str(HERE / "symbol_index.py"), "--only", sub], check=True)
    main_cpp = out / "seam_main.cpp"
    main_cpp.write_text(main_source(args.entry))
    scaffolds = sorted((ROOT / "rebuild" / "seam_scaffold").glob("*.cpp")) if args.entry == "play" else []
    if scaffolds:
        print("scaffolds: " + ", ".join(p.name for p in scaffolds))
    extra = [a for p in [main_cpp, *scaffolds] for a in ("--extra", str(p))]
    seed = {
        "startup": "00/40/Global_WinMain_00403480.cpp",
        "play": "00/41/CGame_Play_00412f90.cpp",
        "system": "00/9a/CSystemManager_Initialise_009a6610.cpp",
    }[args.entry]
    closure = subprocess.run([sys.executable, str(HERE / "link_closure.py"), "--seed",
                              seed, *extra, "--out", str(out)],
                             capture_output=True, text=True)
    summary = [l for l in closure.stdout.splitlines() if l.startswith(("round", "frontier"))]
    print("\n".join(summary[-2:]))
    if closure.returncode:
        print(closure.stdout[-2000:], closure.stderr[-2000:])
        return 1
    subprocess.run([sys.executable, str(HERE / "seam_traps.py"), str(out / "frontier.txt"),
                    str(out / "seam_traps.cpp")], check=True)
    env = V.env()
    sdk_root = Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")) / "Windows Kits/10/Lib"
    sdk_libs = sorted((p for p in sdk_root.glob("*/um/x86") if (p / "ole32.lib").exists()),
                      key=lambda p: tuple(int(n) for n in p.parents[1].name.split(".")))
    if sdk_libs:
        env["LIB"] = env.get("LIB", "") + ";" + str(sdk_libs[-1])
    V.cl(["/nologo", "/c", "/O2", "/MT", f"/Fo{out / 'obj' / 'seam_traps.obj'}", str(out / "seam_traps.cpp")], env)
    objs = [str(out / "obj" / (Path(s).stem + ".obj")) for s in (out / "closure_sources.txt").read_text().split()]
    objs += [str(out / "obj" / (p.stem + ".obj")) for p in scaffolds]
    objs += [str(out / "obj" / "seam_main.obj"), str(out / "obj" / "seam_traps.obj")]
    alts = re.findall(r'/alternatename:([^"]+)"', (out / "seam_traps.cpp").read_text())
    (out / "seam.rsp").write_text("\n".join('/ALTERNATENAME:"%s"' % a for a in alts) + "\n")
    # The object list outgrew the 32K command line: it goes in its own response file.
    (out / "seam_objs.rsp").write_text("\n".join('"%s"' % o for o in objs) + "\n")
    link = subprocess.run([str(V.VC / "bin" / "link.exe"), "/nologo", "/subsystem:console", "/FORCE:MULTIPLE",
                           f"/out:{out / 'seam.exe'}", f"@{out / 'seam_objs.rsp'}", "kernel32.lib", "ole32.lib", "user32.lib", "advapi32.lib",
                           f"@{out / 'seam.rsp'}"],
                          capture_output=True, text=True, env=env)
    errors = [l for l in link.stdout.splitlines() if "error" in l and "LNK4006" not in l]
    if link.returncode or errors:
        print("LINK FAILED")
        print("\n".join(errors[:20]))
        return 1
    run = subprocess.run([str(out / "seam.exe")], capture_output=True, text=True, timeout=120)
    print(run.stdout[-3000:].rstrip())
    print(f"seam exit {run.returncode}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
