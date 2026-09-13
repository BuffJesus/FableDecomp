# New Oakvale Lua marathon checkpoint — 2026-09-09

## Resume objective

Run New Oakvale with exactly one quest authority: Forge Lua must replace the native
`Q_NewOakValeIntro` allocator, not register the additive `NewOakValeIntro` quest alongside it.
Then retest the guard lecture, Father cutscene skip/audio, warehouse outcome, Bully/Victim, and the
childhood-to-Guild handoff.

## Last-run evidence

- Fable was closed cleanly at bedtime; no test process remains intentionally.
- The player observed a complete guard lecture, a brief pause, and the complete lecture again.
- Archived log: `refs/script_recovery/new_oakvale_intro/runtime_evidence/guard-double-scold-dual-authority-20260909.log`
  (SHA-256 `CCE4A1147AC045CFD45E1ACDD83AC3179A5375D732E63F79E1A47BA189040604`).
- The log contains exactly one Lua guard claim, one Lua movie sequence, and one issue of every
  `TEXT_QST_048_GUARD_CAUGHT_YOU_10` through `_60` key.
- Startup says the retail override is disabled and separately registers `NewOakValeIntro`; retail
  gameflow still owns/activates `Q_NewOakValeIntro`.
- Competing native and additive Lua authorities are therefore the evidence-backed cause inference.
  The next single-authority run is required confirmation; do not call it runtime-proven yet.

## Staged correction (not deployed)

- `refs/script_recovery/new_oakvale_intro/runtime_playtest/retail_override.lua`: checked in disabled,
  with exact native identity and all 16 retail entity bindings.
- `refs/script_recovery/new_oakvale_intro/runtime_playtest/quests.lua`: `Quests = {}`, so it adds no
  competing New Oakvale quest.
- The quest Main has a read-only `NOVI_AUTHORITY activeName/native/legacy` diagnostic.
- `tools/script_recovery/validate_new_oakvale_authority.py`: 3/3 focused tests pass; combined New
  Oakvale/Gameflow authority tests pass 6/6. The authority report is `ok=true`.
- Latest built DLL: `D:\Code\ForgeFSE-retail-shadow\Release\FableScriptExtender.dll`, SHA-256
  `0D5C73F6A50CE289C758F554CEF8ACCF33084F6CABB26B3004BADF80F5618D8E`. It includes safe
  pre-native `RunCutsceneABI` logging and is not deployed. Installed DLL SHA-256 remains
  `53B29F73D82D63EB16FC073B8C4272B564232D448A4541BC1BC35C287FC6495B`.

## Tomorrow's first sequence

1. Confirm `Fable.exe` is stopped and archive/hash the installed FSE profile for rollback.
2. Deploy the latest DLL to both the game root and game `FSE` directory.
3. Deploy the reconstructed `NewOakValeIntro/` tree.
4. Install `runtime_playtest/quests.lua` and `runtime_playtest/retail_override.lua`, changing only the
   deployed override copy to `enabled = true`.
5. Ensure no active `FinalAlbion.qst` or custom registry line calls `AddQuest("NewOakValeIntro", TRUE)`.
6. Launch only through `FSE_Launcher.exe` and inspect startup before gameplay.

Startup acceptance: the log arms and replaces `Q_NewOakValeIntro`, never registers a separate
`NewOakValeIntro`, and reports
`NOVI_AUTHORITY activeName=Q_NewOakValeIntro native=true legacy=false`. Gameplay acceptance is one
guard chase/lecture only. Capture the Father cutscene's `RunCutsceneABI` line; it decides whether the
missing skip flag is lost in Sol optional resolution or the lower compiled call. Keep renderer/Escape
hooks disabled because the prior renderer hook caused a confirmed GUI crash.

## Remaining gates

- Father in-game cutscene prompt/Escape skip and downstream 3D speech attenuation.
- Retest Barrel Man while the Hero remains among the barrels. Native judgment is visibility or
  distance-under-10, not a direct broken-barrel test; do not add a guessed flag override.
- Bully visible run-off and Victim release/follow behavior.
- Repeat the already successful childhood-to-Guild transition under single authority.
- Lua Gameflow remains validated but disabled pending save-field semantics and single-authority tests.
- Broad script-recovery tests are 119/121; two foundation bookkeeping expectations need updating for
  new package/profile directories. Focused authority tests are green.

## Single-authority runtime result (2026-09-10)

The disposable playthrough completed childhood and handed off successfully. The log records
`complete_quest activeName='Q_NewOakValeIntro'`, `SetQuestAsCompleted` entry/exit,
`AttackOver=true`, and clean Lua quest-host teardown. Archived log:
`runtime_evidence/single-authority-childhood-complete-20260910.log` (SHA-256
`39519E33CF88022185036D6798D029D7B45E8FBCD2763CE2F67592F8E1CB085E`).

Confirmed passes: one guard lecture under one native identity; Father cutscene Escape skip after the
typed Sol overload fix; complete childhood-to-Guild progression. Confirmed remaining seams: the
warehouse clock bar renders blue; Barrel Man sees `bad=1` but takes the thanks branch when the Hero
is 5.092 m away; Affair Wife begins her argument away from the visible husband, requiring live
thing-handle and distance-position diagnostics.

## Offline RE continuation — 2026-09-10

The blue warehouse bar is now explained and fixed in the canonical ForgeFSE tree. PDB-derived
`CRGBColour_u_0_s_0` gives byte offsets `B=0, G=1, R=2, A=3`; retail New Oakvale independently writes
the red away-state as `ff 00 00 ff` at `0x00DB50F8-0x00DB511F`. ForgeFSE declared the object RGBA,
so Lua red became retail blue. `GameInterface.h` now declares BGRA and `ParseColorTable` emits BGRA.
Release x86 builds successfully; DLL SHA-256 is
`AD184A3BC69C657CFF28E54D3CAE0FFB00124CC239C5AEDD3DEA2A3A17E4DDC3`. It is deliberately not
deployed while the player is away.

The wife route was rechecked instruction-by-instruction. Retail gets `NOVI_AffairMan`, snapshots his
position through `CScriptThing` vtable `+0x18`, issues exactly
`MoveToPosition(position, 2.0, 1, false, true)` at `0x00DB3476-0x00DB3486`, and then independently
loops on `IsDistanceBetweenThingsUnder(wife, husband, 3.0)` at `0x00DB3497-0x00DB34A9`. The Lua
control flow and Forge fastcall prototype match those operands. Do not replace the snapshot with a
guessed follow action; the next build needs live wife/husband position and handle-identity evidence
at route start and at the 3 m transition.

Those two probes are now present in the reconstructed wife script as `ROUTE_START` and
`ROUTE_REACHED`. Each records both XYZ positions and the engine-reported distance, so the next live
run will distinguish a stale husband handle, a moving target, and a false distance transition
without changing the retail route. All 110 package fixtures were regenerated and pass; the full
script-recovery suite is 123/123. Reconstruction manifests remain 46/46 applicable functions
implemented and traced (six lifecycle entries are explicitly not applicable).

`tools/script_recovery/audit_forgefse_colour_abi.py` now makes the colour fix mechanically
checkable against both the PDB layout and Forge source. Its two regression tests reject an RGBA
Forge declaration and accept only a BGRA declaration plus named-channel-to-BGRA conversion.

The broader New Oakvale ABI pass also found stale evidence notes left over from older, narrower
Forge bindings. The live bindings now expose both `EntitySetAsKillable` booleans, all three
`SetThingHasInformation` booleans, the facing snap flag, and the fifth yes/no-question flag. The
wife and guard inventories have been synchronized with the already-correct Lua calls first; the
remaining entity inventories are an evidence-hygiene queue, not known runtime defects.

Barrel Man's outcome branch was also reconfirmed in raw bytes. The only write of
`HeroLetMeDown=true` in Main is `0x00DB5B3C`, inside the undetected/away path; the detected path at
`0x00DB5C28` awards the good deed. Thus `bad=1` is not an input to the immediate judgment. The
observed 5.092 m Hero distance genuinely selects retail's thanks edge case; changing that branch to
test broken stock would be a deliberate design correction, not recovered retail behavior.

## Runtime closure — 2026-09-10

The single-authority run confirmed the competing-authority diagnosis. Startup armed and replaced
only `Q_NewOakValeIntro`, reported
`NOVI_AUTHORITY activeName=Q_NewOakValeIntro native=true legacy=false`, and never registered the
legacy additive quest. The player observed exactly one guard lecture; the log independently contains
one `Guard claimed lecture` record and one issue of each `_10` through `_60` line. Evidence:
`runtime_evidence/single-authority-guard-success-20260910.log`, SHA-256
`5DF7B7DF1C02338C68390499AEE03AECEBB9EB5FDE41454D2FBC21C72EE4560E`.

The same run also closed Father cutscene skipping after the explicit Sol boolean overload fix.
Remaining observed failures are the blue barrel timer and Barrel Man selecting `THANKS` after broken
barrels when the Hero is nearby (`bad=1`, distance `5.092`).

## Evidence and animation ABI cleanup — 2026-09-10

The remaining entity inventories were synchronized with the current Forge bindings and the
already-correct Lua calls. No stale one-boolean/four-argument limitation note remains for
`EntitySetAsKillable`, `SetThingHasInformation`, `EntitySetFacingAngleTowardsThing`, or
`GiveHeroYesNoQuestion`. Regenerated manifests still report 46/46 applicable functions implemented
and traced; package validation is 110/110 and the recovery suite is 123/123.

The animation concern was also resolved from Forge source rather than deferred to gameplay. The
entity methods registered in `LuaManager.cpp` select `PlayAnimation_NonBlocking`,
`PlayLoopingAnimation_NonBlocking`, and `PlayCombatAnimation_NonBlocking`; these expose every native
flag and forward them in order. Consequently the Affair animations and Dead Father's seven looping
flags are representable exactly. The reduced blocking convenience wrappers are separate methods and
are not selected by these Lua entity calls. `FORGEFSE_API_GAPS.md` now records that distinction and
no longer reports the obsolete reduced-signature claims.

The generated count of 15 blocked classifications is not a count of 15 missing APIs: it includes
host-managed quest helpers, lifecycle entries, cross-references such as `see seq 24`, and other
classifier artifacts. The only executable `NOVI.unsupported` sites remain the explicit nested
`StartScriptingEntity` and `ReleaseScriptingEntity` scheduler fallbacks in `common.lua`.

That result is superseded by the subsequent scheduler audit. `AcquireControl` and `ReleaseControl`
are mandatory registered methods in this Forge profile, so the unreachable legacy fallback branches
were removed. Validation now reports zero executable unsupported sites while all 110 fixtures still
pass. `audit_forgefse_control_abi.py` locks down method registration, caller-priority forwarding,
native retry/yield behavior, termination cleanup, cross-VM borrowing, and the rule that releasing a
borrowed handle must not destroy its owner's resource.

The API manifest parser now normalizes explanatory inventory text instead of inventing bindings
named `actors table`, `quest helper`, `see seq 27`, and similar fragments. The blocked count fell
from 15 to two: bare post-frame termination semantics and engine-owned
`CTCVillage::OnInitialActivate`. There are no blocked reconstructed functions. The former apparent
`MsgIsTalkedToByHero` gap was also only a name mismatch; Forge's `IsTalkedToByHero` wrapper constructs
`SCRIPT_NAME_HERO` and dispatches retail vtable slot `+0x6c` (`MsgIsTalkedToBy`).
Final offline verification after these changes: 23 Lua files parse, 110/110 fixtures pass, zero
missing/wrong-scope/unsupported call sites are reported, and the full recovery suite is 126/126.
Both colour and control ABI audits pass. No game process was started and nothing was deployed.

## Conversation ABI and blocker classification — 2026-09-10

The two remaining semantic deviations now carry explicit `blocking: false` evidence metadata.
Bare termination checks are folded into their adjacent `NewScriptFrame` result, and
`CTCVillage::OnInitialActivate` belongs to the engine host lifecycle. The generated API manifest now
reports zero blocked requirements without deleting either deviation from the evidence record.

The recurring `AddLineToConversation` uncertainty was resolved at the ABI level. The retail PDB
signature is `(id, key, bool, speaker, listener)`, and Forge's wrapper forwards its trailing Lua bool
into precisely that native bool slot before the two things. `audit_forgefse_conversation_abi.py`
cross-checks the PDB decoration, native typedef, and wrapper call order. The label `showSubtitle`
remains descriptive rather than symbol-proven, but all New Oakvale operands are false and preserved
exactly. Barrel Man's where-gone call now spells the false operand explicitly.

Forge's named Heal Life message helper also contained a confirmed latent mismatch: it passed decimal
30 even though New Oakvale native instructions push `0xE` and Forge's own enum defines
`HERO_ABILITY_HEAL_LIFE_SPELL = 0xE`. `MsgIsHitByHealLifeFromHero` now passes the named enum.
`audit_forgefse_hero_ability_abi.py` prevents recurrence. The canonical x86 Release build succeeded;
its undeployed DLL SHA-256 is
`64B1E8AE67BE00B1562F96A6348C782992CADA116BD180AD32C7D7F0E84406F4`.
Final verification for this tranche is 110/110 fixtures and 131/131 recovery tests, with zero
blocked, missing, wrong-scope, or unsupported requirements. Colour, control, conversation, and hero-
ability ABI audits all pass. Entity evidence JSON parses cleanly and the game remained closed.

## Speech termination propagation — 2026-09-10

Forge already polled the correct entity-thread termination predicate inside `Speak_Blocking`, but
discarded the result through a `void` API. It now returns `false` on termination/failure and `true`
on normal speech completion; all three Lua overloads return that value. Barrel Thug, Book Trader,
and Teddy Girl propagate it through their alive-guarded speech helpers, unwind the active movie
bracket, and skip retail post-speech state/actions after termination. Three new fixtures explicitly
force `Entity.Speak=false` and verify cleanup plus forbidden post-speech actions.

`audit_forgefse_speech_termination.py` locks down the bool API, Lua return forwarding, entity-thread
termination poll, and false/true outcomes. Canonical x86 Release builds successfully; the latest
undeployed DLL SHA-256 is
`38BC47A3B2B326CD0C0E5575B1C916616D2CCFDF80AD9889F87D791F589C35B2`.
Offline verification after this change is 113/113 fixtures and 133/133 recovery tests, with zero
blocked/missing/wrong-scope/unsupported requirements. Entity JSON parses and the game remained
closed. The next speech pass should propagate the same completion result through the remaining
alive-guarded helpers (Affair family, Guard, Father, Theresa, Victim, Bully, and Barrel Man) and add
branch-specific termination fixtures before declaring the whole speech surface closed.

## Package-wide speech propagation — 2026-09-10

That next pass is complete. Every direct `me:Speak` consumes the Forge boolean and every
`speak_if_alive`/`speak_to_hero` call is returned, assigned, or guarded. Interrupted speech now
unwinds the relevant movie/pause/control resources and skips later retail state changes across the
Affair family, Barrel Man, Bully, Guard, Live Father, Theresa, Victim, and Villager as well as the
three entities fixed in the prior pass. Of particular relevance to the observed affair sequence,
the Affair helpers now immediately propagate the host's false result before their extra task loop.

`audit_reconstructed_speech_propagation.py` scans all entity source and currently verifies 102
speech/helper call sites with no discarded results. New dynamic fixtures force interruption during
the Guard's first lecture and Barrel Man's initial favour; they prove the lecture flag and warehouse
handoff actions are skipped while cleanup still runs. Final offline verification is 115/115 fixtures
and 135/135 recovery tests. All five Forge ABI audits plus the package speech-propagation audit pass;
the manifest remains zero blocked/missing/wrong-scope/unsupported. The latest x86 DLL remains
undeployed at SHA-256 `38BC47A3B2B326CD0C0E5575B1C916616D2CCFDF80AD9889F87D791F589C35B2`.

## Movement ABI closure — 2026-09-10

The remaining Barrel Thug `FollowThing(..., 1.0, true)` uncertainty was semantic naming only. PDB
decorations prove the native layouts `(thing, float, bool)` for `FollowThing` and
`(vector, radius, moveType, bool, bool)` for `MoveToPosition`; Forge's active nonblocking wrappers
forward those operands in exactly that order without inversion or conversion. Thus the Thug and
Guard true follow operands and Affair Wife's recovered `false, true` move operands are ABI-exact,
even though names such as `avoidObstacles` are not present in the decorated symbols.

`audit_forgefse_movement_abi.py` now cross-checks both PDB signatures, both Forge typedefs, and both
wrapper calls. Its regression test rejects an inverted follow boolean.
Final offline verification for this tranche is 115/115 fixtures and 137/137 recovery tests. All six
Forge ABI audits plus the package speech audit pass, entity JSON parses, the manifest remains at zero
blocked requirements, and the game remained closed. No deployment was performed.

## Quest timer ABI and semantics closure — 2026-09-10

The New Oakvale timer path is now covered by `audit_forgefse_timer_abi.py`. It cross-checks the four
PDB signatures, retail vtable slots 87–90, Forge typedefs and slot assignments, and unchanged Lua
operand forwarding. Native decompilation proves registration initializes the stored value to zero,
`SetTimer` writes it, `GetTimer` reads it (returning zero for an absent handle), and deregistration
removes the map node.

Archived single-authority runtime evidence independently records WatchTimer values 45, 44, every
intermediate integer, and 0. Countdown direction and exact-zero termination are therefore observed
retail behavior rather than inference. The accessors do not by themselves prove the wall-clock length
of one integer unit, so Created Beetle and Villager evidence now preserve that narrower uncertainty.
The timer audit and both regression tests pass; the full recovery suite is 139/139 and all 17 entity
evidence files parse as JSON. The game remained closed and nothing was deployed.

## Speech operand and API metadata closure — 2026-09-10

