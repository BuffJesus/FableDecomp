# Toolchain & exact commands

*Everything needed to reproduce the RE environment. Adapted from Fable2RE's TOOLCHAIN.md, minus
the Xbox/recomp parts (not applicable to a native x86 exe).*

## Target binary
- `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe`
- **PE32, x86-32, ImageBase `0x400000`**, 6 sections (`.text` 14.8 MB, `.rdata`, `.data`, `.idata`,
  `_PDATA`, `.rsrc`). **No packer / no SteamStub** — verified this session. Ghidra loads it directly.
- Origin: PC build of the Xbox codebase (`_PDATA` section + `titleid 4D530937` in `Fable.exe.cfg`).
- Era compiler: ships `msvcr71.dll` / `msvcp71.dll` → **MSVC 2003 (VC7.1)**. Use this when compiling
  oracle/test functions to match codegen (Phase 4).

## Ghidra
- Ghidra **12.1 PUBLIC** — installed at **`D:\Subuwu\tools\ghidra-public`** (verified: this is the
  real path; the `D:\ghidra_12.1.2_PUBLIC` in Fable2RE's docs is stale). Java 21 (Temurin). The
  REPlugins were built for 12.1.2 → their `extension.properties` version is edited to `12.1` to load here.
- **Extensions:** install `REPlugins/GhidraMCP` (from `D:\Documents\Fable2RE\REPlugins\GhidraMCP`) —
  HTTP MCP server on **port 8089**, ~249 RE-automation tools for AI-driven decompile/rename/xref.
  **Do NOT install XEXLoaderWV** — that's Xbox-only; TLC is a native PE Ghidra reads natively.
- **Headless import** (verified command, run 2026-07-17):
  ```
  D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat D:\Documents\FableTLC\ghidra_proj FableTLC ^
    -import "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe" ^
    -max-cpu 12
  ```
  Log → `D:\Documents\FableTLC\ghidra_out\analyze.log`. Ghidra auto-detects `x86:LE:32:default` +
  Windows compiler spec. Full auto-analysis of the 14.8 MB `.text` takes a while (not "minutes").
- **Enable the RTTI analyzer** (MSVC RTTI is intact → recovers many C++ class names/vtables).
- **Seed real names/types:** `python tools\fse_import\fse_manifest_to_ghidra.py` → in the GUI,
  `File > Parse C Source` on `tools\fse_import\fse_api.h`, then *Analysis > Apply Function Datatypes*.
- **Verify GhidraMCP:** `curl http://127.0.0.1:8089/check_connection` (expect ghidra_version 12.1.2).

## Ghidra scripting suite (`tools/ghidra_scripts/`, copied from Fable2RE)
Point Ghidra's Script Manager at this dir (Script Directories → add). Key scripts:
- `LabelApply.java` / `LabelDump.java` — TSV-driven mass (re)naming; `labels_*.tsv` in `ghidra_out/`
  is the reproducible source of truth for the DB (git-track the TSV, not the `.rep`).
- `FindStrXref.java` / `FindStrRaw.java` / `FindRefs.java` — string→function pinning (primary anchor).
- `FindLuaNatives{,2,3,4,5}.java` / `FindLuaLibs.java` — locate the game's own Lua binding tables.
- `DecompFuncs.java` / `DisasmRange.java` / `DumpFuncsInRange.java` — batch decompile/disasm to files.
- `FindAccessors.java` / `FindAllCodePointers.java` / `FindCodeAddrTaken.java` — vtable/accessor discovery.
- `ParsePdataInBlobs.java` / `DumpAllPdata.java` — exploit the `_PDATA` section for function bounds.
- Probes: `WhatIs.java`, `NameAt.java`, `ReadStr.java`, `ReadMem.java`, `ListSyms.java`, `FindValueInData.java`.

## Live analysis (native — no cdb guest math needed)
- **x64dbg** (32-bit `x32dbg`) or WinDbg to breakpoint engine functions and confirm signatures live.
- **FSE Lua probes** — fastest dynamic check: write a small quest/entity Lua (via FSE or FQT) that
  calls a suspected API and observe `FSE/FableScriptExtender.log`. The FSE runtime log already prints
  `g_fableBase` (`0x400000`) and vtable addresses.

## FSE source & manifest
- Source: `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\` (read, don't rebuild
  unless extending). Hook site `0xCDB355`; `g_fableBase`-relative pointers = engine call targets.
- Manifest → header/roadmap: `python tools\fse_import\fse_manifest_to_ghidra.py` (see §Ghidra above).

## auto-re-agent (Phase 4, optional)
- `D:\Documents\Fable2RE\auto-re-agent` — AI+Ghidra parity-checked function reversing (produces
  compileable C). See its `docs/getting-started.md` / `configuration.md`.

## FSE address extraction
- `powershell -ExecutionPolicy Bypass -File tools\ExtractFseAddresses.ps1` reads the local FSE source's
  literal `ASLR<...>(0x...)` engine addresses and writes:
  - `refs\fse_engine_addresses.tsv` (all extracted function/data/vtable addresses)
  - `ghidra_out\labels_fse_addresses.tsv` (conservative function labels for later `LabelApply.java`)

## Lua tooling (`tools/lua_mod/`, copied from Fable2RE)
- `luadis.py` / `luadis51.py` — Lua 5.0/5.1 bytecode disassembler (confirm TLC's Lua version first).
- `script_index.py`, `bnk_repack.py`, `apply_mod.py` — script catalog + archive repack patterns.
