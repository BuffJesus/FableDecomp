"""Main composition checks; native phase bodies are explicit doubles here."""
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime


class TheresaMainCompositionTests(unittest.TestCase):
    def test_dispatch_precedence_and_outer_ownership(self):
        for mode in ('intro','offer','chocolates','other-present','talk','hit','idle','busy','outro','cancel-phase'):
            with self.subTest(mode=mode):
                lua=LuaRuntime();lua.globals().mode=mode
                lua.execute('''
events={}; local function log(s) events[#events+1]=s end
local given=false
function waitForHeroToApproachTheresa() log("approach");return true end
function meetTheresa(q,me,r,c,state,p) log("meeting");state:SetStateBool("DoneIntro",true);return true end
function offerTheresaChocolates(q,me,r,c,p)
    log("offer");p.givenChocolates=true;return true
end
function classifyTheresaPresentedItem() log("classify");return mode end
function acceptPresentedTheresaChocolates(q,me,r,c,p) log("accept");p.givenChocolates=true;return true end
function rejectTheresaPresent() log("reject");return true end
function talkToTheresa(q,me,r,c,p,state) log("talk");state.askedForPresent=true;return mode~="cancel-phase" end
function handleTheresaHit() log("hit");return true end
function finishTheresaChildhood() log("outro");return true end
local frames=0
quest={NewScriptFrame=function() frames=frames+1;log("frame") end,
    IsActiveThreadTerminating=function() return frames>=2 end}
local done=mode~="intro"
state={GetStateBool=function(_,key) if key=="DoneIntro" then return done end return false end,
    SetStateBool=function(_,key,v) log("state:"..key);if key=="DoneIntro" then done=v end end}
resources={
    NewResource=function() log("control");return 24 end,
    NewTheresaDepartureTrigger=function() log("trigger");return 44 end,
    PrepareResource=function(_,c) assert(c==24);log("prepare") end,
    TryAcquire=function(_,c,me,priority) assert(c==24 and me==17 and priority==4);log("acquire");return true end,
    NewPresentedItemOutput=function() log("presented");return 16 end,
    TheresaTalkOffersChocolates=function() log("offer-query");return mode=="offer" or mode=="outro" end,
    WasVillagerTalkedTo=function() log("talk-query");return mode=="talk" or mode=="cancel-phase" end,
    IsHitByHeroExceptAbility=function(_,me,ability) assert(me==17 and ability==14);log("hit-query");return mode=="hit" end,
    IsPerformingScriptTask=function() log("task");return mode=="busy" end,
    PlayTheresaSkip=function() log("skip") end,
    IsTheresaHeroNearTrigger=function(_,t) assert(t==44);log("distance");return mode=="outro" end,
    DestroyPresentedItemOutput=function(_,p) assert(p==16);log("presented.close") end,
    DestroyThing=function(_,t) assert(t==44);log("trigger.close") end,
    ReleaseResource=function(_,c) assert(c==24);log("control.close") end,
}
''')
                phase=lua.execute(Path(__file__).with_name('theresa_main_body.lua').read_text())
                phase(lua.globals().quest,17,lua.globals().resources,lua.globals().state)
                events=list(lua.globals().events.values())
                prefix=['frame','control','trigger','prepare','acquire']
                paths={
                    'intro':['approach','meeting','state:DoneIntro'],
                    'offer':['offer-query','offer','distance'],
                    'outro':['offer-query','offer','distance','outro'],
                    'chocolates':['offer-query','classify','accept','distance'],
                    'other-present':['offer-query','classify','reject'],
                    'talk':['offer-query','classify','talk-query','talk','state:AskedForPresent'],
                    'cancel-phase':['offer-query','classify','talk-query','talk','state:AskedForPresent'],
                    'hit':['offer-query','classify','talk-query','hit-query','hit'],
                    'idle':['offer-query','classify','talk-query','hit-query','task','skip'],
                    'busy':['offer-query','classify','talk-query','hit-query','task'],
                }
                expected=prefix+([] if mode=='intro' else ['presented'])+paths[mode]
                if mode!='intro':expected+=['presented.close']
                if mode not in ('outro','cancel-phase'):expected+=['frame']
                expected+=['trigger.close','control.close']
                self.assertEqual(events,expected)

    def test_entry_cancellation_constructs_no_owners(self):
        lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('theresa_main_body.lua').read_text())
        q=lua.eval('{NewScriptFrame=function() end, IsActiveThreadTerminating=function() return true end}')
        phase(q,17,lua.table(),lua.table())
