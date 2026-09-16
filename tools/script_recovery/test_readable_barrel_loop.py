import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_barrel_loop import HELPER, LOOP


class BarrelLoopReadabilityTests(unittest.TestCase):
    def test_outer_loop_frame_order_and_cleanup_match(self):
        before=Path(__file__).with_name('readable_barrel_loop_before.lua').read_text()
        interaction=Path(__file__).with_name('readable_barrel_interaction_before.lua').read_text()
        start=before.index('    alive = not quest:IsActiveThreadTerminating()',before.index('    warehouseGuardMarker ='))
        original='local alive,cVar2\n'+before[start:]
        original=original.replace(interaction,'        if not handleBarrelInteraction() then goto LAB_00db6afd end\n')
        cleanup='resources:DestroyThing(3)\nresources:DestroyThing(2)\nresources:ReleaseResource(1)\n'
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(source,frames,stopPhase,stopInteraction,failAt)
            local events={};local live={[1]=true,[2]=true,[3]=true};local frame=0
            local function event(s)events[#events+1]=s;if #events==failAt then error('BOUNDARY',0)end end
            local resources={}
            function resources:DestroyThing(id)assert(live[id]);live[id]=nil;events[#events+1]='destroy'..id end
            function resources:ReleaseResource(id)assert(live[id]);live[id]=nil;events[#events+1]='release'..id end
            function resources:Pause()error('unreachable pause')end
            function resources:DestroyMovie()error('unreachable movie cleanup')end
            local env={resources=resources,me='actor',quest={
                IsActiveThreadTerminating=function()event('term'..frame);return frame>=frames end,
                NewScriptFrame=function(_,actor)assert(actor=='actor');event('frame'..frame);frame=frame+1 end},
                advanceBarrelPhase=function()event('phase'..frame);return frame~=stopPhase end,
                handleBarrelInteraction=function()event('interaction'..frame);return frame~=stopInteraction end}
            local f=assert(load(source,'outer-loop','t',env));local ok,err=pcall(f)
            for id=3,1,-1 do if live[id] then if id==1 then resources:ReleaseResource(id) else resources:DestroyThing(id) end end end
            return ok,err,table.concat(events,',')
        end''')
        for frames,phase,interaction,error in itertools.product((0,1,4),(-1,0,2),(-1,0,2),(0,1,2,3,5,9)):
            self.assertEqual(run(original+cleanup,frames,phase,interaction,error),run(LOOP+cleanup,frames,phase,interaction,error))

    def test_interaction_order_and_short_circuit_match_previous_lua(self):
        run=LuaRuntime(unpack_returned_tuples=True).execute('''return function(source,phase,hit,approach,talk,overhear,cancelAt,stop)
            local events={};local terms=0
            local function event(name,...)local args={...};for i=1,#args do args[i]=tostring(args[i])end;events[#events+1]=name..':'..table.concat(args,',')end
            local function api(prefix)return setmetatable({}, {__index=function(_,name)return function(_,...)event(prefix..name,...);return true end end})end
            local resources=api('resources.');local quest=api('quest.');local state=api('state.')
            local me=setmetatable({}, {__tostring=function()return 'actor'end})
            function me:IsTalkedToByHero()event('talk');return talk end
            function quest:IsActiveThreadTerminating()event('term');terms=terms+1;return terms==cancelAt end
            function state:GetStateInt(key)event('phase',key);return phase end
            function state:GetStateBool(key)event('heard',key);return true end
            function resources:IsHitByHeroExceptAbility(actor,ability)event('hit',actor,ability);return hit end
            function resources:IsHeroWithinBarrelApproachDistance(actor)event('approach',actor);return approach end
            function resources:ShouldBarrelOverhear(actor,heard)event('overhear',actor,heard);return overhear end
            function resources:StartMovie(key)event('movie',key);return 11 end
            local env={me=me,quest=quest,resources=resources,__native_entity_state=state,
                require=function(name)assert(name=='NewOakValeIntro.native_quest_helpers');return {AddBadDeed=function(_,actor,value)event('bad',actor,value)end}end}
            for _,name in ipairs({'acquireBarrelControl','playCarefulMovie','playReturnInteraction','playInitialInteraction'}) do
                env[name]=function(...)event(name,...);return name~=stop end
            end
            setmetatable(env,{__index=_G});local result=assert(load(source,'interaction','t',env))()
            return result,table.concat(events,';'),env.barrel_interaction_movie
        end''')
        before=Path(__file__).with_name('readable_barrel_interaction_before.lua').read_text()
        original=before+'::LAB_00db6933:: do return true end\n::LAB_00db6afd:: return false'
        structured=HELPER+'return handleBarrelInteraction()'
        for phase,hit,approach,talk,overhear,cancel,stop in itertools.product((0,4,5),(False,True),(False,True),(False,True),(False,True),range(3),('', 'acquireBarrelControl','playCarefulMovie','playReturnInteraction','playInitialInteraction')):
            self.assertEqual(run(original,phase,hit,approach,talk,overhear,cancel,stop),run(structured,phase,hit,approach,talk,overhear,cancel,stop))
