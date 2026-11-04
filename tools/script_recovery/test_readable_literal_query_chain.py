from pathlib import Path
import importlib.util,json,re,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_literal_query_chain as P
SOURCE=(W/'testdata/literal_query_chain.lua').read_text()

def test_nested_scopes_and_declarations_are_kept():
    after,proof=P.fold(SOURCE)
    assert proof and proof['queryCount']==8 and proof['noLookupHoisted']
    assert 'local flags, scratchValue, predicateResult' in after
    for i in range(1,9):
        name='hero' if i==1 else 'hero'+str(i)
        assert '    '*i+'local '+name+' = quest:GetHero()' in after
    assert 'goto ' not in after and 'flags = ' not in after
    assert 'not not quest:IsWearingHairstyle' in after
    assert after[after.index('\n-- BB_'):]==SOURCE[SOURCE.index('\n-- BB_'):]
    LuaRuntime().compile(after)
    assert P.fold(after)==(after,None)

@pytest.mark.parametrize('old,new',[
    ('flags = 1\n','flags = quest:ReadFlags()\n'),
    ('flags = 255','flags = 254'),
    ('flags = flags & 127','quest:ReleaseResource(flags)'),
    ('flags = flags & 191','flags = flags & 127'),
    ('scratchValue = flags & 253','predicateResult = flags & 253'),
    ('return predicateResult','return flags'),
    ('local hero2 = quest:GetHero()','local hero2 = other:GetHero()'),
    ('local hero2 = quest:GetHero()','local hero = quest:GetHero()'),
    ('IsWearingHairstyle(hero2,','IsWearingHairstyle(hero,'),
    ('predicateResult = false','predicateResult = nil'),
    ('predicateResult = true','predicateResult = 1'),
    ('goto LAB_00e53856','goto different'),
    ('local flags, scratchValue, predicateResult','local flags <close>, scratchValue, predicateResult'),
    ('flags = 3','flags = 3\n        quest:Observe(flags)'),
    ('local hero2 = quest:GetHero()','local hero2 = quest:GetHero()\n        local flags = 0'),
])
def test_changed_operands_effects_types_scope_or_exit_refuse(old,new):
    assert old in SOURCE
    source=SOURCE.replace(old,new,1)
    assert P.fold(source)==(source,None)

@pytest.mark.parametrize('hazard',sorted(P.HAZARDS))
def test_reflection_and_environment_refuse(hazard):
    source=SOURCE.replace('    flags = 1','    '+hazard+'()\n    flags = 1',1)
    assert P.fold(source)==(source,None)

def test_body_annotations_stay_but_trailing_headers_are_preserved():
    source=SOURCE.replace('    flags = 1','    -- TODO retain native field evidence\n    flags = 1',1)
    assert P.fold(source)==(source,None)
    after,proof=P.fold(SOURCE)
    assert proof and '-- E53CB0:' in after

def test_generic_identifiers_match_without_renaming_methods():
    source=SOURCE
    for old,new in [('IsHeroWearingBeard','CheckStyles'),('quest','host'),('me','actor'),('flags','bits'),('scratchValue','unused'),('predicateResult','result')]:
        source=re.sub(r'\b'+re.escape(old)+r'\b',new,source)
    after,proof=P.fold(source)
    assert proof and proof['function']=='function CheckStyles(host, actor)'
    LuaRuntime().compile(after)

def run(source,mask,kind='bool',throw_get=0,throw_query=0):
    vm=LuaRuntime(unpack_returned_tuples=True)
    return vm.eval('''function(source,mask,kind,throwGet,throwQuery)
        local events,active={},{}
        local env={};setmetatable(env,{__index=_G})
        assert(load(source,"@query_chain","t",env))()
        local gets,queries=0,0
        local function collect(label)
            collectgarbage("collect")
            local ids={};for i=1,8 do if active[i] then ids[#ids+1]=i end end
            events[#events+1]=label..":"..table.concat(ids,",")
        end
        local q={}
        function q:GetHero()
            gets=gets+1;collect("before_get"..gets)
            if gets==throwGet then error("fixture getter failure",0) end
            local id=gets;active[id]=true
            local handle=setmetatable({id=id},{__gc=function() active[id]=nil;events[#events+1]="retired:"..id end})
            events[#events+1]="get:"..id
            return handle,"ignored second lookup result"
        end
        function q:IsWearingHairstyle(hero,literal)
            queries=queries+1;assert(hero.id==queries,"lookup was hoisted/reordered")
            collect("query"..queries)
            events[#events+1]="style:"..hero.id..":"..literal
            if queries==throwQuery then error("fixture query failure",0) end
            local value=(mask & (1 << (queries-1)))~=0
            if not value then
                if kind=="nil" then return nil,"ignored" end
                return false,"ignored"
            end
            if kind=="zero" then return 0,"ignored" end
            if kind=="empty" then return "","ignored" end
            if kind=="object" then return {},"ignored" end
            return true,"ignored"
        end
        local result=table.pack(pcall(env.IsHeroWearingBeard,q,{}))
        collect("after_return")
        return tostring(result.n).."|"..tostring(result[1]).."|"..tostring(result[2]).."|"..table.concat(events,";")
    end''')(source,mask,kind,throw_get,throw_query)

@pytest.mark.parametrize('mask',range(256))
def test_all_truth_vectors_and_finalizable_handle_roots(mask):
    after,proof=P.fold(SOURCE);assert proof
    assert run(SOURCE,mask)==run(after,mask)

@pytest.mark.parametrize('kind',['bool','nil','zero','empty','object'])
@pytest.mark.parametrize('mask',[0,1,2,8,64,128,255])
def test_truthiness_and_single_boolean_return_without_type_premise(kind,mask):
    after,proof=P.fold(SOURCE);assert proof
    assert run(SOURCE,mask,kind)==run(after,mask,kind)

@pytest.mark.parametrize('index',range(1,9))
def test_lookup_and_query_errors_keep_observations_and_release_roots(index):
    after,proof=P.fold(SOURCE);assert proof
    assert run(SOURCE,0,'bool',index,0)==run(after,0,'bool',index,0)
    assert run(SOURCE,0,'bool',0,index)==run(after,0,'bool',0,index)
