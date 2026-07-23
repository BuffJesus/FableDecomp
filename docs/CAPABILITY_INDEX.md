# FableTLC — Capability Index

**Status: 2026-07-19.** One-page map of every reversed subsystem: what parses, what
round-trips, what a modder can do today, and the remaining blockers. Per-subsystem
detail lives in the cited docs; this is the index. Canonical hash facts are in
FINDINGS.md "★ CANONICAL CRC REFERENCE".

Legend — **READ**: parse status · **WRITE**: byte-exact round-trip / rebuild ·
**MOD**: what's achievable now. ✅ done · ⚠ partial/gate-pending · ⛔ blocked.

| Subsystem | READ | WRITE | Moddability today | Tools / docs | Gap |
|---|---|---|---|---|---|
| **game.bin defs** | ✅ 100% (14,761 entries / 249 types, field-level) | ⚠ field re-serialize proven; container gate not run | **HIGH** — edit any def field by name; field-level merge at **100% coverage** | FINDINGS, DEF_SCHEMA_COVERAGE, `forge defs decode/merge`, `tools/decode_game_bin_field.py` | container round-trip gate pending before a def writer ships |
| **.big containers (all 8)** | ✅ 100% | ✅ **byte-exact 8/8** (SHA) | **GO** — replace-entry payload, same- & size-changing | BIG_FAMILY_FORMATS, BIG_ROUNDTRIP, BIG_WRITER, `tools/bigb_*` | no add/remove entries or Info-descriptor edits yet |
| **Textures (GBANK)** | ✅ **FULL** — LZO1X decompress cracked; coherent images decoded | export **GO**; **new art needs LZO1X *compressor*** (miniLZO port) | **GO** for read/export retexture; authoring pending the encoder | BIG_TEXTURE_FORMAT, LIONHEAD_LZ (`lzo1x_decompress` @ `0x00c06b90`), `tools/lionhead_lz.py` + `parse_texture.py` | codec was standard **LZO1X-1**, not proprietary; only the encoder remains (a coding task) |
| **Meshes (MBANK)** | ⚠ metadata 100% (descriptor/LOD/submesh/materials/skeleton); **geometry OPEN** | metadata + material re-bind + whole-mesh swap **GO** | retarget textures, edit LOD, swap whole meshes | BIG_MESH_FORMAT, `tools/parse_mesh.py` | vertex/index decode UNRESOLVED — MBANK is **not** LZO; a different layout/codec (Ghidra RE in progress) |
| **Text + dialogue (BIGB)** | ✅ 100% (28,913 text + 4 LIPSYNC banks) | ⚠ read validated; write gate pending | **HIGH** — edit strings, tags, speech links; text→audio→lipsync join by `<N>` | TEXTBIG_FORMAT, BIG_FAMILY_FORMATS, `tools/parse_bigb.py` | text-writer round-trip gate not run |
| **Audio / voice (.lut)** | ✅ 100% (20,213 clips, Xbox IMA ADPCM) | ✅ **byte-exact 4/4**; same- & size-change clip replace | **GO** — replace voice lines (supply Xbox-ADPCM mono 22050) | LUT_AUDIOBANK_FORMAT, LUT_WRITER, `tools/parse_lut.py` + `lut_write.py` | runtime load untested; ADPCM encoder is the caller's job |
| **Saves (.sav)** | ✅ 100% (container + HEADER map + hero stats) | ✅ **byte-exact 5/5**; signature solved (seed-0) | **HIGH** — edit gold/morality/age/renown, world/region, toggles; working editor | SAVEGAME_FORMAT, SAVE_WRITER, SAVE_HEADER_MAP, SAVE_HERO_STATS, `tools/save_edit.py` | XP-pool field *names* [hypothesis]; deep inventory/quest = positional schema TBD; runtime load untested |
| **Script VM / cutscenes** | ✅ 100% (text-command; 184 verbs) | ⚠ payload re-serialize (def gate) | **HIGH** — author cutscenes as text commands; 43 unused verbs = headroom | SCRIPT_VM_MAP, `forge script` | not a bytecode VM; some vtable-offset→method maps are follow-ups |
| **Controller / input** | ✅ enums recovered (empirical) | data-edit (`CControlsDef`) | **HIGH** — rebind UI feasible (DIK scancodes; 28-byte records) | CONTROLLER_ENUMS | ~75 EGameAction + face-button 0–12 names unsourced; motor rumble is dead code (screen-quake only) |
| **Script bindings (FSE)** | ✅ 891 methods cataloged; 101 candidates | N/A | **HIGH** — bind 12 Tier-A verbs (fishing/digging/minigames/houses) + guard predicates | SCRIPT_BINDINGS_CANDIDATES, `gamescriptinterface_catalog.tsv` | ~18 are dup variants; FSE integration out of scope |

## Cross-cutting facts
1. **One hash everywhere** — seed-0 reflected CRC-32 (poly `0xEDB88320`, no final inversion): game.bin field tags, text.big symbols, save-stream field tags, and the save trailer. There is **no** seed-`0xFFFFFFFF` variant on disk. (`tools/save_edit.py:get_crc` defaults to the wrong seed — pass seed 0 for field tags.)
2. **text → audio → lipsync** all join on the `<N>` ordinal (proven by duration cross-check across 4 LIPSYNC banks).
3. **The "proprietary codecs" were not proprietary.** Textures = standard **LZO1X-1** (cracked, coherent images). Mesh geometry compression is the one remaining unknown (MBANK is not LZO).
4. **Field-level def merge = 100% game.bin** (249/249 types, all 14,761 entries) after folding in `def_schema_missing.json`.

## Write-gate status
- **PASS (byte-exact):** `.big` (8/8), `.lut` (4/4), saves (5/5).
- **Not yet run:** game.bin container writer, text.big writer.
- **New-texture authoring:** needs an LZO1X *compressor* (miniLZO port); the `.big` container writer already handles size-changing payloads.

## Bottom line
**Read is essentially complete** across the modding surface. **Write** is proven for `.big` / `.lut` / saves; gated (one-time) for the game.bin and text writers; and the last true RE unknown is **mesh geometry decode**. Everything else — retexture, voice, save editing, def/field merge, cutscene and controller data — is actionable today.
