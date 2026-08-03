# game.bin / names.bin Definition-Load Contract

*RE'd 2026-07-24 via the Claude Workflow decomp loop (`defload-contract-re`): 8 decode
agents over the def-load cluster → synthesis → adversarial verify. Verdict **CONFIRMED**
(no refutations; corrections folded in below). On-disk layout + both append bugs are
**high confidence** (byte-proven against the retail base); the exact in-memory per-class
resolution structure is **medium** (rel32 call targets are relocation-masked).*

Solves the standing blocker: a creature def **appended** to game.bin is seen by `forge`
but **not** by the engine (`CreateCreature("CREATURE_MESHY_HUNTER") → nil` + unstable load).
The counts were never the problem — **two payload/CRC bugs** are.

## The one hash — `crc0`
`crc0` = standard **reflected CRC-32, poly `0xEDB88320`, seed `0`, NO final inversion**
(engine `CCRC::Calc(0,…)` / `CCharString::ComputeCRC32` @ `0x00404310`). This single hash
keys everything. Proven across the entire table: **13593/13593** stored names.bin CRCs equal
`crc0(name)`; `0/13593` equal any other variant.
- `crc0("CREATURE_TRADER_01") = 0xAA22BB08` (== retail stored CRC, byte-exact)
- `crc0("Graphic") = 0x2E6B63C8` (== the game.bin field tag)
- It is **NOT** `0xFFFFFFFF - crc32`, NOT `~crc32` via subtraction, NOT zlib `crc32`, NOT seed-`0xFFFFFFFF`.

## On-disk layout (byte-exact vs pristine retail base = 14761 entries / 13593 names)

**names.bin** (shared string+CRC table):
```
+0  u32  0x0007AB1E   version/tag (preserved verbatim)
+4  u32  0xA8E36C34   MAGIC (PC); echoed in game.bin @+5
+8  u32  nameCount    = 13593        ← append: +1 per new name
+12 u32  tableSize    = 396904 = record_bytes + 4 (writer nextOffset-4 convention) ← append: grow
+16 u32  0            reserved
+20 ...  nameCount × { u32 crc0(name) ; ASCIIZ name }
```
A "name reference" stored in game.bin = byte offset of the name TEXT relative to +20, i.e.
`(pos_of_crc - 20) + 4`. `-1 / 0xFFFFFFFF` = "no name" (sub-defs).

**game.bin** (compiled def store):
```
+0  u8   0x00
+1  u8   FORMAT FLAG: 0xAA=Xbox (rejected), 0x21=PC   (part of a 9-byte header blob, verbatim)
+5  u32  0xA8E36C34   MAGIC
+9  u32  entryCount   = 14761        ← append: +1 per new entry
+13 ...  MAIN TABLE: entryCount × 12-byte rows {
             i32 defNameOffset   (names.bin ref → class name, e.g. "CREATURE")
             i32 nameOffset      (names.bin ref → instance name, or -1)
             i32 indexInDefinition }
     u32  chunkCount (= realChunks + 1)
          (chunkCount-1) × { i32 firstEntryIndex ; i32 cumulativeCompressedOffset }
          + end sentinel { i32 entryCount ; i32 totalCompressed } + u32 totalCompressed
     ...  zlib chunks (each ≤ ~32–64 KiB inflated). Each inflated chunk begins with
          u16 payloadOffset[nInChunk] (biased by nInChunk*2), then concatenated payloads.
```
`indexInDefinition` = **per-def-class dense running counter**, 0..N-1 within its class.
Verified: the CREATURE class has **517** entries with dense indexInDefinition 0..516;
`CREATURE_TRADER_01` = global entry **1549**, indexInDefinition **270**. Entry order IS the
runtime global definition index.

## Runtime resolution flow (opcode-proven structure; masked call targets = inference)
1. `CGameDefinitionManager` ctor `0x0044C6C2` — builds base `CDefinitionManager`, installs
   vtable `0x1232C24`, zeroes 4 derived dwords @ this+0xD0..0xDC, reserves a `0x80000` (512 KiB) def arena.
