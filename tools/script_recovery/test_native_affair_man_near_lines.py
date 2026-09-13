import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_conversation as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_conversation,recover_affair_man_near_lines
from tools.script_recovery.lift_native_lua import Lifter


class AffairManNearLinesTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManConversationTests().inputs()
        source,_=recover_affair_man_conversation(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_keys_order_roles_and_cancellation(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_near_lines(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        snippet=evidence[0]['edits'][0]['new']
        self.assertIn(snippet,result)
        # Supply the recovered native field value as a parameter for this isolated
        # line-queue test. Counter wrap and actor cleanup are outside its scope.
        snippet=snippet.replace('native_arg_man_line_index = *(int *)(param_1 + 0x20);','')
        lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True,live_termination=True)
        body='\n'.join(lifter.lift('NearLines','{\n'+snippet+'\nreturn true;\nLAB_00db1d7c:\nreturn false;\n}',parameters={'native_arg_man_line_index':'number','uVar8':'number','local_native_cached_woman':'thing'}))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me,native_arg_man_line_index,uVar8,local_native_cached_woman)\n'+body+'\nend')
        for index in (10,20,30,40,50):
            for terminating in (False,True):
                events=[]
                quest=lua.table_from({'IsActiveThreadTerminating':lambda q:events.append(('termination',)) or terminating,
                    'AddLineToConversation':lambda q,ident,key,speaker,listener,flag:events.append((ident,key,speaker,listener,flag))})
                self.assertEqual(run(quest,'man',index,173,'woman'),not terminating)
                expected=[('termination',)]
                if not terminating:
                    man=(173,'TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_'+str(index),'man','woman',False)
                    woman=(173,'TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_'+str(index),'woman','man',False)
                    expected.extend((woman,man) if index==10 else (man,woman))
                self.assertEqual(events,expected)

    def test_changed_prefix_rejects(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                return bytes(size) if address==0x12d8aa0 else data.bytes_at(address,size)
        result,evidence=recover_affair_man_near_lines(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
