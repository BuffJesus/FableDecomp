"""Resume probe for the converted Gameflow Main (usage: probe_gameflow_resume.py <Gameflow.lua> <report.json>): for each persisted PostSavePosition value, run Main under a
mock runtime where every Msg*/Is* poll succeeds after one frame, and record the sequence of
SetMasterGameState("PostSavePosition", N) writes. A correct stage chain resumes at the stage itself and
then writes every later stage in order (0 -> 100 -> ... -> 2800)."""
import sys, json
from lupa.lua54 import LuaRuntime

STAGES = [0, 100, 150, 200, 300, 400, 450, 500, 550, 600, 700, 800, 850, 856, 870, 875, 900, 1000, 1050, 1100, 1200,
          1250, 1300, 1450, 1500, 1550, 1600, 1700, 1900, 2100, 2300, 2400, 2500, 2600, 2800]
path = sys.argv[1]
source = open(path, encoding='utf-8').read()
HARNESS = r'''
return function(SOURCE, START)
    local writes, frames, terminating = {}, 0, false
    local polls = {}
    local function make_thing(label)
        return setmetatable({}, { __index = function(_, k)
            return function(self, ...)
                if k == "GetPos" then return {x=0,y=0,z=0} end
                if k == "GetName" or k == "GetDataString" then return "" end
                if k == "IsAlive" or k == "AcquireControl" then return true end
                if k:match("^Is") or k:match("^Msg") then return false end
                if k:match("^Get") then return 0 end
                return nil
            end end })
    end
    local master = { PostSavePosition = START }
    local quest = setmetatable({}, { __index = function(_, k)
        return function(self, ...)
            local a = {...}
            if k == "NewScriptFrame" then frames = frames + 1; if frames > 4000 then terminating = true; return false end; return true end
            if k == "IsActiveThreadTerminating" then return terminating end
            if k == "GetMasterGameState" then return master[a[1]] or 0 end
            if k == "SetMasterGameState" then if a[1] == "PostSavePosition" then writes[#writes+1] = a[2] end; master[a[1]] = a[2]; return end
            if k == "RetailResources" then return RES end
            if k:match("^GetAll") or k == "GetStateListCopy" then return {} end
            if k == "GetHero" or (k:match("^Get") and (k:match("Thing") or k:match("With"))) then return make_thing(k) end
            if k == "RegisterTimer" then return 1 end
            if k == "GetTimer" then return 0 end
            if k == "GetStateString" then return "" end
            if k == "IsRegionLoaded" or k == "IsLevelLoaded" or k == "IsQuestCompleted" then return true end
            -- a poll succeeds the second time it is asked (one frame of waiting), so wait loops advance
            if k:match("^Msg") or k:match("^Is") or k:match("^Has") then
                polls[k] = (polls[k] or 0) + 1
                return polls[k] % 2 == 0
            end
            if k:match("^Get") or k:match("^Read") then return 0 end
            return nil
        end end })
    RES = setmetatable({}, { __index = function(_, k)
        return function(self, ...)
            if k == "TryAcquire" or k == "IsAlive" then return true end
            if k:match("^New") or k:match("^Start") then return 1 end
            if k:match("^Is") then return false end
            if k:match("^Get") then return 0 end
            return nil
        end end })
    local env = setmetatable({ quest = quest, resources = RES, print = print, tostring = tostring, tonumber = tonumber, pairs = pairs,
        ipairs = ipairs, string = string, math = math, table = table, setmetatable = setmetatable, error = error, pcall = pcall,
        select = select, type = type }, { __index = function() return nil end })
    local chunk, err = load(SOURCE, "@gameflow", "t", env)
    if not chunk then return { error = err } end
    chunk()
    local count = 0
    debug.sethook(function() count = count + 1; if count > 20000 then error("instruction limit", 2) end end, "", 1000)
    local ok, rerr = pcall(env.Main, quest)
    debug.sethook()
    local out = {}
    for i, w in ipairs(writes) do out[i] = w end
    return { ok = ok, error = (not ok) and tostring(rerr) or nil, writes = out, frames = frames }
end
'''
lua = LuaRuntime(unpack_returned_tuples=True)
run = lua.execute(HARNESS)
report = {}
for stage in STAGES:
    r = run(source, stage)
    writes = list(r['writes'].values()) if r['writes'] else []
    report[stage] = {'ok': bool(r['ok']), 'error': r['error'], 'writes': writes, 'frames': r['frames']}
    idx = STAGES.index(stage)
    expected = STAGES[idx:]
    verdict = 'CHAIN-OK' if writes[:len(expected)] == expected else ('RESUMES' if writes[:1] == [stage] else 'NO-RESUME')
    if stage == STAGES[-1] and not writes and r['frames'] >= 4000 and not r['error']:
        # retail's last case writes nothing: the free-roam frame loop runs until the thread terminates
        verdict = 'LOOPS-OK'
    print(f'{stage:5d}: {verdict:10s} writes={writes[:8]}{"..." if len(writes) > 8 else ""} frames={r["frames"]} err={r["error"]}')
json.dump(report, open(sys.argv[2], 'w'), indent=1)
