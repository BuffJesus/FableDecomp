#!/usr/bin/env python3
"""Execute a built shadow package with semantics matching RetailScriptShadowRunner."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

try:
    from lupa import LuaError, LuaRuntime
except ImportError:  # pragma: no cover
    LuaError = RuntimeError
    LuaRuntime = None


HARNESS = r"""
TRACE = {}
local function make_proxy(scope, receiver)
    local object = { __shadow_scope = scope, __shadow_receiver = receiver }
    return setmetatable(object, {
        __index = function(_, name)
            return function(_, ...)
                table.insert(TRACE, { scope=scope, receiver=receiver, operation=name, argc=select('#', ...) })
                if name == 'NewScriptFrame' then return false end
                if name == 'IsRegionLoaded' or name == 'IsLevelLoaded' or
                   name == 'GetStateBool' or name == 'IsThingWithThisUIDAlive' then return false end
                if name == 'GetActiveQuestName' then return SHADOW_NAME end
                if name == 'RegisterTimer' or name == 'StartAmbientConversation' then return 1 end
                if name == 'GetTimer' then return 1 end
                if name == 'GetAngleXY' or name == 'GetHealth' or
                   name == 'GetDistanceBetweenThings' or name == 'GetGameTime' or
                   string.sub(name, 1, 6) == 'GetNum' then return 0 end
                if name == 'GiveHeroYesNoQuestion' then return 0 end
                if name == 'GetHero' or name == 'GetThingWithScriptName' or
                   name == 'GetThingWithUID' then return make_proxy('Entity', name) end
                return nil
            end
        end
    })
end
function RunLifecycle(function_name, kind, instruction_budget)
    local target = _G[function_name]
    if type(target) ~= 'function' then return true end
    local instructions = 0
    debug.sethook(function()
        instructions = instructions + 1000
        if instructions > instruction_budget then error('shadow instruction budget exceeded') end
    end, '', 1000)
    local quest = make_proxy('Quest', 'quest')
    local ok, message
    if kind == 'entity' then ok, message = pcall(target, quest, make_proxy('Entity', 'me'))
    else ok, message = pcall(target, quest) end
    debug.sethook()
    if not ok then error(message) end
    return true
end
"""


def run(package: Path) -> dict[str, object]:
    if LuaRuntime is None:
        raise RuntimeError("run_shadow_preflight requires lupa")
    config_runtime = LuaRuntime(unpack_returned_tuples=True)
    config_runtime.execute((package / "FSE" / "retail_shadow.lua").read_text(encoding="utf-8-sig"))
    entries = config_runtime.globals().RetailShadow["entries"]
    results = []
    for index in range(1, len(entries) + 1):
        entry = entries[index]
        name, file, kind = entry["nativeName"], entry["file"], entry["kind"]
        source = package / "FSE" / f"{file}.lua"
        runtime = LuaRuntime(unpack_returned_tuples=True)
        runtime.execute(HARNESS)
        runtime.globals().SHADOW_NAME = name
        try:
            runtime.execute(source.read_text(encoding="utf-8-sig"))
            runtime.globals().RunLifecycle("Init", kind, 100_000)
            runtime.globals().RunLifecycle("Main", kind, 100_000)
            results.append({"nativeName": name, "passed": True})
        except (LuaError, OSError) as error:
            results.append({"nativeName": name, "passed": False, "error": str(error)})
    failed = [row for row in results if not row["passed"]]
    return {"schema": "forgefse-retail-shadow-offline-preflight/0.1",
            "complete": not failed, "passed": len(results) - len(failed),
            "failed": len(failed), "scripts": results}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("package", type=Path)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = run(args.package.resolve())
    if args.output:
        args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({key: result[key] for key in ("complete", "passed", "failed")}, sort_keys=True))
    if result["failed"]:
        for row in result["scripts"]:
            if not row["passed"]:
                print(f"FAIL {row['nativeName']}: {row['error']}")
    return 0 if result["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
