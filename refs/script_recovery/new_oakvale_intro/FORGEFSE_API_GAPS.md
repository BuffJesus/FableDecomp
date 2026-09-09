# Q_NewOakValeIntro ForgeFSE API-gap ledger

Updated 2026-09-08. `api_requirements.json` is the generated, call-site-complete authority.
Keep missing functions separate from reduced signatures and host-managed behavior.

## Missing callable operations

- `IsActiveThreadTerminating`: a query that does not advance a frame.
- `IsDistanceFromThingToPositionOver(thing, C3DVector, distance)`.
- `MsgIsPresentedWithItem(CCharString&)`.
- A conversation-line overload that accepts a null `CScriptThing` listener.
- Explicit nested `StartScriptingEntity` acquisition/release with retail AI priority.
- Entity `OnPredicateFail` (native vtable slot 5).
- Direct `RunCutsceneMacro_Func` with actor and flag tables.
- Quest `std::vector<CCharString>` access for the eight `VillagerSpeech*` tables.

## Existing bindings with reduced signatures or semantics

- `EntitySetAsKillable`, `SetThingHasInformation`, `EntitySetFacingAngleTowardsThing`,
  `EntityTeleportToThing`, and `RemoveThing` omit retail boolean operands.
- `MoveToPosition` omits or converts radius, move type, and boolean arguments.
- `FadeScreenOut` can include a second float and explicit colour in retail.
- `GiveHeroYesNoQuestion` has additional retail operands.
- `PlayAnimation`, `PlayLoopingAnimation`, and `PlayCombatAnimation` have larger native flag sets.
- Speak/task waits interleave task checks, frame advancement, and termination checks.
- Explicit `EndMovieSequence` is not equivalent to retail destructor cleanup on interruption.

## Host/runtime boundaries

- ForgeFSE owns primary entity `AcquireControl`/`ReleaseControl`; nested-resource priority and
  lifetime behavior still need runtime verification.
- `CTCVillage::OnInitialActivate` is engine lifecycle bookkeeping.
- Lua randomness does not prove exact CRT `rand() % 100` behavior.

Validation currently reports two executable scheduler fallbacks and 59 blocked semantic/signature
requirements. The low executable count must not be mistaken for API parity.
ndMovieSequence` matches normal cleanup but is not RAII-equivalent on interruption.

## Host/runtime boundaries

- ForgeFSE owns primary entity `AcquireControl`/`ReleaseControl`; nested resources and exact
  priority/lifetime behavior still need runtime verification.
- `CTCVillage::OnInitialActivate` is engine lifecycle bookkeeping.
- Lua random generation does not prove exact CRT `rand() % 100` behavior.

Validation currently reports two executable `NOVI.unsupported` scheduler fallbacks and 59 blocked
semantic/signature requirements. The low executable count must not be mistaken for API parity.