A read-only Ghidra export now preserves Barrel Thug's complete retail instruction stream in
`ghidra_out/script_recovery/barrel_thug_speech_disasm.txt`. The WELLDONE call at 0x00DB7271 and the
TEMPT call at 0x00DB732D directly push `false, true, false, 2`; both then call the decorated retail
`Speak` implementation. Forge names method 2 `GROUP_SELECT_RANDOM_NO_REPEAT` and its full Lua overload
forwards every operand unchanged. `audit_barrel_thug_speech_operands.py` locks this down, so the prior
register-assignment uncertainty was removed. Other stale evidence notes were also retired where the
current tree already proved `F.set_master`, Live Father's information target, and ability 14 as
`HERO_ABILITY_HEAL_LIFE_SPELL`.

That audit exposed stale Forge manifest sources: `Speak_Blocking` had been changed to return bool, but
the aggregate declaration and generated registry still said void. The aggregate header is corrected,
both checked-in manifests and the generated registry were regenerated, and the curated `SpeakAndWait`
entry was corrected to its actual void implementation. The canonical native overlay now covers all
940 generated functions and passes Forge's structural/policy validator. The workspace's richer 947-row
manifest and overlay were updated too; `validate_tooling_sdk.py` now verifies exact manifest/overlay
key parity instead of an obsolete hard-coded 933 count.

The canonical Forge x86 Release build succeeds. Latest undeployed DLL:
`D:\Code\ForgeFSE-retail-shadow\Release\FableScriptExtender.dll`, SHA-256
`EDA4DBF3639DF089EE5CA5A047154ED027BC9E9FCE9D45994AB1419DE2B3E103`.
New Oakvale remains at 137 API requirements with zero blocked APIs and 115/115 fixtures; the full
recovery suite is 140/140. No game process was started and nothing was deployed.

## Affair Wife live-distance route closure — 2026-09-10

The retail instruction stream resolves the apparent “scolded the air” ambiguity. `0x00DB3426–
0x00DB3486` looks up `NOVI_AffairMan`, snapshots his current position, and issues exactly one
`MoveToPosition(position, 2.0, run, false, true)`. That snapshot is not the argument trigger:
`0x00DB3497` and `0x00DB357B` repeatedly pass the wife in ECX and the same live husband
`CScriptThing` local in EDX to `IsDistanceBetweenThingsUnder(..., 3.0)`. Only a true result reaches
`0x00DB3593`, followed by `ClearCommands` at `0x00DB35A6` and the argument loop.

`audit_affair_wife_route.py` locks that sequencing to the read-only disassembly export and verifies
Forge's `__fastcall` helper plus raw/shared Lua-object normalization. Route probes now record the
husband's definition/data identity, both positions, and computed distance, letting the next run
distinguish a wrong handle from a model-versus-script-position mismatch without altering behavior.

An adjacent regional lookup wrapper had a proven operand-order bug: its contract is `(scriptName,
regionName)`, but it forwarded `(regionName, scriptName)`. Canonical Forge now forwards name then
region; the name-only lookup used by Affair Wife was already correct. Both lookup/position and route
audits pass. Release|x86 builds with zero warnings/errors; the undeployed DLL SHA-256 is
`11B78FC81E8CF292D500FD1BBF1C1512761089D1277F5CFFCA2E2BE2DF2588B8`.

Offline gates: 23 Lua files, 115/115 fixtures, 137 requirements with zero blocked/missing/wrong-
scope/unsupported sites, 142/142 recovery tests, 17 valid entity inventories, Forge overlay policy
PASS (940 functions), and tooling SDK PASS (947 functions). The game remained closed and nothing
was deployed.

## Barrel Man consequence branch closure — 2026-09-10

A fresh read-only Ghidra export of the full `CNOVI_BarrelMan::Main` instruction stream confirms that
the observed thanks after breaking barrels is surprising retail behavior, not a reconstruction bug.
The automatic return judgment at `0x00DB5A88–0x00DB5ABC` tests only whether the man can see the Hero
or the Hero is within 10.0. It does not read `BarrelBrokenPersistent`; either true path reaches
`TEXT_QST_048_BARRELMAN_THANKS` and `AddGoodDeed` at `0x00DB5C28–0x00DB5D96`.

Broken stock affects only later phase-5 talk. `0x00DB65D7` first requires `HeroLetMeDown`, which is
set only by the absent-Hero return branch. Its true path then reads `PARENT+0x75`
`BarrelBrokenPersistent` at `0x00DB65F4` and chooses `LETDOWN_BROKEN` versus
`LETDOWN_NOT_BROKE`. Therefore a Hero who remains near the warehouse can break barrels, receive the
barrels' bad-deed consequences, and still be thanked by this man; his broken-barrel scold is only
reachable after also failing the stay-nearby judgment and talking to him afterward.

`audit_barrel_man_consequence.py` locks all nine branch facts to
`ghidra_out/script_recovery/barrel_man_main_disasm.txt` and the reconstructed Lua. No behavior was
changed. The recovery suite is now 143/143; the game remained closed and nothing was deployed.

## Guard repeat-lecture closure and stale-evidence cleanup — 2026-09-10

The “guard only scolds once now” observation is confirmed as the successful removal of duplicate
authority, not a lost repeat-offence path. A fresh `CNOVI_Guard::Main` instruction export shows the
loop comparing `BadDeedsPerformed` at `PARENT+0x58` against `GuardsDealtWithBadDeeds` at `+0x68`,
rechecking after the chase, and copying the current bad count into the dealt count at
`0x00DACA1C–0x00DACA1F` before speaking. `GuardsSpokenOnce` at `+0x92` selects the long first branch
or `TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN`; the first branch sets that byte at `0x00DAD31B`.

`audit_guard_repeat_lecture.py` cross-checks those instructions, the Lua state flow, and archived
runtime evidence containing distinct claims at `1/1` and `2/2` plus both first and repeat text lines.
This proves one lecture per newly incremented bad-deed count and preserves later scolds. Stale Lua/
JSON comments that still called already instruction-proven ally ordering and ability 14 “inferred”
were corrected without changing behavior. The dedicated disassembly is preserved at
`ghidra_out/script_recovery/guard_main_disasm.txt`.

## Affair conversation operands and uncertainty hygiene — 2026-09-10

Affair Wife's three `AddNewConversation` sites are no longer based on dropped decompiler arguments.
The PDB decoration proves `(CScriptThing const&, bool, bool)`, and the native call sites at
`0x00DB3334–0x00DB333F`, `0x00DB350B–0x00DB3510`, and `0x00DB3B6B–0x00DB3B70` each push
`false, false, ME_THING`. Forge's wrapper forwards both booleans unchanged. The reconstructed calls
were already correct; their evidence and comments are now instruction-level, and
`audit_affair_wife_route.py` expanded from nine to twelve checks.

The entity uncertainty lists were also audited for classification quality. Entries merely restating
proven rdata values, explicit instruction operands, tested Forge forwarding, reciprocal ally order,
or observed runtime behavior were removed. Genuine unknowns—such as timer wall-clock units, external
Barrel Man phase-1 provenance, Theresa's message-read stability, and the Affair Wife runtime identity/
visual-position mismatch—remain. No behavior changed.

## Theresa presented-item repeatability closure — 2026-09-10

Theresa's immediate second `MsgIsPresentedWithItem` call is now proven safe rather than an assumption.
Her retail instructions call vtable slot `0x8c` at `0x00DBA499`, compare the returned name with the
chocolate box, and reach a second call to the same slot at `0x00DBA4D1` on either false or mismatch.
There is no script frame between the reads.

The underlying retail `CGameScriptThing::MsgIsPresentedWithItem` implementation at `0x008D23C0`
locates interface `0x42`, traverses its current event list, and returns the definition name for event
type 10 or 11. It writes only the caller's output string; it does not erase, unlink, advance, or
otherwise mutate the event record. `CScriptThing` is a direct slot-`0x8c` delegate, and Forge invokes
that native accessor on every Lua call before publishing `g_PresentedItemName`. The reconstructed Lua's
two calls therefore reproduce stable retail reads without a cache or workaround.

`audit_presented_item_repeatability.py` locks the PDB identities, delegate, event-list scan, two Theresa
calls, absence of an intervening frame, Forge forwarding, and Lua re-query. Theresa's message-stability
uncertainty is removed; no runtime behavior or DLL changed, and nothing was deployed.

Final offline gates: 23 Lua files, 115/115 fixtures, 145/145 recovery tests, 137 API requirements with
zero blocked/missing/wrong-scope/unsupported sites, documentation checks clean, tooling SDK PASS with
947 Forge functions, and `git diff --check` reports no errors. The game remained closed.

## Dead Father acquisition-termination closure — 2026-09-10

The last assumption around `NOVI.acquire` is now a proved host contract. Dead Father's retail Main
loops on `StartScriptingEntity`, yields one frame after each failed attempt, tests active-thread
termination, and jumps to the scripted-resource destructor at `LAB_00DB8507`. Forge's persistent
`AcquireControl` implements the same order: failed native acquire, `NewScriptFrame`, entity/quest-host
termination test, pending-handle destructor and free, then explicit `false`.

`NOVI.acquire` preserves that false result and Dead Father releases/unwinds before marker lookup,
teleport, facing, or animation. The new `entity_dead_father_acquire_terminated` fixture forces this
path, while `audit_dead_father_acquire_termination.py` locks the native, Forge, helper, entity, and
fixture evidence together. The corresponding entity uncertainty was removed. No game launch or
deployment was required.

## Dead Father minimap-marker closure — 2026-09-10

Dead Father's two remaining minimap notes were stale uncertainty classifications. The Init record at
`0x00DB8290` constructs `HUD_ORB_QUEST_CORE`, selects `this+8` (`ME_THING`), and invokes GSI slot
`0x578`. The PDB decoration independently fixes the two operands as
`(CScriptThing const&, CCharString const&)`; Forge accepts and forwards the same thing/string order.

The removal side is already instruction-proven at `0x00DB84E8–0x00DB84EE`, and the fact that retail
leaves the marker behind when termination precedes `DadFound` is a known cleanup-path behavior, not an
unknown. `audit_dead_father_minimap_marker.py` now locks the native setup, signature, Forge forwarding,
Lua call, and lifecycle evidence. `OVI_DeadFather` consequently has no remaining uncertainties.

Offline gates after both Dead Father closures: 116/116 fixtures, 147/147 recovery tests, 23 Lua files
without errors or warnings, 137 API requirements with zero blocked sites, clean documentation checks,
and tooling SDK PASS at 947 Forge functions. The game remained closed and nothing was deployed.

## Victim Init identity closure — 2026-09-10

Retail `0x00DAEEB0` is now reproducibly identified as `CNOVI_Victim::Init`, despite Ghidra's 0.715
Book Trader donor label. The allocator at `0x00DAEDE0` creates a 32-byte object and installs vtable
`0x012D87B8`; its Init slot at `0x012D87C0` points directly to `0x00DAEEB0`. The body zeros offsets
`0x1c` and `0x1d`, matching the donor PDB layout names `DisplayedGameInfo` and `DoneThanks`.

The negative control is independent: actual Book Trader allocation installs vtable `0x012D8E80`,
whose Init slot points to `0x00DB3F00`, and that body initializes only its single `0x1c` field.
`audit_victim_init_identity.py` locks allocator, vtable, body, donor symbol/layout, and negative-control
evidence. The Victim inventory now has zero uncertainties; no behavior changed.

Offline gates after the Victim identity audit: 116/116 fixtures, 148/148 recovery tests, 137 API
requirements with zero blocked sites, all 23 Lua files clean, documentation checks clean, and tooling
SDK PASS with 947 Forge functions. The game remained closed and nothing was deployed.

## Logbook helper ABI and deed-page correction — 2026-09-10

Fresh native exports replace all donor-name inference for the three adjacent logbook helpers. Retail
`0x00CBE87F` consumes the numeric id in ECX, converts it to decimal, prefixes
`TEXT_QST_LOG_STORY_`, and submits category 1. `0x00CBE960` consumes a `CCharString` in ECX, derives
`_NAME` and `_DESC`, and also submits category 1. The New Oakvale first-good and first-bad paths do
not call that story-string overload: both construct `TEXT_QST_LOG_BASICS_MAP` and call `0x00CBE9EE`.
That third helper derives `<key>_TITLE`, retains the base key, submits category 2, then calls GSI slot
`+0x1c` for its trailing script yield.

This exposed a real reconstruction/Forge mismatch. `deeds.lua` formerly called
`AddLogbookStoryEntryString`, routing to `0x00CBE960`; it now calls `AddLogbookTutorialEntry`.
Forge's tutorial wrapper formerly built three literal `CWideString` values through generic
`AddLogBookEntry`; it now passes a retail `CCharString` directly to a dedicated `0x00CBE9EE` thunk,
preserving key localization, category, and the native yield. `audit_logbook_helpers.py` locks the
three helper bodies, distinct addresses, Forge declarations/wiring, and New Oakvale call together.
The Q_NewOakValeIntro helper uncertainty was removed.

The canonical Forge Release|x86 build succeeded with zero warnings/errors. Undeployed DLL SHA-256:
`2B24EDDBA821F1D1F49FF8DE7B1ADE341FD03A4728A54648CBE4D42C96DDA5E2`.
Offline gates: 23 Lua files clean, 116/116 fixtures, 149/149 recovery tests, 137 API requirements with
zero blocked/missing/wrong-scope/unsupported sites, 46/46 applicable functions traced with zero
uncertainty, documentation checks clean, and tooling SDK PASS with 947 Forge functions. The game
remained closed and nothing was deployed.

## Dead-father single movie-bracket closure — 2026-09-10

The final Q_NewOakValeIntro movie-bracket note is now a proved contract. Retail explicitly orders
StartMovieSequence, entity pause, fixed camera, the dead-father macro, camera release, unpause, and
movie teardown. The Lua preserves that one outer bracket. Forge keys active movie handles by the
calling Lua VM; `RunCutsceneWithSetup` finds the existing handle and skips its own open, pause,
camera setup, close, and teardown when borrowing it. The successful father-skip log independently
records `Reusing caller-owned movie sequence`, entry and exit of the skippable native macro, and clean
continuation. `audit_dead_father_movie_bracket.py` locks all three evidence layers together.

After manifest regeneration the package has 137 unique API requirements, zero blockers, and no
remaining Q_NewOakValeIntro uncertainties. Offline suite: 116/116 fixtures and 150/150 recovery
tests. The game remained closed and nothing was deployed.

## Retail platform-query restoration — 2026-09-10

The Barrel inventory still described `IsXbox` as unbound even though Forge now exposes and directly
forwards the retail CGSI virtual. The shared helper and all three New Oakvale consumers (Barrel,
Live Father, Victim) now call `Quest:IsXbox()`; the PC executable naturally returns false, while the
retail branch and Xbox text keys remain faithfully represented. The obsolete uncertainty and stale
Live Father comment were removed. Package validation remains 116/116 and the full recovery suite
remains 150/150.

## Barrel predicate lifecycle and return-consequence closure — 2026-09-10

The remaining barrel predicate note is now closed across native construction, callback ownership,
Forge dispatch, and runtime evidence. Retail allocates `CNOVI_Barrel` as an active entity script bound
to a copied `CScriptThing`; derived vtable `0x012D94F0` points to its `OnPredicateFail` at slot
`0x012D9504`. Donor symbols independently identify the active-entity `CIsThingAlive` infrastructure.
The callback sets both break flags and copies the bound thing's position. Forge performs the native
`IsAlive` virtual check when the Lua Main unwinds and dispatches `OnPredicateFail(Quest, Me)` once.
The archived single-authority run reaches Barrel Man with `bad=1`, proving this destruction path ran.

The automatic return judgment was rechecked against the existing instruction audit. It tests sight,
then distance under 10 m, and never reads `BarrelBrokenPersistent`. The archived hero was unseen but
5.092 m away, so retail necessarily selects `BARRELMAN_THANKS` and `AddGoodDeed`. Broken-stock scolding
is only selectable during later talk after `HeroLetMeDown` was set by failing that presence judgment.
The fixture now named `entity_barrel_man_nearby_broken_scolds` originally locked this exact,
surprising
behavior and forbids the absent-hero lecture. No non-retail scold was added.

Offline totals are now 117/117 fixtures and 151/151 recovery tests; API requirements remain 137 with
zero blockers. The game remained closed and nothing was deployed.

## Theresa stack-local lifetime and Barrel uncertainty hygiene — 2026-09-10

Theresa's `given_chocs` flag is no longer an inference-only note. The Main prologue reserves its
fixed stack frame and initializes the byte at base `esp+0x17` (temporarily `esp+0x1f` with two
arguments pushed). All three true writes use `esp+0x1b` with one push outstanding, and all three
later reads use `esp+0x17`; they therefore address the same Main-local byte. The Lua keeps precisely
that run lifetime while retaining `GivenTheresaChocs` as a separate persistent quest write. The new
`audit_theresa_given_chocs_lifetime.py` locks those instruction and reconstruction facts together.

The Barrel's tail note was also reclassified: its post-instruction frame loop is explicitly present
as operation 8 and faithfully reconstructed, so it is established retail behavior rather than an
uncertainty. Both Theresa and Barrel now have empty uncertainty inventories. Offline totals are
117/117 fixtures and 152/152 recovery tests. The game remained closed and nothing was deployed.

## Barrel Man dead phase proof — 2026-09-10

A fresh headless donor export recovered the exact `CNOVI_BarrelMan` constructor and Init. Construction
only installs the vtable and parent/master pointers; Init writes `MyPhase = 0`. Retail Main's complete
set of writes to `this+0x20` is `{2, 2, 3, 4, 5}` and contains no write of 1. Phase 1's walk-off body
is therefore unreachable from a clean class lifecycle—preserved dead retail code, not evidence of an
unknown external transition. `audit_barrel_man_phase_reachability.py` locks the donor constructor,
donor Init, retail write set, PDB layout, Lua preservation, and manifest classification together.

