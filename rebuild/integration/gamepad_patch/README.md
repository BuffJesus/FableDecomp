# Gamepad-redefine patch package

Two halves of the retail gamepad-redefine patch (design: `docs/GAMEPAD_REDEFINE_PATCH.md`).
VERIFIED in retail: the 5th Options row "Redefine Keys (Gamepad)" renders in-game.

## DATA half  (VERIFIED working)
`tools/build_gamepad_redefine_data.py <out> [action=284]` appends to frontend.bin/names.bin
(clone options row #344->gi810 Action 284; clone screen #238->gi811; clone the label chain
#345->gi813 / #346->gi812 with CWideString "Redefine Keys (Gamepad)"; add gi810 to options
list #219 Children). Round-trip + full-parse self-validated. Deploy the two files to
`data/CompiledDefs/` (back up first). The engine renders the literal label (no text.big edit).

## CODE half  `gamepad_redefine_hook.c`  (boot-safe; Action routing pending live click-test)
Single boot-safe inline hook on `CFrontEndManager::Action @0x59A238`: action 284 resolves
`UI_FRONTEND_SCREEN_REDEFINE_KEYS_GAMEPAD` by name and GotoNextScreen to it (Init2 is NOT
patched -> boot is untouched). Capture needs no hook (Redefine is device-agnostic).

Build (VS2022 x86 + Win SDK):
  cl /O2 /MT /LD /Gz gamepad_redefine_hook.c /link /OUT:gamepad_redefine.dll kernel32.lib user32.lib
Deploy: copy the DLL to `<install>\Mods\gamepad_redefine.dll` and add `gamepad_redefine.dll=1`
to `Mods.ini`. Inject by launching via `FSE_Launcher.exe` (reads Mods.ini, LoadLibraryA into
the game). Revert: remove the Mods.ini line.

Binding authority: `gamepad_binding_table.md` (proven XBOX_PAD_* values).
Enums: `ghidra_out/controller_enums_proven.md`.
