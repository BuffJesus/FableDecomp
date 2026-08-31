#!/usr/bin/env python3
"""Execute a recovered ForgeFSE Lua lifecycle function against deterministic mocks."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
from typing import Any

try:
    from lupa import LuaError, LuaRuntime
except ImportError:  # pragma: no cover - exercised by the CLI dependency error
    LuaError = RuntimeError
    LuaRuntime = None


HARNESS = r"""
TRACE = {}
FIXTURES = {}
FIXTURE_POS = {}

local function normalize(value, seen)
    local kind = type(value)
    if kind == "nil" or kind == "boolean" or kind == "number" or kind == "string" then
        return value
    end
    if kind ~= "table" then return "<" .. kind .. ">" end
    if value.__trace_handle then
        return { handle = value.__trace_handle, scope = value.__trace_scope }
    end
    seen = seen or {}
    if seen[value] then return "<cycle>" end
    seen[value] = true
    local result = {}
    for key, child in pairs(value) do
        if string.sub(tostring(key), 1, 8) ~= "__trace_" then
            result[key] = normalize(child, seen)
        end
    end
    seen[value] = nil
    return result
end

local make_proxy
make_proxy = function(scope, handle)
    local object = { __trace_scope = scope, __trace_handle = handle }
    return setmetatable(object, {
        __index = function(self, name)
            return function(_, ...)
                local key = scope .. "." .. name
                table.insert(TRACE, {
                    event = "api-call", scope = scope, receiver = handle,
                    name = name, arguments = normalize({...})
                })
                local sequence = FIXTURES[key]
                if sequence == nil or #sequence == 0 then return nil end
                local position = (FIXTURE_POS[key] or 0) + 1
                FIXTURE_POS[key] = position
                local result = sequence[math.min(position, #sequence)]
                if type(result) == "table" and result.handle then
                    return make_proxy(result.scope or "Entity", result.handle)
                end
                return result
            end
        end
    })
end

function ConfigureFixtures(fixtures)
    FIXTURES = fixtures
    FIXTURE_POS = {}
end

function RunLifecycle(function_name, kind, instruction_budget)
    local target = _G[function_name]
    if type(target) ~= "function" then error("missing lifecycle function " .. function_name) end
    local ticks = 0
    debug.sethook(function()
        ticks = ticks + 1000
        if ticks > instruction_budget then error("instruction budget exceeded") end
    end, "", 1000)
    local quest = make_proxy("Quest", "quest")
    local ok, message
    if kind == "entity" then
        ok, message = pcall(target, quest, make_proxy("Entity", "me"))
    else
        ok, message = pcall(target, quest)
    end
    debug.sethook()
    if not ok then error(message) end
    return TRACE
end
"""


def _lua_table(runtime: Any, value: Any) -> Any:
    if isinstance(value, dict):
        table = runtime.table()
        for key, child in value.items():
            table[key] = _lua_table(runtime, child)
        return table
    if isinstance(value, list):
        table = runtime.table()
        for index, child in enumerate(value, 1):
            table[index] = _lua_table(runtime, child)
        return table
    return value


def _python_value(value: Any) -> Any:
    if hasattr(value, "items"):
        pairs = list(value.items())
        if pairs and all(isinstance(key, int) and key >= 1 for key, _ in pairs):
            keys = sorted(key for key, _ in pairs)
            if keys == list(range(1, len(keys) + 1)):
                return [_python_value(value[index]) for index in keys]
        return {str(key): _python_value(child) for key, child in pairs}
    return value


def run_trace(source: Path, function: str, kind: str, fixtures: dict[str, list[Any]],
              instruction_budget: int = 100_000) -> dict[str, Any]:
    if LuaRuntime is None:
        raise RuntimeError("run_lua_trace requires the Python package 'lupa'")
    runtime = LuaRuntime(unpack_returned_tuples=True)
    runtime.execute(HARNESS)
    runtime.globals().ConfigureFixtures(_lua_table(runtime, fixtures))
    source_bytes = source.read_bytes()
    runtime.execute(source_bytes.decode("utf-8-sig"))
    try:
        trace = runtime.globals().RunLifecycle(function, kind, instruction_budget)
    except LuaError as exc:
        raise RuntimeError(f"Lua trace failed for {source}:{function}: {exc}") from exc
    events = _python_value(trace)
    for event in events:
        if event.get("arguments") == {}:
            event["arguments"] = []
    return {
        "schema": "forgefse-lua-event-trace/0.1",
        "source": source.as_posix(),
        "sourceSha256": hashlib.sha256(source_bytes).hexdigest(),
        "function": function,
        "kind": kind,
        "fixtures": fixtures,
        "events": events,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("--function", default="Main")
    parser.add_argument("--kind", choices=("quest", "entity", "village", "global"), required=True)
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--instruction-budget", type=int, default=100_000)
    args = parser.parse_args()
    fixtures = json.loads(args.fixtures.read_text(encoding="utf-8-sig")) if args.fixtures else {}
    payload = run_trace(args.source, args.function, args.kind, fixtures, args.instruction_budget)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(json.dumps({"events": len(payload["events"]), "output": str(args.output.resolve())}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
