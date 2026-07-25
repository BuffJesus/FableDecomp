# FINDINGS — cited technical truth

*Every entry lists its evidence. Nothing here is assumption.*

## 2026-07-19 — text.big / dialogue.big BIGB localization format CRACKED + validated (task #14)

Full spec: `docs/TEXTBIG_FORMAT.md`. **Three independent sources agree** (raw bytes + EgoCore
completed-RE loader + retail Fable.exe engine loader).

**Format (all little-endian):** `text.big` is a Lionhead **`BIGB`** bank archive.
- Header (16B): `char[4] "BIGB"`, `u32 Version(=100)`, `u32 FooterOffset(=0x658267)`,
  `u32 FooterSize(=0x2A)`. Payload blobs occupy `0x10 .. FooterOffset`.
- Footer = sub-bank directory: `u32 SubBankCount(=1)` then per sub-bank
  `{ cstr Name("TEXT_ENGLISH_MAIN"); u32 Version(106); u32 EntryCount(28913); u32 TOCOffset;
  u32 TOCSize; u32 Align }`.
- TOC: stats header `u32 StatsCount(=3)` + `StatsCount` `(u32 type,u32 count)` pairs
  = `(0,26807)(1,2105)(2,1)` (sums to 28913). Then `EntryCount` variable-length records:
  `u32 Magic(=42) | u32 ID | i32 Type | u32 Size | u32 Offset(file-absolute) | u32 CRC |
  {u32 len + Name bytes} | u32 Timestamp | u32 DepCount | Deps[] | u32 InfoSize | Info[InfoSize]`.
- Payload by `Type`: **0=string** `{ UTF-16LE Content \0\0; u32len SpeechBank; u32len Speaker;
  u32len Identifier; u32 TagCount; Tags[]{ i32 Position; cstr Name } }` — tag names carry
  `ANIM:`/`CAM:(...)`/`CONVERSATION_ATTITUDE_*` conversation directives. **1=group**
  `{ u32 Count; u32 memberIDs[Count] }` (random-variant bundle). **2=narratorList** (Offset/Size=0;
  located by scanning for ASCII `"[NarratorList]"` sig, then padding+`u32 DataSize`+`u32 Count`+
  Count NUL-term names).

**Byte evidence (retail `data\lang\English\text.big`, 6,652,561 bytes):** validating parser
(`scratchpad/parse_textbig.py`) decoded all 28,913 entries. Samples:
- `[1] TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC` speaker=NONE bank=ScriptDialogue.lug
  content="If you get three hits in a row without reply, you can Flourish by pressing [B]…"
- `[835] TEXT_CS_035_END_10` speaker=FARMER, 1 tag `(0,"ANIM:SCRIPT_CHEER_1")`,
  content="Well done! You really taught the Hobbes a lesson."
- group `TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND` → count=4, member IDs [16879,16872,16761,16858].
Type histogram matches the stats header exactly; every TOC entry `Magic==42`; TOC walk terminates
exactly at `FooterOffset`.

