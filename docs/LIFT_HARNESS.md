# Lift harness plan — bar #2 (buildable source), first module

*Created 2026-07-19. Prereqs and setup for the module-by-module source lift per
`docs/FULL_DECOMP.md` step 4-5.*

## Toolchain prerequisite — resolved

The exact MSVC 2003 compiler is installed at `D:\Tools\vc71` (`cl.exe`
13.10.3077). VC7.1 also successfully compiles, links, and runs a Win32 +
Direct3D 9 executable against Windows SDK `10.0.22621.0` x86 headers and
import libraries. Reproduce with `tools/ProbeVc71Sdk.ps1`, or dot-source
`rebuild/vc71_sdk_env.ps1` to activate the validated environment.

Historical acquisition options retained for provenance:

1. Visual Studio .NET 2003 installer/ISO (user-supplied; installs on modern
   Windows with compatibility settings, or into a Windows XP VM).
2. The standalone VC++ Toolkit 2003 (freeware "VCToolkit" — cl 13.10 without
   the IDE); smaller and sufficient for the oracle compiles.
3. Interim fallback: lift modules now and compile them with modern MSVC in
   C++03 mode for *behavioral* checks only; defer byte-parity to when VC7.1
   lands. (auto-re-agent's parity engine is heuristic/structural, so this
   path still produces verified-useful source.)

## Tooling chain

- `D:\Documents\Fable2RE\auto-re-agent` — reverser/checker loop, objective
  verifier, parity engine. Python 3.10+, installable via `pip install -e`.
- **ghidra-ai-bridge** (github.com/Dryxio/ghidra-ai-bridge) — auto-re-agent's
  Ghidra backend; needs installing and pointing at
  `ghidra_proj/FableTLC` / `Fable.exe`. (GhidraMCP on port 8089 is the
  alternative interface, plugin at `D:\Documents\Fable2RE\REPlugins\GhidraMCP`.)
- LLM provider: `ANTHROPIC_API_KEY` (or Codex CLI / OpenAI-compatible).
- Source-context retrieval: point the project profile at
  `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master` (API
  surface) and EgoCore (format loaders) as "nearby source".

## First module: chest/reward subsystem

Best-evidenced candidate — every function below has a verified retail address,
a donor-PDB name, decompile logs, and behavioral ground truth (FableForge
`forge chest list` / `forge defs families` + the installed game):

| Function | Retail addr | Evidence |
|---|---|---|
| `CTCChest::GetNumberOfKeysToUnlock` | `0x750050` | decomp log, FSE manifest |
| `CTCChest::DisplayLockedMessage` | `0x750110` | decomp log |
| `CTCChest::Open` | `0x750540` | decomp log, 4 call sites |
| `CTCChest::FrameUpdate` | `0x750780` | RTTI slot validation |
| `CChestDef::Transfer` | `0x4DE204` | field map decoded |
| `CContainerRewardHeroDef::Transfer` | `0x4E8987` | payload decoded vs game.bin |
| `CObjectFamilyDef::Transfer` | `0x46C294` | payload decoded vs game.bin |
| `CObjectFamilyDef::GetRandomObject` | `0x8ED590` | weighted-draw decoded |
| `CTCContainerRewardHero::GetRewardItems` | `0x74E860` | full expansion decoded |
| `CTCContainerRewardHero::GetDynamicPotionDefIndex` | `0x74E280` | decoded |
| `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` | `0x8966C0` | decoded |
| `CGameScriptInterface::GetNumberOfItemsOfTypeInInventory` | `0x897190` | decoded |
| `NScript::CGlobal_OpenChestScript::Main` | `0xEEC890` | full control flow decoded |

Parity oracle for the module: the FableForge native decoders
(`forge::bin::decodeObjectFamilies` / `decodeContainerRewards`) already
reproduce the persisted data structures these functions consume, and
`ghidra_out/installed_game/object_families.json` gives concrete input/output
fixtures (e.g. REWARD_GUARD_01's exact weighted table).

## Setup steps (once VC7.1 or the fallback is chosen)

1. `pip install -e D:\Documents\Fable2RE\auto-re-agent` into a venv; install
   ghidra-ai-bridge and point it at `D:\Documents\FableTLC\ghidra_proj\FableTLC`.
2. `re-agent init` in a new `D:\Documents\FableTLC\lift\` working dir; set the
   project profile (VC7.1 target, `__thiscall` heavy, MSVC 2003 STL).
3. Trial run: `re-agent reverse --class CObjectFamilyDef` (smallest class,
   one nontrivial method, data oracle available).
4. Grade output with the parity engine; hand-check `GetRandomObject`'s
   LCG/ROR13 random draw against the decomp log.
5. Scale to `CTCChest` / `CTCContainerRewardHero`, then the script interface
   functions.
