# Content authoring plan — scripts, quests, dialogue, cutscenes, and Blender

*Created 2026-07-19. Grounded in docs/ECOSYSTEM.md, docs/COMPREHENSIVE_MODDING_ENVIRONMENT.md,
the FSE API manifest (`tools/fse_import/fse_api_index.md`), and the EgoCore source tree.
Answers: how do the "content" systems fit into FableForge, and where does Blender plug in.*

## Where each system actually lives (evidence)

| System | Native (retail engine) | Runtime mod layer (today) | Asset/format tool |
|---|---|---|---|
| **Scripts / quests** | C++ `NScript::` classes (`CQ_*`, `CV_*`, `CGlobal_*`, `CScriptThing`, `CScriptBase`, spawned funcs) compiled into `script.bin` (611 entries) | **FSE Lua** — 843-fn Quest API + 88-fn Entity API. Retail embeds NO Lua; FSE adds it. FQT already generates this. | `forge::bin` reads script.bin; FSE manifest = the API contract |
| **Dialogue / text** | Babel text system; speech via `SpeakAndWait`, subtitles via `CSubtitleRenderer::SetText` | FSE `AddScreenMessage`, `SpeakAndWait`, quest-info bars | `data\lang\English\text.big` (2.8 MB), `dialogue.big` (550 KB), fonts.big — EgoCore parses banks + `LipSyncParser` |
| **Cutscenes** | native cutscene macro (`RunCutsceneMacro_Func` @ `0x00CBFB7D`, 74 KB — now decompiling) | FSE `Camera*` family (`CameraCircleAroundThing`, `CameraDefault`, …) + `Speak*` | camera/timeline is script-driven |
| **Meshes / anims** | BBM mesh + anim formats | — | **EgoCore: full round-trip** — `MeshParser`/`MeshCompiler`, `AnimParser`/`AnimCompiler`, `GltfExporter`/`GltfMeshImporter`/`GltfAnimImporter`, `LipSyncCompiler`+`SpeechAnalyzer`, `TextureBuilder`, `ParticleCompiler`, font builders |

Key consequences:
- **Quests/dialogue/cutscenes are authored as scripts.** The pragmatic path is FSE Lua (proven, safe,
  live-deployable) — not native patching. The native decomp is the long-horizon way to edit *shipped*
  quests/cutscenes beyond FSE's fixed API.
- **The Blender round-trip already exists in code.** EgoCore has both glTF import *and* the BBM/anim
  compilers, so Blender ⇄ Fable for custom meshes/animations is a wrapping/orchestration job, not a
  from-scratch RE job. This is the single biggest unlock and was previously assumed hard.

## FableForge integration — staged, each stage ships value

### Stage A — text & script visibility (near-term, on current forgecore)
- **Dialogue / Text editor**: read `text.big` / `dialogue.big` (Babel string tables) as a searchable,
  editable table; write back through the bank compiler. Highest value-per-effort (translations, line
  rewrites, name changes) and it reuses the `forge::bin`/bank machinery already shipped.
- **Quest browser (read-only)**: index `script.bin` (611) + the `NScript` quest classes; show quest
  IDs, the things they bind, and their region/objective structure. Orientation before authoring.

### Stage B — quest & script authoring via FSE (the practical "quests now" path)
- A **visual quest/script editor** in FableForge that emits **FSE Lua**, validated against the 931-fn
  API manifest (unknown call = editor error). This folds in FQT's proven approach as a native panel.
- Node/graph model: triggers → objectives → actions (spawn, speak, give item, camera, screen message),
  compiled to Lua. Live-deploy + test through FSE; iterate without restarting.
- Deploy uses the loose-file + FSE script path already understood (`forge stage`).

### Stage C — dialogue lines + cutscene sequencer
- **Dialogue authoring** wired to the text table: assign lines to speakers, preview, and — via
  EgoCore `SpeechAnalyzer`/`LipSyncCompiler` — generate lipsync from a WAV so custom voice lines animate.
