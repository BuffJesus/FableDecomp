# Fable TLC reverse-engineering notes — shared

- **FableTLC_RE_MasterNotes.md** — START HERE. Everything we've reversed, by
  subsystem: compiled-def containers (`game.bin`/`frontend.bin`), terrain (baked
  STB landscape + codec stack), regions/levels, teleport, meshes/graphics,
  textures, audio (`.lug`), quests & cards, saves (FableSav), text/UI, cutscenes
  (`CCutsceneDef` — all 595), scripting/FSE, and the debug-build unlock. Addresses
  + proven-vs-open status throughout.

- **FableTLC_RE_Notes_for_OpenFable.md** — terrain-focused intro tailored to your
  renderer (the "STB LOD tiles" your TERRAIN.md §7 notes retail uses and OpenFable
  skips).

- **STB_Chunk_Layout_Spec.md** — deep field-by-field byte spec of one baked
  landscape chunk, with the round-trip proof (18/18 LZO frames decode byte-exact,
  100% byte accounting, reserialize == original).

All from decompiles of retail `Fable.exe`, the FableWin 1.5 dev editor, and the
Anniversary debug PDBs, cross-checked against real game data with round-trip
proofs. Verify addresses against your own binaries. Shared freely — happy to
trade format notes or hand over the byte-exact reader/writer code.