The two top-level marker handles and unused guard-point lookup remain faithfully reconstructed but
are no longer listed as uncertainties: their lookup and termination cleanup are directly visible in
retail code. Only the human-readable phase labels remain inferred; all numeric values and transitions
are exact. The game remained closed and nothing was deployed.

## Affair Wife movement/position narrowing — 2026-09-10

The apparent movement-flag mismatch was traced through Lua registration before any edit was made.
The old blocking `GainControlAndMoveToPosition` helper has four arguments and default flags, but the
`MoveToPosition` method used by reconstructed entities is registered to `MoveToPosition_NonBlocking`.
That path accepts both optional booleans and forwards the wife's exact retail `false, true` operands.
The movement ABI audit now checks the registration and caller-operand plumbing, preventing the older
helper from producing a false positive; a negative test proves dropped booleans fail the audit.

A fresh donor export also proves `CScriptThing::GetPos` validates its counted implementation and
dispatches implementation vtable slot `0x18`; `CGameScriptThing::GetPos` returns its inline vector at
`+0x30`. Forge calls precisely that implementation slot and copies x/y/z unchanged, so its direct read
is equivalent to the retail wrapper for a valid handle. Together with the already-proven hidden-result
lookup and live distance helper, the air-scolding observation is now narrowed to runtime husband
identity/replacement or model-vs-thing-position state. No speculative behavior change was made. The
game remained closed and nothing was deployed.

## Native timer cadence closure — 2026-09-10

The timer accessors alone exposed only an integer map value, but the PDB-named
`CGameScriptInterface::WorldUpdate(bool)` donor body closes its unit. On eligible world updates it
reads `CWorld::GetFrame()` and `CMainGameComponent::GetConstantFPS()`, enters the timer walk only when
`frame % constantFPS == 0`, and decrements each value only when it is positive. Combined with the
retail accessor bodies, exact Forge forwarding, and archived 45-to-0 trace, this proves one timer unit
per constant-FPS second with saturation at zero.

The timer audit now enforces the cadence and positive guard. Created Beetle's value 5 is therefore a
five-second lifetime, and Villager's shared value 3 is a three-second speech throttle; their stale
wall-clock uncertainties were removed. The game remained closed and nothing was deployed.

## Villager hit-control lifetime closure — 2026-09-10

The remaining Villager reconstruction-only timing difference is closed. Retail retains the scripted
entity resource acquired by the hit branch across `LAB_00dae98e`'s bottom `NewScriptFrame`, then clears
it at the top of the next loop before testing another hit. Lua now tracks that ownership with a Main
local, releases at the same loop-head point, and explicitly releases on termination during the bottom
frame, matching the native cleanup label.

The focused lifetime audit locks the ordering and manifest classification. The hit-then-reproach trace
now records `NewScriptFrame` before `ReleaseControl`, all 117 fixtures pass, and the recovery suite is
155/155. Villager now has zero uncertainties. The game remained closed and nothing was deployed.

## RemoveThing operand and metadata closure — 2026-09-10

`CGameScriptInterface::RemoveThing` at `0x008910d0` resolves the Created Beetle's two true operands
without relying on speculative parameter names. Its first boolean is passed directly to
`CThing::Kill` (recovered elsewhere as `destroyImmediately`); when the second is true, the function
sets bit `0x02` in the underlying `CThing+0x93` flag byte before calling `Kill`. Forge's native type
and Lua wrapper preserve that order, and the beetle supplies `true,true`. A new ABI audit locks the
native effects, Forge plumbing, and call site together; Created Beetle now has zero uncertainties.

Barrel Man's remaining note described only the readability labels assigned to exact numeric phases.
Those labels remain explicitly tagged `nameEvidence=inference` beside each enum value, but are now a
design note rather than a behavioral uncertainty. The phase audit enforces that distinction as well
as the exact native write set and unreachable phase 1. The only remaining entity-inventory uncertainty
is Affair Wife's runtime identity/position anomaly, for which probes are already staged. All 117
fixtures and 156 recovery tests pass. The game remained closed and nothing was deployed.

## Stale parity evidence and wrapper closure — 2026-09-10

A source-to-manifest audit found several old limitation notes that no longer described the current
Forge fork. Dead Father's registered nonblocking `PlayLoopingAnimation` accepts and forwards all seven
native booleans, and the Lua call already supplies the instruction-proven
`false,true,false,true,true,false,false` sequence. Victim likewise supplies both
`EntitySetAsKillable` booleans and all three `SetThingHasInformation` booleans; Forge's native types
and wrappers preserve them in order. Focused audits now prevent either path from regressing.

Randomness is exact rather than Lua-compatible approximation: every one of the 13 reconstructed random
sites calls `RetailRandModulo`, which invokes the relocated retail MSVCR71 `rand` thunk at `0x00BFEB16`
and applies the native modulus. Another audit rejects Lua RNG use and locks the retail thunk, calling
convention, and site count. `FadeScreenOut` also does not drop New Oakvale's colour either: Forge
constructs `{0,0,0,255}` and passes it as the native third operand, exactly matching both Barrel Man
and Theresa.

Finally, Barrel Man's preserved unreachable phase 1 now reproduces retail's `DAT_0143e8e0` zero-vector
fallback when its walk-off marker lookup is null, instead of dereferencing a missing Lua handle. The
phase reachability audit covers this dead-body fidelity as well. Stale zero-blocker documentation and
the two obsolete Wife “inference” comments were corrected. All 117 fixtures and 161 recovery tests
pass; coverage remains 46 applicable functions traced with zero uncertainty and 137 API requirements
with zero blockers. The game remained closed and nothing was deployed.

## Guard distance and movie-destructor closure — 2026-09-10

The Guard inventory's last assumption-language note was caused by a decompiler register artifact, not
an unknown operand. Retail instructions at `0x00DAC8A1`, `0x00DAC987`, and `0x00DAC9BD` independently
load `DAT_013ac840`; each path pushes the loaded value before `GetHero` and invokes the same
`IsDistanceBetweenThingsUnder` helper at `0x00CBE2FF`. The retail data word is `0x40400000`, exactly
3.0f. Thus the pre-follow check, loop-entry test, and post-frame recheck all match Lua's single
`LECTURE_DISTANCE = 3.0`. A focused audit locks all three instruction sequences.

The remaining Guard, Teddy Girl, and Villager movie-cleanup notes were also stale inference labels.
Forge resolves `MovieResource_Destroy_API` directly to retail address `0x006E7B80`, and
`EndMovieSequence` invokes it on the live per-Lua-VM movie handle before erasing ownership. A package-
wide movie-destructor audit now ensures every recorded native movie destructor maps to this exact path
without inference language. All 117 fixtures and 163 recovery tests pass. The game remained closed
and nothing was deployed.

## Created Beetle timer operand and named-enum closure — 2026-09-10

Created Beetle's second `GetTimer` argument was missing only from decompiler output. Retail x86 at
`0x00DB8116-0x00DB8166` shows the `RegisterTimer` result copied from `EAX` into `EDI`, then `push edi`
before the first slot-`0x168` call at `0x00DB8138`. The loop repeats `push edi` at `0x00DB8159`; on
the non-terminating fall-through, the second slot-`0x168` call at `0x00DB815E` consumes it. This proves
both polls use the same registered timer ID without relying on a recovered signature. The new focused
audit locks that register flow, Lua lifetime, and direct-disassembly evidence classification together.

The same stale-evidence sweep resolved Barrel Man's two unnamed numeric operands. The retail-aligned
Forge `EntityScriptingAPI.h` names movement value 1 as `ENTITY_MOVE_RUN` and hero ability `0xE` as
`HERO_ABILITY_HEAL_LIFE_SPELL`; Lua and the entity manifest now record those names, and a header-backed
audit rejects regression to unknown-enum language. Two obsolete Affair conversation comments were
also aligned with their already instruction-proven saved handles. All 117 fixtures and 165 recovery
tests pass. The game remained closed and nothing was deployed.

## Zero-gap source consolidation — 2026-09-10

The shared Lua module still carried unused `UNKNOWN_DISTANCE`, `unsupported_value`, and
`NOVI.unsupported` infrastructure from the early reconstruction phase despite every retail constant
and call site now being resolved. No caller referenced it. The dead scaffold was removed, and a
package-wide audit now rejects any unresolved-call/value placeholder while cross-checking generated
coverage and API requirements.

Two generated “documented deviation” rows were duplicate classifications rather than API needs.
Barrel Man's adjacent bare termination reads are exactly folded into the preceding frame result
because termination changes only across a frame boundary. `CTCVillage::OnInitialActivate` is already
represented by the quest's host-managed lifecycle operation. Removing their redundant `apiGaps`
entries initially left 135 generated rows, all direct or host-managed, with zero blockers. The suite is
now 117/117 fixtures and 166/166 recovery tests. The game remained closed and nothing was deployed.

## AddBadDeed field-write closure — 2026-09-10

The Live Father and Theresa inventories still labeled their inherited
`WhichBadDeedsPerformed[2]` write as inference even though they both call the recovered shared
`AddBadDeed(PARENT, 2)` helper. Retail x86 at `0x00DAEA77` increments
`BadDeedsPerformed` (`this+0x58`), loads the `EBadDeeds` argument into `EDI` at `0x00DAEA98`, and
writes byte 1 to `[ESI+EDI+0xFC]` at both `0x00DAEB4B` and `0x00DAEBE2`. The ordinary path also
reads the same indexed byte at `0x00DAEB67` before deciding whether to display the repeated-deed
message.

A focused audit now checks the actual instruction bytes plus the caller inventories. The inherited
violence-index writes are direct helper effects, not inference. Offline totals are 117/117 fixtures
and 167/167 recovery tests; the then-current 135 generated rows remained direct or host-managed.
The game remained closed and nothing was deployed.

## Affair static world identity narrowing — 2026-09-10

Both canonical ChocolateBox FinalAlbion TNG trees were scanned recursively. Each contains exactly
one `NOVI_AffairMan`, one `NOVI_AffairWoman`, and one `NOVI_AffairWife`, all in
`StartOakValeWest.tng`. The husband is UID `18446741874686306552` at local position
`(24.422607,89.966431,16.433842)`; the Wife is UID `18446741874686307009` at
`(94.591553,117.561218,9.000546)`. Their independently recorded world positions share sector origin
`(3456,736,0)`, and their authored separation is 75.765 m. Husband and lover begin only 1.026 m apart.

This rules out duplicate authored script identity as the air-scolding cause. It does not eliminate a
runtime replacement/handle issue or divergence between the rendered actor and the CScriptThing
position used by the native distance helper, so the uncertainty remains narrowly staged for the
existing route probes. A focused audit preserves the static uniqueness, UIDs, positions, and retained
runtime seam. Offline totals are 117/117 fixtures and 168/168 recovery tests. The game remained closed
and nothing was deployed.

## Installed affair payload and API-count correction — 2026-09-10

The installed-game catalog records `StartOakValeWest.tng` as FinalAlbion WAD entry 405 with size
576,997. Its extracted payload under `work/local_detail_re/tng` has SHA-256
`A444FCC91B17EA72CBAAC5012439420F540D099C68529FB5E717B4D02C3B48EA` and is byte-identical to the
canonical FableDataFolder source. Thus the affair UID/position/uniqueness result applies to the
installed payload represented by the local extraction, not just an editor-side copy.

The API builder also still aggregated `n/a` and `n/a (data)` into two apparent API rows. Those were
state-only and static-data sentinels containing no callable binding. The builder now excludes all
nonbinding sentinels while retaining the explicit host-managed category. The corrected inventory is
133 initial requirements: 132 direct registered bindings and one host-managed lifecycle category;
every non-host row resolves to a Forge registration. A regression test and the zero-gap audit enforce
that distinction. A follow-up registration-scope audit found that `SetIsPushableByHero` exists on
both Quest and Entity Lua surfaces but was reported as Quest-only because the builder chose its first
matching set. The builder now emits `Quest|Entity` for dual registrations, and the zero-gap audit
requires complete scopes for every direct row. Offline totals are 117/117 fixtures and 170/170
recovery tests. The game remained
closed and nothing was deployed.

A final call-site purity pass found one more conceptual alias: 15 Barrel Thug, Book Trader, and Teddy
Girl manifest operations said `SpeakAndWait`, but their `speak_if_alive` Lua helpers actually call
`me:Speak`. Forge registers that method to termination-aware `Speak_Blocking`; the separately
registered `SpeakAndWait` convenience method is never called by this package. Normalizing those
inventories removes the false row without changing behavior. The corrected total is 132 requirements:
131 direct call names plus one host-managed category. The zero-gap audit now rejects a reintroduced
`SpeakAndWait` row unless the package actually adopts that binding.

## Executable Lua/API reconciliation — 2026-09-10

The validator's old receiver whitelist could omit legitimate entity locals even though its binding
check appeared green. Colon-call discovery now accepts arbitrary Lua identifiers, which adds the
previously missed `marker:GetAngleXY` call. The only resulting non-engine match was the parser in
`fields.lua` using Lua's native `value:match`; it is narrowly excluded by observed method name rather
than weakening discovery with another receiver whitelist.

The regenerated API manifest records 145 scoped executable calls. All 145 resolve to actual Forge
registrations, and their operation names cover all 131 direct retail requirements. Thirteen additional
helper/probe operations remain deliberately visible instead of being mistaken for retail inventory
requirements. The zero-gap audit now asserts that every direct requirement occurs on the executable
surface. A follow-up hardening pass made this comparison receiver-scope exact: a Quest-only
observation no longer satisfies an Entity requirement (or vice versa), and dual-scope requirements
must be observed through both surfaces. The call scanner now blanks quoted Lua strings before matching,
preventing documentation-like `receiver:Method(` text from becoming a false executable API call.
Offline totals are 117/117 fixtures and 174/174 recovery tests, with 132 requirements and
zero blockers. The game remained closed, the DLL was unchanged, and nothing was deployed.

The generated API manifest had also placed every non-empty operation `note` under
`semanticDifferences`, causing 429 exactness/evidence annotations to look like compatibility defects.
Schema 0.3 preserves those annotations under `evidenceNotes` and reserves `semanticDifferences` for
actual behavioral deviations. All 131 direct requirements currently have zero known semantic
differences; the zero-gap audit enforces that separation.

## Reproducible native source corpus — 2026-09-10

The inventory still cited historical `tu/...` decompile filenames that were not present anywhere in
the workspace. A new headless Ghidra exporter now opens the analyzed `FableTLC/Fable.exe` project
read-only with `-noanalysis`, requires each requested address to be an exact function entry, and emits
one UTF-8 decompile per address with a provenance header. All 52 inventoried functions now reference
50 present exports; the count difference is the two deliberately shared native entries. The new
audit verifies locator normalization, file presence, embedded entry address/generator identity, and
absence of unreferenced exports. Offline totals are 117/117 fixtures and 176/176 recovery tests. The
game remained closed and nothing was deployed.

The repeatable entry point is
`powershell -File tools/script_recovery/export_new_oakvale_native_sources.ps1`. It derives the unique
address set from all entity JSON inventories, invokes Ghidra read-only with `-noanalysis`, and runs
the source audit after export.

A coverage-classification review found that `NOVI_Villager.QuestInit(vectors)` was reported as
empty/data-only solely because the builder treated any zero-operation function containing `vectors`
that way. The function is actually a second inventory view of the non-empty quest Init at
`0x00DAADD0`, whose vector setup is reconstructed and traced. It now carries an explicit
`Q_NewOakValeIntro.Init` coverage alias; the source audit proves both names resolve to the same entry,
and the zero-gap audit fixes the exact expected classification of all six not-applicable rows.

The fixture builder previously accepted secondary function names in `covers` without any
function-specific trace evidence. The five transitive fixtures now attach `coverEvidence` to the two
`GivenTeddy` helpers, `GetVillagerSpeechIndex`, `AttackStuff`, and `PostAttackStuff`; every listed event
must also be a validator-enforced expectation. Missing or unverified evidence no longer marks a helper
traced. The strengthened result remains 46/46 applicable functions, 117/117 fixtures, and 177/177
recovery tests.

An operation-ownership audit then exposed a real coverage blind spot: Barrel Man uses nine labels of
the form `Main/<lua-helper>`, but the builder selected operations only when the label exactly equaled
`Main`. Forty-eight helper-tagged operations therefore appeared in API requirements while being
omitted from the native Main coverage calculation. Native-owner normalization now maps all nine
helpers back to Main, which accounts for 68 operations. A structural audit requires all 747 operation
rows to have contiguous sequence IDs and declared native owners, and the coverage summary independently
requires all 747 to be assigned exactly once. Offline totals are 117/117 fixtures and 179/179 tests.

Executable parent-state extraction found five inventory omissions: `TalkIntermittentTimer` reads in
Affair Wife, Book Trader, and Bully; `WatchTimer` in Barrel Thug; and `BadDeedsPerformed` in Barrel Man.
The declarations now match the Lua. A new audit scans comment-free `F.get`, `F.get_int`, `F.set`, and
`F.increment` calls; validates all direct and annotated names against 37 canonical state fields plus
the eight static Villager speech-vector members; and preserves indirect deed-helper declarations.
It currently covers 30 read and 28 write families. Offline totals are 117/117 fixtures and 180/180
recovery tests.
Quest and shared Lua are included in the undefined-key pass as well: the complete package directly
reads 34 and writes 32 canonical field families.

