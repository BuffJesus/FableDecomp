# Fable TLC — Visual Effects / Particle System Data (effects.big)

Status 2026-07-20: container + payload format **fully decoded empirically**
(1,165/1,165 entries parse byte-exact, zero slack, strict terminators), reference
chain mapped def→effect→texture, reader shipped (`tools/parse_effects.py`).
Answer key: EgoCore `Particles/ParticleParser.h` + `ParticleCompiler.h`
(`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) — checked FIRST per
the CLAUDE.md gotcha; it is byte-exact against retail.

## 1. Where the data lives

| File | Role |
|---|---|
| `data\Misc\pc\effects.big` | THE effects bank. BIGB container (TEXTBIG_FORMAT.md), one sub-bank `PARTICLE_MAIN_PC` (version 111), 1,165 entries, all type 0, payloads **uncompressed** (no LZO, unlike textures/meshes) |
| `data\Defs\RetailHeaders\pc\particles.h` | BankCreator-generated `enum EParticleEmitter` — entry-id ↔ NAME map for effects.big (id == BIG entry id) |
| `data\Defs\RetailHeaders\pc\particles_frontend.h` | same for the (tiny) frontend particle set |
| `data\graphics\pc\textures.big` `GBANK_MAIN_PC` | sprite textures the emitters index (`SpriteBankIndex`, `TrailBankIndex`, `DecalBankIndex`) — id space of `textures.h` `enum EEngineGraphic` |
| `data\graphics\graphics.big` `MBANK_ALLMESHES` | meshes used by `CPSCRenderMesh.BankIndex`; also the mesh HELPER dummies that auto-spawn emitters (see §4) |

## 2. Entry payload = one serialized `CParticleEmitter`

Little-endian stream; `EBOOL` = 1 byte; `cstr` = NUL-terminated; colours stored
**B,G,R,A**. Quantized floats are stored as u32: `value = raw/maxq*scale - bias`
(encode is the exact inverse, see EgoCore `ParticleCompiler.h`).

```
u32   magic            (0x64 — bank version, same as BIGB header version 100)
cstr  name             (display name, e.g. "BrazierFireFinal")
EBOOL Emitter2D, PreWaterEffect, WaterEffect, ZBufferWriteable,
      ContinuousEmitter, IsScreenDisplacement, ReadZBuffer
f32   MaxSpawnDistance, MaxDrawDistance, FadeOutStart, FadeInEnd, FadeInStart
i32   Priority
EBOOL DieIfOffscreen, OffscreenUpdate, ClipEffectToWeatherMask,
      EnableDithering, CalcBoundingSphereOnceOnly
u32   systemCount
  per system:
    cstr name; EBOOL Enabled, ScaleParticles; f32[3] Scale
    u32 componentCount
      per component:
        cstr className; u32 instanceID; EBOOL enabled
        <component fields — see parse_effects.py / ParticleParser.h>
        u8 terminator = 0x7B ('{')
    u8 terminator = 0x26 ('&')
```

10 component classes appear in retail (histogram over all 1,165 entries):
`CPSCUpdateNormal` 4555 (physics/lifetime), `CPSCEmitterGeneric` 4540 (spawn
shape/rate/direction), `CPSCRenderSprite` 3266 (billboard renderer),
`CPSCRenderMesh` 1222, `CPSCSpline` 360, `CPSCOrbit` 205, `CPSCSingleSprite` 196,
`CPSCLight` 151 (attached point light), `CPSCAttractor` 47, `CPSCDecalRenderer` 44.
A typical effect = one system per visual layer, each with
Emitter + Update + Render components (e.g. torch = Fire/Smoke/Fire2 systems).

Quantization table (u32 → float), from ParticleCompiler and byte-validated:
sizes `/2047*20`, alpha/size fade minima `/127`, flicker bias `/255*2-1`,
anim time `/16383*99.9+0.1`, trail width `/1023*10`, flicker speed `/4095*30`,
particles/sec `/16383*100`, emitter size & min/max speed `/1023*10`,
radial bias `/1023*10-5`, timeline `/32767*30`, life & start `/32767*300`,
non-uniform scale `/2047*20-10`, custom direction `(raw&0xFF)/255*2-1`.

## 3. Reference chain (how the game finds an effect)

Two id spaces, one bank:
- **By id (int32)** — game.bin def fields hold the `EParticleEmitter` value =
  effects.big entry id. ~40 def types do this; examples:
  `CChestDef.OpenParticleEffect`, `CSmashableDef.SmashParticleEmitter`,
  `CSmokeGeneratorDef.ParticleEmitter`, `CHighlightItemDef.ParticleEmitter`,
  `CThingShotDef.PrimaryEffect/AttachedContinuousEffect` (spells/projectiles),
  `CMaterialDef.SwordHitParticleEffect` (Vector_J of ids),
  `CGlobalDef.TeleportIn/OutParticleEffect`, `CObjectAugmentationsDef.*Effect`,
  `CExperienceDef.PickupParticleEffectsPerExperiencePoints` (Map id→id).
  Verified: `CChestDef` entry 9240 `OpenParticleEffect = 391` == `CHEST_OPEN_01`.
- **By name (string)** — placed level emitters: TNG `Thing` with
  `DefinitionType "PARTICLE_EMITTER_PLACEABLE/NORMAL"` carries
  `StartCTCDParticleEmitter; ParticleTypeName "SUNBEAMS"; EndCTCDParticleEmitter`.
  Also mesh CREATEPARTICLE helpers (§4) and `CCoopSpiritDef.*Particle`
  (CDefString) reference by name.
- **CParticleAttacherDef** (game.bin, 27 entries, attached to creature OBJECTs):
  `ParticlesToAttach = vector of CParticleAttachmentInfo`, each record 13 bytes:
  `u32 particleId` (EParticleEmitter), `u32 helperNameOffset` (offset into
  `CompiledDefs\names.bin`, +0x14 file header; −1 = none; e.g. "EMITTER_LOWERARM"),
  `f32` (10.0 in all inspected — likely max distance/scale), `u8` (1 —
  NParticleAttachment type). Verified vs earth-troll drips (ids 209/210/211/271).
- Inside an emitter: `SpriteBankIndex/TrailBankIndex/DecalBankIndex` →
  textures.big `GBANK_MAIN_PC` entry id (= `textures.h` EEngineGraphic);
  `CPSCRenderMesh.BankIndex` → graphics.big mesh id;
  `CPSCUpdateNormal.DecalEmitterName` → another effects.big entry by name.

`OBJECT.Components` (CThingComponentSet) decode note: `u32 count`, then count ×
9-byte records `{u32 names.bin offset of CTC class name, u32 param, u8 flag}`,
then a constant-length tail (≈100 B) of `{hash,bool}`-looking pairs shared by all
OBJECTs (only bool bytes differ) — tail semantics not yet pinned (Ghidra item).

## 4. Worked example — the torch flame

`OBJECT_BANDIT_RAG_TORCH_LIT` (game.bin entry 2961):
1. `Components` includes `CTCLight` + `CTCMeshAutomaticEntityCreator`.
2. `Graphic` (CEngineGraphic) id **4594** → graphics.big `MBANK_ALLMESHES`
   entry 4594 `MESH_OBJECT_BANDIT_RAG_TORCH_LIT`.
3. That mesh's helper block contains dummy `CREATEPARTICLE` with string
   **`BRAZIERFIREFINAL`** (plus `LIGHT_POINT` for the CTCLight and
   `ACTION_POINT_1`) — the auto entity creator spawns the emitter there.
4. effects.big entry **926 `BRAZIERFIREFINAL`** ("BrazierFireFinal", 1,404 B):
   3 systems — *Fire* (RenderSprite sprite 4404, start colour RGBA
   255,217,147,127 → mid 255,104,32 → end red, additive blend 3, 10 p/s,
   life 1.6 s), *Smoke* (sprite 4404, blend 2, 2 p/s, life 4.2 s, grows
   0.23→0.86), *Fire2* (sprite 4469, 10 p/s).
5. Sprites: textures.big `GBANK_MAIN_PC` 4404 = `SPRITE_SMOKEPUFF_LONG_02`,
   4469 = `SPRITE_FLAME_BUNCH_02`.

To make every bandit rag-torch burn green today: decode entry 926, set the Fire
systems' Start/Mid colours, re-serialize with the ParticleCompiler field order
(or EgoCore GUI), and `big_write.rebuild(effects.big, edits={926: payload})` —
effects.big round-trips byte-exact (BIG_ROUNDTRIP.md) and payloads are
uncompressed, so no compressor is needed. Placed-emitter swaps are even easier:
edit `ParticleTypeName` in the TNG (plain text).

## 5. Tooling

- `tools/parse_effects.py <effects.big> --list | --extract out.json [--id N |
  --name SUBSTR] | --validate` — full reader, strict (terminators 0x7B/0x26
  enforced, consumed==size). Validation: **1165 exact / 0 slack / 0 failed**.
- Writer path: EgoCore `ParticleCompiler.h` is the exact inverse (same field
  order, quantization encode) + `tools/big_write.py rebuild(adds=/edits=)` for
  the container. EgoCore also has `--extract-particles <effects.big> <out.json>`
  headless mode in `main.cpp`, but the checked-in
  `x64/Release/EgoCore.exe` (built Jul 11) PREDATES it (added Jul 17) — rebuild
  needed before that flag works; parse_effects.py covers the read side without it.

## 6. Editable today vs needs Ghidra

Editable now (no Ghidra):
- Every emitter parameter (colours, rates, sizes, lifetimes, blend modes,
  lights, decals, splines) — decode + inverse-quantize + repack.
- Which effect an object/spell/chest/material uses — int32 id fields in game.bin
  defs (forge defs merge can patch them field-level).
- Placing/swapping world emitters — TNG text (`ParticleTypeName`).
- New emitters: append a new entry (new id, name) to PARTICLE_MAIN_PC via
  big_write `adds=`; defs reference by int id, TNG by name. (Untested in-game;
  BIG add-entry path itself is proven for graphics.big meshes.)

Ghidra follow-ups (lock was taken this run):
1. Confirm loader quantization constants + terminator-byte handling in Fable.exe
   (xref strings "CPSCRenderSprite" etc.; validates the EgoCore scales
   independently and whether 0x7B/0x26 are checked or skipped).
2. `CThingComponentSet` trailing {hash,bool} table — hash fn + semantics.
3. `CParticleAttachmentInfo` field 3 (f32) and field 4 (u8 NParticleAttachment
   enum values) semantics.
4. `CTCMeshAutomaticEntityCreator` — how CREATEPARTICLE helper args are parsed
   (arbitrary emitter names vs a filtered set), and whether `LIGHT_POINT`
   coords come from the same block.
5. Runtime cap on particle detail (`CConfigOptionsDefaultsDef.EffectsDetail`)
   interaction with `Priority`/`MaxDrawDistance`.
