# Marathon: preserve saved conversation handles, 2026-09-27

Continued from 9bdb299 in the existing Lua lane, applying the project recovery and
readability skills. Scratch: `work/codex_lua_marathon_20260927/`.

## Fix and evidence

The dead-store pass treated plain stack-scalar copies as pointer aliases and used
textual order across control-flow edges to decide whether they were dead. It
removed four decompiler copies restoring BookCollecting's conversation handle.
Retail 0x00E56C25 reloads ESI from [ESP+0x1C]; 0x00E56C35 pushes that saved handle
for IsConversationActive. The single-animation branch at 0x00E56CB0 joins that
reload. This is independent of the comparison return values previously left in Lua.

The pass now requires an explicit address or pointer cast before considering a
pointer alias dead, and preserves it across branches, jumps and labels. Plain
scalar copies survive. Straight-line, proven unused pointer stores still fold.

## Validation and promotion

- 144 tests / 28 subtests pass, exit 0 (`focused.log`).
- All eight new conversation cases fail on the previous generated code and pass
  in both corrected draft and readable code: boy/girl, dialogue/NULL, loop/NULL.
  They check the saved handle through yielding and removal, plus animation routing.
  Conversation data and engine resources are mocked; this is not in-game proof.
- All 29 registered units regenerated A/B from identical inputs. 29 draft Lua
  files change across 11 units: Arena, BanditCamp, BookCollecting, Bordello,
  GuardianSisterInfo, GuildTraining, SickChild, SummoningTheShip, TourGuide,
  TraderConflict and WhiteBalverine. Restored values include timers, actor handles
  and branch-local scalar copies. Some formerly stale inferred operands now remain
  explicit missing values. See `comparison.json` and `generated.diff`.
- All affected readables parse; 21 readable Lua files change. Before/after smoke
  has no new failing callback, and error text matches except Witch's renamed local.
  Counts: Arena 5/5, BanditCamp 0/0, BookCollecting 3/3, Bordello 4/4,
  GuardianSisterInfo 0/0, GuildTraining 0/0, SickChild 4/4, SummoningTheShip 0/0,
  TourGuide 2/2, TraderConflict 0/0, WhiteBalverine 0/0. These are limited smoke runs.
- Promoted 75 Lua/report files; old bytes retained in `promoted_baseline/`.
- Full readability audit: 369 files, zero syntax failures, 1,844 unresolved
  diagnostics; ledger refreshed. The preceding full-suite failures remain recorded
  in CODEX_LUA_CONTINUATION_2026-09-27.md; this pass used focused tests and corpus A/B.

## Continuing

No game launch or installation. V32 remains the latest packaged candidate; the
next candidate will incorporate this checkpoint with the following fixes.
Next: preserve pooled string-address operands through concatenation (teacher boy/
girl prefixes); correct BookOwned's retail +0xAC container evidence so persistence
cannot misclassify it as a scalar bool. Its byte-vector binding remains unsupported.
Other lanes' native-engine changes were preserved.

## Second checkpoint: pooled literal operands

After 03aa36b, preserved &DAT literal addresses through the string-helper operand
rewrite so the existing read-only .rdata resolver can decode them. The teacher's
opinion helper 0x00E56D10 now uses "boy" (0x012448EC) and "girl" (0x012448F0).
No literal value is guessed from its label; unresolved addresses remain unresolved.
Scratch: `work/codex_lua_literals_20260927/`.

- 37 focused tests pass. The actual generated helper failed on the baseline's
  DAT_012448EC concatenation and now looks up boy0/girl0 in both forms.
  That test only covers an empty pupil population; iteration/copy gaps remain.
- 29-unit A/B: only BookCollecting's teacher Lua changes. Three Lua/report files
  promoted after validation. All affected files parse.
- BookCollecting smoke improves 3 to 2 failures; remaining BookReaction and
  DoConversation failures are nil global-conversation data in the smoke harness.
- Corpus audit still has 369 files, zero syntax failures and 1,844 unresolved
  diagnostics. No installation or game launch; packaging follows the metadata pass.