2. `CGameDefinitionManager::InitAndCompile` `0x0044C72B` — registers def **CLASSES**
   (className → factory-fn-ptr) into `std::map<unsigned_long, CDefClassInfo>` via registrar
   `0x564395`; map **key = `crc0(className)`**. It bakes **138** factory-fn-ptr literals here;
   the remaining classes (→249 total) register in `CStartupDefinitionManager::InitAndCompilePlatformSpecific`
   `0x004336BC`. Binds four game.bin stream readers @ this+0xD0/D4/D8/DC, then runs load/compile
   drivers (`0x190F45`, `0x233285`, `0x564195`) that stream game.bin and populate per-class def arrays.
   *(`std::pair<unsigned_long, CDefinitionManager::CDefClassInfo>` is a real demangled RTTI symbol
   — bundle `0x0047C250` — so the crc-keyed class map is confirmed to exist.)*
3. `CGameScriptInterface::CreateCreature` `0x008A9100` (by-name script path) — loads def-manager
   @ this+0x10, hashes the creature-name string, feeds the u32 to a bool-returning registry query;
   `test al,al; je` → **on miss skips the whole spawn and returns the null `CObjectRef`
   (typetag `0x1238C8C`) → Lua nil**. On hit, `[def+0x11C]` feeds the spawn. *(Control flow &
   offsets opcode-proven; that the hash IS crc0 and the query is crc-keyed rests on masked targets.)*
4. `CGameScriptInterface::CreateCreatureNearby` `0x0089F300` — computes placement, dispatches
   CreateEntity; the def name is resolved by the same by-name machinery in a callee.
5. `CDefinitionManager::GetDef<T>(long id)` `0x00410890` (numeric path) — validates `id > 0`,
   delegates to `0x59D150(id, &out)`; the class def-array is indexed by `indexInDefinition`.
   *(The `id*0x1C` record-stride allocation lives in a separate helper after this fn, likely
   `GetDefsOfType` — mis-attributing the stride to GetDef(id) itself is wrong.)*

## The two append bugs (both must be fixed; either alone still fails)

### BUG #1 — wrong names.bin CRC (fixed in FableForge `bin.cpp`)
`forge`'s `nameCrc` computed `0xFFFFFFFF - mz_crc32(name)` — matched **0/13593** real names.
Existing names round-trip only because the writer copies their *loaded* CRCs verbatim; every
truly-NEW name got an un-resolvable CRC. For `CREATURE_MESHY_HUNTER` it wrote `0xE4EACB91`; the
correct `crc0` is `0x5A11F1E5`. **Fixed** → seed-0 reflected CRC-32, no final inversion (verified
`bin.cpp` produces `0xAA22BB08`/`0x2E6B63C8`/`0x5A11F1E5`).

### BUG #2 — stale internal global entry-index references (fixed in `02_add_creature.cpp`)
A creature payload carries **absolute global entry indices** as self/owner back-references.
`CREATURE_TRADER_01` (global 1549) holds `u32 == 1549` at payload offsets **{25, 193, 301}** and
nowhere else. A byte-for-byte clone keeps `1549`, but the clone lands at global 14761+, so the engine
wires the def to the wrong entries → instability. **Fix**: rewrite every `u32 == donorGlobalIdx` in the
cloned payload to the append landing index (value-keyed, robust to record layout). **Leave component/
sibling refs alone** — `10744` CCreatureDef / `10745` CAppearanceDef / `10746` CEnemyDef and the
stride-9 table @314..638 (indices 1175..1307) are **shared** sub-defs (10744 is referenced by 5
CREATUREs and does not back-ref 1549), so they are already valid for a shared-component clone.

**Header counts were never the bug** — the writer already extends `nameCount +1`, `tableSize`,
`entryCount +1`, and recomputes dense `indexInDefinition` (`bin.cpp:243 perDefinition[def]++`),
all verified by diffing base vs forge output (13593→13594, 14761→14763 for a 2-entry card set).

