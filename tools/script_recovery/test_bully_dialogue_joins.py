import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_full_resource_candidate import generate
from tools.script_recovery.test_bully_full_resource_candidate import HARNESS

class BullyDialogueJoinTests(unittest.TestCase):
    def test_question_cancellation_before_movie_or_while_waiting(self):
        source,_=generate()
        for before in (True,False):
            harness=HARNESS.replace("local resources={}","local resources={}\n    local afterPredicate,asked=false,false")
            harness=harness.replace("return mode=='question' end", "afterPredicate=true;return true end")
            harness=harness.replace("function quest:IsActiveThreadTerminating() return false end",
                "function quest:IsActiveThreadTerminating() return "+('afterPredicate' if before else 'asked')+" end")
            harness=harness.replace("function quest:GiveHeroYesNoQuestion(...) rec('question');error('QUESTION') end",
                "function quest:GiveHeroYesNoQuestion(...) rec('question');asked=true end\n    function quest:MsgIsQuestionAnsweredYesOrNo() return -1 end")
            lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
            events,ok,error,polls=lua.execute(harness)(source,'question',1.0);events=list(events.values())
            self.assertTrue(ok,error);self.assertEqual(polls,0)
            self.assertEqual(events[-3:],['destroy:presented','destroy:victim','destroy:control'])
            self.assertEqual(events.count('destroy:movie'),0 if before else 1)
            self.assertEqual(events.count('pause:false'),0 if before else 1)

    def test_yes_answer_waits_for_victim_without_decompiler_sub41(self):
        source,_=generate();self.assertNotIn('SUB41(',source)
        harness=HARNESS.replace("local resources={}","local resources={}\n    local complaints=0")
        harness=harness.replace("function quest:GiveHeroYesNoQuestion(...) rec('question');error('QUESTION') end",
            "function quest:GiveHeroYesNoQuestion(q,y,n,empty,flag) assert(flag and empty=='');rec('question') end\n    function quest:MsgIsQuestionAnsweredYesOrNo() return 1 end")
        harness=harness.replace("function quest:GetStateBool(name) return false end",
            "function quest:GetStateBool(name) if name=='VictimComplainsAboutLosingTeddy' then complaints=complaints+1;return complaints==1 end;return false end")
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
        events,ok,error,_=lua.execute(harness)(source,'question',1.0);events=list(events.values())
        self.assertFalse(ok);self.assertIn('AFTER_ITEM',error)
        self.assertIn('bad.deed',events)
        self.assertEqual(events[-2:],['destroy:victim','destroy:control'])
        self.assertEqual(events.count('destroy:movie'),1);self.assertEqual(events.count('destroy:presented'),1)

    def test_nasty_streak_post_speech_cancel_closes_movie_before_hit_processing(self):
        source,_=generate()
        harness=HARNESS.replace("local resources={}","local resources={}\n    local spoke=false")
        harness=harness.replace("return true end\n    function resources:PresentedItemMatches", "return false end\n    function resources:PresentedItemMatches")
        harness=harness.replace("rec('speak:'..key) end", "rec('speak:'..key);if key=='TEXT_QST_048_BULLY_NASTY_STREAK' then spoke=true end end")
        harness=harness.replace("function me:IsTalkedToByHero() error('AFTER_ITEM') end", "function me:IsTalkedToByHero() return true end")
        harness=harness.replace("function quest:IsActiveThreadTerminating() return false end", "function quest:IsActiveThreadTerminating() return spoke end")
        harness=harness.replace("function quest:GetStateBool(name) return false end", "function quest:GetStateBool(name) return name=='HeroAttackedVictim' end")
        harness=harness.replace('SetStateBool("DoneIntro",true) end', 'SetStateBool("DoneIntro",true); __native_entity_state:SetStateBool("SaidPieceAboutAttackingVictim",true); __native_entity_state:SetStateInt("HitsTaken",0) end')
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
        events,ok,error,polls=lua.execute(harness)(source,'nasty',1.0)
        events=list(events.values());self.assertTrue(ok,error);self.assertEqual(polls,2)
        self.assertIn('speak:TEXT_QST_048_BULLY_NASTY_STREAK',events)
        self.assertEqual(events[-4:],['pause:false','destroy:movie','destroy:victim','destroy:control'])

if __name__=='__main__':unittest.main()