The separate Sunnyvale master-data lane exposed stale generated prose: the persistence manifest
claimed `TeddySolution` was written by Teddy Girl and Bully, while Bully has no master-data access.
Both native operations and both Lua calls are Teddy Girl paths (`B` in `GivenTeddy`, `C` in the ruined
branch). Persistence schema 0.2 now generates structured access rows from inventories instead of a
hardcoded fallback. The audit cross-checks the sole `F.master.TeddySolution` definition and executable write
against those two evidence records; there are no master-data reads in this quest.

Persistence owner aggregation also treated annotated strings literally. Declarations such as
`BadDeedsPerformed/WhichBadDeedsPerformed[2] (via AddBadDeed)` therefore failed to attach their six
entity writers to either canonical field. Normalization now expands slash groups and strips array/
helper annotations. This revealed that the scalar field parser omitted `WhichBadDeedsPerformed`
entirely; persistence schema 0.2 now records it as one `bool[5]` family spanning `0xFC..0x100`, with
Guard reading and all six deed paths writing it. The manifest contains all 37 canonical field
families. Offline totals are 117/117 fixtures and 182/182 recovery tests.

Scanning executable deed-helper calls found six more inventories that omitted indirect counter/array
writes: Affair Man, Affair Wife, Affair Woman, Barrel Man, Bully, and Victim. The audit now derives
required side effects from every `Deeds.add_good`/`Deeds.add_bad` call. The corrected persistence
owners are four writers for `GoodDeedsPerformed` and twelve each for `BadDeedsPerformed` and
`WhichBadDeedsPerformed`; helper annotations can no longer hide or invent those relationships.

Fresh `0x00DAADD0` decompilation exposes all 42 Villager reaction literals, enabling a three-way audit
against the structured inventory and evaluated `villager_speech.lua`. All eight vectors match exactly
in membership and order at offsets `0x9C..0xF0`; their 6/6/4/5 per-sex lengths and the retail vector
construction order are guarded too. Offline totals are 117/117 fixtures and 183/183 recovery tests.

A package-wide symbolic-resource audit now flattens the native inventories and scans comment-free Lua
strings across text, object, creature, quest, cutscene, animation, marker, region, theme, music, and
script categories. All 238 executable identifiers are covered: 224 exact literals and 14 deliberate
prefixes used for indexed or sex-suffixed construction. Eighteen probe/authority formats are classified
as diagnostics rather than resources. Current totals are 117/117 fixtures and 184/184 tests.

Entity-local state has a separate source-to-reconstruction guard as well. The audit checks every one
of the 32 inventory `localFields` names against comment-free entity Lua and requires its native
`initValue` to appear in an assignment. Fourteen assignments currently resolve through named constants
or enum members, including Barrel Man's `PHASE.AT_WAREHOUSE`, so symbolic defaults receive the same
fidelity check as literal ones. Current totals are 117/117 fixtures and 185/185 tests.

Constant fidelity is now mechanically guarded across every native inventory as well. Of 214 constant
records, 200 map to same-name Lua declarations and compare equal after parsing scalars, keyed tables,
and tier arrays. The remaining 14 are not ignored: explicit value plus executable-context checks cover
split fade constants, shared fields/modules, native symbolic zero vectors, animation operands, timer
conditions, and differently named enum constants. Current totals are 117/117 fixtures and 186/186
tests.

The native inventories' cleanup-path layer is now audited rather than treated as free-form prose.
Seventeen inventories contain 73 resource-lifetime records and 149 acquisition sequence references;
all references resolve to real operations in the owning inventory. Owner-only/nonlocal acquisitions,
release and missing-path arrays, and the few intentionally empty lifecycle records must also carry an
explicit ownership classification. Current totals are 117/117 fixtures and 187/187 tests.

Phase metadata now has an executable anchor audit. It covers all 74 phase records in 16 inventories,
requires unique IDs and populated entry/exit descriptions, and resolves their 100 named anchors to
actual Lua function declarations. Barrel Man's six numeric phases also match both the inventory enum
metadata and reconstructed `PHASE` table. Current totals are 117/117 fixtures and 188/188 tests.

The cutscene/thread metadata pass covers 18 cutscene records, nine named cutscene resources, 13 actor
keys, and all four spawned quest functions. It found that `spawnedThreads[].lua` still claimed the old
lowercase, one-argument `quest:CreateThread` form. Those rows now exactly match the reconstruction's
`Quest:CreateThread(name, { region = "" })` calls, and the audit verifies both spawning and target
functions exist. Current totals are 117/117 fixtures and 189/189 tests.

A direct PE vtable scan now proves class ownership for every New Oakvale entity inventory. Each of the
16 entity `[Main, Init]` address pairs occurs exactly once in the SHA-256-pinned retail executable;
reading the preceding slot recovers its vtable address and destructor. Ten formerly missing `vtable`
properties were added. The checked-in snapshot and reproducible extractor guard all 32 Main/Init
slots against inventories, translation-unit allocator evidence, and native decompile files. Four notes
that cited a nonexistent `entity_vtables.json` now point to the actual snapshot. Current totals are
117/117 fixtures and 190/190 tests.

Vtable identity also makes the allocator/object layout mechanically recoverable. The translation unit
contains one allocator for each of the 16 entity classes, allocating 28-44 bytes and installing the
expected vtable. A layout snapshot and audit prove that all 31 true class-local fields fit their retail
offsets and inferred widths; Theresa's explicitly documented Main stack local is not misclassified as
object storage. The layouts have only 11 aggregate tail-padding/slack bytes. Current totals are
117/117 fixtures and 191/191 tests.

Fresh read-only Ghidra export of the 16 recovered entity destructor slots shows one byte-equivalent
decompiled cleanup body after provenance headers: shared entity-base destruction, conditional
`operator_delete(this)`, and return. This caught and corrected an initial overreach: although the quest
object's `Main` and `Init` pointers are adjacent, the preceding entry at `0x00DAACE0` decompiles as a
spawned-function factory, not a destructor. The quest vtable claim was removed; vtable/destructor
evidence is explicitly scoped to the 16 entity classes. Current totals are 117/117 fixtures and
193/193 tests.

The 16 entity allocators now have separate read-only Ghidra exports driven by the layout snapshot.
Their audit validates the recovered object size, embedded `CScriptThing` construction, class vtable
store, parent/bound-thing fields, and the `0xC` counted ownership block plus delete callback. This
independently corroborates the translation-unit layout evidence at function granularity. Current
totals are 117/117 fixtures and 195/195 tests.

## Shared entity cleanup classification — 2026-09-10

Machine-level call-target recovery shows that all 16 entity deleting destructors invoke the same
routine at `0x00F35B40`. Its standalone read-only decompile installs base vtable `0x012C3224`, restores
the embedded `CScriptThing` vtable `0x01238C8C`, decrements/releases the counted binding at `+0x10`,
clears `+0x0C/+0x10`, and tails to the exact relocation-matched `CBase_RestoreVTable` function at
`0x0099A2E0`.

Ghidra labels this routine `NUISystem::CFrontEndScreen::~CFrontEndScreen` from a BSim donor. That class
name is structurally incompatible with the allocator and vtable evidence and is now explicitly stored
only as donor provenance. The durable classification is “entity-binding base cleanup; exact class name
unresolved.” The vtable extractor derives the shared call target from retail relative-call bytes, the
destructor exporter refreshes the support routine from the snapshot, and the audit guards both vtable
stores, ownership cleanup, and the tail target's manifest evidence. Offline totals remain 117/117
fixtures and 195/195 recovery tests. The game remained closed and nothing was deployed.

## Affair Wife position-chain closure — 2026-09-10

The sole remaining explicit reconstruction uncertainty, the observed mid-route air-scolding, was
traced one layer deeper without launching the game. Fresh read-only exports prove that retail
`CScriptThing::GetPos` (`0x004AA980`) null-falls back to the shared zero vector and otherwise dispatches
implementation slot `+0x18`; concrete `CGameScriptThing::GetPos` (`0x008CFE20`) returns its inline
position at `+0x28`. Retail `IsDistanceBetweenThingsUnder` (`0x00CBE2FF`) validates both wrappers via
slot `+0x12C`, obtains both positions through wrapper slot `+0x18`, and compares squared 3D distance.

Forge's registered fastcall points to that exact predicate, normalizes both raw and shared Lua object
forms before calling it, and its `Entity:GetPos` follows the same implementation-object dispatch. A
manifest-driven exporter and seven-part Forge/native audit now guard this chain. This excludes Lua-side
distance mathematics and calling convention as causes; only runtime replacement/handle state or a
rendered-model-versus-script-position divergence remains for a future live observation. Offline totals
are 117/117 fixtures and 197/197 recovery tests. The game remained closed and nothing was deployed.

## Quest construction-chain proof — 2026-09-10

The earlier entity-vtable pass intentionally excluded the quest object, but the native cluster already
contained a separate, structurally valid quest ownership chain. Fresh exports now prove allocator
`0x00DBEF70` allocates `0x10C` bytes and calls `CQ_NewOakValeIntroScript` constructor `0x00DAAC00`.
The constructor installs vtable `0x012D7A28`, stores the game interface and script database, clears the
state/vector range beginning at `+0x9C`, and creates timers at `+0x104/+0x108`.

That vtable's five recorded slots are destructor, `RegisterMain`, `Main`, `Init`, and `OnPersist`.
The shared base constructor at `0x00CB8110` is included as an eighth export; its BSim donor name is not
treated as quest identity. `RegisterMain` independently allocates a `0x3C` spawned function named `Main`, sets its callback and
parent pointers, and registers it. A cluster-driven read-only exporter plus audit guards all seven
unique allocator/constructor/lifecycle functions plus that base constructor and rejects unrelated adjacent exports. The quest
inventory now records its vtable, allocator, constructor, and 268-byte size. Offline totals are
117/117 fixtures and 199/199 recovery tests. The game remained closed and nothing was deployed.

## Quest object-layout reconciliation — 2026-09-10

The 268-byte allocation now has a complete contiguous region map rather than only endpoint evidence.
Offsets `0x00..0x47` are the inherited script base; `0x48..0x9B` contain named scalar/string state;
`0x9C..0xFB` are eight 12-byte villager-speech vectors; `0xFC..0x100` is the five-byte bad-deed-kind
array; `0x101..0x103` is alignment padding; and `0x104..0x10B` holds the two timer handles. All 37
persistence-manifest field families occupy 93 bytes at their declared native widths with no overlap or
out-of-bounds extent. Constructor exports guard both inherited and derived region anchors. Offline
totals are 117/117 fixtures and 200/200 recovery tests. The game remained closed and nothing was
deployed.

## Quest initialization provenance — 2026-09-10

Field defaults are now separated mechanically from producer-owned state. Retail `Init` directly
stores 27 of the 37 field families: 25 scalar Lua resets, `TalkIntermittentTimer` through the timer
API, and `WhichBadDeedsPerformed` as one dword plus one byte. The remaining ten are populated by a
host/API or an event before their meaningful consumer, or are unused.

This exposed three metadata-only false defaults. `StopTimeIndex` is filled by `SetTimeAsStopped` before
the release call; `DadOfferedRewards` has no recovered New Oakvale access; and
`lastVillagerSpeechIdx` is not initialized by retail `Init`, then receives the first accepted random
index. Their Lua keys were already absent from both reset lists, so changing their declared defaults
to nil does not change execution. The persistence descriptions for all ten producer-owned fields now
state their actual lifecycle. Offline totals are 117/117 fixtures and 201/201 recovery tests. The game
remained closed and nothing was deployed.

## Quest teardown/layout closure — 2026-09-10

Fresh read-only exports close the ownership chain in the opposite direction. Deleting thunk
`0x00DBEFA0` delegates to `0x00DBEFC0`, which deregisters timer handles `+0x108/+0x104`, then walks,
destroys, and frees all eight 12-byte speech-vector triplets in reverse construction order from
`+0xF0` through `+0x9C`. Its final direct call is `0x00CBD510`, whose body destroys the inherited
script storage at `+0x30`, `+0x18`, `+0x08`, and `+0x04` before restoring the root base vtable.

The existing `CParticleEmitter_Dtor_7` and `DeleteAllParticles` names are donor/autoname residue and
are preserved only as provenance; their object offsets and call graph establish quest-derived and
script-base cleanup semantics. The cluster-driven exporter now includes both implementations, and a
new teardown snapshot/audit checks exact timer resolution, reverse coverage of the complete vector
layout region, eight frees, and the terminal base-cleanup target. Offline totals remain 117/117
fixtures and rise to 202/202 recovery tests. The game remained closed and nothing was deployed.

The base cleanup's class identity is no longer left at “CScriptBase-compatible.” FSE's independent
address source names `0x00CB8110` as `CScriptBase_Construct`; that constructor initializes the list
and three inherited vectors at `+0x04`, `+0x08`, `+0x18`, and `+0x30` that `0x00CBD510` releases.
Helper IR adds population evidence: all 125 consumers of `0x00CBD510` are script destructor roles.
The teardown snapshot, quest inventory, cluster, and audit now record the exact `CScriptBase`
classification while retaining the misleading Ghidra label only as provenance.

## Villager-speech vector lifecycle audit — 2026-09-10

The quest's largest derived data region is now checked end to end. A new audit parses the 42 exact
`TEXT_QST_048_VILLAGER_DONE_*` literals from retail `Init` `0x00DAADD0`, derives each vector base
from its native end-pointer access, and compares the sequences with both the named Villager inventory
and the generated Lua prefix/suffix tables. It also verifies the unusual retail fill order—female bad,
male bad, female both, male both, female good, male good, female neither, male neither.

The same audit proves the eight 12-byte vectors exactly cover `+0x9C..+0xFB` without gaps and that the
recovered destructor releases them in reverse layout order. All 42 keys are unique. Offline totals
remain 117/117 fixtures and rise to 203/203 recovery tests. The game remained closed and nothing was
deployed.

## Retail-byte quest vtable extraction — 2026-09-10

The quest lifecycle chain no longer depends circularly on addresses already copied into its native
cluster. A new PE exporter reads vtable `0x012D7A28` directly from the installed retail executable,
hashes the exact 20-byte table, and records its five pointers: deleting destructor `0x00DBEFA0`,
RegisterMain `0x00DAACE0`, Main `0x00DABAC0`, Init `0x00DAADD0`, and OnPersist `0x00DAADA0`.
It also decodes the thunk's relative call independently as destructor implementation `0x00DBEFC0`.

The construction audit now requires the retail-byte snapshot to equal the cluster lifecycle and to
share the complete executable SHA-256 with the existing entity-vtable snapshot. A test regenerates the
snapshot from the installed retail image when available. Offline totals remain 117/117 fixtures and
rise to 204/204 recovery tests. The game remained closed and nothing was deployed.

## Retail-byte quest ownership extraction — 2026-09-10

The constructor half of the ownership chain now has the same machine-level independence as the
vtable. A Capstone-backed PE exporter decodes allocator `0x00DBEF70`: its allocation immediate is
`0x10C`, its first direct call is retail `operator_new` at `0x00BFEA1A`, and its second direct call is
quest constructor `0x00DAAC00`. Decoding the constructor proves its first call reaches
`CScriptBase_Construct` at `0x00CB8110` and its root-object immediate store installs vtable
`0x012D7A28`.

The snapshot stores exact hashes for the allocator and constructor byte ranges plus the complete
retail executable hash. The construction audit reconciles all decoded facts with the cluster,
inventory, layout, and vtable snapshot; another installed-image test regenerates it byte for byte.
Offline totals remain 117/117 fixtures and rise to 205/205 recovery tests. The game remained closed
and nothing was deployed.

## Two-timer lifecycle closure — 2026-09-10

The warehouse timer's earlier truthy-zero failure is now covered across every layer. Retail constructor
`0x00DAAC00` invokes game-interface slot `0x15C` twice, storing the first handle at
TalkIntermittentTimer `+0x104` and the second at WatchTimer `+0x108`. Derived destructor `0x00DBEFC0`
deregisters them through slot `0x160` in reverse order. The retail vtable catalog proves the adjacent
four-slot sequence `RegisterTimer`, `DeregisterTimer`, `SetTimer`, `GetTimer` at `0x15C..0x168`.

Forge maps those exact operations to indices 87..90 with matching signatures and exposes all four to
Lua. The new audit requires reconstructed Init to register both handles unconditionally—never behind a
Lua truthiness check—then zero TalkIntermittentTimer only, while both field definitions retain nil
initial sentinels. It also corrected a stale comment that incorrectly called this the WatchTimer
SetTimer operation. Offline totals remain 117/117 fixtures and rise to 206/206 recovery tests. The
game remained closed and nothing was deployed.

## Direct retail-byte timer and Init proof (2026-09-10)

- Added `export_new_oakvale_timer_lifecycle.py` and a checked-in retail snapshot. It decodes the two
  slot-`0x15C` calls and `eax` stores to `+0x104/+0x108` in constructor `0x00DAAC00`, plus the two
  slot-`0x160` calls fed from `+0x108/+0x104` in destructor `0x00DBEFC0`.
- Added `export_new_oakvale_init_writes.py` and a checked-in retail snapshot. It decodes all 27 scalar
  stores in `Init` `0x00DAADD0`, retains their exact byte/dword access widths, and identifies the
  slot-`0x164` zero reset of TalkIntermittentTimer at `+0x104`.
- Both existing cross-layer audits now require this direct-byte evidence, and reproduction tests compare
  the snapshots against the installed retail executable when present.
- No executable was launched and no deployment was performed.

## Direct retail-byte OnPersist proof (2026-09-10)

- A new PE exporter hashes and decodes all 33 bytes of quest `OnPersist` at `0x00DAADA0`.
- The function contains exactly one transfer: string `AttackOver` at `0x012D7A58`, field `+0x50`,
  one-byte value and false default, targeting `0x004045C0`.
