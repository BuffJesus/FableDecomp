# Gamepad-redefine patch package

The two halves of the retail gamepad-redefine patch (design: `docs/GAMEPAD_REDEFINE_PATCH.md`).

- **DATA**: built by `tools/build_gamepad_redefine_data.py` (clones options row #344
  Action 283→284 + redefine screen #238 → `..._GAMEPAD`, adds the 5th row to options
  list #219). Output = modified `frontend.bin`+`names.bin` (written to a scratch dir,
  full-parse self-validated; never the install).
- **CODE**: `gamepad_redefine_hook.cpp` — the 2-hook reference impl (Action 284 → key
  0x17; Init2 post-hook binds 0x17 → the gamepad screen by name). UNTESTED; needs a
  MinHook-class lib + live validation + an XInput→engine bridge (capture itself needs
  no hook — `CKeyRedefiner::Redefine` 0x557D20 is device-agnostic).
- Binding authority: `rebuild/integration/gamepad_binding_table.md` (proven
  XBOX_PAD_* values). Enums: `ghidra_out/controller_enums_proven.md`.