**EgoCore second source (MIT):** `EgoCore\Banks\BankLoader.h` (`CreateBankFromDisk`,
`LoadSubBankEntries`), `BankBackend.h` (`BankEntry`, `ReadBankString`), `Text\TextParser.h`
(`CTextParser::Parse`/`Recompile`) at
`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\`. Container is shared by
graphics/textures/frontend/effects/fonts/shaders/dialogue banks (sub-bank name prefix selects
kind).

**Retail Fable.exe loader confirmed (third source, Ghidra headless):** reader class
`NGameText::CDataBank : CBankFile`.
- `.big` family strings: `text.big` @ `0x0122da24`, plus graphics/textures/dialogue/fonts/shaders/
  effects/temp `.big` — one shared BIGB container.
- `NLocalisation::GetLanguage` @ `0x00415070` → `"TEXT_ENGLISH_MAIN"` + per-language variants
  `TEXT_*_MAIN` (0x0122e8e8..0x0122e974) = the footer sub-bank Name.
- `CBankFile::GetEntryDataType(id)` returns per-entry `Type`. `NGameText::CDataBank::GetTextInfo`
  @ `0x009c7d00` requires `Type==0` then indexes `*(this+0x1e0)[id]` (numeric-ID lookup).
- `NGameText::CDataBank::GetTextEntryFromGroup` @ `0x009c9280` requires `Type==1`, iterates member
  IDs, weighted-random-picks a variant (engine RNG `state*0x24a1+0x24df`).
- `NGameText::CDataBank::GetTextBySymbol` @ `0x009c95e0`:
  `id = FUN_009cc410( CCRC::Calc(0, name, len) )`, requires `Type==0`, returns the `CWideString`
  (UTF-16) content. **Symbol→ID uses CRC-of-name seeded 0** — the SAME seed-0 reflected CRC-32 that
  game.bin field-tags AND save-stream field tags use (see "★ CANONICAL CRC REFERENCE"; there is no
  seed-0xFFFFFFFF variant). `FUN_009cc410` = CRC→entry-index map = `CVectorMap<u32,CDefString>::LowerBound`.
Logs: `ghidra_out/decomp_localisation.c`, `decomp_textbig_loader.c`, `decomp_textbig_strxref.log`,
`decomp_textbig_bankfile.log`.

**Read path for FableForge:** header→footer→sub-bank TOC→seek each type-0 `Offset`, decode payload.
Editing is possible (mirror `CTextParser::Recompile` + rebuild TOC offsets/sizes + footer), but a
**byte-exact `.big` write round-trip is NOT yet proven** — must be verified before shipping a
writer (same discipline as game.bin). **[hypothesis]** TOC `CRC`(+0x14) and `Timestamp` fields:
plausible from EgoCore struct names/positions, not independently recomputed; not needed for reads.

## 2026-07-18 — Retail `Fable.exe` embeds NO Lua (open question resolved)

**Claim:** Retail TLC has no embedded Lua VM of any version. The "Lua 5.0 vs 5.1"
question from HANDOFF is moot for the retail binary; all Lua in the modding
ecosystem is injected by FSE.

**Evidence:**
1. Full-binary regex scan of `Fable.exe` (16,666,624 bytes, scratchpad
   `scan_fable.py`): zero hits for `Lua 5.x`, `$Lua`, `Tecgraf`, `lua.org`,
   `LUA_*`, `lua_*`/`luaL_*` API-name strings. A real static Lua link always
   carries these strings (version banner + error messages).
2. Case-insensitive sweep for any `lua` substring: only 4 hits — two are
   `FrameLastEvaluated…` property names ("...Evalua..."), two are coincidental
   x86 instruction bytes at `0x579535`/`0x61c532` (verified by byte context —
   mid-instruction, not text).
3. Consistent with the architecture: TLC quest scripts are compiled C++ classes
   (`CGlobal_*Script`, 843 Quest API entries in the FSE manifest), and FSE's
   whole design is injecting a Lua VM (vendored **Lua 5.4.8** + Sol2, per
   `FSE Vendor\lua\lua.h` LUA_VERSION defines) at the native
   script-registration hook `0xCDB355`.

**Consequence:** `luadis.py`/`luadis51.py` are irrelevant to retail TLC.
Fable 2's Lua lineage does not apply to TLC's shipped engine.

## 2026-07-18 — RTTI is fully present in retail `Fable.exe` (open question resolved)

**Claim:** The retail binary keeps complete MSVC RTTI: **1,973 unique class
type descriptors** (`.?AV…@@`) and 8 struct descriptors (`.?AU…@@`).

**Evidence:** binary scan (`scan_fable.py`) over the same exe; descriptors
include gameplay classes we already located by other means
(`.?AVCChestDef@@`, `.?AVCContainerRewardHeroDef@@`,
`.?AVCCreatureAction_OpenChest@@`), which cross-checks the scan.

**Consequence:** Ghidra's RTTI analysis can pin vtables/class names for ~2k
classes in the retail DB independently of BSim/PDB porting — and
`.?AVCChestDef@@`'s type descriptor → complete object locator → vtable chain
is a direct route to mapping retail `CChestDef` methods (incl. `Transfer`).

## 2026-07-18 — String-hash constant: no murmur/FNV; CRC32 present

**Claim:** TLC does not use Fable 2's murmur-like `0x5BD5E995` hash, nor FNV.
CRC32 machinery exists in the binary.

**Evidence:** little-endian immediate scan of the full exe: `0x5BD5E995` 0 hits;
FNV basis/prime (`0x811C9DC5`/`0x01000193`) 0 hits; CRC32 reversed polynomial
`0xEDB88320` 2 hits (file offsets `0xE9A368`, `0xEAF980` — .text-region code,
likely table-less CRC or zlib), forward poly `0x04C11DB7` 1 hit.

**Status:** partially open — which hash keys resource lookup (BIG entry IDs /
`Data\CompiledDefs` names) still needs the actual lookup function read in
Ghidra. The CRC32 sites are the first candidates to inspect.

## 2026-07-18 — Retail `CChestDef::Transfer` = `0x004DE204` (RTTI vtable-slot port)

**Claim:** In retail `Fable.exe`, `CChestDef::Transfer(CPersistContext&)` is at
`0x004DE204` (vtable slot 18); also `Copy` = `0x004E0564` (slot 19),
`GetSizeofClass` = `0x004D3840` (slot 20), vector-deleting dtor = `0x004D8072`
(slot 0). Labels: `ghidra_out/labels_chestdef_rtti.tsv`; tool:
`tools/rtti_map.py` (walks TypeDescriptor → CompleteObjectLocator → vtable in
both binaries; resolves the donor's incremental-link `jmp` thunks).

**Evidence (3 independent):**
1. **RTTI slot alignment:** donor (FableWin, PDB-named) and retail `CChestDef`
   vtables both have exactly 25 slots; the class-specific override cluster
   (Transfer/Copy/GetSizeofClass) sits at slots 18–20 in the donor, and retail
   slots 18–20 are the only non-stub, non-shared entries in that range.
2. **Method validation on a known class:** the same walk on `CTCChest` puts
   retail slot 9 at `0x750780`, which equals the independently confirmed
   `CTCChest::FrameUpdate` (decompile evidence in SYSTEMS_ANALYSIS.md); donor
   slot 9 is `?FrameUpdate@CTCChest@@UAEXXZ`.
3. **Body shape:** retail `0x4DE204` disassembles as a canonical Transfer —
   `mov esi,ecx` (this), then repeated `lea eax,[esi+0xNN]; push eax;
   mov ecx,edi; call …` field-by-field persists (+0x28, +0x25, +0x2C, +0x30 …).

**Caveats:** the FableWin donor is an Anniversary-era 2013 dev build — its
`CTCChest` vtable has 32 slots vs retail 28, so slot indices are NOT blindly
portable for every class; they must re-align per class (identical counts and a
matching override cluster, or a confirmed anchor like FrameUpdate).

**Payoff insight:** retail strips def field-name strings (0 hits for
`OpenParticleEffect`, `PersistOnOpening`, `OpenersRequired`,
`DisplayMessageOnEmpty` — all referenced by the donor's Transfer). So
`game.bin` payload decoding must go by field ORDER, and the donor's Transfer
provides the ordered field-name list while the retail Transfer provides the
runtime struct offsets. Reading the two side by side yields the
`CChestDef` payload decode map for the chest/reward editor.

## 2026-07-18 - `forge validate` cross-checks the install clean

FableForge's native consistency check (`forge validate <game-root>`) on the
Steam install: 398 WLD maps / 141 regions, 796 WAD entries, all 398 LEV + 398
TNG resolve inside the WAD, 0 unresolved region map references, 21,764 things
across 397 loose TNGs with 0 missing DefinitionType and 0 missing UID -
matching `fqt_modding_catalog.json` exactly. LEV reader verified cell-exact
(16,384/16,384 heights, dominant themes, strengths) against
SilverChest.LevBridge's BarrowFields theme grid.

## 2026-07-18 - Bulk RTTI vtable-slot port applied to retail Ghidra DB

**Claim:** `tools/rtti_port_all.py` produced 6,653 high-confidence virtual
method labels by porting class-local PDB names from the FableWin donor to retail
`Fable.exe` through MSVC RTTI vtable slot identity.

**Evidence:**
1. Generation summary: 1,973 retail classes scanned; 1,574 classes had at least
   one matched vtable; 372 classes skipped for slot-count mismatch; 4
   image-folding/name-conflict rows dropped.
2. Conservative apply first renamed 4,473 functions and created 4,642 new
   functions. `DemangleAll.java` demangled all 4,473 with 0 failures.
3. `CompareLabels.java` categorized the already-named rows:
   4,474 agree, 2,179 disagree, 0 missing. The disagreement sample shows BSim
   generic/collided names such as `GetBankHandle` and
   `InitialiseDefaultCameraTags` sitting on unrelated RTTI-identified methods.
4. `LabelApplyForce.java` then overrode the 2,180 current-name conflicts from
   the RTTI source with 0 failures. `DemangleAll.java` demangled 6,650 names
   with 0 failures.

**Current DB stats:** `DumpStats.java` after the RTTI force pass reports
49,082 total functions, 40,187 named, 8,895 default-named, 427 thunks, 202,189
symbols. The named count is unchanged from the conservative pass because the
force pass replaces wrong BSim names, not default names, but semantic accuracy is
substantially higher. The compare audit is preserved at
`ghidra_out/rtti_port_compare.tsv`.

**Caveat:** this pass intentionally skips classes whose donor and retail vtable
slot counts do not match. The FableWin donor is an Anniversary-era build, so the
skip is correctness-preserving, not a failure.

## 2026-07-18 - Retail `CChestDef::Transfer` own-field map

**Claim:** Retail `CChestDef::Transfer` at `0x004DE204` persists six own fields.
The silver-key requirement is encoded by `OpenerObject` + `OpenersRequired`;
reward contents are not in these six own fields.

**Evidence:** paired donor/retail Transfer analysis. Donor provides stripped
field names/order; retail decompile at `ghidra_out/decomp_chestdef_transfer_retail.log`
provides runtime offsets:

| retail offset | field | note |
|---|---|---|
| `+0x25` | `PersistOnOpening` | bool |
| `+0x28` | `OpenParticleEffect` | ref/def-like field |
| `+0x2C` | `DisplayMessageOnEmpty` | ref/def-like field |
| `+0x30` | `OpenAnimationForCreature` | animation/string-like field |
| `+0x34` | `OpenerObject` | ref/def-like field |
| `+0x38` | `OpenersRequired` | count/int |

**Consequence:** the chest editor can read required key count from
`OpenersRequired` and the required opener/key object name from `OpenerObject`.
It should not treat `CChestDef` itself as the reward payload; reward contents
likely live in `CTCContainerRewardHero` data/sub-definitions/scripts, not
ordinary TNG placement fields.

## 2026-07-18 - `CTCChest::GetNumberOfKeysToUnlock` reads `CChestDef` only

**Claim:** `CTCChest::GetNumberOfKeysToUnlock` does not inspect player inventory
or quest state. It reads the chest's definition sub-def and returns the static
requirement.

**Evidence:** `ghidra_out/decomp_chest_core.log` and
`ghidra_out/decomp_chest_interface_after_rtti.log`:

- `CTCChest::GetNumberOfKeysToUnlock` gets the chest sub-def from the owning
  thing's def pointer (`thing + 0x70`).
- It checks `subdef[0x0d]` / offset `+0x34` (`OpenerObject`) and `subdef[0x0e]`
  / offset `+0x38` (`OpenersRequired`) are both positive.
- It resolves the `OpenerObject` global def index back to a def name string and
  writes that into the output `CCharString`.
- It returns `OpenersRequired`; otherwise it clears the output string and returns
  `0`.
- `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` only validates the
  script thing, finds `CTCChest` via interface type `0x3c`, and calls
  `CTCChest::GetNumberOfKeysToUnlock`.

**Status:** key-count semantics are now grounded. The remaining unknown is where
the script checks the hero's inventory against the returned opener object/name.

## 2026-07-18 - `CContainerRewardHeroDef::Transfer` static reward field

**Claim:** `CContainerRewardHeroDef` persists one own field, `ObjectFamilies`,
at retail offset `+0x28`.

**Evidence:**
1. RTTI vtable-slot port maps retail `CContainerRewardHeroDef::Transfer` to
   `0x004E8987`, `Copy` to `0x004E249B`, and `GetSizeofClass` to `0x004E2497`.
2. `GetSizeofClass` returns `0x34`; `Copy` copies/assigns the tail field at
   `param2 + 0x28`.
3. Donor `CContainerRewardHeroDef::Transfer` contains a single field-name string,
   `ObjectFamilies`, and transfers donor `this + 0x28`.
4. Retail bytes at `0x004E8987` begin with `add ecx,0x28; push ecx; ...`,
   matching a single transferred own field at `+0x28`. Ghidra currently
   decompiles the helper target with a stale unrelated name, so byte shape and
   donor field-name order are the stronger evidence.

**Consequence:** static chest rewards are driven by `ObjectFamilies`, not a flat
item list in `CChestDef`. Runtime `CTCContainerRewardHero::GetRewardItems` turns
that family list into actual reward items, with special handling for dynamic
potion/resurrection dummy objects and attached `CTCGold`.

## 2026-07-18 - FableForge STB reader validates `FinalAlbion_RT.stb`

**Claim:** FableForge can now read/extract the retail static-map STB container
and validate it against the world file.

**Evidence:**
1. Format source: decompiled FableMod.BBB / FableMod.STB shows STB is a
   BBB-style container with a 32-byte header, 12-byte dev header before the
   entry table, 28-byte entry headers, and a special
   `__STATIC_MAP_COMMON_HEADER__` entry.
2. Retail parse: `forge stb list FinalAlbion_RT.stb` reports 424 entries,
   398 static maps, 2048-byte alignment, table offset `597919744`.
3. Cross-check: `forge validate <game-root>` now reports `stb: 424 entries,
   398 static maps` and `stb static maps: 0 missing`; the STB static-map names
   match all 398 WLD `LevelName` values after the `Data\Levels\` prefix.
4. Tests: `ctest --test-dir D:\Code\FableForge\build --output-on-failure`
   passes, including a synthetic STB fixture.

## 2026-07-18 - Chest key gate: full retail runtime chain located

**Claim:** the hero-inventory check for silver-key (and any keyed) chests is
`CGameScriptInterface::GetNumberOfItemsOfTypeInInventory` at retail `0x00897190`,
called from `NScript::CGlobal_OpenChestScript::Main` at retail `0x00EEC890`.
Keys are counted, never consumed.

**Evidence:**
1. `FindRefs` on `CTCChest::GetNumberOfKeysToUnlock` (`0x750050`) shows exactly one
   caller: `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` (`0x8966C0`)
   (`ghidra_out/findrefs_chest_callers.log`).
2. `ListSyms` finds the retail script bodies:
   `CGlobal_OpenChestScript::Main` = `0xEEC890`,
   `CGlobal_GiveHeroItemsFromRewardChestScript::Main` = `0xEEC410`
   (`ghidra_out/chest_script_syms.log`).
3. `CGameScriptInterface` vtable base = `0x1260F0C` (slot 0 -> `0x6E7D50`), proven by
   three independent slot hits: `+0x6CC` -> `0x896390` `OpenChest`, `+0x6D4` ->
   `0x8966C0` `GetNumberOfKeysNeededToUnlockChest`, `+0x6D8` -> `0x896730`
   `DisplayLockedChestMessage` (data refs at `0x12615D8/E0/E4`).
4. `CGlobal_OpenChestScript::Main` decomp (`ghidra_out/decomp_open_chest_scripts.log`):
   `n = vtbl+0x6D4(keyNameOut)`; if `n < 1` -> open path; else
   `have = vtbl+0x2DC(keyName)`; if `n <= have` -> open path; else looks up game text
   `"CHEST_OPEN_FAIL"` (`vtbl+0xAC4`) and calls `vtbl+0x6D8` DisplayLockedChestMessage.
   No call removes key items in either branch.
5. Vtable slot `+0x2DC` = `0x12611E8` -> `0x897190` =
   `?GetNumberOfItemsOfTypeInInventory@CGameScriptInterface@@UBEJABVCCharString@@@Z`
   (`ghidra_out/decomp_inventory_count_slot.log`). Its body resolves the def name via
   `CDefinitionManager::GetDefGlobalIndexFromName` (`0x9AD410`), finds the hero thing's
   `CTCInventoryBase` (TC interface type `0x11`), and returns
   `NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory` (`0x5BDF08`).
6. FSE Quest API manifest independently lists both `GetNumberOfKeysNeededToUnlockChest`
   and `GetNumberOfItemsOfTypeInInventory` as script-interface functions.

**Consequence:** the whole key gate is data-driven from `CChestDef::OpenerObject` /
`OpenersRequired`; a chest can require N of ANY object def. Editor support: two fields,
no code patch needed.

## 2026-07-18 - Reward payload fully decoded: ObjectFamilies -> CObjectFamilyDef -> weighted random

**Claim:** static chest rewards are: `CContainerRewardHeroDef::ObjectFamilies`
(`vector<long>` of family def indices, persisted at `+0x28`) -> pick ONE family at
random -> `CObjectFamilyDef::GetRandomObject` (`0x8ED590`) picks one object by
WEIGHTED random from `vector<pair<long objectDefIdx, int weight>>` at family `+0x3C`.

**Evidence:**
1. Fresh post-RTTI decomp of `CTCContainerRewardHero::GetRewardItems` (`0x74E860`,
   `ghidra_out/decomp_reward_expand_after_rtti.log`): when the runtime reward list at
   `this+0xC..0x10` (stride 8: `pair<long, CCharString>`) is empty, it reads the owning
   thing's def sub-def, iterates `subdef+0x28..+0x2C` with stride 4 (`>> 2`), calls
   `GFRandom(count)`, resolves the chosen index via `CDefinitionManager::GetDef<...>`,
   then calls `CObjectFamilyDef::GetRandomObject`. (Several callee names in the decomp
   are stale BSim template collisions; data flow is what is claimed.)
2. `CObjectFamilyDef::GetRandomObject` (`ghidra_out/decomp_objectfamily_def.log`):
   iterates `this+0x3C..0x40` stride 8, sums `entry+4` (weights), draws
   `rand % totalWeight` (LCG state at `DAT_013B86A0+0x1613C`, mul `0x24A1`, add
   `0x24DF`, then ROR13), walks entries accumulating weight, returns `entry+0` (the
   object def index) of the first entry crossing the draw.
3. Transfer thunks (disasm): `CObjectFamilyDef::Transfer` `0x46C294` =
   `add ecx,0x3C; push ecx; mov ecx,[esp+8]; call 0x46C2A4`;
   `CContainerRewardHeroDef::Transfer` `0x4E8987` = same shape with `+0x28` and helper
   `0x4E8997`. Both helpers call `CPersistContext::CheckCRC(ctx, "")` - the field-name
   string at `0x122D70E` is EMPTY, confirming retail strips def field names - then
   `GFSerialiseVectorBinaryIn/Out` depending on ctx mode (`ctx+0x18 == 2` = read from
   `CMemoryDataInputStream` at `ctx+0x24`).
4. Helpers renamed in the DB via `LabelApplyForce`
   (`ghidra_out/labels_chest_reward_chain.tsv`, rows=2 renamed=2 failed=0); they
   previously carried a stale BSim name (`CCreatureAction_FireMissileWeapon::FrameUpdate`).

**Consequences for FableForge:**
- Chest reward UI: resolve each `ObjectFamilies` index to its `OBJECT_FAMILY_*` def
  name; family members + weights come from the family def payload (one binary vector
  each; both defs persist exactly ONE own field).
- Rewards are stochastic by design (family choice AND member choice are random) except
  when a family has one member, or the runtime list / attached `CTCGold` drives it.
- Gold rewards: attached `CTCGold` TC (interface type `0xCC`), gated by thing flag
  `+0x38 & 0x1000`; `GetDynamicPotionDefIndex` (`0x74E280`) picks health/super/mana
  potion by hero inventory counts; failed dynamic resurrection converts to +20 gold.
- `CGlobal_GiveHeroItemsFromRewardChestScript::Main` (`0xEEC410`) holds no item tables;
  it special-cases the container def name `OBJECT_CHEST_REWARD_ON_DEATH` and otherwise
  drives the generic reward-container machinery.

## 2026-07-18 - game.bin reward payloads decoded end-to-end; families enumerated

**Claim:** all 41 retail object families and 72 container-reward defs are now enumerated with
exact payload layouts; fixed chest contents are TNG-side, not def-side.

**Evidence:**
1. `tools/bin_dump` (SilverChest.Formats.Bin) gained a hex mode; raw payloads in
   `ghidra_out/installed_game/objectfamily_payloads.tsv` and `rewarddef_payloads.tsv`.
2. `OBJECT_FAMILY` payload = 9-byte header + `u32 count` + `count x (u32 objectDefIdx, f32 weight)`.
   `CContainerRewardHeroDef` payload = 7-byte header + `u32 count` + `count x u32 familyDefIdx`.
3. Def indices ARE game.bin entry indices: reward payload value 2820 = row 2820 =
   `REWARD_VILLAGER_01`; family member 4642 = `OBJECT_GOLDBAG_MEDIUM_WITH_COINS`, 4290/4291 =
   the dynamic potion/resurrection dummies named in `CTCContainerRewardHero::GetRewardItems`.
4. Weights are per-mille floats; `REWARD_GUARD_01` sums to exactly 1000 with two `defIdx 0`
   entries (100+400 = 50% chance of no drop). Decoder: `tools/DecodeObjectFamilies.ps1` ->
   `ghidra_out/installed_game/object_families.{json,csv}` (41 families, 72 reward defs).
5. The 72 `CContainerRewardHeroDef` entries cluster after `CCreatureDef` parents - they are
   creature death-loot tables. Chest OBJECT defs (485-byte payloads, e.g. `OBJECT_CHEST_OPENABLE`
   3688, `OBJECT_SILVERKEY_CHEST_5..100` 3689-3694) embed their components with no adjacent
   sub-entries.
6. Fixed chest contents live in the TNG placement: `StartCTCChest; ContainerContents[0]
   "OBJECT_SUPER_MANA_POTION"; ChestOpen FALSE; EndCTCChest` (HobbeCaveLarder five-key chest).
   162 chest placements exist across the 397 loose TNGs; 115 have fixed contents
   (`forge chest list`).

**Consequence:** chest reward editing = TNG `ContainerContents[n]` (already covered by the
FableForge TNG mutation API); loot-table editing = game.bin family/reward payload writes (simple
vector formats, decoder proven). Weights being floats means the retail runtime converts or
consumes them as raw bits at load; treat the persisted format as authoritative.
## 2026-07-19 - Cutscene macro command stream and native handler xrefs

**Claim:** `CCutsceneDef` payloads in `script.bin` contain a readable macro-command
stream. The native executor is `RunCutsceneMacro_Func` (`0x00CBFB7D`), with a
preload helper for camera commands at `CutsceneCameraPreloader` (`0x00CBF29F`).

**Evidence:**

- `forge script cutscenes <game-root>` decodes all 595 retail `CCutsceneDef`
  records. Observed payload layout: 9-byte header, u32 command count at `+0x09`,
  then that many null-terminated command strings at `+0x0D`.
- `forge script cutscene <game-root> CS_ARENA_ROUND_1` decodes entry 15 as 2,310
  bytes / 69 commands, beginning with `UseCamera P_ARENACUOC,-1,NULL,0,0`,
  `PutUpYourSwords`, and `Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1`.
- Exact Ghidra string-xref scan (`tools/ghidra_scripts/FindExactStrXref.java`,
  output `ghidra_out/cutscene_exact_verb_xrefs.log`) maps command strings to
  native code:
  - `UseCamera` -> `RunCutsceneMacro_Func+0xA3BC`, and
    `CutsceneCameraPreloader+0x10C`.
  - `DoCameraPreloading` -> `RunCutsceneMacro_Func+0x8B53`, and
    `CutsceneCameraPreloader+0x29`.
  - `FadeIn` -> `RunCutsceneMacro_Func+0x10DA7`.
  - `FadeOut` -> `RunCutsceneMacro_Func+0x10E0E`.
  - `Create` -> `RunCutsceneMacro_Func+0xC6C9`.
  - `CreateEffect` -> `RunCutsceneMacro_Func+0xC01D`.
  - `NoLoadUseCamera` -> `RunCutsceneMacro_Func+0xA2EC`.
  - `PutUpYourSwords` -> `RunCutsceneMacro_Func+0x9785`.
  - `GamePause` -> `RunCutsceneMacro_Func+0x8D54`.
  - `DoScriptFrame` -> `RunCutsceneMacro_Func+0x7508`.
  - `RemoveExtras` -> `RunCutsceneMacro_Func+0x6F50`.
  - `WaitActiveDialog` -> `RunCutsceneMacro_Func+0x69EE`.

**Consequence:** shipped cutscenes are immediately browsable as macro timelines,
and the next native RE step is not "find the VM" broadly; it is to slice
`RunCutsceneMacro_Func` into per-verb handler blocks, beginning at the exact xref
addresses above.

## 2026-07-19 - Retail cutscene verb census (full install)

**Claim:** the 595 retail `CCutsceneDef` records contain 18,648 macro commands
across 1,563 distinct verbs (most of the long tail is entity-scoped
`<NAME>.Verb` forms; the global-verb head is small).

**Evidence:** `forge script command-stats <game-root>` (FableForge `82be9d7`),
aggregating every decoded command. Head of the histogram with cutscene spread:
`GamePause` 2,982 in 480 scenes; `UseCamera` 1,713 in 512; `DoScriptFrame` 762
in 293; `FadeOut` 684 in 416; `FadeIn` 569 in 424; `HERO.Teleport` 521 in 347;
`WaitActiveDialog` 365 in 135; `DoCameraPreloading` 338 in 311;
`HERO.PlayAnimation` 314 in 162; `Create` 312 in 131; `RemoveExtras` 301 in 155.

**Consequence:** GUI timeline labeling can bootstrap from `--json` output of
`command-stats`; the entity-scoped `X.Verb` family means the native dispatcher
must split on `.` and route the suffix per named cutscene actor — a concrete
thing to look for inside `RunCutsceneMacro_Func`.

## 2026-07-19 - RunCutsceneMacro_Func interpreter architecture (verb-slice pass)

**Claim:** the cutscene macro executor is a blocking, frame-pumped interpreter
with a linear `strncmp` verb-dispatch chain; command arguments are pre-split
into `CCharString` locals before dispatch; waiting verbs spin internal loops
pumping the engine at a nominal 15 ticks per second.

**Evidence:** full decompile of `RunCutsceneMacro_Func` (`0x00CBFB7D`, 14,801
pseudo-C lines, 186 s decompile at 512 MB payload cap) sliced at the 12 exact
verb-string xrefs — `tools/ghidra_scripts/DumpVerbSlices.java`, outputs
`ghidra_out/cutscene_verb_slices.md` (all 12 slices) and
`ghidra_out/runcutscenemacro_full.c` (1.02 MB). Helper decompiles in
`ghidra_out/decomp_cutscene_helpers.log`.

- **Dispatch:** for each verb the code does
  `CCharString(verbLiteral) -> GetLength -> strncmp(command, verb, len) == 0`
  in a long if/else chain (prefix match, case-sensitive). Args are already
  split: arg1 `local_44`, arg2 `local_40`, arg3 `local_3c`, arg4 `local_38`,
  arg5 `local_34` as `CCharString`s.
- **Blocking waits:** `GamePause <seconds>[,clock]` — with `clock` (stricmp) it
  waits on `GFGetTime()` until start+seconds; otherwise it iterates
  `seconds * 15.0` engine ticks. Both loops per-tick: optional pump via
  cutscene-manager vtbl `+0x1c`, `PlayCutsceneSound(soundMap, DAT_013B83C8++)`,
  and break on `IsCutsceneSkipped()`. `WaitActiveDialog` loops while manager
  vtbl `+0x5C0` (dialog-active query) returns true, same pump/skip pattern.
- **Cutscene manager global `DAT_0143E8F8`**, vtable slots pinned so far:
  `+0x1C` per-tick engine pump, `+0x118` get hero as `CScriptThing`,
  `+0x120` find `CScriptThing` by name, `+0x5C0` dialog-active query.
- **Actor resolution:** `GetCutsceneActorScriptThing` (`0x00CBF9DE`, was
  FUN_00cbf9de): empty name or `"HERO"` -> manager `+0x118`; else
  `FindRegisteredCutsceneActor` (`0x00CD2B86`) over the registered-actor map
  (populated by the `RegisterActor` verb), fallback manager `+0x120`.
- **Arg parsing:** `GFCharStringToFloat` (`0x0099E690`, was FUN_0099e690) is a
  hand-rolled sign/int/fraction float parser over `CCharString{ptr,len}`;
  `GFStringToFloat`/`GFStringToInt` handle `char*` args elsewhere.
- **New verbs found adjacent in the chain:** `SetDoorOpen` (after GamePause),
  `RegisterActor` (after WaitActiveDialog).
- **`DAT_013B83C8`** is the sequential cutscene-sound cursor; wait loops also
  early-exit when `*(char*)(DAT_013D2838 + 5)` is set.
- Labels applied to the DB (default-named only):
  `ghidra_out/labels_cutscene_helpers.tsv`, rows=3 renamed=3 failed=0
  (`apply_cutscene_helper_labels.log`).

**Caveat:** inside `RunCutsceneMacro_Func` many surrounding symbol names are
stale BSim/RTTI-port artifacts on generic template helpers (e.g.
`_Cons_val<...>`, `C3DClothPrimitive::~C3DClothPrimitive` used as a generic
dtor, `SetMiniMapRegionExitTextOffsetX`). Trust control-flow structure and
verified helpers over incidental names in this function.

**Consequence:** FableForge can now model cutscene playback semantics (15 Hz
tick budget, blocking waits, actor registry, HERO aliasing) for timeline
preview, and the remaining per-verb RE is mechanical: walk the strncmp chain in
`runcutscenemacro_full.c` to enumerate ALL verbs and their arg signatures.

## 2026-07-19 - COMPLETE native cutscene verb API (184 verbs) + content census

**Claim:** retail TLC's cutscene macro language has exactly 184 verbs — 61
entity-scoped `.Verb` actions and 123 globals — extracted from the dispatch
chain itself, and 43 of them are never used by any shipped cutscene.

**Evidence:** regex extraction over `ghidra_out/runcutscenemacro_full.c`: every
`CCharString::CCharString(_, "<literal>", -1)` whose 700-char lookahead
contains `strncmp` against the command string `local_c`. Full list in
`ghidra_out/cutscene_native_verbs.txt`; cross-reference against the shipped
content census (`forge script command-stats --json`, 156 used actions) in
`ghidra_out/cutscene_verb_census.tsv`.

- **Native but unused in all 595 shipped cutscenes (43):** includes
  `CreateLight`, `CameraRig`, `CameraLookAt`, `CameraEffect`, `LiftRock`,
  `ExitGame`, `HUD`, `Print`, `GiveHeroMorality`, `HeroTattoo`, `WaitFlag`,
  `RegisterScript`, `.TurnInto`, `.Decapitate`, `.FightWith`, `.ModifyHealth`,
  `.AILevel`, `.Killable` — working engine features with zero content usage,
  i.e. free headroom for modded cutscenes.
- **Dispatch is prefix-match** (`strncmp(cmd, verb, len(verb))`), so several
  malformed content commands still execute: `DoScriptFrame1` ->
  `DoScriptFrame`, `.PlayLoopingAnimation` -> `.PlayLoopingAnim`,
  `.PlayCombatAnimation` -> `.PlayCombatAnim`, `RemoveThing` -> `Remove`,
  `.DialogSpeakSpeak` -> `.DialogSpeak`.
- **Genuinely dead content commands (silent no-ops in retail):**
  `.ClearActions`, `DoScriptName`, `.SetEntityMaxWalkingSpeed` (correct form
  is `.EntitySetMaxWalkingSpeed`), `.SetScriptedMode`, `AnimationPhysics`,
  `ClearCommands` (global form; only entity-scoped `.ClearCommands` exists),
  `dGamePause`, `nop` — Lionhead shipped typos the engine ignores.

**Consequence:** FableForge's cutscene editor has the complete, closed verb
vocabulary for validation/autocomplete (flag anything not prefix-matching the
184), and can warn on the known-dead forms above. Handler chain order matters
for prefix collisions (`Remove` vs `RemoveAll*`) — enumerate order from the
if/else chain in `runcutscenemacro_full.c` when wiring exact routing.

## 2026-07-19 - Shipped-content cutscene bug census (validator ground truth)

**Claim:** running the 184-verb dispatch model over all 595 retail cutscenes
(18,647 commands) finds 10 distinct commands the engine silently ignores and 20
that only execute by strncmp prefix-match accident — i.e. Lionhead shipped
these bugs in the retail game.

**Evidence:** `forge script validate <game-root>` (FableForge `18f81e7`;
`forge::cutscene::resolveVerb` replicates the case-sensitive strncmp prefix
dispatch, unit-tested).

- **Dead (no handler, no-op in retail):** `Teacher.ClearActions` (27 uses across
  27 scenes — native verb is `.ClearCommands`, not `.ClearActions`),
  `DoScriptName` (x4), `HERO.SetEntityMaxWalkingSpeed` (x2 — correct form is
  `.EntitySetMaxWalkingSpeed`), `GamePAuse`, `Fadeout` (case typos of
  `GamePause`/`FadeOut`), `Hero.SetScriptedMode` (native is `.AddScriptedMode`),
  `AnimationPhysics`, `ClearCommands` (bare global form), `dGamePause`, `nop`.
- **Loose (dispatched only because strncmp stops at the native length):** the
  `.PlayLoopingAnimation`/`.PlayCombatAnimation` forms (trailing `ation` past
  `.PlayLoopingAnim`/`.PlayCombatAnim`), `DoScriptFrame1` -> `DoScriptFrame`,
  `RemoveThing` -> `Remove`, `Mother.DialogSpeakSpeak` -> `.DialogSpeak`.

**Consequence:** confirms dispatch is case-sensitive prefix `strncmp` (not exact
match), independent of the source-slice reading. The validator is now a
shippable FableForge feature that catches these in modded content before
release. The 27-use `Teacher.ClearActions` no-op means those cutscenes never
actually clear that actor's action queue in retail — a latent behavior bug worth
noting if a faithful decomp/port ever "fixes" the typo.

## 2026-07-19 - Cutscene verb dispatch ORDER + heuristic arg signatures

**Claim:** the 184-verb strncmp chain is ordered longest-prefix-first for every
colliding-prefix group, so prefix dispatch is unambiguous by construction —
Lionhead ordered the chain deliberately.

**Evidence:** `tools/extract_cutscene_arg_sigs.py` walks
`ghidra_out/runcutscenemacro_full.c`, recovers each verb's dispatch anchor in
file order (= chain order), and classifies the arg slots (local_44=arg1 ..
local_18=arg12, stride 4) each handler reads. Output
`ghidra_out/cutscene_verb_args.tsv` (184 rows: verb, order, argcount, signature).

- Colliding prefixes resolve correctly: `RemoveAllThings` (order 75) <
  `RemoveAll` (76) < `RemoveExtras` (79) < `Remove` (168); had `Remove` come
  first it would swallow all three. Same pattern holds for the `Camera*` and
  `Crowd*` families.
- Arg-slot mapping verified against the hand-read slices (UseCamera arg1=camera
  name, arg2=time, ...). Signatures are a useful lower bound, NOT exact:
  - float args are parsed by the __fastcall `GFCharStringToFloat` (`0x0099E690`),
    which the decompiler shows argless, so those args fall back to `str`;
  - some entity `.Verb` handlers delegate arg parsing to shared helpers and show
    argcount 0 even when content passes args.

**Consequence:** exact dispatch order is now known (feeds correct prefix-routing
in any reimplementation), and the arg hints drive FableForge autocomplete
(`forge script verbs`) without being trusted as a hard arg-count contract.

## 2026-07-19 - Complete def schema: 240 def types, 3,212 fields (Transfer sweep)

**Claim:** the field layout of nearly every persisted definition class in TLC is
now mapped — offset and size of every field — by decompiling all 257
`CxxxDef::Transfer` methods and reading the offsets they stream through
`CPersistContext`.

**Method/evidence:** `tools/ghidra_scripts/DecompDefTransfers.java` batch-
decompiled all 257 retail Transfer methods (addresses from the RTTI port,
`ghidra_out/def_transfer_targets.tsv`) to `ghidra_out/def_transfers.c` (257/257
clean, 0 failures). `tools/parse_def_field_layouts.py` extracts the per-def field
offsets (each field shows as a persist-helper call on `this + 0xNN`), infers
sizes from consecutive offsets, and records the persist-helper as a type
discriminator. Outputs `ghidra_out/def_field_layouts.{json,tsv}`.

- **Validated against ground truth:** `CChestDef` extracts exactly
  `0x25 0x28 0x2c 0x30 0x34 0x38` — the six hand-mapped fields.
- 240 of 257 defs yielded field maps; **3,212 fields total**. Largest:
  `CPlayerGuiDef` (432), `CInventoryDef` (378), `CTavernGameDef` (93),
  `CEnvironmentDef` (62), `CThingCreatureDef` (50), `CHeroCombatDef` (42),
  `CShopDef` (37).
- The persist-helper *address* per field distinguishes types (e.g. CChestDef
  `0x30` uses `FUN_00431143` vs the common helper for the others), even though
  the decompiler mislabels the helper NAME (stale BSim/RTTI artifact
  `CEngineLightingManager::UpdateShadowScene`). Trust offsets + helper-address,
  not the incidental names.

**Caveats:** (1) field NAMES are not in retail (game.bin decodes by ORDER) — the
FableWin donor Transfer methods carry the names and are the next step to align
by order. (2) 17 defs yielded no offsets (Transfer delegates to a base class or
persists nothing directly). (3) huge field counts (CPlayerGuiDef/CInventoryDef)
are arrays persisted element-by-element; offsets are still exact.

**Consequence:** this is the backbone schema for both project goals — FableForge
can now model/edit ANY definition type's payload (not just chests), and a
buildable decomp gets struct layouts for 240 classes. Ordered offsets + the
game.bin per-entry payloads = a general def decoder.

## 2026-07-19 - Full NAMED + TYPED def schema (donor Transfer merge)

**Claim:** the complete game.bin definition schema is recovered — field names,
types, and serialization order for 269 def types, 4,332 named fields.

**Method/evidence:** the FableWin donor Transfer methods (Anniversary-era, PDB-
named) call named persist helpers that carry BOTH the field name (string literal
first arg) and the field type (encoded in the helper mangling). `DecompDefTransfers.java`
batch-decompiled all 269 donor Transfers (`donor_def_transfers.c`, 269/269
clean); `tools/merge_def_schema.py` extracts per-field (name, type, call-order)
and attaches retail memory offsets from the retail sweep where field counts
agree (`ghidra_out/def_schema.json`, mirrored to FableForge
`docs/re_reference/def_schema.json`).

- **Serialization order = call order, NOT offset order.** CChestDef persists
  0x28 before 0x25; CPersistContext streams in call order, so the donor call
  order IS the game.bin payload field order. This is the key to decoding
  game.bin payloads field-by-field.
- **Type mangling decoded:** `J`=int32, `K`=uint32, `M`=float, `_N`=bool,
  `E`=uint8, `VCCharString`=CCharString, `VCDefString`=CDefString,
  `VC2DVector`/`VC3DVector`, `VCRGBColour`, `VCFloatRange`, `W4Exxx`=enum,
  `TransferVectorOfSubComponents_VCxxx`=vector<Cxxx> (sub-def lists).
- **Validated on CChestDef** (game.bin order): OpenParticleEffect(int32),
  PersistOnOpening(bool), DisplayMessageOnEmpty(bool),
  OpenAnimationForCreature(CCharString), OpenerObject(int32),
  OpenersRequired(int32) — retail offsets align to the known
  0x28/0x25/0x2c/0x30/0x34/0x38 after correcting the donor's +4 Anniversary
  shift.
- 4,332 of 4,375 fields named (99%); 173 defs also carry aligned retail memory
  offsets. The few unnamed fields have their name string stored in a `&DAT_`
  global Ghidra didn't inline (resolvable later).

**Consequence:** FableForge can decode/edit EVERY game.bin definition type into
named, typed fields — the complete Creation-Kit-grade record editor schema. A
buildable decomp gets 269 real struct definitions with named members. This is
the single biggest data-schema result in the project so far.

## 2026-07-19 - Level-editor save/create recipe (FableWin editor RE)

**Claim:** the Lionhead level-editor pipeline for creating and saving levels is
recovered from FableWin.exe — including how a NEW level is registered — so
new-level authoring is a known recipe, not a guess.

**Method/evidence:** `DecompDefTransfers.java` batch-decompiled 50 `CEdit*`
editor functions (`ghidra_out/leveleditor_decomp.c`, 50/50 clean;
targets `leveleditor_targets.tsv`).

- **New level** — `CEditWorldMap::CreateAndSaveNewLevel(name:CWideString,
  coord:C2DCoordI, mapInit:CMapInit)` (`0x0296D2D0`): converts the name, adds a
  `CMapInfo` to the world-map `CArray<CMapInfo>`, initialises its 2D bounding
  box (`C2DBox::Initialise`) at the given world coordinate, sets map flags
  `+0x2c`/`+0x2d`, allocates the 0x1D78-byte map object. So **a new level = a
  new `CMapInfo` in the WLD world map at a free 2D coord + bounding box, plus its
  LEV+TNG files.** We already read/write WLD, so registration is in reach.
- **Save level to files** — `CEditWorldMap::SaveSingleLevelToFiles`
  (`0x0296E8B0`): writes (1) the TNG via `CThingManager::SaveToScript`, (2) the
  LEV via `CMap::SaveToFile(file, CFileFormatLevel)` using a
  `CReplaceSerialise<CFileFormatLevel, COldFileFormatLevel3>` version wrapper
  (matches our decoded LEV version 6404), and (3) brush/terrain paint data.
  `CMap::SaveToFile` is the authoritative LEV writer to mirror.
- **Regions / navmesh graph** — `CEditWorldMap::CreateNewRegion` (`0x0296D920`)
  maintains parallel `vector<CRegion>` and `vector<CEditRegion>` (asserts equal
  size); regions are the region-graph/navmesh units added per level.

**Consequence:** grounds the FableForge new-level and terrain-writer work: LEV
write = replicate `CMap::SaveToFile`/`CFileFormatLevel`; new level = add a
`CMapInfo` to the world map + emit LEV+TNG; navmesh = the region-graph builders
(next RE target). The engine demonstrably supports authoring novel maps because
this IS the editor that did it.

## 2026-07-19 - CGameScriptInterface: the native scripting API (891 methods)

**Claim:** retail TLC exposes a native game-scripting interface of 891 methods —
the native counterpart to FSE's 931-function Lua API. FSE wraps 790 of them; 101
native methods have NO FSE binding.

**Method/evidence:** `DecompByName.java` (new, general "decompile all functions
matching name substrings") swept `CGameScriptInterface`/`CScriptManager`/etc. to
`ghidra_out/scriptvm_decomp.c` (1,010 functions, 0 fail). Catalog:
`ghidra_out/gamescriptinterface_catalog.tsv` (891 methods + retail addresses).
Cross-referenced against `refs/fse_api_manifest.json` names.

- The interface is the entity/quest scripting surface: `GiveThingItemInHand`,
  `HeroGoDigging`, `OpenHouseDoors`, `SetTimer`/`RegisterTimer`,
  `EntitySetAsAllowedToFollowHero`, `WaitForCameraMessage`, conversation
  builders (`AddNewConversation`/`AddLineToConversation`/`AddPersonToConversation`),
  fishing (`GetHeroFishingLevel`/`DisableFishingSpot`), quests
  (`ActivateMultipleQuests`), logbook (`AddLogBookEntry`), text
  (`GetTextString`/`GetFormattedString`).
- **790 of 891 are already FSE-exposed** (FSE binds this interface). The **101
  not in FSE** (`ghidra_out/native_methods_not_in_fse.txt`) are direct candidate
  new FSE bindings — proving the "expand FSE from RE" thesis concretely
  (UNIFIED_SCRIPTING.md sense A): the natives exist and are named; binding them
  is mechanical.

**Consequence:** this is the authoritative index of native scripting capability.
The 101-method gap is a ready work-list for extending FSE; the full 891 catalog +
addresses feed FableForge autocomplete and the FSE binding-stub generator.

## 2026-07-19 - Native input/controller system decompiled (1,253 fns)

`DecompByName.java` swept CInputManager/CActionInputControl/CJoystick/
CInputType/XboxControllerButton -> `ghidra_out/input_decomp.c` (1,253 fns, 0 fail).
Key: **CActionInputControl** is the binding model with `GetDirection`/
`IsDirectional` (analog-stick aware), `IsSameActionAndButton`/`IsSameButton`
(remap comparison), and `TransferBinaryIn`/`TransferBinaryOut` (the binary
serialization for a persisted sub-component). `CControlsDef.Controls` is a
`vector<CActionInputControl>` in game.bin, so control remapping is a data edit
(feeds #16). BONUS for #12: `CActionInputControl::TransferBinaryIn/Out` are a
concrete example of the persist BINARY-mode read/write to model the string/vector
value encoding from.

## 2026-07-19 - game.bin field serialization: per-field hash-tagged values (PARTIAL)

**Claim (confirmed):** each field in a compiled-def payload is stored as a
constant **4-byte field-name hash tag** followed by its value — this is how
retail decodes defs by field without storing field-name strings.

**Evidence:** three vanilla `CChestDef` payloads compared byte-for-byte (entries
24=NULLDEF, 9247=normal chest, 12266=5-silver-key chest):
- `OpenersRequired` (int32) value at payload offset 38-41: `05 00 00 00` in the
  5-key chest, `00 00 00 00` in the others — matches the chest requiring 5 keys.
  Its tag `07 55 7d c0` (offset 34-37) is IDENTICAL across all three.
- `OpenerObject` (int32) value at 30-33: `d9 10 00 00` (0x10D9, the key def
  index) in the 5-key chest, `0` in the others. Its tag `d4 8f 85 e2` (26-29) is
  constant.
- Other constant 4-byte groups (`2a 90 2f a0`, `74 23 94 82`) sit at other field
  boundaries — the tags for the earlier fields.

**Open (NOT yet cracked):**
- The hash function is NOT crc32/fnv1/fnv1a/djb2/djb2x/sdbm/jenkins (all tested,
  as-is/upper/lower, ±null-terminator). It is a game-specific string hash — this
  IS the long-open "engine string-hash constant" question. Next step: decompile
  the CPersistContext binary `Transfer(name, T&)` helper (and the hash it calls)
  from retail to get both the hash and the exact per-type value encoding
  (especially CCharString length prefix and vector count).
- The leading-field interleaving (bools + the CCharString region, offsets 0-25)
  is not fully parsed from hex alone.

**Consequence for field-level merge (the practical win):** decoding does NOT
require cracking the hash. Given `def_schema.json` (field type order) plus the
per-type value sizes, a payload can be walked field-by-field ([tag][value]) into
named, valued fields — and two payloads for the same def can be diffed/merged per
field structurally (same tag, different value = changed field). So the field-
level merge in `forge merge` is unblocked once the per-type value encoding
(fixed sizes known; string/vector TBD) is nailed via the persist-helper decompile.

## 2026-07-19 - THE ENGINE STRING-HASH IDENTIFIED: field tag = CCharString::GetCRC()

> **⚠ SUPERSEDED (see "★ CANONICAL CRC REFERENCE" and "game.bin FIELD ENCODING
> FULLY CRACKED" below).** The headline below is WRONG: the game.bin field tag is
> **seed-0** reflected CRC-32, NOT `CCharString::GetCRC()` (seed `0xFFFFFFFF`). The
> whole section — including its own "CORRECTION" note — is kept only for history.

**Claim (SOLVED, long-open question):** the 4-byte per-field tag in game.bin is
the **CRC of the field name**, computed by `CCharString::GetCRC()` =
`CCRC::Calc(0xFFFFFFFF, chars, byteLength)` — a table-based CRC seeded with
`0xFFFFFFFF` (a non-standard variant, which is why plain crc32/fnv/djb2/jenkins
all failed to match).

**Evidence (donor FableWin decompiles, `ghidra_out/decomp_persist_helpers.log`,
`decomp_getcrc.log`):**
- `CPersistContext::CheckCRC` (donor `0x018747b0`) contains the assert string
  **`"stream_crc==CCharString(name).GetCRC()"`** — it reads a u32 from the stream
  and asserts it equals `CCharString(fieldName).GetCRC()`. So each field is
  `[u32 CRC(fieldName)][value]`, confirmed from the code (not just the payload).
- `CCharString::GetCRC()` (donor `0x0186d2a0`) = `GetByteLength()` +
  `GetAsCharArray()` + `CCRC::Calc(0xFFFFFFFF, chars, len)`.
- **`CCRC::Calc(unsigned long seed, void const*, unsigned long)`** = donor
  `0x0186d2d0` (`?Calc@CCRC@@SIKKPBXK@Z`). Also `CCRC::CalcFromString` /
  `CalcFromPathname`. DECOMPILE `CCRC::Calc` next for the exact table/polynomial.
- Binary persist templates (donor addrs, the per-type value encoders):
  `Transfer<long>` `0x018951e0` -> writer `<J,ABJ>` `0x01895220`;
  `Transfer<bool>` `0x018951a0`; `Transfer<uint32>` `0x01895cb0`;
  `Transfer<float>` `0x01895cf0`; `Transfer<CCharString>` `0x01895d30` ->
  writer `<...ABV1>` `0x0189c530`; `TransferVector<...>` `0x01894de0`.

**Consequence:** (1) field-level merge (#12) is unblocked structurally (same-def
payloads share tags; split on [4-byte tag][value] using def_schema type order).
(2) `.fmp` hash->name (#13) and authoring new fields need the exact CRC — get it
by decompiling `CCRC::Calc` `0x0186d2d0`, OR build the name->CRC table empirically
from donor field names (we have them) since retail payloads carry the CRCs.

**CORRECTION (2026-07-19, later):** the `[4-byte CRC(name)][value]` hypothesis is
WRONG for the binary game.bin format. The CRC table was dumped
(`ghidra_out/crc_table.txt`) and confirmed to be the STANDARD CRC-32 table
(`table[1]=0x77073096`, `table[0]=0`), and `CCharString::GetCRC` =
`~crc32(name)` (seed 0xFFFFFFFF, no final inversion). But computing that hash for
the CChestDef field names produces values that **do NOT appear anywhere** in the
chest payloads. So the recurring 4-byte groups in the payload are NOT field-name
CRC tags. The `CheckCRC`/"stream_crc==...GetCRC()" assert belongs to the persist
TEXT mode (case 0/1); the BINARY mode game.bin actually uses is a different
encoding, still not cracked. The recurring 4-byte groups are unexplained (a
different per-field marker, or constant default values). **Field-level def merge
remains blocked** on the binary encoding; record-level (whole-payload) merge is
the shipped fallback. The string-hash IS identified (`~crc32`) for other uses
(e.g. `.fmp`/name lookups) even though it does not explain the def payload tags.

Original (partly-superseded) notes below:

**CCRC::Calc decompiled** (`ghidra_out/decomp_ccrc_calc.log`): a standard
table-driven CRC-32 update loop —
`crc = (crc >> 8) ^ table[(data[i] ^ (crc & 0xFF)) & 0xFF]`, seed `0xFFFFFFFF`,
**no final inversion** (returns `crc` directly; standard crc32 does `~crc`, which
is exactly why standard crc32 didn't match). The 256-entry u32 table is at donor
VA `0x0449BE30` (`DAT_0449be30`) — dump it (1024 bytes) to replicate the hash
exactly, or note hash = `~crc32_customtable(name)`.

**Persist has MODES** (switch on `*(ctx + 0x18)`): case 0 = LoadText, case 1 =
SaveText, higher cases = binary load/save (what game.bin uses). The
`Transfer<CCharString>`/`TransferVector` binary-mode branches hold the exact
string length-prefix / vector count encoding — the remaining piece for
field-level decoding of string/vector-typed fields. FIXED-type fields
(int32/uint32/float = 4B, bool = 1B, enum W4 = 4B) are fully decodable now:
`[4-byte CRC tag][value]` per field in def_schema order.

## 2026-07-19 (latest) - ★ game.bin FIELD ENCODING FULLY CRACKED + corpus-validated

**The prior "CORRECTION" note above (claiming the CRC hypothesis was WRONG and the
binary format uncracked) is ITSELF WRONG and is hereby superseded.** The game.bin
per-field encoding is now completely solved and validated across the whole corpus.

**★ CANONICAL CRC REFERENCE (single source of truth — reconciles every CRC use in
the engine; if any other doc/section disagrees, THIS is correct):**
The engine has ONE CRC primitive, `CCRC::Calc(seed, data, len)` (donor `0x0186d2d0`):
a standard **reflected CRC-32** (poly `0xEDB88320` / the zlib table dumped to
`ghidra_out/crc_table.txt`, mirrored at donor VA `0x0449BE30`), **no final
inversion**. **Every on-disk use passes seed 0** (updated 2026-07-19 — see the
CORRECTION below; an earlier version of this block wrongly split the uses by seed).

- **Seed 0** (raw CRC accumulated from zero) — the ONE hash used everywhere:
  1. **game.bin** offline-compiled def **per-field tags** —
     `[u32 seed0-CRC(fieldName), little-endian][value]` (145/145 types validated).
  2. **text.big / BIGB symbol→ID** resolution — `CCRC::Calc(0, name, len)` in
     `NGameText::CDataBank::GetTextBySymbol` (`0x009c95e0`).
  3. **save-game binary-stream field tags** — `[u32 seed0-CRC(fieldName)][value]` in
     the `CPersistContext` HEADER/PLAYER/etc. sections (empirically verified: on-disk
     `WorldName` tag `a8de4f22` = `Calc(0,"WorldName")`; 23/23 HEADER + 2/2 PLAYER
     tags matched across 4 real saves).
  4. **save-file trailer checksum** — `Calc(0, file[0:trailer_pos])` over the whole
     file prefix (verified on 5/5 saves).

**★ CORRECTION (2026-07-19): there is NO seed-`0xFFFFFFFF` variant on disk.** Earlier
text here — and SAVEGAME_FORMAT.md / SAVE_WRITER.md — claimed the save-stream field tag
was `CCharString::GetCRC()` = `CCRC::Calc(0xFFFFFFFF, …)`. EMPIRICALLY DISPROVEN: real
save tags are seed-0, confirmed on three independent tags across multiple saves —
`WorldName`=`a8de4f22`, `Money`(gold)=`b03ccbfd`, `Morality`=`79a2d479`, each =
`Calc(0, name)`; the seed-`0xFFFFFFFF` values never appear on disk (e.g.
`0xFFFFFFFF`("WorldName")=`b128a473`, absent). The MECHANISM is still open — two Ghidra
readings of `GetCRC` conflict (one reads `Calc(0xFFFFFFFF, chars, len)`, another reads the
`0xFFFFFFFF` as a `CCharString(name,-1)` length sentinel); a fresh `GetCRC` decompile would
settle WHY, but it does not change the actionable fact. So EVERY name→tag / integrity hash
in the engine (game.bin, text.big, save streams, save trailer) is the single **seed-0**
reflected CRC-32. #12 field-level merge, #13 .fmp name-hash, and all save/text tooling key
off that one hash. (SAVEGAME_FORMAT.md + SAVE_WRITER.md corrected 2026-07-19; note
`tools/save_edit.py:get_crc` still defaults to 0xFFFFFFFF — pass seed 0 for field tags.)

**The field-tag hash (definitive):** each field's 4-byte tag is
`reflected CRC-32(fieldName)` with **seed = 0 and NO final inversion** (poly
0xEDB88320 / the standard zlib table), over the ASCII name **as-is** (no null
terminator), stored **little-endian**.

- This is NOT `CCharString::GetCRC()`, which seeds `0xFFFFFFFF`. That single
  difference (seed 0 vs 0xFFFFFFFF) is exactly why every earlier test
  (crc32/fnv/djb2/GetCRC) failed. The offline def compiler uses the seed-0 variant.
- Proof beyond doubt: computing this for the six CChestDef field names reproduces
  all six observed payload tags, **including the two the prior note called
  "unexplained groups"**:
  - `OpenParticleEffect`   -> `2a 90 2f a0`   (was "unexplained ?A")
  - `DisplayMessageOnEmpty`-> `74 23 94 82`   (was "unexplained ?B")
  - `PersistOnOpening`     -> `59 5f e8 00`
  - `OpenAnimationForCreature` -> `9a a0 c9 0c`
  - `OpenerObject`         -> `d4 8f 85 e2`
  - `OpenersRequired`      -> `07 55 7d c0`

**Why CheckCRC confirms it (correcting the persist-mode claim):** in the donor
`CPersistContext::CheckCRC` decompile (`decomp_persist_helpers.log`) the branches
that read/write the CRC are `mode==2` (binary IN) and `mode==3` (binary OUT); the
TEXT modes (0/1) `return` early doing NOTHING. So the CRC field tag belongs to the
BINARY format that game.bin uses — the opposite of the retracted note's claim.
(CheckCRC itself uses GetCRC/seed 0xFFFFFFFF for save-game streams; the offline
game.bin compiler emits the seed-0 variant of the same reflected table.)

**Full payload layout (per compiled-def entry):**
```
[variable prefix][field]*
  prefix = untagged base-class data. Length varies: 3 bytes ('01 00 01') for most
           types, 5 ('01 00 01 00 00') for some, larger for a few. End = offset of
           the first named field's CRC tag (located empirically).
  field  = [4-byte CRC(name) little-endian tag][value]