- A cross-layer audit verifies the sole-field persistence manifest, Lua `PersistTransferBool` call,
  Forge's exact retail target/signature/wrapper, and Lua registration.
- The quest inventory now records direct retail-byte evidence for this lifecycle operation.
- No game launch or deployment was performed.

## Direct retail-byte RegisterMain proof (2026-09-10)

- Decoded and hashed all 139 bytes of `RegisterMain` at `0x00DAACE0`.
- It allocates `0x3C` bytes, names the spawned function `Main`, calls constructor `0x00CDD450`, installs
  vtable `0x012D7A3C`, callback `0x00CDD440`, owner `+0x38`, and registers an empty section through
  `0x00CB7E50`.
- Forge's generic Lua spawned-function vtable has the same scheduler slot and intentionally supplies its
  own deleting destructor. The constructor, registration target, name, and object ABI match retail.
- Added Forge compile-time assertions for size `0x3C` and offsets `+0x34/+0x38`; Release x86 rebuilt
  successfully with zero warnings and errors.
- No game launch or deployment was performed.

## Direct retail-byte Main entity-binding proof (2026-09-10)

- Decoded and hashed the 1,670-byte prefix of quest `Main` that constructs all 16 entity bindings.
- Each retail string pointer is paired with its immediate allocator callback; all 16 match the entity
  layout snapshot and the reconstructed Lua table in exact order.
- Retail allocates 16 objects of `0x1C` bytes and submits all 16 through `0x00CB8230`.
- Added five Forge compile-time assertions guarding binding size and fields `+0x08/+0x10/+0x14/+0x18`.
  Release x86 rebuilt successfully with zero warnings and errors.
- No game launch or deployment was performed.

## Direct retail-byte Main tail proof (2026-09-10)

- Decoded and hashed the remaining 348 bytes of quest `Main`, closing direct-byte coverage of the full
  method when combined with the binding prefix.
- Verified base/interface binding finalization, one-byte `AttackOver` reload logic, termination guard,
  post-attack section deactivation, objective 01, `StartBarrelTimer` construction, and `DoMission` call.
- Lua preserves the retail operation order and Forge maps the four indirect interface slots exactly.
- Removed a stale inventory note that said Forge lacked `IsActiveThreadTerminating`; the binding is now
  present and already used by the reconstruction.
- No game launch or deployment was performed.

## Direct retail-byte DoMission phase proof (2026-09-10)

- Added a retail PE exporter and exact hash for all 1,172 bytes of `DoMission` at `0x00DBDE40`.
- The snapshot records seven phase strings, three watcher callbacks, three byte-width `AttackOver`
  reads, six termination-helper calls, 30 ordered indirect calls across 20 unique interface slots, and
  the final `AttackStuff -> PostAttackStuff` call order.
- The audit requires every slot to be executable in the authoritative interface catalog and reconciles
  the region wait, childhood branch, watcher creation, attack wait, and completion topology with Lua.
- No game launch or deployment was performed.

## Direct retail-byte AttackStuff proof (2026-09-10)

- Decoded and hashed all 285 bytes of the childhood-to-raid transition at `0x00DBE3C0`.
- Recovered the exact post/pre section names, raided theme, objective 06 key, float `23.0`, and both
  zero-delay operands from the installed retail image.
- The six ordered interface slots and five Lua transition operations agree exactly.
- No game launch or deployment was performed.

## Direct retail-byte PostAttackStuff proof (2026-09-10)

- Decoded and hashed the entire 1,095-byte post-raid transition at `0x00DBEB20`; its 324 instructions
  end exactly at `0x00DBEF67`, immediately before the quest allocator alignment gap.
- The checked-in snapshot records nine ordered resource strings, all 34 indirect call sites, key helper
  call sites, music sets 45/57, distance 5.0, hero scripting priority 4, logbook entry 20, and the exact
  one-byte `DadFound=1` store at quest offset `+0x51`.
- The Lua audit covers 20 ordered operations from marker wait through post-cutscene cleanup. Eighteen
  distinct interface slots are proven executable by the catalog; omitted catalog slot `0x5EC` is
  proven separately by Forge's index-379 `PauseAllNonScriptedEntities` binding.
- The quest inventory now classifies `PostAttackStuff` as retail-byte-audited.
- No game launch or deployment was performed.

## Direct retail-byte StartBarrelTimer proof (2026-09-10)

- Decoded and hashed all 562 bytes / 163 instructions of the warehouse timer thread at `0x00DB4F70`.
- The snapshot pins all timer, quest-field, marker, HUD-bar, proximity, color, update, termination, and
  normal-cleanup operands, including the corrected `(45.0, 0.0)` bar arguments.
- Forge's HUD-bar typedefs and vtable slots `0x510`, `0x530`, `0x534`, and `0x548` match retail; the Lua
  reconstruction preserves all 11 operations in order.
- Upgraded the quest inventory's four StartBarrelTimer operations from decompiler-only evidence to
  direct retail-byte evidence.
- No game launch or deployment was performed.

## Direct retail-byte WatchBarrels proof (2026-09-10)

- Decoded and hashed the full 647-byte / 208-instruction barrel consequence thread at `0x00DBE890`.
- The snapshot proves exact quest offsets, strings, counter thresholds, interface calls, four
  termination checks, and the direct first-break call `AddBadDeed(0)` at `0x00DAEA70`.
- The first break has no separate barrel-man signal: his later reprimand depends on
  `BadDeedsPerformed`, which `AddBadDeed` updates. This narrows the observed missing-scold runtime seam
  to shared state propagation or the man's later state machine, not WatchBarrels branching.
- Lua preserves all 15 operations in order. Forge provides exact fallback bindings for omitted catalog
  slots `0x428` (`EntitySetMaxHealth`) and `0x924` (`AddItemToContainer`).
- No game launch or deployment was performed.

## Direct retail-byte BarrelMan judgment split (2026-09-10)

- Added reproducible hashes and operand snapshots for automatic judgment `0x00DB5A48-0x00DB5DED`
  (934 bytes) and follow-up dialogue `0x00DB65BF-0x00DB684B` (653 bytes).
- Automatic judgment sets the returned flag, then uses visibility OR 10.0m distance. A present hero is
  thanked and awarded a good deed; an absent hero is marked HeroLetMeDown and receives bad deed 1.
- The automatic region contains no BarrelBrokenPersistent read. Broken-barrel scolding exists only in
  the later talked-to handler, nested under HeroLetMeDown. A hero who stayed receives NO_TIME instead.
- Lua and the entity inventory preserve this exact retail separation. This explains why breaking the
  barrels alone does not make the man automatically scold on return; changing it would be non-retail.
- No game launch or deployment was performed.

## Direct retail-byte WatchForGotGold proof (2026-09-10)

- Decoded and hashed all 212 meaningful bytes / 65 instructions at `0x00DBE2E0`; the remaining 12
  bytes before `AttackStuff` are alignment traps.
- Retail performs two `GetHeroGold` calls, compares both results strictly greater than 2, and checks
  termination both inside and after the wait.
- On success it obtains the active quest name and sets objective 03 with two exact empty strings.
- Lua matches, and Forge's index-127 binding independently proves catalog-omitted slot `0x1FC` is
  `GetHeroGold` with the correct signature.
- No game launch or deployment was performed.

## Direct retail-byte ManageQuestCoreMarkers proof (2026-09-10)

- Decoded and hashed the full 944-byte / 287-instruction watcher at `0x00DBE4E0`.
- The snapshot captures three actor names, four orb strings, 25 indirect calls, 11 termination checks,
  exact field accesses, gold threshold 3, tutorial 19, and the seven-step marker mutation sequence.
- Lua preserves all 16 audited operations in order, including conditional tutorial dismissal and both
  quest-state gates.
- The four corresponding inventory operations are upgraded from decompiler-only evidence to direct
  retail-byte evidence. This closes all four quest-level spawned watchers.
- No game launch or deployment was performed.

## Direct retail-byte Guard repeat-lecture proof (2026-09-10)

- Added reproducible snapshots for claim/recheck `0x00DAC7A3-0x00DACA21` (639 bytes) and complete
  lecture selection/bodies `0x00DACB33-0x00DAD867` (3,381 bytes), totaling 4,020 retail bytes.
- Retail tests `BadDeedsPerformed - GuardsDealtWithBadDeeds > 0` both at loop entry and after pursuit,
  then atomically claims all current deeds with `GuardsDealtWithBadDeeds = BadDeedsPerformed`.
- Parent byte `GuardsSpokenOnce +0x92` branches to `CAUGHT_YOU_AGAIN`; the first branch speaks lines
  10/20, the crime list, lines 30/40/50/60, then writes `+0x92 = 1`. The repeat branch speaks AGAIN,
  the crime list, and AFTER_READ_LIST.
- Lua already matches the retail state transition and speech order. Existing runtime evidence records
  claims at bad/dealt 1 and 2 and both first/repeat speech keys, so no behavior change was warranted.
- Recovery suite is 234/234. No game launch or deployment was performed.

## Direct retail-byte Affair Wife route proof (2026-09-10)

- Decoded and hashed `0x00DB3426-0x00DB35C0`, all 411 bytes / 118 instructions from husband lookup
  through route completion and movement-in-actions disable.
- Retail snapshots `NOVI_AffairMan:GetPos()` once for `MoveToPosition(position,2.0,run,false,true)`;
  a null husband uses global zero vector `0x0143E8E0`.
- Arrival does not compare against that snapshot: tests at `0x00DB34A2` and `0x00DB3586` both pass
  the same live husband wrapper local to exact helper `0x00CBE2FF` with distance 3.0. False loops to
  `0x00DB34B0`; true reaches `0x00DB3593`, then ClearCommands and movement disable.
- The running line's 10.0 home-distance gate and local `SaidRunningLine +0x1E = 1` store are pinned.
- Lua already matches. No real-game ROUTE probe lines were archived, so the observed air-scolding is
  not yet attributable beyond the previously narrowed runtime identity/position seam.
- Recovery suite is 236/236. No game launch or deployment was performed.

## Direct retail-byte Affair Wife argument proof (2026-09-10)

- Decoded and hashed all 1,770 bytes / 536 instructions at `0x00DB35C1-0x00DB3CAA`, from argument
  entry through the post-frame loop edge.
- The exact husband wrapper local feeds facing slot `0x76C` during the pointing animation, after the
  wife's hit response, and after hero-initiated talk; the corresponding force flags are false/true/true.
- The same husband wrapper is added through conversation slot `0x5B4`, receives the numbered wife
  line, and speaks the optional `TEXT_QST_048_AFFAIRMAN_IN_TROUBLE` response. Speaker/listener operand
  order at both `0x5B8` calls matches Lua.
- Retail strings also pin both pointing animations, ON_HIT, THANKYOU_SINGLE, and numbered-line fallback.
- Route plus argument bytes exclude a Lua recipient substitution as the cause of air-scolding. A
  real-game ROUTE identity/position capture remains necessary to distinguish handle/model divergence.
- Recovery suite is 238/238. No game launch or deployment was performed.

## Retail quest-info-bar render-chain proof (2026-09-10)

- Exported the concrete retail bodies `CDrawQuestInfo::AddBar` `0x0064F7FC`, `ChangeBarColour`
  `0x00644DF8`, `CQuestInfoElementBar` constructor `0x00656B1B`, and update `0x006463FF` read-only.
- AddBar resolves texture/text symbols independently before constructing a `0x58`-byte bar element.
  The constructor copies primary/secondary colors to `+0x28/+0x2C` and sprite ids to `+0x30/+0x34`.
- ChangeBarColour type-checks the element then overwrites only `+0x28/+0x2C`; it does not tint or
  replace the `HUD_CLOCK_ICON` sprite.
- Forge's parser emits retail BGRA and passes both color pointers unchanged. Since the timer's green
  bytes are `00 FF 00 FF`, R/B ordering cannot turn them blue. A blue clock graphic and green/red bar
  fill are separate properties; changing the icon would diverge from retail.
- Recovery suite is 240/240. No game launch or deployment was performed.

## Direct retail-byte Bully health-bar proof (2026-09-10)

- Added exact snapshots for create/update `0x00DBC391-0x00DBC587` (503 bytes / 134 instructions) and
  subdued removal `0x00DBC86B-0x00DBC890` (38 bytes / 11 instructions).
- Retail compares the handle against `-999`, constructs primary `00 FF 00 FF` (opaque green) and
  secondary `FF 00 00 FF` (opaque blue), calls slot `0x510`, and stores its result at parent `+0x64`.
- Each non-final hit stores `HitsTaken+1` and calls slot `0x530` with current
  `InitialHealth-HitsTaken`, maximum `-1.0`, and scale `-1.0`.
- The final hit branches to `0x00DBC86B`, writes `BullySubdued=1` at parent `+0x6C`, reloads the same
  handle from `+0x64`, and removes it through slot `0x548`.
- Lua matches, providing an asymmetric cross-consumer proof that Forge preserves both color slots.
- Recovery suite is 242/242. No game launch or deployment was performed.

## Direct retail-byte Bully run-off proof (2026-09-10)

- Decoded and hashed the continuous `0x00DBC86B-0x00DBCCF4` transition, all 1,161 bytes / 339
  instructions from `BullySubdued` through the final resource destructor.
- Retail acquires Bully, Hero, then victim at priority 4 and constructs exact actor roles
  `HERO=hero`, `BRAT=victim`, and `BULLY=bully`.
- `$BRATLINE` selects `TEXT_QST_048_VICTIM_THANKS_AFTER_HIT` when `HeroAttackedVictim +0x6F` is true,
  otherwise `TEXT_QST_048_VICTIM_THANKS`; only `BULLYRUN1` receives this input map.
- `GivenHeroTeddy +0x6E` selects `BULLYRUN2` (then clears victim information and writes the field) or
  `BULLYRUNDUMMY`. Both follow-up cutscenes retain the actor map but receive null input/flag maps.
- Cleanup order is camera off, pause off, movie/input/actor destruction, victim release, Hero release,
  `BullyRanOff +0x6D`, good deed, `RemoveThing(bully,false,true)`, then Bully release.
- Lua and inventory agree with every audited operand. Offline totals are 117/117 fixtures and 244/244
  recovery tests; tooling SDK validation passes with mirrors intentionally skipped because the two
  noncanonical mirror repositories remain stale. No game launch or deployment was performed.

## Interactive red-timer / barrel / bully capture and offline follow-up (2026-09-10)

- Archived the completed-childhood run as
  `refs/script_recovery/new_oakvale_intro/runtime_evidence/interactive-20260910-redtimer-bullyrun-barrels.log`
  (SHA-256 `39519E33CF88022185036D6798D029D7B45E8FBCD2763CE2F67592F8E1CB085E`).
- Timer red is runtime-confirmed. Barrel Man logged `bad=1`, Hero distance 5.092 m, `nearby=true`,
  then retail `BARRELMAN_THANKS`; Guard separately claimed the destruction deed and spoke
  `GUARD_CRIME_BARREL_BREAKING`. This is the executable's two-axis outcome, not lost morality state.
- `BULLYRUN1` received correct HERO/BRAT/BULLY resources and `$BRATLINE`. Bully moved from
  `(3606.752,861.396,13.679)` to `(3604.479,860.742,13.707)`, then the shipped cutscene's explicit
  `Drawable FALSE` made him disappear. There was no actor-map or Lua removal error.
- Forge now has undeployed `MoveToPositionDiag` instrumentation for thing/resource identity,
  destination, radius, move enum, path flags, and missing handles. Release|x86 builds cleanly.
- Installed TNG places `MK_OIBR_BULLY2` at local `(12.740,83.895,13.375)`, about 14.6 m from the
  captured pre-cutscene Bully, proving the 2.36 m movement was incomplete. Lua now conditionally
  completes that exact marker move when the native macro stops outside 2 m, with a 300-frame safety
  cap, then restores the payload's hidden state. No fallback runs when native `RunTo` succeeds.
- Added exact Bully GivenTeddy and full teddy-dispatch snapshots/audits. Recovery tests are 248/248.
  Package validation is 117/117 with zero warnings, tooling SDK validation passes, and the diagnostic
  DLL SHA-256 is `C41F54F9B0D43700CEE096662F2A01752B9CB3DDA216070F22E60DCB2B4C2D7E`.
  Fable was closed before offline work resumed and was not relaunched or redeployed.

## Complete retail-byte Affair Man proof (2026-09-10)

- Added a reproducible installed-PE snapshot for complete Init `0x00DB0950-0x00DB09E0`
  (144 bytes / 57 instructions) and complete Main `0x00DB09E0-0x00DB1DA2`
  (5,058 bytes / 1,446 instructions). Both boundaries decode exactly through a terminal `ret`.
- The continuous Main range covers the single live `NOVI_AffairWoman` and `NOVI_AffairWife`
  lookups, wife confrontation, hush-money branch, walk-home loop, near/ranged dialogue, kiss/hug
  actions, and `0x00DB1D7C-0x00DB1DA1` cleanup.
- This excludes an Affair Man Lua dispatcher omission as the cause of Wife scolding empty space.
  Runtime ROUTE_START/ROUTE_REACHED identity and position evidence is still required to distinguish
  a live thing/rendered-model divergence; no speculative route behavior was added.
- Recovery tests are 250/250. Package validation is 117/117 with zero warnings; tooling SDK
  validation passes. The game remained closed and nothing was deployed.

## Complete retail-byte Affair Woman proof (2026-09-10)

- Added reproducible snapshots for complete Init `0x00DB1E80-0x00DB1EF4`
  (116 bytes / 42 instructions) and Main `0x00DB1F00-0x00DB299A`
  (2,714 bytes / 768 instructions), both ending at exact `ret` boundaries.
- This continuous evidence covers the live Affair Man/Wife lookups, hit and talk movies, home return,
  kiss/hug reception, Wife-proximity flight to `AffairWomanRunOffPoint`, removal, and terminal cleanup.
