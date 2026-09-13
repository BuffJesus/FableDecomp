# Q_NewOakValeIntro ForgeFSE API-gap ledger

Updated 2026-09-10. `api_requirements.json` is the generated, call-site-complete authority.
Keep missing functions separate from reduced signatures and host-managed behavior.

## Retail operations without a one-to-one public Lua method

- `IsActiveThreadTerminating`: a query that does not advance a frame.
- `IsDistanceFromThingToPositionOver(thing, C3DVector, distance)` (covered by the direct retail
  `IsDistanceFromPositionOver` helper binding used by reconstructed Lua).
- `MsgIsPresentedWithItem(CCharString&)` (covered by Forge's fresh native presented-item query).
- A conversation-line overload that accepts a null `CScriptThing` listener (the active wrapper
  accepts Lua `nil` and constructs the null listener operand).
- Explicit nested `StartScriptingEntity` acquisition/release with retail AI priority (covered by
  entity `AcquireControl`/`ReleaseControl`, including retry and termination cleanup).
- Entity `OnPredicateFail` (native vtable slot 5; dispatched one-shot by `LuaEntityHost`).
- Direct `RunCutsceneMacro_Func` with actor and flag tables (covered for this package by the
  ownership-aware `RunCutsceneWithSetup` adapter).
- Quest `std::vector<CCharString>` access for the eight `VillagerSpeech*` tables (exact recovered
  contents are mirrored as immutable Lua data).

## Existing bindings with reduced signatures or semantics

- `MoveToPosition` converts the Lua position table to the native vector; its radius, move type,
  and two trailing booleans are exposed and forwarded.
- `FadeScreenOut` can include a second float and explicit colour in retail.
- `EntitySetAsKillable`, `SetThingHasInformation`, `EntitySetFacingAngleTowardsThing`,
  `EntityTeleportToThing`, `RemoveThing`, and `GiveHeroYesNoQuestion` now expose the retail boolean
  operands used by this package.
- The active nonblocking `PlayAnimation`, `PlayLoopingAnimation`, and `PlayCombatAnimation`
  bindings expose and forward their native flag sets in order. Dead Father's seven looping flags
  are therefore representable; the older blocking convenience wrappers still use reduced,
  semantic option names but are not selected by these entity methods.
- Speak/task waits interleave task checks, frame advancement, and termination checks.
- `EndMovieSequence` invokes the exact retail derived movie-resource destructor at `0x006E7B80`
  on the live per-VM handle, matching explicit native scope cleanup; host unwinding covers interruption.

## Host/runtime boundaries

- ForgeFSE owns primary entity `AcquireControl`/`ReleaseControl`; priority, retry/yield,
  borrowed-handle ownership, and termination cleanup are now statically and fixture-audited.
- `CTCVillage::OnInitialActivate` is engine lifecycle bookkeeping.
- Random branches use Forge's `RetailRandModulo`, which calls the relocated retail MSVCR71
  `rand` import thunk at `0x00BFEB16`; no package code uses Lua or plugin-CRT randomness.

Validation reports no executable unresolved-call placeholders. The registered `AcquireControl` and
`ReleaseControl` entity methods cover the explicit nested scheduler path, including caller priority,
retry/yield, termination cleanup, and borrowed-handle ownership. The generated manifest now filters
inventory annotations from callable names. Retail's adjacent bare termination reads are exactly folded
into the preceding frame result, while `CTCVillage::OnInitialActivate` is already represented by its
host-managed lifecycle operation. Neither is an API gap, so the generated inventory contains only
direct or host-managed requirements and zero blockers.

The inventory also names the registered Lua method actually called. Local `speak_if_alive` helpers
invoke `me:Speak`, whose registration routes to termination-aware `Speak_Blocking`; they never invoke
the older `SpeakAndWait` convenience method. Normalizing those conceptual aliases leaves 132 actual
requirements (131 direct call names and one host-managed category) rather than counting an unused
registered method.
