# Retail in-game cutscene skip path (2026-09-09)

Scope: animated `CCutsceneDef` macros such as `CS_OAKVALE_INTRO_FATHER`, not WMV playback.

## Proven call ABI

`RunCutsceneMacro_Func @ 0x00CBFB7D` is `__fastcall`: macro name in ECX, actor map in EDX,
then four stack arguments. Its epilogue is `ret 0x10`. With the function's unusual frame pointer
(`EBP = entry ESP - 0x6c`), the stack arguments are at `EBP+0x70`, `+0x74`, `+0x78`, and
`+0x7c`. Therefore `EBP+0x7c` is the sixth and final `skippable` argument.

At `0x00CBFCE1..0x00CBFCF4`, the executor pushes `[EBP+0x7c]`, then `1`, and invokes CGSI
vtable slot `0x604`, `CGameScriptInterface::SetCutsceneMode(bool,bool)`. The argument order is
therefore exactly:

```text
SetCutsceneMode(true, skippable)
```

ForgeFSE's declaration and invocation pass the same ABI and place the Lua `true` in this final
argument. The native New Oakvale father caller also supplies `true` there
(`0x00DB88DB..0x00DB88F8`). Argument reversal is not the remaining defect.

## Proven prompt/state initialization

`CGameScriptInterface::SetCutsceneMode @ 0x0089D600`:

1. calls `CSubtitleRenderer` method `0x0049319C` with the first argument (`inCutscene`);
2. writes its second argument (`skippable`) to player-GUI-manager byte `+0xF8`;
3. updates the player-control mode through CGSI vtable slot `0x640`.

`CSubtitleRenderer::SetCutsceneMode @ 0x0049319C`, when entering:

- writes renderer `+0xDE = 1` (manager `+0xF6`, in-cutscene);
- clears renderer `+0xDF` (manager `+0xF7`, skipped);
- starts the fade/timer at renderer `+0xE8` from `CPlayerGuiDef+0x2C4`, whose recovered PDB field
  is `TextBoxCutsceneSecondsBeforeSkip`;
- initializes both cutscene text controls through `0x0048F7C0` and `0x0048F881`. The latter is
  the routine anchored by `TEXT_GUI_SKIP_SCENE`.

Thus retail itself owns both the delayed prompt and input-mode transition. Forge should not draw a
replacement prompt or poll Escape in Lua.

## Proven input path

`CWorld::EASkipCutScene @ 0x00629F50` rejects input unless manager `+0xF6` is set and
`CSubtitleRenderer::IsCutsceneSkippable @ 0x0048DE44` succeeds. On success it posts the native
cutscene-skipped game event and sets renderer `+0xDF = 1` / `+0xE0 = 0`.

The macro interpreter polls `IsCutsceneSkipped @ 0x00CBEB7E`, which selects CGSI message query
slot `0xA8` or `0xB0` based on action-mode byte `0x0143E8F4`. Numerous verb wait loops terminate
through this query, so a native skip performs normal interpreter cleanup rather than aborting the
Lua thread.

## Runtime evidence and remaining boundary

Existing Forge logs show before and after each macro:

```text
actionMode=0 guiActionMode=0 inCutscene=0 skippable=1
skippableWhilePaused=0 alternateSkipInput=0
```

Those snapshots occur outside the blocking executor and cannot prove the transient state while it
runs. The next runtime probe must sample or intercept `SetCutsceneMode`, renderer timer/eligibility,
and `EASkipCutScene` during the macro. Specifically record manager bytes `+0xF6..+0xFA`, renderer
timer state at manager `+0x100`, and whether `EASkipCutScene` is dispatched after Escape. This will
separate: (a) prompt/timer never becoming eligible, (b) player input mode not dispatching the
action, and (c) the skipped event being posted but not consumed. No corrective write is justified
until that boundary is observed.

## Implemented diagnostic build

The first diagnostic build hooked four boundaries. Its renderer-eligibility hook at `0x0048DE44`
was implicated by the runtime crash at `0x0048E082`, so that build is rejected. The reduced build
installs only the passive `SetCutsceneMode @ 0x0089D600` skip diagnostic hook (5-byte,
instruction-aligned and byte-validated). It does **not** patch renderer eligibility,
`CWorld::EASkipCutScene`, or macro-side `IsCutsceneSkipped`.

All three Forge macro wrappers bracket only their blocking native executor call with the diagnostic
enable flag. The safe boundary records both arguments after retail handles them and reports
`inCutscene`, `skipped`, `skippable`, paused and alternate-input flags, the two prompt-control flags,
and timer-active byte. It modifies no retail state and synthesizes no input.

The ABI was re-audited after an experimental argument swap. Native father code pushes
`1, 0, 0, 0` at `0x00DB88EB..0x00DB88EF`; right-to-left x86 ordering makes the callee slots
`flagMap=0, inputMap=0, setupCondition=0, skippable=1`. The executor reads the last slot
(`[EBP+0x7c]`) for `SetCutsceneMode`'s second argument. Therefore the exact declaration remains
`(..., flagMap, inputMap, setupCondition, skippable)`. The experimental swap was wrong and has
been reverted.

After the reduced live probe still reported `SetCutsceneMode(inCutscene=1, second=0)`, the ABI was
checked a third time directly against both sides of the call. `RunCutsceneMacro` ends in `ret 0x10`
(exactly four stack arguments), and its frame construction makes `EBP+0x70/+0x74/+0x78/+0x7C`
those four slots. The father caller's pushes make them `0,0,0,1`, respectively. There is therefore
no evidence supporting another native-parameter swap.

The next build logs the Sol-resolved option values immediately before the native call as
`[RunCutsceneABI] setupOptional=... setup=... skippableOptional=... skippable=...`. This isolates
whether Lua's explicit `nil,false,true,false` reaches C++ as intended. If that line reports
`skippable=1` while the safe `SetCutsceneMode` hook still reports `second=0`, the remaining boundary
is the compiled indirect call itself; an explicit audited x86 call thunk is then justified. If it
reports `skippable=0`, the defect is in argument conversion/binding before the native ABI.

Release x86 build: succeeded with 0 warnings and 0 errors on 2026-09-09. The DLL was intentionally
not deployed while the existing game process was running.

## Runtime closure (2026-09-10)

The failure was in ForgeFSE's direct Sol member binding, not the retail ABI or input path. With the
Lua call shape `(actors, nil, false, true, false)`, the old binding logged
`skippableOptional=1 skippable=0` and retail consequently received
`SetCutsceneMode(inCutscene=1, second=0)`.

ForgeFSE now gives that exact `nil, bool, bool, bool` shape an explicit typed overload before the
general member binding. Build SHA-256
`9D2222DEDF6C5968CE7FB1FD192F75CBE66570403CA064F71936DE2DBEF9B497` logged
`setupOptional=1 setup=0 skippableOptional=1 skippable=1`, followed by
`SetCutsceneMode(inCutscene=1, second=1)`. The user then successfully skipped
`CS_OAKVALE_INTRO_FATHER` in game. Archived log:
`father-cutscene-skip-success-20260910.log` (SHA-256
`56C04A04352EA6509141A057EBB2969FF8A91AEAC86945448D8D0AD1C416E5A8`).
