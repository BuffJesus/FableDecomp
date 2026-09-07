# Hero skin/morph texture chain — what the live hero ACTUALLY samples

*2026-07-20 (stage-2 smoke-test agent). Empirical, NO Ghidra (lock was taken).
Evidence: MESH_HERO material list read straight from graphics.big
(`tools/parse_mesh.py` descriptor), CHeroMorphDef decoded from game.bin
(`forge defs decode`), full texture-name census of textures.big. Motivated by
the stage-1 smoke-test miss: tinting `GRAPHIC_HERO_FACE` green produced NO
in-game change on an adult hero — wrong target.*

## 0. TL;DR — the answer

- The playable adult hero's body mesh is **`MESH_HERO`** (graphics.big
  `MBANK_ALLMESHES` id **4299**, type 5 skinned). Its 12-material list IS the
  set of sheets the hero samples:

  | tex id | name | role |
  |---|---|---|
  | 1250 | `HERO_SKIN_FACE` | **the face** (512², DXT1) |
  | 1248 | `HERO_SKIN_ARMS` | **arms + HANDS** (512², DXT1) |
  | 1252 | `HERO_SKIN_TORSO` | chest/front |
  | 1249 | `HERO_SKIN_BACK` | back |
  | 1251 | `HERO_SKIN_LEGS` | legs/feet |
  | 1253 | `HERO_SKIN_MOUTH` | mouth interior (256²) |
  | 1233–1237 | `HERO_BUMP_SKIN_{FACE,TORSO,LEGS,BACK,ARMS}` | bump layers |
  | 1045 | `HERO_EYESHADOW` (dev-path name) | eye shadow card |

  (Eyes are a separate static mesh `MESH_HERO_EYE` id 120 →
  `HERO_EYE_NORMAL` 1046.)
- **`GRAPHIC_HERO_FACE` (id 45) is referenced by NO mesh in graphics.big**
  (exhaustive descriptor scan of all 3295 compiled meshes; ditto
  `GRAPHIC_HERO_MOUSTACHE` 46). Whatever it is (GUI/dev leftover), tinting it
  cannot touch the rendered hero — this was the stage-1 failure.
- Other hero-aged meshes: `MESH_YOUNGHERO_02` (4300) → `YOUNGHERO_SKIN_FACE`
  792 / `YOUNGHERO_SKIN_TORSO` 794; `MESH_TEEN_HERO` (4301) → same
  `HERO_SKIN_*` sheets but `HERO_TEEN_FACE` 1254 instead of 1250. A mid-game
  **adult** hero uses `MESH_HERO` + `HERO_SKIN_FACE`.

## 1. The morph side: CHeroMorphDef.TextureMorphs

game.bin has 2 populated `CHeroMorphDef` entries — **10535** (4217 B) and
**11246** (3857 B) — plus null/stub entries (0, 10543). Both populated entries
carry **identical** `TextureMorphs` content (156 records). Fields (schema
`docs/re_reference/def_schema.json`): `TextureMorphs`, `SkeletalMorphs`,
`ParticleMorphs`, `IdleParticleMorphs`.

`CTextureMorphs` blob layout (decoded here, self-consistent for all 156×2
records; trailing dword is a constant 2, terminator `bb6cab01` = crc0 tag):

```
u32 count
count × 24 bytes:
    u32 statA        # hero stat driving the morph (statA == statB always)
    u32 statB
    u32 layer        # compositing layer/slot (0,2,3,4,5,7,8 observed)
    u32 texBase      # textures.big GBANK_MAIN_PC id of the RETAIL base sheet
    u32 texOverlay   # COMP_HERO_* overlay composited onto it
    u32 flag = 2
```

So the hero-morph system does NOT swap the mesh's texture id — it
**composites `COMP_HERO_*` overlay sheets onto the base `HERO_SKIN_*` sheets**
keyed by hero stats. The base sheet named in the mesh material is always the
compositing input. Observed stat → overlay families (both defs identical):