## Track A vs Track B
- **Track A (in-place `Graphic.modelId`, `03_inplace_modelid.cpp`)** — sidesteps BOTH bugs by
  construction: no entry count / order / name-set / new-payload-ref change, only the 4 modelId bytes
  of one existing entry. **Confirmed safe & engine-accepted.** Ships now; scope = every instance of
  the edited creature. ⚠ Rebuild from a **pristine** retail base — the current install is at 14781
  (+20 stale appends), which may carry the very BUG-#1 wrong-CRC names.
- **Track B (append a distinct named creature)** — needs both fixes above **and** a FableForge
  rebuild (`libforgecore.a`) so the append tool + `forge validate` use the corrected `crc0`. For a
  creature with its OWN sub-defs, append the full coordinated set and retarget **every** self+sibling
  index to its new landing slot (see `work/append_fix/APPEND_ALGORITHM.md`).

## Open questions / residual risk (not blockers)
- Which stored field the engine matches at resolve time (the names.bin CRC field, a CRC it
  recomputes from the name at load, or both) was not isolated. Fix #1 makes the appended field
  consistent with `crc0(name)` under either hypothesis. Recommended A/B/C test: append with
  (a) correct crc0 only, (b) index-rewrite only, (c) both — see which flips the nil.
- The exact in-memory per-class name→def structure built by the un-decompiled load callees
  (`0x190F45`/`0x233285`/`0x564195`) — whether `indexInDefinition` alone indexes it, or an extra
  per-class count/hash is trusted — is unproven (masked targets).
- No game.bin-level integrity/trailer checksum was found this pass (unlike save files, which carry a
  seed-0 CRC trailer); forge round-trips game.bin edits in other subsystems, so believed absent but
  not explicitly ruled out.

## EgoCore cross-reference (2026-07-31, see docs/EGOCORE_ASSESSMENT_20260731.md)

EgoCore (open-source) independently corroborates the crc0 hash (its `CalculateFableCRC32` = seed 0,
no invert, poly 0xEDB88320), the crc0-keyed class registry (`map<u32, DefClassInfo>`), the header
layout, and the chunk offset-bias (`nInChunk*2`). Three things it adds that are worth acting on:

- **A/B/C test now has a ground-truth oracle.** EgoCore's production def path does NOT append — it
  deletes game.bin/names.bin/frontend.bin/script.bin and regenerates them by driving `Fable.exe`
  headless (`dbugst.ini`: `AllowDataGeneration TRUE; UseCompiledDefs TRUE; BuildRetailStaticMaps TRUE`;
  one-byte force-patch at IDA VA `0x00C90613+6`). This gives us a byte-exact generator to diff our
  appended bins against — the cleanest way to settle the "which field the engine matches" open question.
- **Two byte-exact game.bin parity details we don't document** (verify in retail before relying):
  (a) the compressed stream is **zlib deflate level 1, fed one byte at a time**, to bit-match Lionhead;
  (b) game.bin carries a **`dependencyCRC = 0xE86E4CDE`** immediately after the safeBool byte, before
  randomID; (c) EgoCore emits a per-class **`NULLDEF_<name>`** string + null instance — check whether
  retail names.bin carries these and whether our append tool must preserve them.
- **Do NOT adopt two EgoCore choices** (they contradict our byte-proven data): its standalone
  `CDefStringTable::GetCRC` uses standard CRC-32 + `tolower` (wrong for names.bin — we verified 13,593/
  13,593 as crc0, case-sensitive; filed as EgoCore issue #4), and its standalone writer emits
  `classIndex = 0` for every entry (contradicts our dense per-class `indexInDefinition`). Keep crc0 +
  the dense counter in FableForge.

*Source artifacts: `work/def_load_re/` (bundles, `manifest.json`, `contract_md.txt`,
`fix_recipe_md.txt`); full workflow result in the task output journal.*