value sizes by def_schema type:
  int32 / uint32 / float / enum(W4Exxx) / CDefIndex / J   = 4 bytes
  bool (_N)                                               = 1 byte
  CCharString                                             = null-terminated
                                                            (no length prefix;
                                                             empty string = 0x00)
  Vector_<T>                                              = [4-byte count]
                                                            [count x element(T)]
  struct / Map_* types                                   = recurse via sub-schema
```
Serialization order == `Transfer()` CALL order in `def_schema.json` (NOT offset
order), which is what the field-tag sequence follows.

**Empirical string proof:** `CFireheartMinigameDef` entry 10516 shows
`14 c9 3b 4a` `GATEWAY_IDLE_01\0` then `da 32 88 a2` `FLOURISH_WISP_SHORT_01\0` —
chars begin immediately after the tag, null-terminated, no length prefix.
**Vector proof:** `CContainerRewardHeroDef` entry 8931 = tag `58 3d f0 08`
(CRC "ObjectFamilies") + `01 00 00 00` (count 1) + `04 0b 00 00` (def index 0x0b04).

**Corpus validation (scratchpad `validate_defs.py`, `validate2.py`):**
- Naive fixed-3-byte-header + fixed/vector/string decoder: **85 def types decode
  byte-exact (all tags match, leftover 0)**; the rest fail only on (a) exotic value
  types not yet sized (`C3DVector`, `CRGBColour`, `CDefString`, `Map_*`, nested
  structs) or (b) the 5-byte-prefix types.
- Resync tag-match decoder (auto-detected prefix): **145 / 145 schema'd types that
  appear in game.bin match EVERY field tag, in order, zero misses.** (100 of the
  269 schema types don't occur in game.bin; 4 have no named fields to check.)
- Prefix-length distribution across the 145: 3 bytes x124, 5 bytes x11, 11 x6,
  19 x2, plus 2 outliers with large leading regions (defs whose first field is a
  big vector/string, or extra base data) — to be characterized during the C++
  value-decoder build.

**Reusable reference:** `tools/decode_game_bin_field.py` (`field_tag()` + `decode()`
with the full spec in its docstring). This is the C++ port contract.

**Consequences (both long-blocked items now UNBLOCKED):**
- **#12 field-level def merge:** a def payload can be walked into named, typed field
  VALUES; two payloads of the same def diff/merge per field (same tag = same field).
  Ready to implement in forgecore (`forge defs decode` + field-level `defs merge`).
- **#13 .fmp hash->name:** the `.fmp` section entries are hash-keyed; the same
  reflected-CRC family is the key hash to resolve entry names.

## 2026-07-19 - Script-binding candidates: the 101-method FSE gap, spec'd + prioritized

**Claim:** the 101 native `CGameScriptInterface` methods with no FSE Lua binding
(`ghidra_out/native_methods_not_in_fse.txt`) are now a prioritized, evidence-backed
binding spec: **101/101 resolved** to a retail address (all present in
`gamescriptinterface_catalog.tsv`) and cross-checked against `refs/fse_api_manifest.json`
(931 already-bound functions). Full spec: `docs/SCRIPT_BINDINGS_CANDIDATES.md`.

**Method/evidence (≥2 sources per claim):** catalog gives name→retail addr for all 101;
manifest gives FSE's binding taxonomy (2 scopes — Quest/Entity — with typed parameters,
return type, blocking flag). Normalized/infix-stripped name matching against the manifest
surfaced **~18 VARIANT DUPLICATES** — native base overloads already reachable in Lua under a
typed/suffixed FSE name (cited pairs: `EntitySetAsOpinionSource`→`...ByInt/ByString`;
`EntitySetOpinionDeedMask`→`...ByInt/ByString`; `EntitySetPersonalityOverride`→`...ByInt/ByString`;
`SetIsGossipForPlayer`→`_ByName/_ByObject`; `RadialBlurFadeTo`→`_NoPos/_WithPos`;
`GiveHeroQuestCardDirectly`→`GiveQuestCardDirectly`; `SetHeroGuideToShowQuestCardsWhenSpokenTo`→
`SetHeroGuideShowsQuestCards`; `SetQuestCardGoldReward`→`SetQuestGoldReward`). One candidate
(`IsActiveThreadTerminating`, `0x006e71b0`) is confirmed by decomp
(`decomp_open_chest_scripts.log:109…`) as `CScriptBase`'s script-yield/abort guard — FSE-internal,
Tier-D.

**Top-tier (novel, high mod value, no FSE equivalent — bind first):** `HeroGoFishing`
(`0x895a90`), `HeroGoDigging`/`HeroStopDigging` (`0x8df80`/`0x8dfa0`), `HeroPlayOracleMinigame`
(`0x895b60`), `HeroPlayFireheartMinigame` (`0x895c90`), `SetHouseOwnedByPlayer` (`0x895ed0`),
`SetBuyableHouseAsScripted` (`0x896000`), `Open`/`CloseHouseDoors` (`0x8dfd0`/`0x8e000`), `JamDoor`
(`0x895e10`), `AddLogBookEntry` (`0x8fe00`). High-value read-backs: `IsThingWithThisUIDAlive`
(`0x8e260`), `IsHeroControlledByPlayer` (`0x8e980`), `GetHeroFishingLevel` (`0x895b00`),
`GetAllThingsInLevel` (`0x8a8af0`).

**Unresolved / needs decomp (7, exact arg set unverifiable from name alone — NOT fabricated):**
`CreateEffect` (`0x89f910`), `GetAllThingsInLevel` out-param shape, the conversation builder
(`AddNewConversation`/`AddLineToConversation`/`AddPersonToConversation`, `0x8906c0`/`0x890750`/
`0x890710`), the house/door bool args, `AddLogBookEntry` string-vs-id, and whether
`UpdateOnlineScore_*` (`0x8a1040`…) are XBL no-ops on the native PC build. Next step:
`DecompByName.java` on those ~7 addresses when the Ghidra lock frees.

**Consequence:** `SCRIPT_BINDINGS_CANDIDATES.md` is a ready work-list for the FableForge FSE
binding-stub generator; RVA = retail addr − ImageBase `0x400000`, thiscall through the
`CGameScriptInterface` vtable base `0x1260F0C`, FSE hook `0xCDB355`.

---

## 2026-07-19 — CONTROLLER / INPUT SUBSYSTEM RE (tasks #16/#17)

Full native input-binding + rumble map. Evidence: `ghidra_out/decomp_controller_donor.log`
(FableWin donor, 684 fns), `decomp_controller_retail.log` (retail Fable.exe, 87 fns),
`decomp_controller_retail2.log` (13 Rumble fns), `decomp_rumbledef_donor.log` (41 fns).
Cross-checked against `ghidra_out/def_schema.json` (CControlsDef, CRumbleDef). Retail addresses
are RTTI vtable-slot ports (`labels_rtti_port.tsv`) + BSim; donor addresses carry PDB names.

### Class / method map (donor @ 0x018xxxxx  ->  retail Fable.exe @ 0x00xxxxxx)

| Symbol | Donor | Retail | Notes |
|---|---|---|---|
| `CControlsDef::Transfer` | `017bce88` | `004330f0` | the game.bin controls def (binding table) |
| `CActionInputControl::ctor` | `017b8027` | - | one binding record, 28 bytes (7 dwords) |
| `CActionInputControl::IsSameButton` | `01848591` | - | reveals tagged-union device layout |
| `CActionInputControl::IsSameActionAndButton` | `017b746f` | - | action-id compare at +0x00 |
| `CActionInputControl::GetDirection` | `01790194` | `01889840` | reads C2DVector +0x14/+0x18 |
| `CPersistTraits<CActionInputControl>::TransferOut` | `017e149a` | - | the per-record persist writer (field order) |
| `CPersistTraits<EXboxControllerButton>::TransferIn/Out` | `017b2e7e`/`017a5922` | - | enum stored as LONG (numeric) |
| `CInputManager::GetJoystick` | (donor) | - | `GetJoystick(J)` returns CJoystick& |
| `CInputManager::GetKeyboard` | `018451e8` | - | |
| `CInputManager::IsUsingMouse` | `017bdbcb` | - | |
| `CJoystick::SetRumbleGloballyActive` | `017bb763` | (static, see below) | `DAT_04a67e31 = bool` - single global gate byte |
| `CJoystick::CInitRumble::ctor(f,f,f,f,bool)` | `01799195` | `0070c050` | rumble params: 4 floats + bool, defaults 1.0f |
| `CJoystickDX::AddRumble` | `018516e6` (**nop**) | `00ab7930` (**nop**) | **stubbed in BOTH builds** |
| `CJoystickDX::ClearAllRumbles` | - | `00ab7940` (**nop**) | stubbed |
| `CJoystickDX::GetRumbleWithID` | - | `00ab7910` (**ret 0**) | stubbed |
| `CJoystickDX::PeekRumbleWithID` | `0181c8b0` (**ret 0**) | `00ab7920` (**ret 0**) | stubbed |
| `CJoystickDX::GetJoystickDeviceNumber` | (donor) | `00ab7900` (**ret 0**) | stubbed |
| `CRumbleDef::Transfer` | `01bbcb57` | `004e676a` | screen-QUAKE tuning def (not motor) |
| `CGameScriptInterface::CreateRumble` | `01855dcc` | `0089fda0` | spawns a CTCDRumble quake thing |
| `CGameScriptInterface::ClearAllRumbles` | - | `00892df0` | |
| input-event classes | - | `00445630`+ | CInputTypeXboxPadButtonEvent / XboxPadLeftStickEvent / XboxPadRightStickEvent / MouseMovementEvent / MouseWheelMovementEvent |

### CControlsDef binding-table layout (the remapping surface)

`CControlsDef::Transfer` persists (donor offsets; **retail = donor - 0x10**, from the
RTTI-mapped retail Transfer at `004330f0` whose 6 toggle bools sit at retail +0x48..+0x4D):

| Field | Type | Donor off | Retail off (derived) |
|---|---|---|---|
| `Controls` | `vector<CActionInputControl>` | +0x44 | **+0x34** |
| `ToggleZTarget` | bool | +0x58 | +0x48 |
| `ToggleSpells` | bool | +0x59 | +0x49 |
| `ToggleSneak` | bool | +0x5A | +0x4A |
| `ToggleExpressionMenu` | bool | +0x5B | +0x4B |
| `ToggleExpressionShift` | bool | +0x5C | +0x4C |
| `FlourishNeedsAttackButtonHeld` | bool | +0x5D | +0x4D |

The `Controls` vector holds the action->button map. **Each `CActionInputControl` = 28 bytes**
(verified: `_Copy_backward` and `GetAssignedInputForAction` both stride 7 dwords per element).
Per-record persist order (from `CPersistTraits<CActionInputControl>::TransferOut` @ donor
`017e149a`, source-path literal `...\fable1_5mainpc\fablelib\defs\controls_def.hpp` = retail):

| Off | Field | Type | Meaning |
|---|---|---|---|
| +0x00 | `GameAction` | `EGameAction` (LONG) | the bound action id |
| +0x04 | `ControllerType` | `EControllerType` (LONG) | device discriminator: **1=Xbox pad, 2=keyboard, 3=mouse** |
| +0x08 | (keyboard key) | `EInputKey` (LONG) | used when ControllerType==2 |
| +0x0C | (pad button) | `EXboxControllerButton` (LONG) | used when ControllerType==1 |
| +0x10 | (mouse button) | `EMouseButtonControl` (LONG) | used when ControllerType==3 |
| +0x14 | C2DVector.x | float | analog/direction (runtime-derived, not persisted) |
| +0x18 | C2DVector.y | float | analog/direction |

It is a tagged union: `TransferOut` switches on `ControllerType` (+0x04) and writes ONLY the
matching device field (Xbox->+0x0C, key->+0x08, mouse->+0x10). `IsSameButton` (donor `01848591`)
confirms the same dispatch (type 1->cmp +0x0C, 2->+0x08, 3->+0x10). All three button/key values
are stored as plain 4-byte integers (`CPersistTraits<enum>` transfers via the SLONG path).

### Rumble path

Two distinct notions of "rumble" in the engine:
1. **Controller motor rumble** - `CJoystick::CInitRumble` (4 floats + bool) fed to
   `CJoystickDX::AddRumble`. Gated by the static `CJoystick::SetRumbleGloballyActive(bool)` which
   writes ONE global byte (donor `DAT_04a67e31`). **In retail Fable.exe the entire `CJoystickDX`
   rumble vtable is stubbed** - `AddRumble`/`ClearAllRumbles` are `nop`,
   `GetRumbleWithID`/`PeekRumbleWithID`/`GetJoystickDeviceNumber` `return 0`. The Anniversary
   donor `AddRumble` is *also* a nop, so DirectInput force-feedback was never wired on PC.
   => No motor-strength value is written anywhere on retail PC; controller rumble is dead code.
2. **Screen "rumble"/quake** - `CGameScriptInterface::CreateRumble` (retail `0089fda0`) spawns a
   `CTCDRumble` thing at a 3D position; strength/duration come from **`CRumbleDef`** (game.bin def,
   retail Transfer `004e676a`): `QuakeIntensities` (map<EQuakeStrength,float> @ +0x28) and
   `QuakeDurations` (map<EQuakeLength,float> @ +0x34). `ERumbleType` classifies events
   (NULL/WILL/DAMAGED/HITTING/QUAKE, from `header_enums.csv`). This IS live and tunable via data
   edit - but it drives camera shake, not the controller motor (which is stubbed).

### EXboxControllerButton enum values - GAP (hypothesis only)

The enum is persisted numerically (LONG). Retail strips def field-name/enum-name strings, and the
controls headers are NOT in the extracted DevHeaders archive (`header_enums.csv` has `ERumbleType`
but no `EXboxControllerButton`/`EGameAction`/`EInputKey`). The distinct native input-event classes
(`CInputTypeXboxPad{Button,LeftStick,RightStick}Event`) confirm the button-vs-stick split but
expose no literal constants (field-equality compares, not switches). **To make button ids
human-readable, the exact `EXboxControllerButton`/`EGameAction`/`EInputKey` integer->name tables
still need recovery** - best sources: (a) retail `controls_def.hpp` / `inputkey.h` DevHeaders if
the archive can be re-extracted, (b) empirical: dump a real CControlsDef payload from game.bin and
correlate integers with in-game bindings, (c) the fabletlcmod.com wiki controls page. Not resolved
this session (2 enum-scan attempts on both binaries + header_enums failed; stopped per
loop-prevention).

### Moddability verdict (tasks #16/#17)

- **Remapping = YES, pure data edit.** `CControlsDef` is a normal game.bin def; the `Controls`
  vector at retail +0x34 is a flat array of 28-byte `CActionInputControl` records, each
  `[EGameAction][EControllerType][keyVal][xboxVal][mouseVal][dirX][dirY]`. Editing the button/key
  integer for an action, or the ControllerType tag, remaps that action. This is field-level
  game.bin editing (same `[tag=CRC(fieldName)][value]` persist format already cracked; tag for
  `Controls` = seed-0 reflected CRC-32 of `"Controls"` — NOT `GetCRC`; see "★ CANONICAL CRC
  REFERENCE"). No native patch required for remapping. The
  layout is fully known; the only blocker to a turnkey remap UI is the enum value dictionary (the
  GAP above).
- **Controller-motor rumble tuning = NOT possible on retail PC short of a code patch** - the
  DirectInput rumble path is compiled out to nops. "Disable rumble" is already the de-facto state
  on PC. A native patch could re-implement AddRumble against XInput, but that is new code, not data.
- **Screen-quake ("rumble") tuning = YES, pure data edit** via `CRumbleDef` QuakeIntensities /
  QuakeDurations maps (retail offsets +0x28/+0x34; already in def_schema.json). This is the real
  moddable "rumble strength/tuning" surface on PC.

## Navmesh subdivision algorithm — CRACKED and oracle-validated (2026-07-20)

Full details in `docs/NAVMESH_RE.md` ("SUBDIVISION ALGORITHM — RECOVERED"); tool in
`tools/parse_navmesh.py` (`correlate` + `regen` CLI modes). Cited FableWin decompiles:
`CNavQuadTree::Initialise` 0x03290030, `CNavQuadTreeNode::Initialise` 0x032851b0,
`IsAreaClear` 0x0328c7e0, `IsAreaAllSamePreferability` 0x0328d400,
`GetPreferabilityCostForNode` 0x0328d710 (ghidra_out/decomp_navmesh*.c).

- **On-disk field semantics corrected**: node byte +0xc = quadtree detail level (0..7, cell
  size = 32/2^level map units, size table `DAT_044f344c`), +0xd = vertical nav-layer index,
  leaf tail byte +0x1c = preferability (A* cost). Root tiling = (mapW/32)x(mapH/32) cells.
- **Subdivision rule**: split while area is non-uniform (mixed walkability OR mixed
  preferability); depth cap 5 (1-unit), 6 (0.5-unit) inside "detailed areas"; switchable
  leaves only at max depth (never merged); all-blocked subtrees collapse to the singleton.
- **Walkability predicate** (IsAreaClear): TopologyWeights u8 grid @1 map unit (0xFF = hard
  blocked) + static blocking lines (8x8-unit buckets) + switchable blockage lines keyed by
  door u64 uid (keys become the leaf's on-disk switchKeys).
- **TopologyWeights pinned to LEV terrain, 0 exceptions over 149 nav-bearing retail LEVs
  (201,951 cells)**: 0xFF if LEV cell walkable byte(+15)==0; 0x00 if cell byte +20==1
  (preferred path — this classifies a previously-unknown LEV cell byte); else 0x80.
  Nav-walkable is a strict subset of LEV-walkable (obstacle lines carve ~19% more).
- **Oracle**: `parse_navmesh.py regen` rasterizes shipped leaves (0.5-unit), re-runs the
  recovered algorithm, compares node-for-node: **398/398 retail LEVs EXACT topology match**
  (every section, every layer). 249/398 LEVs ship with an empty nav directory.
- **Remaining gap for from-scratch generation**: `CWorldMap::GetMapNavigationAreaInit`
  (ego_r 0x004df950; retail BSim ~0x0050a650) — how placed-thing collision produces the
  blocking-line lists / switchable blockages / detailed areas. Terrain-only generation is
  already possible (over-approximates walkability only where things stand).

## 2026-07-20 - SAVE SIGNATURE IS ENFORCED ON LOAD (decompile-confirmed; editor unblocked)

**Question settled: retail Fable.exe DOES enforce the save trailer signature on load.**
Full evidence: `docs/SAVE_SIG_ENFORCEMENT.md`; logs `ghidra_out/decomp_save_sig_load1..4.log`.
- Verifier is `CUserProfileManager::VerifySignature` @ retail **0x00409730**: checks
  `FableSav`/`FablePro` magic (+ u32 `0x2165` = "e!\0\0" for saves), reads u32 @0x0C as
  `total_data_len`, computes `CCRC::Calc(0, file[0:total_data_len])` and compares to the
  stored u32 trailer at `total_data_len`; then requires file size EXACTLY 0x4B000 (saves;
  0x4000 profiles) and all pad bytes == 0x00 (saves; 0x20 profiles). Any failure -> false.
- `CWorld::LoadGameStateInternal` @ **0x004A21F0** calls it as its FIRST statement:
  `if (!VerifySignature(path)) return false;` - hard gate before any parsing. Also gates
  `CUserProfileManager::Load` @ 0x0040D350 (Profile.bin) and two cached save-slot-validity
  helpers (`FUN_005957d9`/`FUN_0047ed0a`).
- **No off-switch in retail**: raw image scan finds NO `UseSaveGameSignatures` /
  `UseBinarySaveGames` strings (donor-only); `UseRetailSaveGameSystem` @ 0x0122E85C exists
  (scan sanity check). SAVEGAME_FORMAT.md paragraph 8 "flip the flag" mitigation is impossible in retail.
- Consequence: edited saves MUST be re-signed with the seed-0 CRC over the literal file
  prefix + exact 300KB zero-padded slab - exactly what `tools/save_edit.py` (gates A-D 5/5)
  already produces. `forge save edit` is unblocked.

## 2026-07-20 - .QST QUEST REGISTRY FORMAT FULLY CRACKED (it's plain text)

**`.qst` is not binary** - ASCII CRLF script, tokenized by the engine at world load.
Full spec: `docs/QST_FORMAT.md`; parser: `tools/parse_qst.py` (parse/roundtrip/diff).
- Two retail files in `data/Levels/`: `FinalAlbion.qst` (187 `AddQuest` + 112 `AddTestQuest`)
  and `GlobalQuests.qst` (13 `AddQuest`). Zero non-printable bytes in either.
- Grammar: `AddQuest("Name", TRUE|FALSE);` (register quest; TRUE = active at start) and
  `AddTestQuest("Name","StartHSP",int,"Display","Ini","EndScript","QuestCardDef");`
  (dev debug-menu entries). Engine keyword strings confirmed in Fable.exe @ file offsets
  0xe38e98/0xe38ea8, next to "Load Quests"/"Init Quests" progress strings.
- Cross-validated: all 6 `FinalAlbion.wld` START_INITIAL_QUESTS names are exactly the
  .qst TRUE entries (+2 extra TRUE: ChapterAndSceneManager, NPCDeath); HSP names exist as
  TNG things; `OBJECT_QUEST_CARD_*` names exist in names.bin. SilverChest.Modern's
  `QstFile.cs` independently parses the same grammar.
- Ground truth mod: ArenaRevisited 1.0 = literally a 1-line diff, `Q_Arena` FALSE->TRUE
  (matches its readme). Parser roundtrips all 3 samples byte-identical; decoded diff
  reports exactly that flip.
- **Custom quest registration = append an `AddQuest` line** (proven by FSE's shipped test
  qst which appends MyFirstQuest/DemonDoorLUA etc.) - unblocks the Demon Door capstone's
  quest registration. Quest-mod merging = statement-level set union.
- Gaps for a later Ghidra pass (see QST_FORMAT.md): AddTestQuest arg-3 semantics (0/1/2),
  loader function address, engine comment support, GlobalQuests.qst load timing.

## Quest binding: logic is compiled C++; everything around it is data (2026-07-20)

**VERDICT (decompile-confirmed): TLC quest logic is NATIVE-CODE-BOUND — no quest VM exists.**
Full evidence: `docs/QUEST_VM_RE.md`; artifacts `ghidra_out/quest_*`.
- Registration is a compiled-in routine `FUN_00cd52d0 @ 0x00CD52D0` (ends at FSE's hook
  `0x00CDB355`): 161 straight-line `AddScript(0x00CB5C90)` calls, each registering
  `CScriptInfo{Name="Q_...", pAllocFunc=<native code ptr>, pAllocDataFunc=0x00CDBD20, "S_x" section}`.
  Extracted table: `ghidra_out/quest_registry_table.tsv`. Real classes = `NScript::CQ_<Name>Script`.
- Binding = `CQuestManager (DAT_013b89fc)::ActivateMultipleQuests @ 0x004B4260` →
  `FUN_004b3ce0 @ 0x004B3CE0`: `obj = (*pAllocFunc)()` (indirect call through the registered
  code pointer), wrap in `CActiveQuest`, `CScriptBase::Activate`, optional per-quest
  `LoadGameState`. Unregistered names are silently dropped (`IsQuestRegistered` gate).
- Quest object contract = 5-slot `CScriptBase` vtable (dtor/RegisterMain/Main/Init/OnPersist;
  e.g. `0x012D3994` for Q_HeroSoulsArena); threads = `CSpawnedFunc` raw code pointers.
- Two-level registry: `.qst AddQuest` lines register the *name* (data — see QST_FORMAT.md);
  the compiled table supplies the *logic*. Name-without-script is an anticipated state
  ("QuestNotInScripts"/"DUMMY_QUEST_HAS_NO_SCRIPT").
- Boundary for custom quests: cards/rewards/objective text/cutscenes/region scripts/initial
  activation/save state = DATA; new quest control flow = NATIVE CODE ⇒ FSE (or DLL) is
  structurally required, and its CScriptInfo-injection design is the minimal correct seam.
- Gotcha: BSim misnames in DB — `0x00CB8110` "CHeroMorphDef" is really the CScriptBase ctor,
  `0x00CBFAB8` "SetMiniMapRegionExitTextOffsetX" is really SetScriptActiveStatus (FSE-verified).

## Texture WRITE path: native image->big entry works (2026-07-20)

**Retexturing is UNBLOCKED — `tools/texture_build.py` turns a PNG/TGA into a valid
textures.big/frontend.big texture entry** (DXT1/DXT3/A8R8G8B8, full mip chain, engine LZO,
34-byte subheader), and patches it into a container copy via `big_write`. Full recipe +
field table: `docs/TEXTURE_WRITER.md`.
- **Layout correction to BIG_TEXTURE_FORMAT.md §4.1**: only MIP 0 is chunked-LZO; mips 1..n-1
  are stored RAW, concatenated. `Info+24` (`MipSize0`) = on-disk size of the mip-0 compressed
  region; **`MipSize0 == 0` means the whole payload is raw** (resolves the §6 stored-block
  question; this is what ChocolateBox mods emit and loaders accept). Source: EgoCore
  `TextureParser.h:289` + exact byte-accounting on 227/230 random retail entries.
- 34-byte Info = EgoCore `CGraphicHeader`(28)+`CPixelFormatInit`(6); DXT3 tail is
  `02 08 00 00 00 00` (old doc's `03 04` claim is DXT1-only).
- Validation: identity re-encode of 10 retail entries 46–67 dB; end-to-end new-image replace
  of TEXTURE_OV_SMITH_TORSO_01 in a copy re-decodes at 39.5 dB through BOTH our reader and
  the Blender addon's `fable_core` (6,323 untouched entries byte-identical); SilverChest
  `--texture-import` oracle produces a field-identical subheader (only MipSize0/LZO-length
  differs) and its payload decodes in our reader.
- Gaps: multi-frame sprites (FrameCount>1), rare fmts 0x23/0x18, DXT1 punch-through alpha,
  in-game screenshot proof.

## NEW-MESH COMPOSITION: custom static meshes as NEW graphics.big entries (2026-07-20)

**Custom-asset authoring is UNBLOCKED for static meshes — `mesh_rw.compose_mesh` builds a
complete compiled-mesh payload from arbitrary geometry (new topology/counts) and
`big_write.rebuild(adds=)` inserts it as a NEW TOC entry.** Full payload recipe, add-entry
design and validation: `docs/MESH_COMPOSE.md`. Key facts (EgoCore `MeshCompiler.h`
CompileSingleLOD = write-side answer key, cross-checked on retail entries):
- **Ghost LOD**: retail single-LOD type-1 entries append an empty duplicate mesh block
  (header + zero mats/prims + identity RootMatrix) NOT counted in the Info `LODCount`
  (PLATE: 1189 B payload = 971 LOD0 + 218 ghost). Composer mirrors it.
- **Sentinel material**: retail material lists end with a `DegenerateTriangles` material
  (all map IDs 0, DegenerateTriangles=1); `MaterialCount` includes it.
- **Info blob = EgoCore `SerializeEntryMetadata`** (`PhysicsIndex, bounds, LODCount,
  LODSizes[], SafeBoundingRadius, LODErrors[n-1], TextureIDs[]`) — confirms parse_mesh.py's
  descriptor: `lod[]` are LOD byte SIZES (LOD0 size == offset of the ghost/next LOD).
- **Retail type-1 vertex layouts** (400-entry survey): init 0x04/stride 12 (packed pos+norm+UV,
  dominant), 0x14/20 (FLOAT3 pos + packed norm + i16 UV), 0x06/20 and 0x16/28 (bump). No
  float2-UV layout exists in type 1; composer emits 0x14/20 ('float') or 0x04/12 ('packed').
- **TOC conventions**: ids unique + sorted per subbank (new id = max+1); name lpstr stored
  WITHOUT trailing NUL; stats header = type histogram (must be bumped); footer entry_count
  bumped; entry CRC is NOT CRC-32 of name/dep (bear and PLATE share ae689191) — new entries
  ship CRC=0/ts=0 like EgoCore's.
- **Bear statue gotcha**: type-1 "static" entries can carry a full 38-bone skeleton block
  while their primitives stay static (abc=0); compose v1 walks past and drops it.
- Validation (all PASS, `tools/blender_addon/tests/compose_test.py` + `blender_compose_test.py`):
  recompose oracle on bear/plate/sign (verts/faces/UVs EXACT, texture ids preserved);
  synthetic cube+grass added to a graphics.big COPY as ids 8113/8114, TOC CLEAN, re-decoded
  identical; Blender imports both from the modified copy as real textured meshes
  (proof: `tools/blender_addon/tests/proof/fable_compose_new_meshes.{blend,png}`).
- Limits: static only (no skinning), LOD0+ghost only, tri lists, no bump layouts/cloth/
  helpers; engine load not yet observed in-game (payload is grammar-identical to retail).

---

## Dormant multiplayer / co-op subsystem (2026-07-23, HIGH confidence, 2-source)

**Claim:** Fable: The Lost Chapters retains a near-complete but disabled **co-op multiplayer**
subsystem — networking transport, a client/host/local event-replication protocol, multi-player
management, and a combat-capable co-op "spirit" entity. This is the plumbing for Fable's famously
cut cooperative play. Reviving it is a *reverse-a-dormant-subsystem* problem, not a
*write-netcode-from-scratch* one.

**Evidence (retail `Fable.exe` name DB AND `debug_build/ego_r.exe` PDB agree):**

1. **Transport — `LHNetworkLib` (Lionhead network library).** Retail: `LSocket` (`CreateSocket`,
   `Bind`, `Listen`, `Accept`, `CheckForAcceptedConnectionsOnSocket`, `LSocket::Send`,
   `LSocket_ReceiveWithTimeout`, `SetNonBlocking`, `CNetworkConnection_Cleanup`). Debug build adds
   RTTI/classes `LSocketServer`, `DatagrammPacket` (UDP), `LSocketConnError`, plus `CNetworkClient`
   and `CNetworkServer` RTTI and the init string `"was unable to initialise the network manager."`
   Client + server + UDP + error handling + connection pooling (`std::list<LSocket*>`).

2. **Replication protocol — `CNetworkClient` + `CGameEventPackage`.** One class serves three roles:
   `InitialiseAsLocal` / `InitialiseAsNetworkClient` / `InitialiseAsNetworkHost`. The event flow:
   `SendGameEvent` -> `GetLocalGameEventPackageSet` / `GetGameEventPackageSet` ->
   `ReceiveGameEventPackageSet` -> `IsGameEventPackageWaiting` / `IsServerGameEventPackageWaiting` ->
   `CheckForLocalFrameUpdate`, with `CheckSync`, `ConfirmFeedbackGameEventPackage`,
   `UpdateFromEventPackageSet`, and save integration (`GetEventPackageSetFromSave`,
   `AddEventPackageSetToSave`). The commands themselves are the `EA*` action set (`EAMoveCreature`,
   `EAUseProjectileWeapon`, `EAControlledCreatureUseAbility`, `EASkipCutScene`, ...). This is a
   command/event-replication model (local events packaged, exchanged, sync-checked, applied per
   frame) — the standard shape for input-replicated netplay.

3. **Multi-player management — `CPlayerManager`.** `CreatePlayers` / `DestroyPlayers`,
   `GetMainPlayer`, `DowngradePlayerToNonMainPlayer`, and controller mapping
   (`GetPlayerNumberFromJoystickDeviceNumber`, `IsPlayerAssociatedWithJoystickDeviceNumber`,
   `GetMainPlayerJoystickDeviceNumber`) => local multi-controller co-op. Plus hero-swap
   (`GetPlayerHeroSwapScriptName`/`SetPlayerHeroSwapScriptName`), `IsMultiplayerGameActive` (a real
   108-byte query at retail `0x00449d20`, NOT a stub), `GetMultiplayerColour`, `GetSpiritDefName`,
   `GetSpiritScoreText`.

4. **The co-op player entity — `CTCCoopSpirit` / `CCoopSpiritDef`.** A first-class Thing type
   (`CTCCoopSpirit::Construct` retail `0x004d55d0`; full `CDefPointer`/`GetTC`/`PeekTC` machinery).
   It is combat-capable and score-bearing: `ApplyMovementVector`, `FrameUpdate`, `OnHit`, `OnStrike`,
   `GetMeleeTargetRange`, `GetAttackEffectName`; `AddExperience`, `AddScore`, `GetScore`,
   `ResetScore`; and it tethers to a master player: `SetMasterPlayer`, `GetAttractToMasterDistance`,
   `GetNoFramesForOffscreenReturnToMaster`. Definition constants name **four** slots:
   `COOP_SPIRIT_PLAYER_ONE..FOUR`. `CCoopSpiritDef::Transfer(CPersistContext&)` (retail `0x004526xx`)
   means the spirit state is **serializable** through the same persist context as the save-entity
   graph — so a co-op session integrates with existing save plumbing.

**Interpretation.** The design was up to 4 players joining as combat-capable "co-op spirits" tethered
to the main hero, earning score/XP, over a client/host `CNetworkClient` exchanging `CGameEventPackage`
sets across `LHNetworkLib` sockets. This matches the publicly-known cut Fable co-op ("Hero Spirit").

**Caveats / not yet established.** (a) Completeness and *reachability* of the path in retail is
unproven — whether it is complete-but-gated or partially gutted requires decompiling the cluster.
(b) Some debug-build network strings are statically-linked Perforce P4API (`$P4PORT`, "unopened
rpc", "Fatal client error; disconnecting") — those are asset-checkout tooling, NOT Fable netcode,
and were excluded. (c) Determinism for true lockstep is unverified; the event-package model may be
command-replication with server authority rather than lockstep.

**Highest-value next probe.** Decompile the gate + protocol core: `CPlayerManager::IsMultiplayerGameActive`
(`0x00449d20`), `CNetworkClient::InitialiseAsNetworkHost` / `InitialiseAsNetworkClient` (near
`0x004ae940`), `GetLocalGameEventPackageSet` (`0x004aeaa0`) + `ProcessEventPackage` (`0x00416670`),
and `CTCCoopSpirit::Construct` (`0x004d55d0`). Free transport options for reviving it (all $0):
ZeroTier/Radmin virtual-LAN over the built-in `LSocket` direct path (lowest effort); or, since TLC
ships on Steam (AppID 174790), Steamworks P2P (`SteamNetworkingMessages`) for free NAT relay +
lobbies; or GameNetworkingSockets (MIT) for Steam-quality transport without Steam. Transport is the
cheap part — the decomp-dependent work is the `CGameEventPackage` sync model.

### Co-op subsystem decompiled — COMPLETE-BUT-GATED (2026-07-23, Claude decomp loop)

Decompiled 22 functions of the co-op cluster (per-function verdicts): **19 LIVE, 2 GATED, 1 STUB.**
The subsystem is not dead — it is one flag away from running.

- **The single gate: byte flag `[CNetworkClient+0x2662]`.** Guards `Update` (`0x4ae9d0`),
  `IsFreeToRender`… actually `GetGameEventPackageSet` (`0x4aeba0`), and the update path — each
  early-returns when clear. `CNetworkClient::InitialiseAsLocal` (`0x4ae940`) is what SETS that flag
  (+ the active flag) to 1, gated only by a runtime precondition at `0x4eba10`.
- **`CPlayerManager::IsMultiplayerGameActive` (`0x00449d20`) is UNGATED** — real per-slot scan of the
  player vector; returns true once co-op players are seated. `GetMultiplayerColour` (`0x449b60`) is
  live with hardcoded P1..P4 colours (blue/red/cyan/green).
- **Full event-package pipeline is LIVE:** serialize (`CGameEvent::CompressIntoBuffer` `0x9f1810`,
  `CGameEventPackageSet::CompressIntoBuffer` `0x9f19a0`), deserialize (`0x9f1870`/`0x9f1ac0`), ingest
  (`ProcessEventPackage` `0x416670`, `UpdateFromEventPackageSet` `0x41726d`), input capture
  (`CProcessedInput::AddGameEvent` `0xa0d340`), and the co-op-spirit entity (`OnCreate` `0x6700f0`,
  `UpdateAttractionToMaster` `0x6701a0`, `SwapToHero` `0x66ff20`, `EAMoveSpirit` `0x62c0e0`).
- **Shortest path to first sign of life:** force `InitialiseAsLocal` `0x4ae940` to run (or NOP its
  `0x4eba10` predicate) so `[+0x2662]=1` — that lights the update + package-pump path. Directly
  poking the `+0x2662` byte is a faster probe.
- **Only genuine gutting: `CheckSync` (`0x4165e8`)** — deserializes three sync fields then DROPS them;
  no desync compare/report. Co-op will run but can't detect/correct divergence. This is the one piece
  to REBUILD, not merely re-enable.

Decompiled via the Claude Workflow loop (not the Codex re-agent lane). Bytes from retail `Fable.exe`.

## game.bin definition-load contract + the two append bugs (2026-07-24, HIGH confidence, verify=CONFIRMED)

RE'd via the Claude Workflow loop (`defload-contract-re`, 8 decode agents → synthesis → adversarial
verify). On-disk layout byte-proven vs the retail base; both bugs reproduced and fixed. Full write-up:
`docs/DEF_LOAD_CONTRACT.md`. Bytes from retail `Fable.exe` + `data/CompiledDefs/{game,names}.bin`.

**The canonical Fable name hash `crc0`** = reflected CRC-32, poly `0xEDB88320`, **seed 0, NO final
inversion** (`CCRC::Calc(0,…)` / `CCharString::ComputeCRC32` @0x00404310). Proven: **13593/13593**
names.bin stored CRCs equal `crc0(name)`; 0 match any other variant. `crc0("CREATURE_TRADER_01")=
0xAA22BB08`, `crc0("Graphic")=0x2E6B63C8`. Keys game.bin field tags, names.bin entries, and the
`std::map<unsigned_long, CDefClassInfo>` def-class registry (registrar 0x564395).

**Why an appended def was `nil` in-engine (two bugs, both required):**
1. forge's `nameCrc` used `0xFFFFFFFF - mz_crc32` (matches 0/13593 real names) → every NEW name got an
   un-resolvable CRC. Fixed in FableForge `bin.cpp`.
2. Creature payloads carry ABSOLUTE global entry indices as self back-refs (TRADER_01=1549 @ payload
   {25,193,301}); a byte-clone keeps the donor index → wrong wiring → instability. Fixed by value-keyed
   retarget in `02_add_creature.cpp` (shared component sub-defs left intact).

Header counts (nameCount/tableSize/entryCount/dense indexInDefinition) were already correct — never the
cause. Resolution: `CreateCreature 0x008A9100` hashes the name, queries the registry, and on miss returns
the null `CObjectRef` (typetag 0x1238C8C) → Lua nil.

## Co-op cluster — corrections + CheckSync rebuild (2026-07-24, verify=PLAUSIBLE)
Full write-up: `docs/COOP_REVIVAL.md`. Byte-solid (opcode-proven): enable gate = `[CNetworkClient+0x2662]`
(Update no-ops when 0; InitialiseAsLocal 0x4AE940 sets it + stores back-ptr +0x2678); `CGameEvent` wire
format `[u16 hdr(15-bit id|0x8000 flag)][u8 sub][u8 len][payload]` (Compress=pack dense, not compression);
CheckSync 0x004165E8 is genuinely STUBBED (reads 3 remote u32s + world checksum, discards all, no compare).
CNetworkClient is embedded at `CMainGameComponent+0x13AB8`.
**Corrections to the earlier co-op section (all same-lineage phantoms):** (1) the InitialiseAsLocal base-init
precondition `0x4EBA10` is a flat-disassembler ARTIFACT, not a resolved address — real target is
relocation-masked/UNKNOWN. (2) UpdateFromEventPackageSet's world forward is `0x0049DFB0 CWorld::Update`, not
`0x0049E0B0`. (3) the package/event loop accessors are structural GetCount/GetAt but engine_api.tsv only
BSim-mislabels them (no TSV name confirms). Raw-poking +0x2662 without InitialiseAsLocal storing +0x2678 can
CTD (forwarder null-deref) — enable via InitialiseAsLocal, not a bare poke.
