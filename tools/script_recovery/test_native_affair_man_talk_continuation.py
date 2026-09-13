import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_question_entry as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_question_entry,recover_affair_man_talk_continuation
from tools.script_recovery.lift_native_lua import Lifter


class AffairManTalkContinuationTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManQuestionEntryTests().inputs()
        source,_=recover_affair_man_question_entry(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_all_three_jumps_preserve_continuation_and_cleanup(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_talk_continuation(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertEqual(sum(e['count'] for e in evidence[0]['edits']),3)
        lua=LuaRuntime()
        for edit in evidence[0]['edits']:
            self.assertNotIn(edit['old'],result)
            checked='1144' in edit['old']
            lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True,live_termination=True)
            suffix='\nLAB_00db1593:\nreturn 1;\n'+('LAB_00db1d7c:\nreturn -99;\n' if checked else '')
            body='\n'.join(lifter.lift('Continue','{\n'+edit['new']+suffix+'}'))
            self.assertEqual(lifter.todo,[])
            run=lua.execute('return function(quest,me)\n'+body+'\nend')
            for stop in (False,True):
                events=[]
                quest=lua.table_from({'IsActiveThreadTerminating':lambda q:events.append('termination') or stop,
                    'ClearThingHasInformation':lambda q,actor:events.append(('clear',actor)),
                    'PauseAllNonScriptedEntities':lambda q,flag:events.append(('pause',flag)),
                    'EndMovieSequence':lambda q:events.append('end')})
                self.assertEqual(run(quest,'husband'),-99 if checked and stop else 1)
                self.assertEqual(events,(['termination'] if checked else [])+
                    ([('pause',False),'end'] if checked and stop else [('clear','husband')]))

    def test_changed_native_clear_receiver_rejects(self):
        fn,source,data,manifest=self.inputs()
        self.assertEqual(data.bytes_at(0xdb1158,1),b'\x55')
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb1158-address]=0x56
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_talk_continuation(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