## Third checkpoint: container offsets from native persistence (resumed after a crash)

The debug-PDB layout estimated BookOwned at retail +0xB0 (VC7.1 `vector<bool>` size),
which collided with nothing and let OnPersist's `CPersistContext::Transfer` at
0x00E54981 (callee 0x00CDCF80, `this + 0xAC`) be misread as a scalar bool
(`GetStateBool/PersistTransferBool/SetStateBool`). Retail stores both flag vectors as
12-byte `vector<unsigned char>` (BookDonated +0xA0, BookOwned +0xAC; Init resizes
`this + 0xAC` as `vector<unsigned_char>`).

`quest_unit_evidence.persisted_container_offsets` now moves a vector member to the
offset of its uniquely named native transfer, only when the name, owner (`this`),
offset and call-to-site pairing are all unambiguous and no scalar or other member
sits there. The unit JSON keeps `estimatedOffset` and an `offsetEvidence` record.

- `tools/script_recovery/test_persist_container_offsets.py`: 8 pass (six ambiguity
  cases leave rows untouched; the real BookCollecting OnPersist is checked).
- Generated OnPersist now emits an honest `TODO(native)` for BookOwned (byte-vector
  binding missing) instead of a wrong scalar-bool round trip.
- Scratch: `work/codex_lua_persist_20260927/`. No installation or game launch.
- A/B: all 29 units' evidence rebuilt with the new pass; `offsetEvidence` (the only
  thing the pass adds) appears in V_BookCollecting alone. Regenerated BookCollecting:
  only OnPersist changes (draft + readable, both parse; TODO count 173 -> 172).
- **Finding, not promoted:** a full rebuild of the committed unit JSONs shows drift in
  54 other files from earlier committed builder changes never regenerated into them
  (e.g. empty `definitionSnapshots`, Bordello `BooksPreviouslyOpened_*` slot fix,
  `resourceFields`, named `NativeThread_*` spawns). Those were reverted to HEAD here;
  regenerate + corpus A/B them as their own pass before the next bundle.

## Fourth checkpoint: stale unit evidence regenerated

All 55 unit JSONs (29 units) regenerated with the current builder and inputs. A/B:
each unit converted from HEAD JSONs, then from regenerated ones, with the same
converter. Scratch: `work/codex_lua_regen_20260927/` (`before/`, `after/`, logs).

Evidence drift (field-path counts): shared Gameflow globals `BooksPreviouslyOpened[3]`
packed at 0xEF-0xF1 instead of a stray 0xF7 slot (195, repeated per unit), empty
`definitionSnapshots` (54), `resourceFields` for `seh_*` members (47 quest + entity
rows), master `unmapped` (14), four renamed spawned-function keys, one array row.

Generated Lua changes in only three units:
- BookCollecting: `AddGossip` (0x00E55C60) moves position; body identical.
- Bordello 0x00E44980 and TourGuide 0x00EE6A40: a registered native thread with no
  spawn name was emitted as `function null(quest)`; now `NativeThread_<addr>`.
  Neither version starts it (nothing calls CreateThread for it) -- an open gap.

All three readables parse; 426 unit-evidence-dependent tests pass. No install or launch.

## Fifth checkpoint: spawned workers keep their spawn-site names

The two `NativeThread_<addr>` bodies were not orphans: Bordello's Magicman spawns
`CreateThread("WatchForHeroLeavingRegionWithBeer")` (body 0x00E44980, spawn in
0x00E40E80) and TourGuideGuide spawns `CreateThread("WatchForNoFollowers")` (body
0x00EE6A40, spawn in 0x00EE57B0; bsim independently names it
`CV_TourGuideScript::WatchForNoFollowers`). The inventory left `name` null because
the retail name is concatenated at run time ("ParentClass." + member), so the quest
defined the body under a key the entity's CreateThread never found.