- **Cutscene / camera sequencer**: a timeline UI that emits FSE `Camera*` + `Speak*` + effect calls.
  Later, the decompiled native cutscene macro lets us *import* shipped cutscenes into the same timeline
  for editing (parity target: `RunCutsceneMacro_Func`).

### Stage D — native quest/cutscene decomp (long-horizon, from FULL_DECOMP)
- Decompile the native script VM + cutscene macro so FableForge can edit the game's *own* quests and
  cutscenes natively, past FSE's fixed API. Uses the lift harness (docs/LIFT_HARNESS.md); the chest/
  reward subsystem is the warm-up, the script VM is the prize.

## Blender integration — concrete, because the compilers exist

EgoCore is MIT; FableForge already vendors its format knowledge. Blender speaks glTF natively, and
EgoCore has glTF on both sides — so:

### B1 — View / reference (basically free now)
- "Open in Blender" action: FableForge calls EgoCore `--batch <bank.big> <mesh> [anim] out.gltf`, then
  launches Blender on the result. Read-only inspection of any Fable mesh + animation. Ships immediately.

### B2 — FableForge Blender add-on (custom meshes/anims — the real unlock)
- A Python add-on (installed into Blender) with:
  - **Import**: pull a Fable mesh/anim into Blender (glTF via EgoCore).
  - **Export**: hand the edited glTF back to EgoCore `GltfMeshImporter` + `MeshCompiler` /
    `GltfAnimImporter` + `AnimCompiler` → BBM, then repack into the target `.big` (EgoCore
    `BigBankCompiler`; FableForge orchestrates + validates).
- This gives **custom models and animations in-game** through a Blender-native workflow. The gating
  work is orchestration + validation, not new RE — the compilers are written.
- Textures via `TextureBuilder`, particles via `ParticleCompiler`, fonts via the font builders come
  along for free on the same bridge.

### B3 — Level layout in Blender (powerful, uses what we built)
- Export a level as a Blender scene: LEV terrain heightmap as a mesh, TNG things as empties/props at
  their real positions (we already parse both). Lay out placements visually in Blender, then export
  back to TNG — `forge::tng` mutation already round-trips byte-safely. Blender becomes an optional
  level-design front end over the same safe TNG writer.

### B4 — Unreal path (adjacent, already present)
- EgoCore `--batch-ue` / `--batch-many-ue` emit UE-oriented glTF (the UE6Verse/TOA angle). Not the
  primary target, but the same bridge serves an Unreal viewer/remaster experiment.

## Honest gating risks
- **Text/Babel write-back**: confirm the bank compiler round-trips `text.big` byte-safely before
  shipping the dialogue editor (same discipline as the `game.bin` semantic round-trip already proven).
- **glTF round-trip fidelity**: EgoCore has the compilers, but skeleton/weight/material edge cases need
  a byte/behavior oracle (extract → edit-nothing → recompile → diff) before promising custom rigs.
- **FSE API coverage**: cutscene/dialogue authoring is only as capable as the 931-fn surface until the
  native VM is decompiled; document what FSE can't yet express so the editor doesn't over-promise.
- **Native quest/cutscene editing** (Stage D / B-native) is genuinely long-horizon and gated on the
  VC7.1 lift harness.

## Recommended order
1. **Dialogue/Text editor** (Stage A) + **Blender B1 view** — both cheap, high-visibility, on current tooling.
2. **FSE quest authoring** (Stage B) — the headline "make quests" feature, via the proven Lua path.
3. **Blender B2 add-on** (custom meshes/anims) — the biggest creative unlock; wrap EgoCore compilers.
4. **Cutscene/dialogue sequencer** (Stage C) + **Blender B3 level layout**.
5. **Native script-VM / cutscene decomp** (Stage D) — long-horizon, via the lift harness.