- Affair Woman never removes or relocates Affair Man. Together with the complete Affair Man artifact,
  the Wife symptom remains isolated to runtime Wife/man thing identity or rendered-model position.
- Recovery tests are 252/252; package validation is 117/117 without warnings; SDK validation passes.
  The game remained closed and no deployment occurred.

## Complete retail-byte Affair Wife proof (2026-09-10)

- Added exact snapshots for Init `0x00DB2A70-0x00DB2B10` (160 bytes / 58 instructions) and complete
  Main `0x00DB2B10-0x00DB3E2A` (4,890 bytes / 1,485 instructions). Both terminate at exact `ret`
  boundaries and reproduce directly from the installed retail PE.
- The Main artifact subsumes the earlier route `0x00DB3426-0x00DB35BF` and argument
  `0x00DB35C1-0x00DB3CAA` evidence, covering all idle, question, run, arrival, argument, hit/talk,
  conversation, animation, and cleanup paths without gaps.
- It reconfirms a one-time husband-position snapshot for movement and repeated distance tests against
  the same live husband wrapper before argument entry. All subsequent facing and speech operands also
  target that wrapper. The affair trio now has complete Init/Main byte artifacts with no static Lua or
  Forge mismatch.
- The observed empty-space scolding remains a runtime-only thing/rendered-model identity question;
  existing ROUTE probes are the required next evidence. Recovery tests are 254/254, package validation
  is 117/117 without warnings, and SDK validation passes. No launch or deployment occurred.

## Complete retail-byte Book Trader proof (2026-09-10)

- Added exact, reproducible snapshots for Init `0x00DB3F00-0x00DB3F98`
  (152 bytes / 56 instructions) and Main `0x00DB3FA0-0x00DB4F6A`
  (4,042 bytes / 1,187 instructions), both through terminal `ret` instructions.
- The continuous Main range covers home return and Theresa facing, hit response, all purchase and
  insufficient-funds branches, chocolate/gold transfer, objective update, random roll-up speech,
  timer handling, and every movie/control cleanup edge.
- Lua and inventory reconcile without a behavioral correction. Recovery tests are 256/256; package
  validation is 117/117 without warnings; SDK validation passes. The game remained closed and no
  deployment occurred.

## Complete retail-byte Barrel Thug proof (2026-09-10)

- Added exact installed-PE snapshots for Init `0x00DB6BF0-0x00DB6C31`
  (65 bytes / 22 instructions) and Main `0x00DB6C60-0x00DB7CF1`
  (4,241 bytes / 1,259 instructions), both ending on exact `ret` boundaries.
- Continuous coverage locks the leave-in-charge wait, Hero following, intro, timer-tier temptation and
  well-done chatter, why-not-smash/outro talk selection, hit reaction/bad deed 2, and every nested
  movie/control cleanup edge.
- The bytes confirm Barrel Thug does not own Barrel Man's return consequence. His state-dependent
  chatter and Barrel Man's visibility/proximity judgment are independent retail paths; no scold event
  is being dropped between them.
- Recovery tests are 258/258; package validation is 117/117 without warnings; SDK validation passes.
  Fable remained closed and nothing was deployed.

## Complete retail-byte Barrel Man proof (2026-09-10)

- Added reproducible complete snapshots for Init `0x00DB5260-0x00DB5307`
  (167 bytes / 61 instructions) and Main `0x00DB5330-0x00DB6B23`
  (6,131 bytes / 1,747 instructions), both ending at exact `ret` boundaries.
- This covers every offer, walk-off, hidden wait, return, judgment, post-return talk, hit, ambient
  speech, timer, movie/control, and cleanup path. It subsumes the earlier focused judgment slice.
- Immediate thanks is conclusively sight-or-distance only. The broken-barrel scold
  `LETDOWN_BROKEN` is selected later only when `HeroLetMeDown` was set by failing the return judgment;
  `BarrelBrokenPersistent` then chooses broken versus unbroken letdown wording. There is no retail
  destruction override for a Hero who returned within 10 m.
- Recovery tests are 260/260; package validation is 117/117 without warnings; SDK validation passes.
  The game remained closed and nothing was deployed.

## Complete retail-byte Barrel lifecycle and final-gold investigation (2026-09-10)

- Added reproducible snapshots for the shared empty Init `0x00CDEBB0-0x00CDEBB1`
  (1 byte / 1 instruction), OnPredicateFail `0x00DB7DB0-0x00DB7DE1`
  (49 bytes / 19 instructions), and complete Main `0x00DB7E10-0x00DB7FEC`
  (476 bytes / 144 instructions). All end at exact terminal `ret` boundaries.
- TLC `WatchBarrels` at `0x00DBE890` confirms the reported Anniversary behavior: when the break
  count equals `total-1`, retail looks up the remaining `NOVI_Barrel` and calls
  `AddItemToContainer(..., "OBJECT_GOLD_1")`. Smashing that final barrel should release the coin.
- The reconstructed Lua already preserves that precise penultimate-break insertion. The missing
  observed coin therefore narrows to runtime remaining-barrel lookup/container delivery rather than
  absent quest logic.
- Added Forge diagnostics around `AddItemToContainer` to log the item and exact container identity
  before the retail API call and successful return afterward. Release|x86 builds cleanly; undeployed
  DLL SHA-256 is `AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`.
- Recovery tests are 264/264. Fable remained closed and no deployment occurred.

## Complete retail-byte CreatedBeetle proof (2026-09-10)

- Added reproducible snapshots for shared empty Init `0x00CDEBB0-0x00CDEBB1`
  (1 byte / 1 instruction) and complete Main `0x00DB80C0-0x00DB81A8`
  (232 bytes / 79 instructions), both ending at exact terminal `ret` boundaries.
- This closes the entire spawned-beetle lifecycle: register/set the five-second timer, poll the same
  timer ID, remove the beetle with `(true,true)`, and deregister the timer on normal and termination
  exits.
- Lua and inventory reconcile without a behavior correction. Recovery tests are 266/266; package
  validation is 117/117 with zero warnings/errors; SDK validation passes. The game remained closed
  and no deployment occurred.

## Complete retail-byte TeddyGirl proof and boundary correction (2026-09-10)

- Added reproducible snapshots for Init `0x00DAF000-0x00DAF052` (82 bytes / 37 instructions),
  complete Main `0x00DAF080-0x00DB0600` (5,504 bytes / 1,620 instructions), and GivenTeddy
  `0x00DB0600-0x00DB065B` (91 bytes / 29 instructions).
- Direct executable decoding caught a generated-metadata defect: `translation_unit.json` lists Main
  size 5,498 and therefore ends at `0x00DB05FA`, halfway through the six-byte final stack adjustment.
  The exact terminal `ret` is `0x00DB05FF`; the reproducible snapshot locks the corrected boundary.
- Continuous coverage includes the offered/presented teddy branches, good deed and hit-girl bad deed,
  TeddySolution B/C writes, ruined-teddy conversation and walk-off, repeat dialogue, all acquisition
  and movie cleanup, and GivenTeddy helper.
- Lua and inventory reconcile without a behavior edit. Recovery tests are 268/268; package fixtures
  remain 117/117 with zero warnings/errors. Fable remained closed and nothing was deployed.

## Anniversary footage review and nearby broken-stock correction (2026-09-10)

- Downloaded and directly inspected the user-requested 7:40-9:40 interval of
  <https://www.youtube.com/watch?v=4IkkNZxW0MQ&t=8s>. The run shows the unattended-return notice,
  followed on interaction by Barrel Man's `A fat lot of good you were`, damaged-stock, and threat
  lines. A concise timestamped observation is preserved in
  `runtime_evidence/anniversary-barrel-reference-20260910.md`.
- The footage proves the broken-stock rebuke is a real shipped outcome, although its recorded run
  first fails the unattended-return condition. The user's distinct nearby-plus-broken playtest still
  produced contradictory thanks under TLC's exact automatic judgment.
- Added a scoped New Oakvale compatibility branch: if the Hero is detected but stock is broken,
  Barrel Man speaks `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN`, sets the follow-up letdown state, and
  does not award the good deed. It deliberately does not add the separate unattended-warehouse deed.
- Regenerated the affected Barrel Man traces. A regression test asserts broken rebuke present,
  thanks absent, and no good/bad deed counter write in this branch. Recovery tests are 271/271;
  package validation is 117/117 with zero warnings/errors; SDK validation passes. The change remains
  undeployed and Fable remained closed.

## Complete retail-byte Victim proof and boundary correction (2026-09-10)

- Added reproducible snapshots for Init `0x00DAEEB0-0x00DAEF4C`
  (156 bytes / 56 instructions) and complete Main `0x00DBCD60-0x00DBDE35`
  (4,309 bytes / 1,272 instructions), both through exact terminal `ret` boundaries.
- `translation_unit.json` reports Main size 4,303 and truncates its last six-byte stack adjustment and
  `ret`; direct installed-PE decoding establishes the corrected boundary.
- The continuous artifact covers VictimShake, BullySubdued/BullyRanOff, first/repeat Hero attacks,
  reciprocal ally calls, bad deed, BRATHIT setup/cleanup, thanks, tutorial, conversations, and the
  teddy-loss complaint. Lua and inventory reconcile without a behavior correction.

## Complete retail-byte LiveFather proof and boundary correction (2026-09-10)

- Added reproducible snapshots for Init `0x00DAC390-0x00DAC41D`
  (141 bytes / 51 instructions) and complete Main `0x00DB86B0-0x00DB9795`
  (4,325 bytes / 1,301 instructions), both through exact terminal `ret` boundaries.
- Direct decoding corrects another generated six-byte epilogue truncation: the executable continues
  through the final stack adjustment to the `ret` at `0x00DB9794`.
- Continuous coverage includes Father intro setup/skip cleanup, deed counter, every good/bad reward
  selection, pennies and chocolate handoff, hit response/bad deed, and childhood completion state.
- Lua and inventory reconcile without a behavior edit. Recovery tests are 273/273; package fixtures
  remain 117/117 with zero warnings/errors. Fable remained closed and nothing was deployed.

## Complete Theresa, Villager, and Bully entity coverage (2026-09-10)

- Theresa: Init `0x00DAC4F0-0x00DAC57E` (142 bytes / 52 instructions) and Main
  `0x00DB97A0-0x00DBB305` (7,013 bytes / 1,865 instructions). The direct boundary corrects the
  generated three-byte Main truncation and covers meet, chocolate, hit, and raid-outro paths.
- Villager: Init `0x00DADF00-0x00DADF52` (82 bytes / 33 instructions), Main
  `0x00DADF80-0x00DAEA63` (2,787 bytes / 823 instructions), and GetVillagerSpeechIndex
  `0x00DAEBF0-0x00DAEC5D` (109 bytes / 44 instructions). This closes all morality/sex speech axes,
  hit response, and nonrepeating random selection.
- Bully: Init `0x00DAED30-0x00DAEDDC` (172 bytes / 65 instructions), Main
  `0x00DBB310-0x00DBCCFF` (6,639 bytes / 1,909 instructions), and GivenTeddy
  `0x00DBCD00-0x00DBCD58` (88 bytes / 28 instructions). The full artifact subsumes the teddy
  dispatch, health bar, run-off, and helper snapshots and corrects a nine-byte generated Main
  truncation.
- All 15 `NOVI_*` entity inventories now carry a complete reproducible lifecycle/dispatcher byte
  snapshot. Recovery tests are 279/279; package fixtures are 117/117 with zero warnings/errors; SDK
  validation passes. The game remained closed and nothing was deployed.

## Undeployed next-playtest bundle (2026-09-10)

- Added an all-entity coverage audit requiring a valid complete snapshot with terminal `ret`
  boundaries for every `NOVI_*` inventory; it reports 15/15 complete.
- Added `prepare_new_oakvale_test_bundle.py` and generated
  `work/new_oakvale_test_bundle_20260910/` without touching the installed game.
- The bundle contains 24 payload files: the complete 23-file FSE tree plus the diagnostic DLL. All
  manifest hashes reverify; `deploymentPerformed` is explicitly false. DLL SHA-256:
  `AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`.
- Recovery tests are 280/280. The next interactive pass can deploy this exact frozen payload and
  target Barrel Man rebuke/final gold, Bully marker completion, and Affair Wife route identity.

## Complete quest-level retail-byte coverage (2026-09-10)

- Added one reproducible installed-PE artifact covering all 15 named quest-level regions: core
  lifecycle, both deed helpers, all six spawned/phase helpers, post-attack logic, and both destructor
  layers. Total coverage is 11,838 bytes / 3,641 decoded instructions.
- The generated quest Main size (2,018 bytes) stops at the first return. Retail has a second cleanup
  epilogue through the terminal `ret` at `0x00DAC2B3`, making the complete Main 2,036 bytes.
- The 450-byte destructor implementation correctly terminates with a tail `jmp` at `0x00DBF17D` to
  the base `CScriptBase` destructor (`0x00CBD510`), while the deleting-destructor wrapper terminates
  with `ret 4`.
- The quest inventory now cites this complete lifecycle artifact. Recovery tests are 282/282;
  package fixtures are 117/117 with zero warnings/errors; SDK validation passes. Fable remained
  closed and nothing was deployed.

## Package-wide native coverage certificate and DeadFather closure (2026-09-10)

- A package-wide audit exposed `OVI_DeadFather` as the one entity omitted by the earlier `NOVI_*`
  completeness filter. The gate was kept strict and the missing artifact was added.
- DeadFather now covers empty OnPredicateFail `0x00DB8260-0x00DB8261`, Init
  `0x00DB8290-0x00DB82F4`, and Main `0x00DB8300-0x00DB8516`: 635 bytes / 207 instructions. Direct
  decoding corrects Main's generated three-byte epilogue truncation.
- The final certificate requires all 17 inventories: the 15 `NOVI_*` entities, `OVI_DeadFather`, and
  `Q_NewOakValeIntro`. They total 78,891 exact bytes / 23,388 instructions from the single installed
  retail executable SHA-256 `41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10`.
- Recovery tests are 285/285; package fixtures are 117/117 with zero warnings/errors; SDK validation
  passes. Fable remained closed and nothing was deployed.

## Translation-unit boundary metadata fix (2026-09-10)

- Root cause of the repeated apparent size truncations: the exporter labeled Ghidra
  `Function.getBody().getNumAddresses()` as `size`. That is the number of addresses in a potentially
  disjoint body, not a contiguous entry-to-end extent, so address-plus-size is not a safe boundary.
- Upgraded `ExportScriptTranslationUnit.java` output to schema 0.2. It retains legacy `size` while
  explicitly emitting `bodyAddressCount`, `bodyMin`, `bodyMaxInclusive`, `bodyEndExclusive`,
  `bodyExtent`, and the complete `bodyRanges` list.
- Added a regression test for the expanded schema. Recovery tests are 286/286; no runtime payload
  changed, and the game remained closed.
- Regenerated `translation_unit.json` read-only from the Ghidra retail project with the original
  range and definition list. Schema 0.2 preserves all 100 entry addresses; Bully Main demonstrates
  the corrected distinction with 6,630 body addresses, a 6,639-byte extent, and four body ranges.
  The test now validates the checked-in artifact and its exact range metadata.

## Deterministic final-barrel gold diagnostics (2026-09-10)

- Added Lua probes immediately before and after `AddItemToContainer`: break index, initial total,
  stable container-present state, item name, and successful native-call return. Forge's matching
  diagnostics retain the exact native wrapper/resource identity.
- The WatchBarrels trace proves the reward branch executes at break 5 of 6 with a present remaining
  barrel and returns from insertion. A regression test locks ordering and presence.
- The first probe revision used Lua `tostring(table)`, whose address changes between processes and
  caused the stored trace to differ. Replaced it with the stable presence field; two consecutive
  package validations pass 117/117 with zero warnings/errors.
- Rebuilt the hashed undeployed payload as `work/new_oakvale_test_bundle_20260910_v3/`. Recovery tests
  are 287/287. Fable remained closed and nothing was deployed.

## Automated targeted playtest-log analysis (2026-09-10)

- Added `analyze_new_oakvale_targeted_playtest.py` to extract a stable result matrix for the Barrel
  Man rebuke, contradictory thanks, gold arm/container/native return, Bully fallback completion, and
  Affair Wife route start/reached evidence.
- Added a synthetic regression test covering every positive target and the absence of contradictory
  thanks.
- Baseline analysis of `interactive-20260910-redtimer-bullyrun-barrels.log` correctly reports the old
  contradictory thanks and no evidence from any newly added correction/probe. The structured result
  is saved as `runtime_evidence/analysis-interactive-20260910-targets.json` for direct comparison with
  the next v3 playtest log.
- Recovery tests are 288/288. Fable remained closed and nothing was deployed.

## Complete retail-byte Guard proof (2026-09-10)

- Added reproducible installed-PE snapshots for Init `0x00DAC650-0x00DAC760`
  (272 bytes / 92 instructions) and complete Main `0x00DAC760-0x00DADE4B`
  (5,867 bytes / 1,791 instructions), both ending at exact `ret` boundaries.
- The continuous Main artifact covers the initial deed claim and post-chase recheck, first/repeat
  lecture selection and bodies, crime-list construction, hit/chase behavior, shared counter updates,
  and terminal cleanup. It subsumes the earlier 4,020-byte focused repeat-lecture evidence.
- Retail compares `BadDeedsPerformed - GuardsDealtWithBadDeeds > 0`, then claims all currently
  outstanding deeds by copying the current bad-deed total into the dealt total. `GuardsSpokenOnce`
  selects the first six-line lecture or the later `CAUGHT_YOU_AGAIN` path. Thus one lecture for one
  barrel-destruction deed is retail-correct; a repeat requires another new deed.
