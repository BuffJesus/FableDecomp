"""Run a converter package's Lua against a mocked FSE runtime and report what breaks.

    python tools/script_recovery/smoke_run_unit.py --unit orchard_farm [--stage draft|readable] [--frames 200]
    python tools/script_recovery/smoke_run_unit.py --package-dir refs/script_recovery/authored/OakvaleReborn/FSE

`--package-dir` smokes any FSE tree (an authored package rather than a converter unit). Module files
that `return` a table (e.g. `local M = {}; function M.Routine(quest) ... end; return M`) are exercised
too: every function field is called with the mock quest first and mock `resources` for a parameter
of that name.

The mock `quest` / `me` / `resources` objects answer every method with a plausible default (numbers 0,
booleans false, strings "", things → mock thing) and record every call. `NewScriptFrame` returns true
for --frames calls, then `IsActiveThreadTerminating` becomes true and `NewScriptFrame` returns false, so
retail-shaped loops unwind. A Lua instruction-count hook aborts loops that never yield a frame.

Findings per function: Lua errors (attempt to index nil, bad argument…), non-yielding loops, methods
that are not registered in the sidecar DLL, and the call trace of the first frames. This is not a
behavioural oracle — it only proves the code runs under the API shapes the DLL exposes.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402

DLL_SOURCES = [ROOT / 'work/new-oakvale-original-fse-20260912/sidecar-abi-v2/FableScriptExtender' / n
               for n in ('LuaManager.cpp', 'NoviUnitBindings.h', 'LuaRetailResources.h')]

HARNESS = r'''
local calls, frames, budget = {}, 0, FRAME_BUDGET
local terminating = false
local function record(recv, name, ...)
    calls[#calls + 1] = recv .. ":" .. name
    if #calls > 20000 then error("call trace overflow (loop without frames?)") end
end
local function make_thing(label)
    local t = {}
    return setmetatable(t, { __index = function(_, k)
        if k == "__label" then return label end
        return function(self, ...)
            record(label, k, ...)
            if k == "GetPos" or k == "GetHomePos" or k == "GetFocalPos" then return { x = 0, y = 0, z = 0 } end
            if k == "GetName" or k == "GetDataString" or k == "GetDefName" then return "" end
            if k == "IsAlive" then return true end
            if k == "IsDead" or k == "IsNull" or k == "IsEqualTo" or k == "IsBeingCarriedBy" then return false end
            if k == "MsgGetThingsKilledGroups" then return {} end   -- the sidecar's word list (empty = no kill)
            if k:match("^Is") or k:match("^Msg") or k:match("^Has") or k:match("^Can") then return false end
            if k:match("^Get") then return 0 end
            return nil
        end
    end, __tostring = function() return label end })
end
local quest = setmetatable({}, { __index = function(_, k)
    return function(self, ...)
        record("quest", k, ...)
        if k == "NewScriptFrame" then
            frames = frames + 1
            if frames > budget then terminating = true; return false end
            return true
        end
        if k == "IsActiveThreadTerminating" then return terminating end
        if k:match("^GetAll") or k == "GetFollowingEntityList" or k == "GetStateListCopy" then return {} end
        if k == "GetHero" or (k:match("^Get") and (k:match("Thing") or k:match("With") or k:match("Target$"))) or k == "CreateCreature" or k == "GetStateThing" or k == "GetStateListAt" then
            return make_thing(k)
        end
        if k == "RetailResources" then return RESOURCES end
        if k == "GetVillagerSpeechLists" or k == "GetHero" then return make_thing(k) end
        if k == "GetStateBool" or k == "PersistTransferBool" then return false end
        if k == "WithRetailResources" then local body = ...; return body(RESOURCES) end
        if k == "RegisterTimer" or k == "AddNewConversation" or k == "StartAmbientConversation" then return 1 end
        if k == "GetStateString" then return "" end
        if k == "GetStateListCount" then return 0 end
        if k == "MsgIsQuestionAnsweredYesOrNo" then return 1 end
        if k == "MsgIsGameInfoClickedPast" or k == "MsgIsTutorialClickedPast" then return true end
        -- the out-thing messages: the sidecar returns the thing, or nil when the message did not fire (not false)
        if k == "MsgOnHeroPickedPocket" or k == "MsgOnHeroPickedLock" or k == "MsgOnFishingGameFinished" or k == "MsgOnTavernGameFinished" then return nil end
        if k == "IsRegionLoaded" or k == "IsQuestActive" then return true end
        if k == "IsConversationActive" then return false end
        if k:match("^Is") or k:match("^Msg") or k:match("^Has") or k:match("^Was") then return false end
        if k:match("^Get") or k:match("^Add") or k:match("^Read") then return 0 end
        return nil
    end
end })
RESOURCES = setmetatable({}, { __index = function(_, k)
    return function(self, ...)
        record("resources", k, ...)
        if k == "TryAcquire" or k == "IsAlive" then return true end
        if k == "NewBarrelWatchSnapshot" or k == "NewPresentedItemOutput" then return make_thing(k) end
        if k:match("^New") or k:match("^Start") then return 1 end
        if k:match("^Is") then return false end
        if k:match("^Get") or k:match("^Read") then return 0 end
        return nil
    end
end })
local me = make_thing("me")
local env = setmetatable({ quest = quest, me = me, resources = RESOURCES, print = print, require = function() return ENV_FUNCS end,
    tostring = tostring, tonumber = tonumber, pairs = pairs, ipairs = ipairs, string = string, math = math, table = table,
    setmetatable = setmetatable, error = error, pcall = pcall, xpcall = xpcall, select = select, type = type, assert = assert,
    rawget = rawget, rawset = rawset, next = next, unpack = table.unpack },
    { __index = function(_, k) return nil end })
ENV_FUNCS = setmetatable({}, { __index = function(_, k) return function(...) record("helper", k, ...) return 0 end end })
local chunk, err = load(SOURCE, "@" .. NAME, "t", env)
if not chunk then return { load_error = err } end
local ok, err2 = pcall(chunk)
if not ok then return { load_error = tostring(err2) } end
local module = (type(err2) == "table") and err2 or nil
local results = {}
-- Init first (entity state fields are set there), then everything else in file order
local ordered = {}
for _, f in ipairs(FUNCTIONS) do if f == "Init" then ordered[#ordered + 1] = f end end
for _, f in ipairs(FUNCTIONS) do if f ~= "Init" then ordered[#ordered + 1] = f end end
for _, fname in ipairs(ordered) do
    local fn = env[fname]
    if fn == nil and module and fname:find(".", 1, true) then
        fn = module[fname:match("%.(%w+)$")]
    end
    if type(fn) == "function" then
        calls, frames, terminating = {}, 0, false
        local count = 0
        debug.sethook(function() count = count + 1; if count > INSTRUCTION_LIMIT then error("instruction limit: loop without NewScriptFrame?", 2) end end, "", 1000)
        local extra = {}
        for i, kind in ipairs(PARAM_KINDS[fname] or {}) do
            if kind == "string" then extra[i] = "" elseif kind == "number" then extra[i] = 0 elseif kind == "bool" then extra[i] = false
            elseif kind == "resources" then extra[i] = RESOURCES elseif kind == "function" then extra[i] = function() return true end
            elseif kind == "state" then extra[i] = make_thing("state")
            else extra[i] = make_thing("arg" .. i) end
        end
        local rok, rerr
        if ENTITY_FILE then rok, rerr = pcall(fn, quest, me, table.unpack(extra)) else rok, rerr = pcall(fn, quest, table.unpack(extra)) end
        debug.sethook()
        local trace = {}
        for i = 1, math.min(#calls, 40) do trace[i] = calls[i] end
        results[fname] = { ok = rok, error = (not rok) and tostring(rerr) or nil, frames = frames, calls = #calls, trace = trace }
    end
end
return results
'''


# bindings the converter emits ahead of the sidecar: each is a row in docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md
# (the retail transfer it stands for is named there). Reported as `pendingMethods`, not counted as problems.
PENDING_BINDINGS = {'InitialiseArenaRounds',  # compiled source patches; not yet in the installed sidecar
                    'MemberResource', 'MemberStringMap', 'AssignResource', 'ClearStringMap'}


def registered_methods():
    text = ''.join(p.read_text(encoding='utf-8', errors='replace') for p in DLL_SOURCES if p.exists())
    return set(re.findall(r'(?:_type|quest|thing|type)\["(\w+)"\]', text))


LUA_KEYWORDS = {'and', 'break', 'do', 'else', 'elseif', 'end', 'false', 'for', 'function', 'goto', 'if', 'in',
                'local', 'nil', 'not', 'or', 'repeat', 'return', 'then', 'true', 'until', 'while'}
KNOWN_GLOBALS = {'quest', 'me', 'resources', 'require', 'tostring', 'tonumber', 'math', 'string', 'table', 'pairs',
                 'ipairs', 'print', 'error', 'assert', 'type', 'select', 'pcall', 'Quest', 'Quests', '_G', 'os'}


def free_globals(source):
    """Identifiers read as globals that no `local`, parameter, function name or known host global declares —
    the static form of "attempt to compare nil with number" (a Ghidra `_DAT_x` / `extraout_ST0` leak)."""
    body = re.sub(r'--\[\[.*?\]\]|--[^\n]*', '', source, flags=re.S)
    body = re.sub(r'"(?:[^"\\]|\\.)*"', '""', body)
    declared = set(KNOWN_GLOBALS) | set(re.findall(r'^function (\w+)\(', body, re.M))
    for m in re.finditer(r'\blocal\s+(?:function\s+(\w+)|([\w\s,]+?))(?=\s*(?:=|\n|$))', body):
        declared.update(re.findall(r'\w+', m.group(1) or m.group(2)))
    for m in re.finditer(r'\bfunction\s*[\w.]*\s*\(([^)]*)\)', body):
        declared.update(re.findall(r'\w+', m.group(1)))
    for m in re.finditer(r'\bfor\s+([\w\s,]+?)\s*(?:=|\bin\b)', body):
        declared.update(re.findall(r'\w+', m.group(1)))
    declared.update(re.findall(r'::(\w+)::', body))                       # labels
    declared.update(re.findall(r'\bgoto\s+(\w+)', body))
    declared.update(re.findall(r'[{,]\s*(\w+)\s*=[^=]', body))               # table constructor keys {R = .., G = ..}
    used = set()
    for m in re.finditer(r'(?<![\w.:])([A-Za-z_]\w*)\b(?!\s*[:(])', body):
        used.add(m.group(1))
    return sorted(n for n in used - declared - LUA_KEYWORDS if not re.fullmatch(r'\d+', n))


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--unit', default='orchard_farm')
    a.add_argument('--stage', default='readable')
    a.add_argument('--frames', type=int, default=200)
    a.add_argument('--instructions', type=int, default=2_000_000)
    a.add_argument('--json', type=Path)
    a.add_argument('--package-dir', type=Path, help='smoke this FSE tree instead of a converter unit')
    args = a.parse_args()
    from lupa.lua54 import LuaRuntime
    if args.package_dir:
        base = args.package_dir.resolve()
    else:
        u = script_unit(args.unit)
        base = ROOT / 'refs/script_recovery/lifted' / u['package'] / args.stage / 'FSE'
    known = registered_methods()
    report, problems = {}, 0
    for path in sorted(base.rglob('*.lua')):
        source = path.read_text(encoding='utf-8')
        functions = re.findall(r'^function (\w+)\(', source, re.M)
        functions += [f'{mod}.{fn}' for mod, fn in re.findall(r'^function (\w+)\.(\w+)\(', source, re.M)]
        if not functions:
            continue
        lua = LuaRuntime(unpack_returned_tuples=True)
        runner = lua.execute('return function(SOURCE, NAME, FUNCTIONS, FRAME_BUDGET, INSTRUCTION_LIMIT, PARAM_KINDS, ENTITY_FILE) ' + HARNESS + ' end')
        kinds = {}
        for name, params in re.findall(r'^function ([\w.]+)\(([^)]*)\)', source, re.M):
            entity_file = '__native_entity_state' in source
            extra = [p.strip() for p in params.split(',')][2 if entity_file else 1:]
            kinds[name] = lua.table(*[('resources' if p == 'resources'
                                       else 'state' if p in ('state', 'entityState', 'progress', 'talkState')
                                       else 'function' if re.search(r'^(body|predicate|callback|fn|target|add\w+|on\w+)$', p, re.I)
                                       else 'string' if re.search(r'name|comment|key|text|string|def|label', p, re.I)
                                       else 'number' if re.search(r'type|count|index|id|amount|priority|state|param|seconds', p, re.I)
                                       else 'thing') for p in extra])
        result = runner(source, path.name, lua.table(*functions), args.frames, args.instructions, lua.table(**kinds), '__native_entity_state' in source)
        rel = path.relative_to(base).as_posix()
        entry = {}
        leaks = free_globals(source)
        if leaks:
            entry['freeGlobals'] = leaks; problems += 1
        if 'load_error' in result:
            entry['load_error'] = result['load_error']; problems += 1
        else:
            for fname in functions:
                r = result[fname]
                if r is None:
                    continue
                trace = [r['trace'][i] for i in range(1, len(r['trace']) + 1)] if r['trace'] else []
                unknown = sorted({c.split(':')[1] for c in trace if c.startswith(('quest:', 'me:', 'resources:')) and c.split(':')[1] not in known})
                pending = sorted(set(unknown) & PENDING_BINDINGS)
                unknown = [u for u in unknown if u not in PENDING_BINDINGS]
                entry[fname] = {'ok': bool(r['ok']), 'error': r['error'], 'frames': r['frames'], 'calls': r['calls'],
                                'unknownMethods': unknown, 'pendingMethods': pending, 'trace': trace[:12]}
                if pending:
                    print(f'{rel}:{fname}: pending sidecar binding {", ".join(pending)} (docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md)')
                if not r['ok'] or unknown:
                    problems += 1
        report[rel] = entry
    for rel, entry in report.items():
        for fname, r in entry.items():
            if fname == 'freeGlobals':
                print(f'FREE GLOBALS {rel}: {", ".join(r)}')
                continue
            if fname == 'load_error':
                print(f'LOAD ERROR {rel}: {r}')
            elif not r['ok'] or r['unknownMethods']:
                print(f'{rel}:{fname}: {"ERROR " + str(r["error"]) if not r["ok"] else ""} {"unknown=" + ",".join(r["unknownMethods"]) if r["unknownMethods"] else ""} (frames {r["frames"]}, calls {r["calls"]})')
    print(json.dumps({'files': len(report), 'problems': problems}))
    if args.json:
        args.json.write_text(json.dumps(report, indent=1), encoding='utf-8')


if __name__ == '__main__':
    main()
