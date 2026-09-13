import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_hero_distance as fixtures
from tools.script_recovery.native_hero_distance import recover_affair_man_hero_distances
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_question
from tools.script_recovery.lift_native_lua import Lifter,converter_signatures


class AffairManQuestionTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManHeroDistanceTests().inputs()
        source,_=recover_affair_man_hero_distances(fn,source,data)
        return fn,source,data,manifest

    def test_prompt_labels_and_flag_in_order_without_answer_return(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_question(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        call=evidence[0]['edits'][0]['new']
        self.assertIn(call,result)
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('Question','{\n'+call+'\n}'))
        self.assertEqual(lifter.todo,[])
        self.assertNotIn('= quest:GiveHeroYesNoQuestion',body)
        lua,events=LuaRuntime(),[]
        quest=lua.table_from({'GiveHeroYesNoQuestion':lambda q,*args:events.append(args)})
        lua.execute('return function(quest,me)\n'+body+'\nend')(quest,'man')
        self.assertEqual(events,[('TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL','TEXT_OBJECT_HERO_ANSWER_YES','TEXT_OBJECT_HERO_ANSWER_NO','',True)])

    def test_signature_overlay_preserves_input_and_newer_contract(self):
        old={'scope':'Quest','returnType':'int','parameters':[{'name':n,'type':'const std::string&'} for n in ('question','button1','button2','button3')]}
        updated=converter_signatures({'GiveHeroYesNoQuestion':old})
        self.assertEqual(old['returnType'],'int')
        self.assertEqual(len(old['parameters']),4)
        self.assertEqual(updated['GiveHeroYesNoQuestion']['returnType'],'void')
        self.assertEqual(len(updated['GiveHeroYesNoQuestion']['parameters']),5)
        self.assertEqual(converter_signatures(updated),updated)
