"""Older cluster output retains callback traces through the readable pipeline."""
import re

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import lift_cluster
from tools.script_recovery.build_readable_cluster import build

PAIRS = [('V_MazeResearch','MazeResearch'), ('QS_MeetSister','MeetSister'),
         ('QS_ScytheInfo','ScytheInfo'), ('V_Fisherman','Fisherman'),
         ('V_RockTrollFirstEncounter','RockTrollFirstEncounter'),
         ('QS_GuardianTrophyDealerInfo','GuardianTrophyDealerInfo')]

HARNESS = r'''
trace, frames, state = {}, 0, {}
local function describe(value)
    if type(value)=='table' then return value.__tag or 'vector' end
    if type(value)=='number' then return string.format('%.12g',value) end
    if type(value)=='function' then return 'function' end
    return tostring(value)
end
local function record(receiver,name,...)
    local row={receiver,name}
    for i=1,select('#',...) do row[#row+1]=describe(select(i,...)) end
    trace[#trace+1]=table.concat(row,'|')
    if #trace>2000 then error('trace budget exceeded') end
end
local function actor(tag)
    return setmetatable({__tag=tag},{__index=function(_,name)
        return function(self,...)
            record(tag,name,...)
            if name=='GetName' or name=='GetDataString' then return tag end
            if name=='GetPos' or name=='GetHomePos' then return {x=0,y=0,z=0} end
            if name=='IsAlive' then return true end
            if name:match('^Is') or name:match('^Msg') or name:match('^Has') then return false end
            if name:match('^Get') then return 0 end
        end
    end})
end
quest=setmetatable({}, {__index=function(_,name)
    return function(self,...)
        record('quest',name,...)
        local a,b=...
        if name=='NewScriptFrame' then frames=frames+1;return frames<frameLimit end
        if name=='IsActiveThreadTerminating' then return frames>=frameLimit end
        if name=='IsRegionLoaded' then return frames>=enterAfter and frames<leaveAfter end
        if name=='GetActiveQuestName' then return 'quest_under_test' end
        if name=='GetStateBool' then
            if state[a]~=nil then return state[a] end
            return defaultState
        end
        if name=='GetStateInt' or name=='GetStateFloat' then return state[a] or 0 end
        if name=='GetStateString' then return state[a] or '' end
        if name:match('^SetState') then state[a]=b;return end
        if name=='GetHero' then return actor('hero') end
        if name:match('^GetAll') or name=='GetFollowingEntityList' then return {} end
        if name=='GetThingWithScriptName' then return actor(a) end
        if name:match('^Get') and name:find('WithScriptName') then return actor(b or a) end
        if name=='AddNewConversation' or name=='RegisterTimer' then return 7 end
        if name=='PersistTransferBool' then return defaultState end
        if name:match('^Is') or name:match('^Msg') or name:match('^Has') or name:match('^Can') then return false end
        if name:match('^Get') then return 0 end
    end
end})
function replay(name)
    trace,frames,state={},0,{}
    if name~='Init' and type(Init)=='function' then Init(quest) end
    if defaultState and name~='Init' then
        for key,value in pairs(state) do if type(value)=='boolean' then state[key]=true end end
    end
    local ticks=0
    debug.sethook(function()ticks=ticks+1;if ticks>30 then error('instruction budget exceeded') end end,'',1000)
    local ok,err=pcall(_G[name],quest,{})
    debug.sethook()
    return ok,err,frames,table.concat(trace,'\n')
end
'''


@pytest.fixture(scope='module')
def candidates(tmp_path_factory):
    result = {}
    for script, package in PAIRS:
        raw = lift_cluster(script)['lua']
        out = tmp_path_factory.mktemp(package)
        report = build(script, out)
        styled = (out/'FSE'/package/(package+'.lua')).read_text()
        result[script] = raw, styled, report
    return result


def classify(error):
    if not error:
        return None
    # Readable local names and line numbers differ; preserve the failure kind.
    message = str(error).split(': ', 1)[-1]
    return re.sub(r" \((?:local|global|field|upvalue) '[^']+'\)", '', message)


@pytest.mark.parametrize('script,package', PAIRS)
@pytest.mark.parametrize('enter_after,default_state', [(0,False), (2,False), (20,False), (0,True)])
def test_every_named_callback_preserves_trace_state_and_scheduler(candidates, script, package, enter_after, default_state, subtests):
    raw, styled, report = candidates[script]
    assert report['files'][f'FSE/{package}/{package}.lua']['syntax']['ok']
    functions = re.findall(r'^function (\w+)\(', raw, re.M)
    assert functions == re.findall(r'^function (\w+)\(', styled, re.M)
    runtimes = []
    for source in (raw, styled):
        lua = LuaRuntime(unpack_returned_tuples=True)
        lua.execute(source)
        lua.execute(HARNESS)
        lua.globals().enterAfter = enter_after
        lua.globals().leaveAfter = 2 if default_state else 100
        lua.globals().frameLimit = 4
        lua.globals().defaultState = default_state
        runtimes.append(lua)
    completed = 0
    for name in functions:
        with subtests.test(callback=name):
            before = runtimes[0].globals().replay(name)
            after = runtimes[1].globals().replay(name)
            assert before[0], before[1]
            assert after[0], after[1]
            assert (before[0], classify(before[1]), before[2:]) == (after[0], classify(after[1]), after[2:])
            assert dict(runtimes[0].globals().state) == dict(runtimes[1].globals().state)
            completed += bool(after[0])
    # Do not pass the suite merely because both versions hit the same gaps.
    assert completed == len(functions)


def test_invalid_entity_evidence_is_retained_and_reported(candidates):
    _, _, report = candidates['QS_GuardianTrophyDealerInfo']
    row = report['files']['FSE/GuardianTrophyDealerInfo/Entities/GTDI_Maze.lua']
    assert row['notStyled'].startswith('Native draft needs recovery')
    assert not row['syntax']['ok']
    assert row['before'] == row['after']
