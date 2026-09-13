# Staged port persistence contract

Maze Research's actual staged OnPersist entrypoint matches the native field
transfer contract. No persistence fix was needed. The runtime check now builds
a seventh C++/Lua binary to exercise this path rather than only mocking the
Lua adapter's persistence operation.

| Port | Native method | Transferred fields |
| --- | --- | --- |
| V_MazeResearch | EA80A0 | SwordTaken at quest+48, then BookRead at quest+49; one byte each, default false |
| QS_MeetSister | CBD4E0 | None; immediate ret 4 |

Both native Maze calls target 4045C0. Forge's CPersistContext_Transfer_bool_API
targets that same function. The decompiler's signed-char template label does
not imply a different target or a wider field. The current runtime wrapper
passes a temporary bool by address and copies the modified value back as its
return value. The staged Lua stores that returned value for each named field.

`native-port-persistence-20260912.json` contains 26 executed native cases:
24 Maze cases and two empty Meet Sister calls. Maze covers all four initial
boolean pairs, writing them, loading each of four stored pairs, and loading
with both fields missing. Field transfer/storage is doubled; the real native
OnPersist branches, argument addresses, literal names and stack cleanup run.

The new port-persistence.cpp binary compiles the current PersistTransferBool
body and its Lua registration extracted verbatim. It loads the actual staged
MazeResearch.lua, quest.lua and runtime_adapter.lua, then compares all 24 Maze
cases with native outcomes, including order, defaults, returned state and
stored values. The engine transfer and Quest state storage are doubles.
The other six runtime binaries still pass.

Reproduce:

```powershell
python work/runtime-20260912-video-comparison/prove_port_persistence.py
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/runtime-port-persistence-NEW
```

Current passing output: work/runtime-port-persistence-20260912/result.json.

## Lifecycle boundaries not certified by these tests

Maze's native OnPersist does not serialize the retained GoodSword wrapper or
UNLIMBO map. The Lua port likewise transfers only SwordTaken/BookRead. Its
Quest-owned C++ maps/wrappers are transient; EmptyGrave.Init resets UNLIMBO and
the grave body looks up and retains GoodSword. Adding these transient objects
to OnPersist would go beyond the recovered native field contract.

LuaQuestHost creates an isolated VM and Quest state, loads the root script in
Init, and calls a registered OnPersist with the supplied context. Without a
root OnPersist, it logs absence and transfers no fields, consistent with Meet
Sister's empty native callback. This is not proof of the engine's complete
host reconstruction or Init/OnPersist ordering during a real save load.

Ordinary Quest fields use LuaManager's namespaced global-state map. That map
is cleared by LuaManager::Shutdown; the audited per-host methods do not make
it a serialized save graph. Do not infer successful restoration of script
threads, in-flight macros, actor control, entity activation or UNLIMBO timing
from passing field transfers. Those remain live save/load and streaming checks.

The ports remain disabled and installed v23 is unchanged.
