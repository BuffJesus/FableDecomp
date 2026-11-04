from pathlib import Path
import importlib.util,re,sys
import pytest
from lupa.lua54 import LuaRuntime
W=Path(__file__).resolve().parent
from tools.script_recovery.script_units import ROOT as R
from tools.script_recovery import readable_boolean_query_chain as P
NAMES=['IsHeroWearingAnyTash','IsHeroWearingAnyOddHairdo','BordelloTash','BeardyTashQuest','BeardyHairQuest']
COUNTS=dict(IsHeroWearingAnyTash=6,IsHeroWearingAnyOddHairdo=5,BordelloTash=6,BeardyTashQuest=6,BeardyHairQuest=5)
SOURCES={name:(W/'testdata/boolean_query_chain'/(name+'.lua')).read_text() for name in NAMES}
def run(source,mask,kind='bool',throw_get=0,throw_query=0):
    vm=LuaRuntime(unpack_returned_tuples=True)
    return vm.eval('''function(source,entry,mask,kind,throwGet,throwQuery)
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
        local result=table.pack(pcall(env[entry],q,{}))
        collect("after_return")
        return tostring(result.n).."|"..tostring(result[1]).."|"..tostring(result[2]).."|"..table.concat(events,";")
    end''')(source,source.split('(',1)[0].split()[-1],mask,kind,throw_get,throw_query)


@pytest.mark.parametrize('name',NAMES)
@pytest.mark.parametrize('mask',range(256))
def test_truth_patterns_and_handle_roots(name,mask):
 source=SOURCES[name];after,proof=P.fold(source);assert proof
 assert run(source,mask)==run(after,mask)

@pytest.mark.parametrize('name',NAMES)
@pytest.mark.parametrize('kind',['nil','zero','empty','object'])
@pytest.mark.parametrize('mask',[0,1,2,16,32,255])
def test_truthiness_and_single_return(name,kind,mask):
 source=SOURCES[name];after,proof=P.fold(source);assert proof
 assert run(source,mask,kind)==run(after,mask,kind)

@pytest.mark.parametrize('name',NAMES)
@pytest.mark.parametrize('which',['get','query'])
@pytest.mark.parametrize('site',range(1,7))
def test_failures_keep_order_and_roots(name,which,site):
 source=SOURCES[name];after,proof=P.fold(source);assert proof
 args=dict(throw_get=site) if which=='get' else dict(throw_query=site)
 assert run(source,0,**args)==run(after,0,**args)

@pytest.mark.parametrize('name',NAMES)
def test_declarations_scopes_and_idempotence(name):
 source=SOURCES[name];after,proof=P.fold(source);assert proof
 assert proof['queryCount']==COUNTS[name]
 assert [x.strip() for x in source.splitlines() if x.strip().startswith('local ')]==[x.strip() for x in after.splitlines() if x.strip().startswith('local ')]
 assert re.findall(r'^ *local hero.*$',source,re.M)==re.findall(r'^ *local hero.*$',after,re.M)
 assert 'goto ' not in after and 'not not quest:IsWearingHairstyle' in after
 assert P.fold(after)==(after,None)
 LuaRuntime().compile(after)

@pytest.mark.parametrize('name',NAMES)
@pytest.mark.parametrize('old,new',[
 ('predicateResult = false','predicateResult = quest:ReadFlags()'),
 ('predicateResult = false','predicateResult = nil'),
 ('predicateResult = true','predicateResult = 1'),
 ('if predicateResult then','if quest:Observe(predicateResult) then'),
 ('if predicateResult then\n    end','if predicateResult then\n        quest:ReleaseResource(predicateResult)\n    end'),
 ('local hero2 = quest:GetHero()','local hero2 = other:GetHero()'),
 ('IsWearingHairstyle(hero2,','IsWearingHairstyle(hero,'),
 ('local hero2 = quest:GetHero()','local hero = quest:GetHero()'),
 ('local hero2 = quest:GetHero()','local hero2 = quest:GetHero()\n        local predicateResult = 0'),
 ('local hero = quest:GetHero()','debug.getlocal()\n    local hero = quest:GetHero()'),
 ('local hero = quest:GetHero()','-- retain this native diagnostic\n    local hero = quest:GetHero()'),
])
def test_changed_operations_types_bindings_or_annotations_refuse(name,old,new):
 source=SOURCES[name];assert old in source
 altered=source.replace(old,new,1)
 assert P.fold(altered)==(altered,None)

@pytest.mark.parametrize('name',NAMES)
def test_generic_identifier_renames(name):
 source=SOURCES[name]
 for old,new in [(source.split('(',1)[0].split()[-1],'AnyStyle'),('quest','host'),('me','actor'),('predicateResult','privateFlag')]:
  source=re.sub(r'\b'+re.escape(old)+r'\b',new,source)
 after,proof=P.fold(source);assert proof
 assert run(source,0)==run(after,0)
