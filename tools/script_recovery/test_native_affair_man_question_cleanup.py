import re
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_question as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_question,recover_affair_man_question_cleanup
from tools.script_recovery.lift_native_lua import Lifter,known_callee_aliases


class AffairManQuestionCleanupTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManQuestionTests().inputs()
        source,_=recover_affair_man_question(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_all_nineteen_suffixes_preserve_conditional_cleanup(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_question_cleanup(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertEqual(len(evidence[0]['edits']),19)
        lua=LuaRuntime()
        for edit in evidence[0]['edits']:
            self.assertIn(edit['new'],result)
            snippet=re.sub(r'^if \([^\n]+\)', 'if (native_arg_stop)',edit['new'])
            lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True)
            body='\n'.join(lifter.lift('Cleanup','{\n'+snippet+'\nreturn 1;\nLAB_00db1d7c:\nreturn -1;\n}',parameters={'native_arg_stop':'bool'}))
            self.assertEqual(lifter.todo,[])
            run=lua.execute('return function(quest,me,native_arg_stop)\n'+body+'\nend')
            for stop in (False,True):
                events=[]
                quest=lua.table_from({'PauseAllNonScriptedEntities':lambda q,flag:events.append(('pause',flag)),
                    'EndMovieSequence':lambda q:events.append(('end',))})
                self.assertEqual(run(quest,'man',stop),-1 if stop else 1)
                self.assertEqual(events,[('pause',False),('end',)] if stop else [])

    def test_answer_wait_cleans_up_on_cancellation_before_repoll(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_question_cleanup(fn,source,data,manifest)
        start=result.index('iVar10 = GSI->MsgIsQuestionAnsweredYesOrNo();')
        stop=re.search(r'uVar6 = \(undefined1\)uVar8;\s*}',result[start:]).end()+start
        snippet=result[start:stop]
        lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True,live_termination=True,callee_names=known_callee_aliases(fn))
        body='\n'.join(lifter.lift('Wait','{\n'+snippet+'\nreturn iVar10;\nLAB_00db1d7c:\nreturn -99;\n}'))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me)\n'+body+'\nend')
        for terminating in (False,True):
            answers=iter((-1,1))
            events=[]
            quest=lua.table_from({'MsgIsQuestionAnsweredYesOrNo':lambda q:events.append('answer') or next(answers),
                'NewScriptFrame':lambda *args:events.append('frame') or True,
                'IsActiveThreadTerminating':lambda q:events.append('termination') or terminating,
                'PauseAllNonScriptedEntities':lambda q,flag:events.append(('pause',flag)),
                'EndMovieSequence':lambda q:events.append('end')})
            self.assertEqual(run(quest,'man'),-99 if terminating else 1)
            self.assertEqual(events,['answer','frame','termination']+([('pause',False),'end'] if terminating else ['answer']))

    def test_changed_prepared_unpause_flag_rejects(self):
        fn,source,data,manifest=self.inputs()
        self.assertEqual(data.bytes_at(0xdb1176,2),b'\x6a\x00')
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb1177-address]=1
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_question_cleanup(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
