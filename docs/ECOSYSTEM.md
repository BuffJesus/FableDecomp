# Fable TLC RE / modding ecosystem — survey

*Researched 2026-07-17. What already exists, so we build on it instead of re-deriving.*

TLC is a ~20-year-old, well-modded PC game. Unlike Fable 2, the community has already done a huge
amount of the work — especially on **data formats** and **Lua scripting**. Our job (native-exe RE)
is the missing complementary piece: *how the engine itself works*, not *what the data bytes mean*.

## The user's own local tools (integrate directly)

| Tool | Location | Role |
|---|---|---|
| **FableScriptExtender (FSE)** source | `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\` (46 `.cpp/.h`) | The injection DLL + reversed C++ API. Hook at `0xCDB355`, `g_fableBase`-relative engine pointers. Our #1 symbol source and the framework we extend. Installed & running in the game (`.../FSE/`). |
| **FSE API manifest** | `refs/fse_api_manifest.json` (copied) | 931-function catalog, `generated-from-header`. → `tools/fse_import/` turns it into `fse_api.h` (Ghidra) + `fse_api_index.md` (roadmap). |
| **FQT (Fable Quest Tool)** | `D:\Code\FQT\FQT\` (WPF/.NET 8) | Visual quest editor → FSE Lua. Downstream consumer: decompiled name-tables can auto-feed its hand-curated `GameData.cs`. |
| **EgoCore** (source + patched headless CLI) | `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\` | ★ **The completed data-format RE** — see §EgoCore below. Same author as FSE (eeeeeAeoN). Full `.big`/`.lut`/`.lug`/BBM-mesh/anim/LUG-MET-audio/font/lipsync/WAD/def parsers + compilers in C++. Our patched build runs **headless only** (`--list/--batch/--batch-many/--extract-textures/--extract-audio`). |
| **Fable asset staging** | `D:\Code\Fable\{TLC,ChocolateBox,TOA,Anniversary}` | Extracted region/character assets (TLC = this game; ChocolateBox = Fable2; TOA = an Unreal project). Cross-ref for format work. |
| **Fable2RE** | `D:\Documents\Fable2RE\` | Sibling project. Source of the Ghidra scripting suite, Lua tooling, and the doc/RE methodology (see `docs/METHODOLOGY.md`). |

## EgoCore — the data-format Rosetta stone (use as ground truth)

EgoCore (by **eeeeeAeoN**, the FSE author) is "over twenty years of obsession with the inner workings
of Fable" turned into an open-source C++ modding framework. For *this* project it is the single most
valuable cross-check for the **asset/resource half** of the engine RE: EgoCore's parsers already say,
in readable C++, exactly what every Fable file format contains. When we decompile the engine's own
loaders in Ghidra, EgoCore's source is the answer key.

- **Source (RE reference):** header-per-subsystem C++ under `EgoCore-master\EgoCore\`:
  `Banks/` (`BankLoader`, `BigBankCompiler`, `BinaryParser` — `.big`/`.lut`), `Meshes/`
  (`BBMParser`, `MeshParser`, `GltfExporter` — the BBM mesh format), `Animations/`
  (`AnimParser`, `AnimCompiler`), `Audio/` (`LugParser`, `MetParser` — `.lug`/`.met` banks),
  `Definitions/` (`DefBackend`, `CompilerBackend` — the game's `.def`/CompiledDefs), `Levels/`
  (`WADBackend` — `FinalAlbion.wad` → `.lev`/`.tng`), `Fonts/`, `Lipsync/`, and an `FSE/` tab
  (`FSEBackend`, `FSEAutosuggest`) that bridges Lua to the internal Fable API — so EgoCore also
  encodes FSE-API knowledge.
- **Patched headless CLI (this machine):** our local build has `main.cpp` patched to dispatch to
  headless routines. **★ GUI TRAP — never run `EgoCore.exe` bare or with a wrong arg count; it falls
  through to the ImGui GUI and hangs the shell.** Every call must start with a headless flag:
  - `--list <bank.big>` — dump `idx␉type␉size␉id␉name␉friendly` for every entry.
  - `--batch <bank.big> <mesh> [<anim>] <out.gltf>` — export mesh (+anim) as glTF.
  - `--batch-many <bank.big> <plan.tsv>` — many extractions, bank loaded once.
  - `--extract-textures <bank.big> <outdir>` — bulk `.dds` dump.
  - `--extract-audio <bank.lug> <outdir> [filter]` — `.lug` → PCM16 `.wav` + `manifest.tsv`.
  - Two `x64\Release\EgoCore.exe` may exist (repo-root vs inner) — use the newest. Rebuild recipe +
    all vcxproj/main.cpp patches are in memory `egocore-headless-build`.
- **Role in the decomp plan:**
  1. **Cross-check the resource subsystems** — decompile the engine's `.big`/`.tng`/`.lev`/mesh/anim/
     audio/def loaders against EgoCore's parsers; agreement = confidence, disagreement = a real finding.
  2. **Generate test fixtures** — extract known assets headless to validate that a reversed loader
     produces the same bytes (a data-side "oracle", complementing the VC7.1 code oracle).
  3. **Feed data tables** — extracted `--list`/def dumps populate FQT `GameData.cs` and our own
     name tables. (This is already the extraction engine behind the UE6Verse Fable pipeline.)

## Community projects (reference / cross-check)

| Project | What | Use to us |
|---|---|---|
| **fabletlcmod.com** (wiki + forums) | ~20 yrs of format RE: `.big`/`.bin` archives, `.lev`, `.tng`, `.stb`, `.gtg`, `.wad`, `def.xml`, `CompiledDefs`, graphics/banks | The data-format bible. Ghidra shows the *parser*; the wiki gives the *byte layout*. Mirror key pages to `refs/formats/`. |
| **Fable Explorer** (Nexus mods/2, GameBanana 2482) | BIG/BIN editor, FMP mod packages, `script.bin` editing | The de-facto data-mod tool. Our native RE complements it. |
| **FableTools 0.7** | BIZ↔BIG conversion, WAD import/export | Archive round-tripping. |
| **Fable TLC Toolpack** (Nexus mods/4) | "Lost & forgotten" tool collection | Grab-bag of older utilities. |
| **Wyntilda/Unofficial-Fable-Patch** (GitHub) | Bug-fix RE effort | Documented bugs/addresses = cross-check for our findings. |
| **eeeeeAeoN/FableScriptExtender** (GitHub, v0.75) | Upstream FSE | Watch for updates; the local copy is our working source. |
| **Keshire/Fable3LUADecompiler**, **JustSomeGuy1234/Fable2Modding** | Sibling-game Lua RE | Shared Lua lineage across Fable 1/2/3. |
| **Shaw, "Lua in Fable II"** (`refs/…LuaInFableII.pdf`) | Lionhead's own Lua-in-Fable design paper | The scripting VM's design intent, shared with TLC. |

## Division of labour (why this project is worth doing given all the above)

The community solved **data**. FSE solved **scripting access** (a fixed set of ~931 API calls). What
is *still missing* and only a native-exe decompilation provides:

1. **The engine internals between the known edges** — how World/ThingManager/Hero/combat/Will/save
   actually work, so mods can do things FSE's fixed API and Fable Explorer's data edits cannot.
2. **A larger, documented native modding surface** — beyond FSE's current wrappers.
3. **An automated name/def-table extraction** to feed tools like FQT (which currently hand-curate).
4. **Study-grade source** of chosen subsystems (Phase 4).
