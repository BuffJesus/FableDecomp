# Retail fade state and v22 diagnostic boundaries

Byte evidence: `cutscene-fade-retail-bytes-20260912.json`, including installed
Fable.exe hash. These are retail addresses, relocated through ASLR at runtime.

- Interpreter global GSI pointer: `0x0143E8F8`. GSI +0xC is CDisplayEngine.
- Vtable +0x5DC (`0x0088E4D0`) returns display +0xBC AND +0xC9.
  Interpreter FadeOut checks this before dispatch unless `override` is supplied.
- Vtable +0x5D0 (`0x008907E0`) calls +0x5D4 with opaque black. Only if the
  latter returns true does it set display +0xD8 to hold until FadeIn.
- +0x5D4 (`0x00890820`) refuses an active fade (+0xBC) when GSI +0x2D is false.
  Otherwise it calls `0x006E7370`, forwarding to display StartScreenFadeOut
  (`0x00434C00`), and returns true.
- StartScreenFadeOut sets +0xBC active, +0xC9 fading out, +0xC8 not fading in,
  +0xC0 out duration, +0xC4 in duration, +0xCC out elapsed=0, +0xD0 in remaining,
  and +0xD4 colour. Existing +0xD8 instead clears +0xE8 and returns early.
- FadeIn (`0x0088E4C0`) clears +0xD8 and calls EndScreenFadeOut (`0x00434C90`),
  which sets +0xC9 false and +0xC8 true when the fade is active.
- DoRender at `0x00435840` uses +0xE8 to select zero or frame delta before
  updating the fade object at display +0xBC. Update (`0x00434870`) advances
  +0xCC toward +0xC0 while fading out; fading in reduces +0xD0.
- GetFadeInOutAlpha (`0x004348D0`) uses elapsed/duration or remaining/duration
  according to direction. Snapshot fields describe engine state, not a captured
  rendered frame; do not call an incomplete fade a proven presentation cause
  without the actual test data.

v22 adds ReadProcessMemory snapshots of those existing fields before interpreter
commands using the already-installed FindCharPos hook and proven caller
0x00CC023C. No new native entry point is patched. Unreadable pointers are logged
and not dereferenced. No game method is called to sample the fade or skip state.
The original FindCharPos and skip query still execute exactly once per call.
Command and true skip-query logging now include original native macros outside
Lua brackets (`macro=<none>`); actor/movement hooks retain their previous scope.
Global Lua macro labels are diagnostic brackets, not proof of thread identity.

Native Bully call at 0x00DBCB77 pushes 1,0,input-map,0 before RunCutsceneMacro,
confirming skippable=true and setup=false. This agrees with the wrapper; no
different skip operand or missing setup has been established.

v21 visual skip test failed. v22 is an instrumented build, not a behavior fix.