| stat | overlays (face/arms rows shown) | reading |
|---|---|---|
| 3 | FACE→`COMP_HERO_SKIN_FACE_STUBBLE` | stubble (time since shave?) |
| 4 | every hair/beard/tash sheet → `*_OLD` | hair greying with age |
| 5 | `HERO_SKIN_*`→`*_OLD`, bumps→`BUMP_SKIN_OLD_*`, eye→`EYE_OLD`, also layer-8 `*_STUBBLE_OLD`/`HAIR_OLD_*` | age (skin) |
| 6 | layer 0: →`*_ANTITAN` + `BUMP_SKIN_LIGHT_*`; layer 4: →`*_EVIL` + `BUMP_EVIL_*` + `EYE_EVIL` | alignment (def.xml: Evil=6) |
| 7 | layer 0: →`*_TAN`; layer 4: hair/beard→`*_BLONDE`, skin→`HAIR_BLONDE_*`/`STUBBLE_BLONDE` | def.xml: Good=7 (sun-tan/fair) |
| 8 | →`COMP_HERO_MUSCLE_*` + `BUMP_MUSCLE_*` | strength/physique |
| 9 | TORSO→`*_MUSCLE_STOMACH` | fatness/stomach |
| 10 | →`COMP_HERO_WILL_*` + `BUMP_WILL_*` + `EYE_WILL` | will glow lines (def.xml: Will=10) |
| 11 | →`COMP_HERO_HAIR_*_1A/1B` + `STUBBLE` | body hair |

(def.xml comments "Age=4, Evil=6, Good=7, Will=10" corroborate; 4 vs 5 split =
hair-age vs skin-age, empirical.) Scars/bruises/tattoos
(`COMP_HERO_SKIN_*_SCAR/BRUISE/TATTOO*`) are NOT in TextureMorphs — they come
in via other systems (tattoo cards are OBJECTs; bruises likely runtime).

`SkeletalMorphs` (6 records) references graphics.big ids 0x57f00+ region
(morph-target meshes); `ParticleMorphs` references effects by id (evil flies
etc. at stat thresholds with float ranges). Not needed for the texture answer;
layouts only partially examined.

## 2. Consequence for texture modding the hero

To recolor the adult hero's face/hands, edit **`HERO_SKIN_FACE` /
`HERO_SKIN_ARMS`** (GBANK_MAIN_PC 1250/1248) — and, because a saved hero may
have any stat mix composited on top, also the `COMP_HERO_*` FACE/ARMS/MOUTH
overlays (old, evil, tan, antitan, will, stubble, muscle, body-hair). Tinting
only the base is probably visible through most composites (overlays blend),
but tinting all 23 face/hand-related sheets makes the smoke test
save-independent.

## 3. Stage-2 smoke test (staged, NOT deployed)

`work/smoke_test/stage2/build_stage2.py` (re-runnable, self-validating):
- Base: `work/smoke_test/backups/.../textures.big` — SHA256 checked against
  the deploy ledger (pristine retail).
- Tints 23 sheets bright green (65 % blend, same strength as stage 1): the 3
  base sheets (FACE/ARMS/MOUTH) + all 20 face/arms/mouth morph overlays listed
  in §2. Each entry: decode mip-0 → tint → re-encode with the entry's own
  format (DXT1 `0x1f` / DXT3 `0x20`), mip count, real dims, TransparencyType;
  Info patched in place; ONE `big_write.rebuild` for all 23.
- Output: `work/smoke_test/stage2/data/graphics/pc/textures.big`
  (533,502,683 B). Validation ALL PASS: entry census identical to retail,
  all 23 decode back green-dominant (mean G exceeds R and B by >40; PSNR vs
  intended tint 33.9–47.1 dB), and `GRAPHIC_HERO_FACE` is byte-identical to
  RETAIL again (stage-1 tint corrected away).