`quest_unit_evidence.spawn_site_thread_names` fills a null name from the lifter's
own spawn patterns (same order, each spawn consumed once, wrapped typed-decompile
lines joined, "ParentClass." prefix rejected), only when every spawn of that body in
its registration function agrees. Test: `test_spawn_site_thread_names.py` (4 pass).
A/B (`work/codex_lua_threads_20260927/`): only the two function names change.
ChickenKicking's two unnamed bodies (0x00E6AF20, 0x00E6AD20; spawned by an
unreached entity Init) use an allocation shape no spawn pattern matches; no Lua effect.

## Candidate v33 (staged, not installed or run)

`work/new-oakvale-original-fse-20260912/local-candidate-v33` = v32 + eight refreshed
files in five packages (BookCollecting, Bordello, SickChild Witch, SummoningTheShip
BriarRose, GuardianSisterInfo2 MazeAtTavern). A file is refreshed only when v32's copy
equalled its committed readable source at v32's build commit 9bdb299. The 56 other
differing roster files were pinned to older, in-game-proven revisions and are kept.
(A blanket refresh would also have swapped in an older, hand-reviewed GuildTrainingSkill.)
Offline: 210 Lua parse, 215 manifest hashes match; mock smoke of the five packages vs
v32: 0 new failures, 1 fixed (teacher `DAT_012448ec`), 10 pre-existing. Sidecar unchanged.

## Sixth checkpoint: signed-char bit tests and cleanup-flag words

- `lower_signed_char_tests`: Ghidra renders a bit-7 test as `(char)x < '\0'`; the lowering
  dropped the cast (`x < 0`, never true for these unsigned words). Now `(x & 0x80) ~= 0`, and
  `(char)(x >> N) < '\0'` becomes `x & (0x80 << N)`. Corpus: 30 retail sites.
- `cleanup_flag_words`: MSVC's conditional-temporary cleanup words (`uStack_374 |= 0xe` ...
  `if (flags & 1) { flags &= ~1; destroy }`) are zeroed in the prologue, which Ghidra folds away;
  the Lua read nil. A hoisted local touched only by self bit-updates, `(v & K) ~= 0` tests and
  rebuild copies is initialised to 0 at entry. Magicman (0x00E40E80) qualifies. BordelloLady
  and Witch do not: their words share stack slots with a state int / CCharString (open).
- 29-unit A/B (`work/codex_lua_flags_20260927/`): eight units change, only those lines. All
  readables parse; GuildTraining's readable is hand-reviewed, so only its draft changes.
  `test_cleanup_flag_words.py`: 3 pass. No install or launch.

## Seventh checkpoint: out-vectors returned as table fields

SickChild Main (0x00EC5DE0) passes `pOutPos = &CStack_c`, `pOutOrient = &fStack_24` to vtable
0xBC0 and then takes `fpatan(fStack_24, fStack_20)`. Forge's GetSleepingPositionAndOrientationFromBed
returns `{pos, orient}` (LuaQuestState.cpp), so the atan read two nils. `OUT_TABLE_FIELDS` +
`Lifter.bind_out_fields` bind each trailing out-pointer's stack slot from the named field (float slot:
x/y/z at descending offsets; other variables: the whole vector), falling back to Forge's zero vector.
Only SickChild calls it; A/B changes only those lines. Smoke harness now returns that table shape.
SickChild smoke 4 -> 3 failures (Main fixed; TalkingTrader1/Witch flag slots and helper_ECE460 remain).

## Eighth checkpoint: cleanup-flag clusters

TalkingTrader1 spreads one flag word over copies (`uVar5 = uStack_b0` dropped; `uVar13 = uVar5 | 3;
uStack_b0 = uVar13;`, tests on uVar13), so uVar5 read nil. `cleanup_flag_words` now grows a
cluster from each tested word through copies/bit updates and zeroes every member when all their
mentions are those forms and a bit is set. 29-unit A/B: TalkingTrader1 (3 members) and Magicman
(its rebuild copy) only. Smoke: TalkingTrader1 Main fixed. `test_cleanup_flag_words.py`: 4 pass.
Remaining v33-package smoke faults: BordelloClient resource index, BordelloLady flag slot +
dialogue suffix table, Witch flag/CCharString slot overlap, SickChild helper_ECE460.
