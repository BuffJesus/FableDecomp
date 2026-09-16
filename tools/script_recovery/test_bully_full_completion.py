"""Actual generated Init/Main composition through four hits and runoff, disabled."""
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_full_resource_candidate import generate
from tools.script_recovery.test_bully_full_resource_candidate import HARNESS

EXTRA='''
    function resources:InitializeBullyActor(actor) assert(actor==me);rec('init.actor') end
    local fields={GUIBullyHealthCounter=-999,TalkIntermittentTimer=77}
    function quest:GetStateInt(key) return assert(fields[key]) end
    function quest:SetStateInt(key,value) fields[key]=value;rec('int:'..key..':'..value) end
    function quest:GetStateBool(key) return fields[key] or false end
    function quest:SetStateBool(key,value) fields[key]=value;rec('state:'..key) end
    function quest:GetTimer(id) assert(id==77);return 0 end
    local talks=0
    function me:IsTalkedToByHero() talks=talks+1;return talks==1 end
    function resources:PollPresentedItem(id) assert(entries[id].live);polls=polls+1;return false end
    function resources:IsHitByHeroExceptAbility(actor,ability) assert(actor==me and ability==14);return true end
    function resources:AddBullyHealthBar(value) assert(value==4);rec('hud.new');return 73 end
    function resources:SetThingAsAlly(first,second) assert(first and second);rec('ally') end
    function resources:NewConversation(actor,a,b) assert(actor==me and not a and not b);return 91 end
    function resources:AddConversationPerson(id,thing) assert(id==91 and entries[thing].live) end
    function resources:AddConversationLine(id,key,actor,target,flag) assert(id==91 and actor==me and entries[target].kind=='victim' and not flag) end
    function resources:FaceRetainedThing(actor,target,snap) assert(actor==me and entries[target].live and snap) end
    function resources:ReadBullyProximityRange() return 2.5 end
    function resources:IsDistanceBetweenThingsUnder(a,b,range) assert(a==me and b and range==2.5);return false end
    function resources:TryAcquireThing(resource,thing,priority) assert(entries[resource].live and entries[thing].kind=='victim' and priority==4);return true end
    function resources:NewActorMap() return add('actors') end
    function resources:SetActor(map,key,resource) assert(entries[map].live and entries[resource].live);rec('map:'..key) end
    function resources:DestroyActorMap(id) destroy(id) end
    function resources:NewStringMap() return add('strings') end
    function resources:SetString(map,key,value) assert(entries[map].live and key=='$BRATLINE' and value=='TEXT_QST_048_VICTIM_THANKS') end
    function resources:DestroyStringMap(id) destroy(id) end
    function resources:RunMacroWithStrings(name,map,inputs,setup,skip)
        assert(name=='CS_OAKVALEINTRO_BULLYRUN1' and entries[map].live and entries[inputs].live and not setup and skip)
        rec('macro:first');if mode=='macro_error' then error('NATIVE_MACRO_FAULT') end
    end
    function resources:RunMacro(name,map,setup,skip) assert(name=='CS_OAKVALEINTRO_BULLYRUN2' and entries[map].live and not setup and skip);rec('macro:second') end
    function resources:ClearThingHasInformation(id) assert(entries[id].live and entries[id].kind=='victim');rec('clear:victim') end
    function quest:RemoveThing(actor,a,b) assert(actor==me and not a and b);rec('remove:bully') end
    package.preload['NewOakValeIntro.native_quest_helpers']=function() return {AddGoodDeed=function() rec('good.deed') end} end
'''

class BullyFullCompletionTests(unittest.TestCase):
    def test_generated_init_main_four_hits_then_native_ordered_runoff(self):
        source,_=generate()
        for mode in ('normal','macro_error'):
            harness=HARNESS.replace("    source=source..",EXTRA+"\n    source=source..",1)
            harness=harness.replace('fn();fixture()','fn();Init(quest,me)')
            lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
            events,ok,error,_=lua.execute(harness)(source,mode,1.0);events=list(events.values())
            self.assertEqual(ok,mode=='normal',error)
            self.assertEqual(events.count('hud.new'),1);self.assertEqual(events.count('ally'),8)
            self.assertEqual(events.count('api:UpdateQuestInfoBar'),3)
            self.assertIn('state:BullySubdued',events)
            self.assertEqual(events.count('destroy:presented'),3)
            if mode=='normal':
                self.assertIn('macro:second',events)
                tail=events[events.index('destroy:strings'):]
                self.assertEqual(tail,['destroy:strings','destroy:actors','destroy:control','destroy:control','state:BullyRanOff','good.deed','remove:bully','destroy:victim','destroy:control'])
            else:
                self.assertIn('NATIVE_MACRO_FAULT',error)
                self.assertNotIn('state:BullyRanOff',events)
                self.assertEqual(events[-8:],['pause:false','destroy:movie','destroy:strings','destroy:actors','destroy:control','destroy:control','destroy:victim','destroy:control'])

if __name__=='__main__':unittest.main()
