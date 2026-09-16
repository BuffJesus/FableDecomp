import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_presented_output import prove
from tools.script_recovery.bully_presented_native import execute

TEDDY='OBJECT_TEDDY_BEAR_UNGIVEABLE'
class BullyPresentedOutputTests(unittest.TestCase):
    def test_original_output_cfg_requires_construction_both_polls_and_cleanup(self):
        report=prove();self.assertEqual(report['stackOffset'],40)
        self.assertEqual(len(report['polls']),2)
        with self.assertRaises(ValueError):prove(omit_cleanup=True)

    def test_native_classifier_preserves_failed_output_empty_success_and_second_poll(self):
        source='''local r=...; if r:Poll() then if r:Matches("OBJECT_TEDDY_BEAR_UNGIVEABLE") then return "teddy" end end
        if not r:Poll() or r:Matches("OBJECT_TEDDY_BEAR_UNGIVEABLE") then return "none" end return "other"'''
        choices=[(False,TEDDY),(False,None),(True,''),(True,TEDDY),(True,'OTHER')]
        for first in choices:
            for second in choices:
                lua=LuaRuntime();r=lua.table();state={'text':'','polls':0};events=[]
                def poll(_):
                    events.append(('poll.input',state['text']));result,text=(first,second)[state['polls']];state['polls']+=1
                    if text is not None:state['text']=text
                    events.append(('poll.output',result,state['text']));return result
                r.Poll=poll;r.Matches=lambda _,name:state['text']==name
                self.assertEqual((lua.execute(source,r),events),execute(first,second))