- Lua and its inventory reconcile without a behavioral correction. Recovery tests are 262/262;
  package validation is 117/117 with zero warnings/errors; SDK validation passes. The game remained
  closed and no deployment occurred.

## Final-barrel gold ABI certificate (2026-09-10)

- Added `audit_final_barrel_gold_abi.py` and its regression test.
- Retail WatchBarrels callsite `0x00DBEA2F` dispatches slot `0x924` when `broken == total - 1`.
  Direct PE inspection of `CGameScriptInterface` vtable `0x01260F0C` resolves index 585 to
  `0x0089E780`, named `AddItemToContainer` by both the retail catalog and PDB-derived signature.
- The audit locks Forge's `__thiscall` typedef, index-585 binding, and wrapper operand order
  `(interface, container, CCharString item)`, then checks the Lua remaining-barrel insertion and
  return probe in order. This avoids adding an unsafe cross-CRT vector query to the gameplay path.
- Full script-recovery suite passes 289/289. Fable remained closed and nothing was deployed.

## Coherent v4 offline payload gate (2026-09-10)

- Generated `work/new_oakvale_test_bundle_20260910_v4/` from the current reconstructed scripts and
  canonical Forge release DLL. Its manifest covers 24 files, re-verifies with no hash mismatches,
  records `deploymentPerformed=false`, and pins DLL SHA-256 `AE55FE0D...9DF3C`.
- Two consecutive package validations are byte-identical and pass 117/117 fixtures with zero syntax,
  API-scope, callsite, warning, or error findings.
- Affair Wife route/position-chain, Bully run-off, final-barrel gold ABI, and SDK gates all pass.
  Stale noncanonical SDK mirrors were explicitly excluded. Fable remained closed and no deployment
  occurred.

## Wife world-identity diagnostics and v5 payload (2026-09-10)

- The completion audit found one genuinely weak evidence seam: Wife route probes named the husband
  and recorded both positions, but could not distinguish a replacement wrapper/world object.
- `LogCScriptThingDetails` now invokes the proven retail wrapper slots for `GetPThingUniqueID` and
  `GetPos` whenever the wrapper vtable matches, recording `WorldUID` and `WrapperPos` without reading
  guessed implementation fields. Release|x86 builds successfully.
- The targeted analyzer is schema 0.2 and captures the final `NOVI_AffairMan` lookup before
  `ROUTE_START`, including its UID and wrapper position. The archived old log correctly reports the
  new identity evidence absent.
- Generated undeployed `work/new_oakvale_test_bundle_20260910_v5/`: 24 hash-verified files,
  `deploymentPerformed=false`, DLL SHA-256 `1250BBCCF6E8D1C16E57032A331D111C4CC1E3694E6E8A3B53E4A2CDBCEB9268`.
  Recovery tests pass 290/290 and SDK validation passes with stale noncanonical mirrors skipped.

## Native Bully RunTo wait proof and variance census (2026-09-10)

- `RunCutsceneMacro_Func`'s retail `.RunTo` branch uses movement enum 1, interprets the authored
  false wait operand as requiring a loop, and polls scripted-resource vtable slot `0x68` while
  advancing frames and checking skip state. The subsequent 0.5-second pause is not its wait.
- Added `analyze_bully_run_variance.py`, a regression test, structured five-log report, and focused
  evidence note. One archived run reaches `MK_OIBR_BULLY2`; four move only 2.326-2.365 m and stop at
  the exact same point 11.759 m short despite nearly identical starts.
- This rules out a categorical destination, move-enum, actor-map, or macro-ABI failure and localizes
  the symptom to nondeterministic task/path completion. It independently justifies the conditional
  same-marker compatibility move. Recovery tests pass 291/291; no launch or deployment occurred.

## Exact Barrel Man localized outcome asset (2026-09-10)

- Added a reproducible extractor, snapshot, audit, and two tests for the installed English
  `text.big` Barrel Man groups. The source bank SHA-256 is `531C4514...33FC`.
- `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN` resolves in order to IDs 13209/13223/13235: the “fat lot
  of good,” damaged stock/trust, and threat lines. This is the exact sequence in the requested video.
- The audit also contrasts the two-line thanks group and locks Lua's broken-stock branch to the
  three-line rebuke before any thanks path. Recovery tests pass 293/293; no game launch or deployment.

## Final-barrel gold engine-release closure (2026-09-10)

- Decompiled installed `CGameScriptInterface::AddItemToContainer` at `0x0089E780`: it validates the
  target, resolves the definition, obtains interface `0x3c` (`CTCContainer`), and invokes
  `CTCContainer::InsertItem`.
- Decompiled `CTCContainer::InsertItem` at `0x008ED1F0`; it appends the definition index and item
  string to the container reward vector.
- Disassembled `CTCContainerRewardHero::OnDie` at `0x0074F080`. Its call at `0x0074F0D6` enters
  `CreateItemPickupContainer` (`0x0074EC50`), which moves stored entries into a newly created
  `OBJECT_CHEST_REWARD_ON_DEATH` world pickup container.
- `ReleaseItems` at `0x0074E6E0` independently shows direct reward pop-out: it creates one world
  object for each stored entry, applies outward trajectories, and clears the source vector.
- Recorded the static chain in
  `runtime_evidence/final-barrel-gold-native-release-20260910.md`. The penultimate-break insertion is
  deliberately upstream of collectible creation on the final barrel's death. Runtime object/component
  configuration remains the final interactive check; no game launch or deployment occurred.

## New Oakvale barrel component proof (2026-09-10)

- Added `audit_barrel_reward_component.py` and its regression test.
- All nine hashed retail `NOVI_Barrel` TNG records use `OBJECT_BARREL_BREAKABLE`.
- Definition entry 3684 decodes to nine named components and explicitly includes
  `CTCContainerRewardHero` alongside `CTCSmashableBarrier`; component name references are resolved
  from the hashed installed `names.bin`.
- This closes the object-configuration uncertainty: New Oakvale's final barrel has the exact engine
  component whose `OnDie` path creates the reward pickup. A missing visible coin now narrows to
  script timing/lookup/insertion or observation/collection, not a missing reward component.

## Reproducible gold audits and v7 offline payload (2026-09-10)

- Added `audit_final_barrel_gold_release.py` and its regression test. Against installed executable
  SHA-256 `41DC9109...AC10`, it resolves five direct calls: script-interface insertion to
  `CTCContainer::InsertItem`, reward `OnDie` to pickup creation, pickup creation to reward expansion
  and world-object creation, and pickup population back through `InsertItem`.
- The component audit reproduces all nine `OBJECT_BARREL_BREAKABLE` component names from hashed
  retail TNGs, pristine `game.bin.retail-bak`, and installed `names.bin`; both new audits pass.
- Recovery suite passes 295/295. Rebuilt ForgeFSE Release|x86 from current source with 0 warnings and
  0 errors; the rebuilt DLL SHA-256 is `BE0ECDF494A9BD3BEABD121167155A9F012CB723A7D2DA0FE5B2A3F29448C2F2`.
- Generated undeployed `work/new_oakvale_test_bundle_20260910_v7/`: 24 files, every manifest hash and
  size reverified, `deploymentPerformed=false`. Package validation passes 117/117 with zero syntax,
  API, callsite, warning, or error findings. Tooling SDK validation passes with stale noncanonical
  mirrors excluded.
- The broad foundation audit remains 15/18 for pre-existing repository-wide backlog outside New
  Oakvale (global binding reconciliation, three unrelated unregistered entity scripts, and four
  unrelated seed packages without anchors). This is not represented as a New Oakvale gate failure.
  Fable remained closed and nothing was deployed.

## Reward OnDie guard closure (2026-09-10)

- Exported the installed component constructor at `0x0074E6B0`. It initializes the reward vector
  empty and both control bytes `+0x18/+0x19` true; `OnDie` tests `+0x18`, so reward release starts
  enabled.
- Resolved `OnDie` interface guard `0xA0` through the retail PDB enum to
  `TCI_SUMMONABLE_CREATURE`. The decoded barrel component list does not contain that interface, so
  its special suppression path cannot apply.
- Extended the executable release audit to lock constructor defaults, interface identity, and the
  barrel's positive reward-component / negative summonable-component facts. The audit passes.

## `OBJECT_GOLD_1` collectible proof (2026-09-10)

- Added `audit_gold_piece_definition.py` and its regression test.
- Pristine retail entry 4644 is `OBJECT_GOLD_1`; its payload offset `0x45` links directly to anonymous
  `CGoldDef` entry 13648, which decodes to the integer value `1`.
- The object component list includes `CTCGold` with standard physics and graphics. This proves the
  scripted reward is the actual collectible one-gold object rather than a cosmetic placeholder.

## Archived final-barrel branch correlation (2026-09-10)

- Upgraded `analyze_new_oakvale_targeted_playtest.py` to schema 0.3 with legacy branch inference that
  remains explicitly separate from direct native-call proof.
- In `interactive-20260910-redtimer-bullyrun-barrels.log`, nine barrels are enumerated, eight death
  callbacks occur, the singular remaining-barrel lookup succeeds, and the ninth callback follows.
  That lookup is unique to WatchBarrels' penultimate-break gold branch.
- The user's run therefore reached the reward branch with a valid final barrel. Because that older DLL
  did not log the native insertion call, the analysis correctly leaves `goldNativeCallReturned=false`;
  v7's probes will close that last runtime distinction without conflating inference with observation.
- Full recovery suite passes 296/296 after the collectible-definition and legacy-log correlation
  audits. Fable remained closed and nothing was deployed.
# 2026-09-10 evening continuation

- Directly reviewed the requested Anniversary video interval (7:40-9:40) from a local 360p copy,
  including fine-grained frames rather than relying only on TLC text assets. The destructive route
  visibly leads to the three-line `BARRELMAN_LETDOWN_BROKEN` rebuke after the unattended return/bad
  deed. A subsequent 720p frame review corrected the apparent final-destruction “pickup” reading:
  the visible green glow and `+1` are experience/combat feedback, not independent gold proof. Gold
  remains certified by TLC's `OBJECT_GOLD_1`/`CGoldDef=1` insertion and native death-release chain.
  Detailed
  ordering is recorded in `runtime_evidence/anniversary-barrel-reference-20260910.md`.
- User clarified that Affair Wife can reach her husband successfully and only sometimes stops
  short. This rules against a consistently wrong destination and matches an intermittent movement
  task failure. Added a compatibility watchdog that preserves the retail one-shot route normally,
  but reissues the identical live-husband `MoveToPosition` only if the task has ended while the
  native 3-metre arrival predicate remains false (immediate ended-task detection, four retries, no
  teleport).
  `ROUTE_RETRY` probes record positions and distance for the next interactive test.
- Added a full Anniversary-order Barrel Man fixture: unattended warning, bad-deed state write,
  later Hero interaction, then `BARRELMAN_LETDOWN_BROKEN`, with no thanks/good-deed write. Renamed
  the misleading nearby-broken fixture to `entity_barrel_man_nearby_broken_scolds`.
- Split Affair Wife route coverage into normal one-command arrival and a simulated ended-short
  navigation task. The stalled trace reissues the exact radius/run/boolean operands and proves
  `ROUTE_RETRY` precedes `ROUTE_REACHED`; the arbitrary initial cooldown was removed, while the
  four-retry safety cap remains.
- Extended the four-hit Bully trace through the observed short-cutscene-run compatibility path. A
  focused audit proves he is restored drawable before the marker move and hidden only after it,
  before the second retail cutscene and final removal.
- Upgraded package fixture metadata with argument-aware `expectCalls`, `forbidCalls`, and ordered
  `expectCallSequence` checks. Applied them to the Anniversary Barrel Man branches, normal/stalled
  Wife routes, Bully visibility/movement ordering, WatchBarrels' exact `OBJECT_GOLD_1` insertion and
  beetle operands, and good/bad deed field/morality ownership. The package gate can now detect a
  semantically wrong call even when the same API name remains in the trace.
- Added a cross-layer Bully cutscene boundary audit. It locks retail's `__fastcall` register/stack
  layout, Forge's matching six-argument call, borrowed-movie lifetime, the native `.RunTo` task poll,
  the 1-complete/4-identically-short runtime census, and the conditional same-marker fallback. This
  rules out an ABI or premature Forge movie-destruction rewrite; the failure is the engine movement
  task becoming complete at a nondeterministic path endpoint.
- Reconciled the authoritative handoff against the current artifacts. Its opening snapshot now
  pins undeployed bundle v9, the `BE0ECDF...F2F2` DLL, 302/302 recovery tests, and 119/119 package
  fixtures, and explicitly separates current compatibility behavior from the older chronological
  evidence below it. The reconstruction guide now likewise distinguishes retail's literal nearby-
  thanks edge case from the chosen broken-stock compatibility override and records that Wife's route
  failure is intermittent rather than a consistently wrong destination.
- Re-ran the complete recovery suite (302/302), package validation (119/119, zero syntax/API/callsite
  errors or warnings), documentation link/findings/root checks, and README progress consistency.
  Fable remained closed and no bundle or DLL was deployed.
- Corrected the evidence taxonomy for Affair Wife. Her sole remaining note described an intermittent
  runtime movement endpoint, not an unknown native operation or operand, yet `uncertainties` caused
  the generated state manifest to downgrade an otherwise byte-locked reconstruction to medium
  confidence. Moved the preserved note to `runtimeObservations`, taught the manifest generator to
  carry that field independently, and strengthened the static-world audit to require both zero static
  uncertainty and retention of the runtime seam. Regeneration now reports high reconstruction
  confidence plus one runtime observation, without hiding either fact.
- Closed the raid section-swap API boundary offline. A new audit joins the exact 285-byte
  `AttackStuff` region/hash to retail GSI slots `0x450`/`0x460`, Forge's matching pointer/delay
  signatures and forwarding, the exact double-underscore post-attack section spelling, and Lua's
  activation/deactivation/time/theme/objective order. The direct fixture now checks exact arguments
  and the ordered sequence rather than API names alone. Generated runtime-only wording is narrowed
  to engine-side spawned-thing lifetime timing after the proven swap.
- Closed the childhood Hero morph boundary offline. The new audit pins the exact 1,172-byte
  `DoMission` hash, retail GetHero/TurnCreatureInto slots `0x118`/`0x178`, Forge's out-buffer
  signature/allocation/wrapping, and the critical post-section-frame Hero reacquisition before
  killability is changed. Strengthened the fresh-start fixture with exact child-def/control operands
  and ordered calls, and narrowed generated runtime-only wording to the actual rendered morph result.
- Promoted the deed morality magnitude from a prose finding to a reproducible installed-data audit.
  The audit hashes the retained retail `script.bin`, schema-decodes `SCRIPT_DEF` entry 597 with zero
  leftover bytes, obtains `OVI_MoralityChangePerDeed = 0.0010000000474974513`, locks the exact
  AddGoodDeed/AddBadDeed byte hashes and shared `+0xd64` positive/negative use, and verifies Forge
  slot `0x270` and Lua's opposite-sign calls. Removed the stale morality-amount runtime-only label.
- Closed the warehouse timer rendering/cadence evidence chain. Added a scoped runtime-observation
  record for the user's corrected red (formerly blue) result and a composite audit joining it to the
  exact StartBarrelTimer body, native quest-bar storage/render functions, Forge BGRA bridge, and the
  archived contiguous 45-through-0 nonnegative countdown. Removed the obsolete blanket runtime-only
  timer item; this conclusion is explicitly scoped to New Oakvale's warehouse bar.
- Narrowed the dead-father cutscene runtime boundary to visual composition only. A new composite
  audit combines the complete retail PostAttackStuff sequence, Lua's ordered pause/camera-fix/macro
  bracket, and the single-authority completed-childhood log showing HESDEADJIM enter, skippable
  cutscene-mode activation, exit, and restored action/GUI/cutscene state. Generated runtime-only text
  no longer implies that macro execution or camera-state cleanup is unproven.
- Split the broad AI-action runtime caveat into proven and genuinely visual portions. A structured
  observation preserves both successful and short Wife routes, classifying the defect as intermittent
  path-completion variance. The paired audit finds the successful `AFFAIRMAN_GOOD_LAD` endpoint and
  positive-frame `Speak_Blocking` completions in the completed-childhood log. Generated runtime-only
  wording now targets visual Wife/Bully endpoint parity rather than all FollowThing, Speak, and
  conversation semantics.
- Offline gates after that pass: 308/308 recovery tests; 119/119 package fixtures with zero syntax,
  API, callsite, error, or warning findings; documentation links/findings/root and README progress
  all clean. Process census found no running Fable/FSE process; nothing was deployed or launched.
- Closed the broad normal-teardown portion of the unload caveat. The cross-layer teardown audit
  verifies termination guards on quest Main, every spawned thread runner, and entity Main; registry
  unref/clear behavior on both quest destructor modes; unconditional idempotent entity unregister;
  and a captured twelve-class entity unwind -> Dead Father predicate callback -> persistence ->
  completion return -> deleting quest destructor -> retained-reference release sequence. Only a
  repeated mid-childhood region unload/reload re-entry remains runtime-only.
- Gates after teardown closure: 309/309 recovery tests and 119/119 package fixtures, with zero
  syntax/API/callsite/errors/warnings; docs and README checks pass; `gameProcesses=0`.
- Closed scripted father replacement timing across the raid section swap. A composite audit joins the
  exact retail/Forge/Lua activate-then-deactivate boundary to three independent playthrough logs, each
  showing Dead Father allocation/Init, Live Father Main unwind, then Dead Father Main entry. The
  remaining section-swap caveat is now only visual/non-scripted population parity.
- Gates after section-swap runtime closure: 310/310 recovery tests; 119/119 package fixtures with no
  findings; docs and README checks clean; `gameProcesses=0`. No launch or deployment occurred.