- Expected in-game: green face, hands and (if visible) arms/legs-adjacent skin
  on ANY save/age/alignment; mouth interior green when talking. Deploy by
  copying over `data\graphics\pc\textures.big` (game closed, back up first —
  stage-1 `deploy.ps1`/`revert.ps1` pattern applies; stage2 ships no deploy
  script by design).

## 3b. Custom overlay AUTHORING (tools/hero_overlay.py, 2026-07-20)

*Empirical follow-up agent, same day; Ghidra lock still taken. Adds the write
path for the system §1 decoded: custom tattoos / war-paint / skin variants as
NEW COMP-style overlays + TextureMorphs records.*

### What a valid overlay looks like (retail census, 301 COMP_HERO_*/HERO_SKIN_* entries)

All 235 *blending* overlays (skin/hair/beard/muscle/will/stubble/tattoo/scar/
bruise) are **DXT3 with `TransparencyType=1`** — the 4-bit explicit alpha IS
the compositing mask (RGB = overlay colour; background is alpha-0, RGB
don't-care). The remaining 66 are DXT1 `TransparencyType=0`: the TAN/ANTITAN
full-sheet recolours (layer 0) and the eye/mouth variants — i.e. opaque
whole-sheet replacements-or-tints, not masked glyphs. Overlay dims always
EQUAL the base sheet's dims and UV layout (the mesh UVs sample both):

| region | base sheet | id | overlay format |
|---|---|---|---|
| face  | `HERO_SKIN_FACE`  | 1250 | 512×512 DXT3 tt=1, 8 mips |
| arms (incl. hands) | `HERO_SKIN_ARMS` | 1248 | 512×512 DXT3 tt=1, 8 mips |
| torso | `HERO_SKIN_TORSO` | 1252 | 512×512 DXT3 tt=1, 8 mips |
| back  | `HERO_SKIN_BACK`  | 1249 | 512×512 DXT3 tt=1, 8 mips |
| legs  | `HERO_SKIN_LEGS`  | 1251 | 512×512 DXT3 tt=1, 8 mips |
| mouth | `HERO_SKIN_MOUTH` | 1253 | 256×256 (retail: DXT1 tt=0, 7 mips) |

Every GBANK_MAIN_PC TOC record carries the same `crc` **0x7C8D23C4** (bank
constant, mimicked for new entries); deps are dev-path TGA strings (loader
doesn't need them; new entries ship none).

### The authoring tool

`python tools/hero_overlay.py stage glyph.png --region face --stat always`
(also `spec` to print the tables). Given a transparent PNG it stages, without
touching the install:

1. **NEW textures.big entry** (default `COMP_HERO_SKIN_<REGION>_CUSTOM`,
   id = bank max + 1 → **6291** on retail): PNG → DXT3/tt=1 with the region's
   retail mip count via `texture_build.build_entry`, appended with
   `big_write.rebuild(adds=…)`.
2. **Modified `data\CompiledDefs\game.bin`**: appends one 24-byte record
   `{stat, stat, layer, baseId, newId, 2}` per trigger stat to
   `TextureMorphs` (count bumped, tail preserved) in **BOTH** populated
   CHeroMorphDef entries 10535 and 11246 — the record content is identical in
   retail and their owner is still unresolved (§4 q2), so editing both is the
   safe default (`--defs` to restrict). The container rewrite is a Python
   mirror of FableForge `forge::bin::File::save` (main table + names refs
   preserved VERBATIM → names.bin unchanged; payload chunks re-cut ≤32 KiB,
   zlib): retail round-trips semantically identical (14761/14761 payloads).
3. Validation gates (all must pass before the stage is written/declared):
   new entry decodes back (format/tt/mips + PSNR), all other texture entries
   byte-identical, all other def entries byte-identical, edited defs re-parse
   with the appended records and preserved tail, and `forge defs decode`
   reports **leftover 0** on both entries.

### "Always-on" (what a tattoo mod wants) — the honest answer

TextureMorphs records carry **no threshold/range floats** (contrast
ParticleMorphs, whose records end in rank-to-appear/disappear floats, e.g.
evil flies at 0.6..1.1) — so overlay opacity is driven **continuously by the
stat's normalized rank** at composite time (matches observed gameplay:
stubble/tan/will-lines fade in gradually). Consequences:

- No single record can be proven always-on-at-full-strength from data alone;
  retail uses stats 3–11 only, and none is guaranteed maxed on every save.
- `--stat always` therefore emits the SAME overlay under **both stat 6 (evil)
  and stat 7 (good)** — the two halves of the alignment axis. If the engine
  ranks the sides complementarily (rank₆+rank₇ ≈ 1) the pair composites at
  ~full strength on any save; if ranks are `max(0, ±alignment)` a perfectly
  neutral hero shows it faintly. Which of the two is true needs the
  compositor decompile (§4 q3/q4) or a live A/B — flagged below.
- For a GUARANTEED always-on mark, `--bake-base` additionally composites the
  glyph into the base `HERO_SKIN_*` sheet itself (stage-2-proven visible on
  any save; overlays then blend on top of it).

### Worked example (staged 2026-07-20, NOT deployed)

`work/hero_overlay/build_example.py` (re-runnable): bold black cross on the
face-sheet forehead (face front centres on x=256; brow line ≈ y 140) →
`work/hero_overlay/stage/` with `textures.big` (new entry
`COMP_HERO_SKIN_FACE_FORGE_CROSS`, id 6291) + `game.bin` (10535/11246 records
156→158: `{6,6,4,1250,6291,2}`, `{7,7,4,1250,6291,2}` — layer 4 = the retail
skin blend layer of the EVIL overlays) + an unmodified `names.bin` copy so
the stage is a self-contained defs root. ALL validation gates green
(overlay PSNR 63.06 dB; `forge defs decode` leftover 0 on both; manifest with
SHA256s: `stage/hero_overlay_manifest.json`; decode-back artifacts in
`work/hero_overlay/validate/`). Base container: the ledgered pristine
textures.big backup — note the stage does NOT include the stage-2 green tint.
Deploy = manual copy of the two staged files (game closed, back up first).

## 4. Open questions / Ghidra follow-ups (lock was taken this run)

1. What consumes `GRAPHIC_HERO_FACE`/`GRAPHIC_HERO_MOUSTACHE` (MAIN-bank ids
   45/46, no mesh references)? Ghidra: xref the name/id in the texture-manager
   or hero-morph compositor.
2. Which CREATURE/def binds CHeroMorphDef 10535 vs 11246 (identical
   TextureMorphs, different SkeletalMorphs sizes — hero vs. rival-
   hero/apprentice?). Needs the CDef-listing owner walk or Ghidra.
3. `layer` field semantics (compositing order/slot?) and the stat-index → enum
   table (`CHeroMorph::Transfer`, rebuild path around the
   `FUN_00708600`-area noted in APPEARANCE_SYSTEM.md §8).
4. Whether overlays REPLACE or alpha-BLEND per layer (moot for the smoke test
   — every candidate is green). §3b evidence leans BLEND for the DXT3/tt=1
   population (soft-alpha masks, no-threshold records, gradual in-game
   fade-ins); the DXT1/tt=0 TAN/ANTITAN layer-0 sheets are the likely
   REPLACE/tint class.
5. Alignment rank model: does `--stat always` (evil+good pair, §3b) composite
   at full strength on a neutral hero (rank₆+rank₇ ≈ 1) or fade to nothing
   (`max(0, ±alignment)`)? `CHeroMorph` rank derivation in the compositor, or
   a live A/B with a neutral save, decides. Also: engine acceptance of a
   re-chunked game.bin (forge::bin writer output) has not been live-tested.
6. Layer-field ordering for authored overlays: the worked example reuses
   retail layer 4; whether a tattoo should sit on a higher layer to render
   over tan/evil states is unknown until q3 lands.
