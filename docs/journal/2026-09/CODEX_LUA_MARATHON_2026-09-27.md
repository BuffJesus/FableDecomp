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