- Decoded the full retail `CS_OAKVALEINTRO_HESDEADJIM` asset from `script.bin`: entry 485, 3,057-byte
  payload, 89 macro commands, thirteen camera uses over eleven camera records, and six skip-cleanup
  commands, with zero schema leftover. All camera names resolve to hash-locked retail TNG scripted or
  scripted-spline records. The former composition caveat is now rendering/animation presentation only.
- Offline gates after the cutscene-asset audit: 311/311 recovery tests and 119/119 package fixtures;
  all validation/docs/README checks clean; `gameProcesses=0` before the user returned.

## Single-authority regression and overnight handoff (2026-09-10)

- The v9 manual deployment copied the safe source-control defaults instead of the dedicated runtime
  profile. Active startup explicitly said `Retail allocator override disabled by configuration`,
  while installed `quests.lua` separately registered `Q_NewOakValeIntro`. The user consequently saw
  the old dual-authority signature again: Guard and Wife behavior appeared twice even though Forge's
  Lua log contained only one pre-affair Guard claim and one Wife question/accepted response.
- Closed the invalid run and archived its log as `work/dual-authority-regression-20260910.log`.
  Restored installed single authority: `quests.lua` is empty and `retail_override.lua` enables the
  acknowledged identity-preserving replacement. The game is closed (`gameProcesses=0`). Tomorrow's
  first gate is startup confirmation of both `Armed identity-preserving retail override` and
  `Replacing retail allocator`; close immediately if either is absent.
- Extended `analyze_new_oakvale_targeted_playtest.py` to count gold insertions, parse Guard
  `(BadDeedsPerformed, GuardsDealtWithBadDeeds)` claim pairs, distinguish unique deed advancement
  from duplicate authority, verify the exact first-lecture key set, and count Wife disclosure
  questions/accepted responses. Its focused unit test passes. The invalid run records one scripted
  gold insertion, Guard claims `(1,1)` then `(2,2)`, and one Wife question/thank-you; the extra
  visible behavior was therefore outside that Lua authority.
- Revised `prepare_new_oakvale_test_bundle.py` so a generated runnable bundle replaces the standalone
  registry with `runtime_playtest/quests.lua`, enables the acknowledged identity override only in the
  output, and writes an explicit authority block to manifest schema 0.2. Added
  `test_prepare_new_oakvale_test_bundle.py`. That test and the targeted-playtest analyzer test pass
  together (2/2).
- Added a Forge control-stack implementation because retail Wife retains priority-3 Main control
  while nesting priority-4 conversation control. Release|x86 compiles successfully. Repo output DLL:
  `4A1D92C902D97C6FBC1E84C8CDD77ABDC9D530A835E6D704634AEE0072D0AD67`; installed DLL remains
  `85256306BE56A293D70BE6E60EF1056B2E849B96404C8ED8CF582CA7DEEE7453`. The stack change is not yet
  deployed and needs offline review/tests before v10.
- Retail Dead Father behavior is resolved, not a reconstruction gap: PostAttackStuff waits until the
  Hero is within 5 m of `MK_OVI_DAD`, sets `DadFound`, and immediately starts HESDEADJIM. No
  `MsgIsTalkedToByHero` participates. His transient highlight is a retail presentation quirk; making
  the scene interaction-gated would be a deliberate gameplay change.

## Control-stack review, gates, and v10 deployment (2026-09-11)

- Reviewed the nested-control-stack change in `LuaEntityAPI.{h,cpp}` offline. Two defects: the
  suspended outer handle was never removed from `g_controlHandlesByEntityData`, so the nested
  acquire fell into the cross-VM borrow branch and "borrowed" the same VM's own suspended handle
  (no priority-4 resource was ever created, and the subsequent `ReleaseControl` only dropped the
  borrow entry, leaving the outer handle stranded in `m_suspendedControlHandles` until VM teardown);
  and the malloc/termination failure paths never restored the suspended handle. Suspension also ran
  before the required-API gate.
- Fix: new private helpers `SuspendOwnedControlHandle` (push to the per-entity stack, erase the live
  registry entry, clear owned maps) and `RestoreSuspendedControlHandle` (pop, re-own, re-register
  live). `AcquireControl` decides "nest over owned" vs "borrow foreign" up front, borrows only when
  this VM owns nothing for the actor, suspends only after the API gate, and restores on malloc
  failure and on termination mid-wait. `ReleaseControl` restores through the same helper.
- `audit_forgefse_control_abi.py` bumped to schema 0.2 with six new structural checks
  (`suspendedHandleLeavesLiveRegistry`, `nestedAcquireSkipsSelfBorrow`, `suspendOnlyAfterApiGate`,
  `failedNestedAcquireRestoresSuspended`, `releaseRestoresSuspendedOuterHandle`,
  `restoreReturnsHandleToLiveRegistry`); 7/7 focused tests; real tree `ok=true`.
- Rebuilt Release|x86 with zero warnings/errors. DLL SHA-256
  `925B637C51033E738FB594A5CC3CE2F5506BD6964554FF5475CCAA6454F1FBB7`.
- Offline gates: recovery suite 317/317 (312 baseline + 5 new audit tests); package validation
  119/119 with zero syntax/API/callsite/error/warning findings; authority validator `ok=true`,
  `competingAuthorities=[]`; tooling SDK PASS after re-syncing the FableForge/ForgeFSE/FQT overlay
  and header mirrors (yesterday's export had skipped them; the validator reported "mirror drift").
- Generated `work/new_oakvale_test_bundle_20260911_v10/` (24 files, schema 0.2, override enabled in
  the output only, `Quests = {}`). Installed Lua tree was already byte-identical to v10.
- Deployed the v10 DLL to both the game root and `FSE\` after backing up both installed DLLs and the
  profile to `FSE\backups\new-oakvale-v10-20260911-160722\` with `deployment.json`. Before this the
  two installed DLLs were mismatched (root = v9 `BE0ECDF4…`, `FSE\` = `85256306…`). `gameProcesses=0`
  throughout; no launch occurred. Next gate is the user-driven single-authority run described in
  `docs/HANDOFF.md`.

## v10 live run, nested-control contract, and v11 (2026-09-11, evening)

- Launched v10 through `FSE_Launcher.exe`. Startup: `Armed identity-preserving retail override` and
  `Replacing retail allocator` both present; `NOVI_AUTHORITY activeName=Q_NewOakValeIntro native=true
  legacy=true`. The `legacy=true` comes from installed `data\Levels\FinalAlbion.qst` line 321
  `AddQuest("NewOakValeIntro", TRUE);` (also present in the 2026-09-10 05:50 backup). Removing it was
  attempted and blocked by the agent permission classifier; it is the user's next manual step.
- User report: talking to Affair Wife never produced dialogue and stuck the transition (game alive).
  Log (archived `interactive-20260911-v10-wife-nested-acquire-hang.log`, sha256 `99940a7a…`): her
  conversation path logged `Suspended live control handle for nested priority`, then
  `Acquiring script control for entity at priority 4...` with no success line for the rest of the run.
  Separately, after Affair Man's `AFFAIRMAN_GOOD_LAD` speak and release, `PlayAnimation_NonBlocking`
  and then 359 per-frame `MoveToPosition_NonBlocking - No control handle for requested entity` errors
  followed: his Main and conversation acquires are both priority 4, so the conversation reused the
  Main handle and its release destroyed it. All 10 handle initializations logged
  `Helper1 returned false, skipped Helper2`, in successful and failed acquires alike.
- Interpretation (runtime-evidenced, engine internals not yet decompiled): retail nests a second
  StartScriptingEntity resource inside one script owner; a Forge handle carries no owner identity, so
  a second resource for an actor this VM already controls is treated as foreign and never granted
  while the outer one is live. The observable retail contract (outer resource survives nested release)
  is reproduced by depth-counted reuse of the live handle.
- v11 `LuaEntityAPI`: removed the suspend/restore stack; added `m_ownedControlDepth`. Nested acquire
  over an owned handle logs `Reusing live control handle for nested acquire (depth N, requested
  priority P)` and returns true; nested release logs `Nested release: outer control handle retained`
  and returns without destroying; outermost release destroys and clears depth; fresh acquire resets
  depth to 0. Borrowing of a foreign VM handle is unchanged and only attempted when this VM owns
  nothing for the actor.
- `audit_forgefse_control_abi.py` schema 0.3: `nestedAcquireReusesOwnedHandle` (and no
  `m_suspendedControlHandles` anywhere), `nestedAcquireSkipsSelfBorrow`, `freshAcquireResetsDepth`,
  `nestedReleaseRetainsOuterHandle`, `outermostReleaseClearsDepth`; 6/6 tests; real tree ok.
- Rebuilt Release|x86; DLL `BCE143CCB0B45FD96B47F4632278D95810F70D48B895E8747454B1AE697AA73C`. Bundle `work/new_oakvale_test_bundle_20260911_v11/`
  generated and its DLL deployed to both locations after backing up to `FSE\backups\new-oakvale-v11-20260911-162005\`.
  Gates: 316/316 suite, 119/119 fixtures, authority ok, SDK PASS. Game closed; no relaunch by agent.
- User granted permission; deleted `AddQuest("NewOakValeIntro", TRUE);` from the installed
  `FinalAlbion.qst` (22,916 -> 22,880 bytes; backup `FSEackups
ew-oakvale-v11-qst-20260911-162235\`).
  Launched the v11 run through `FSE_Launcher.exe` at 16:22.
- v11 live run: startup `Armed`/`Replacing` present, `NOVI_AUTHORITY ... native=true legacy=false`.
  Zero `!!! ERROR` lines through 12,500+ log lines. Affair Wife conversation played
  (`TEXT_QST_048_AFFAIR_WIFE_LAYABOUT`, `_THANKYOU`); her priority-4 acquire logged
  `Reusing live control handle for nested acquire (depth 1, requested priority 4)` and the matching
  `Nested release: outer control handle retained (depth 0)`. One Guard lecture. `CS_OAKVALE_INTRO_FATHER`,
  `CS_OAKVALEINTRO_BULLYRUN1`, `CS_OAKVALEINTRO_BULLYRUN2` all reached `RunCutsceneABI`.
- v11 defect: three entities (priorities 4, 4, 3; Bully/Victim/Barrel Man family) logged a re-acquire
  every frame and depth climbed past 490. Their Lua Main loops call `NOVI.acquire` at the top of each
  iteration, mirroring retail's per-iteration `StartScriptingEntity` on the same resource object,
  which the engine treats as idempotent. Naive depth counting cannot distinguish that from a nested
  distinct resource.
- Prior-log check: `No control handle` spam exists in `childhood-to-guild-success-20260909.log` (620),
  `instrumented-playthrough-20260909-193731.log` (184), `dual-authority-regression-20260910.log` (99) and
  `playthrough-20260909-190934.log` (26), so the Affair Man handle loss predates today's Forge changes
  and is a Lua-side gap (his Main and conversation both acquire priority 4).
- v12 rule: same-priority re-acquire over an owned handle is idempotent (v9 behavior); only a priority
  change counts a nested level and reuses the live handle; outermost release destroys. Audit schema 0.4
  adds `samePriorityReacquireIsIdempotent` (7/7 tests). Rebuilt; DLL `4E34EBD532DFEC497A40E849689F754323455F382B80367F65F2E664460D8A31`; bundle
  `work/new_oakvale_test_bundle_20260911_v12/`. Suite 317/317. The v11 run was still in progress, so
  the v12 DLL was not yet installed at the time of this entry.
- v11 playthrough completed by the user through the whole childhood: "everything felt great". Final
  log 13,534 lines, zero `!!! ERROR`, zero `No control handle` (Affair Man's walk-home worked because
  the conversation release retained his Main handle). Archived
  `interactive-20260911-v11-single-authority-clean.log` (sha256 `920d670c…`). 10,597 re-acquire lines
  (4,249 at priority 3, 6,348 at priority 4) confirm the per-frame Main-loop pattern.
- v12 (plain same-priority idempotence) was NOT installed: it would restore the v9 handle loss for
  Affair Man. v13 saturates same-priority re-acquire at depth 1 instead; `audit_forgefse_control_abi.py`
  0.4 check `samePriorityReacquireSaturates`; 7/7 tests; DLL `2442A02C7AACF6468386F86608A6FF9FFDCD6D0A434D46DD256B777268842815`; bundle v13 generated and installed
  to both locations with the Lua tree; backup `new-oakvale-v13-20260911-163519`.
- User report: Bully "ran a few units, then teleported forward substantially, then ran out of view".
  Decoded `CS_OAKVALEINTRO_BULLYRUN1` (35 commands) and `CS_OAKVALEINTRO_BULLYRUN2` (5 commands) from
  `script.bin`. RUN1: `BULLY.RunTo MK_OIBR_BULLY2,0,FALSE,FALSE,FALSE,TRUE` (CORRECTED below: retail's interpreter WAITS on this command), followed by
  ~1.7 s of pauses, `BRAT.Speak $BRATLINE`, `HERO.Teleport MK_OIBR_HERO2`, `BULLY.Drawable FALSE`. Retail
  hides him wherever the run got to. Probe: before (3606.75,861.48) -> after macro (3604.48,860.74),
  2.3 m; compatibility completion then re-showed him and moved 8.4 m to (3598.03,853.02) in 40 frames.
  No wrapper touched the Bully handle during the macro (only other entities' re-acquires). The jump is
  the compatibility completion; the underlying stall of the in-macro RunTo is the open RE item.
- User report: Bully health bar colours wrong. Disassembled `0x00DBC400-0x00DBC44C` from the installed
  retail exe (capstone): `[esp+0x88..0x8b] = 00 00 FF FF` and `[esp+0x64..0x67] = 00 FF 00 FF`. With
  the PDB `CRGBColour` B,G,R,A layout that is empty=red, filled=green, matching the barrel timer's
  `0xffff0000`/`0xff00ff00` words. Corrected `NOVI_Bully.lua` (`INFO_BAR_EMPTY_COLOUR` red), the
  `NOVI_Bully.json` inventory wording, `export_bully_health_bar.py`, and the checked-in snapshot
  (`secondaryColourBytes` had been transposed to `FF 00 00 FF / opaque blue`); regenerated three Bully
  traces. Gates: 119/119 fixtures, 317/317 suite.

## Native-to-Lua lifter, first cut (2026-09-11, evening)

- User asked how to make porting less painful and to start on a converter. Built
  `tools/script_recovery/lift_native_lua.py` on top of the existing assets only: native clusters
  (decompiles), `annotate_interface_slots.py` (GSI slot names), the FSE manifest (scope, parameter
  types, return types), the New Oakvale translation unit (entity functions), and the retail exe for
  `.rdata` strings. No new Ghidra work was needed (the Ghidra MCP was down all session).
- Recognised shapes: entity-binding boilerplate, string temporaries recovered in push order and
  placed by manifest parameter type, `Transfer<T>` persistence naming the state offsets, field
  writes (unnamed offsets become `self_0xNN` + TODO, never guessed), the frame/terminate idiom
  (including the bsim-mislabelled entity `IsActiveThreadTerminating` at `0x00F35B30`, keyed by call
  address), `StartScriptingEntity` retry loops collapsed to `me:AcquireControl(p)`, controlled-entity
  resource methods to `me:Method`, `CScriptThing::Method` to `thing:Method`, thread spawns to
  `Quest:CreateThread("Name")` with the body symbol recorded, structured control flow, `if-goto` as
  early return with a verify TODO.
- `benchmark_lifter.py` + `refs/script_recovery/lifted/LIFT_BENCHMARK.md`: MazeResearch and
  MeetSister lift with zero TODOs at recall 1.0; ScytheInfo/Fisherman/HerosOldHouse 0.81-0.87; the
  big thread-heavy scripts (SummoningTheShip, DragonBossFight) stay at ~0.1 because thread bodies are
  not lifted yet. Entity mode: CreatedBeetle recall 1.0 / zero TODOs, DeadFather 0.75, Villager 0.74
  with 118 TODOs dominated by thing-vtable message calls that the slot annotator does not cover.
- `test_lift_native_lua.py` 10/10; full suite 327/327. Design notes and next levers in
  `docs/scripts/LIFTER.md`.

## v13 verification run and Bully correction (2026-09-11, evening)

- v13 run (user): health bar colours confirmed fixed; zero `!!! ERROR`; per-frame re-acquire depth
  stayed at 1 (saturation works); 4 nested releases. Archived
  `runtime_evidence/interactive-20260911-v13-bully-bar-red.log`.
- User report: Bully ran off properly this time but vanished while the camera still framed him, before
  the camera changed target. Log: RUN1 macro ended with `inCutscene=0`, native RunTo again stopped at
  (3604.48,860.74) = 2.3 m; compatibility move covered the rest in 39 frames on the RUN1 tail camera;
  `Drawable FALSE` then applied in frame; RUN2 switched to `CAM_OIBR_BRATHERO` afterwards.
- CORRECTION of my earlier note: `bully-run-native-wait-analysis-20260910.md` proves retail's `.RunTo`
  branch (RunCutsceneMacro_Func 0x00CBFB7D) polls IsPerformingScriptTask (slot 0x68) until the task
  ends; the command is not non-blocking. Retail thus hides him at the marker, off the BRAT camera.
- Tried deferring the compatibility hide until after RUN2's camera cut; it reorders retail's sequence,
  failed one fixture (118/119), and was reverted from source and install (backup
  `new-oakvale-v14-20260911-174451`; bundle v14 discarded). Install is back to v13; 119/119, 344/344.
- The deterministic endpoint (11.759 m short in 4/5 runs; one run reached the marker) is the real
  defect and is under a workflow investigation (geometry at the stop point, native task semantics of
  the RunTo operands, and log differences of the successful run).

