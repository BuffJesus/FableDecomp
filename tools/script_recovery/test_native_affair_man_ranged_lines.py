import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_near_lines as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_near_lines,recover_affair_man_ranged_lines
from tools.script_recovery.lift_native_lua import Lifter


class AffairManRangedLinesTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManNearLinesTests().inputs()
        source,_=recover_affair_man_near_lines(fn,source,data,manifest)
        return fn,source,data,manifest

    def test_ranged_keys_roles_and_shared_cleanup_fallthrough(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_ranged_lines(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertNotIn('LAB_00db1b4f',result)
        self.assertNotIn('GFIntToCharString_API()',result)
        snippet=evidence[0]['edits'][0]['new']
        self.assertIn(snippet,result)
        snippet=snippet.replace('native_arg_man_ranged_index = *(int *)(param_1 + 0x20);','')
        lifter=Lifter(manifest,{},'quest',True,'',data,live_termination=True)
        body='\n'.join(lifter.lift('Ranged','{\n'+snippet+'\nreturn 73;\n}',parameters={'native_arg_man_ranged_index':'number','uVar8':'number','local_native_cached_woman':'thing'}))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me,native_arg_man_ranged_index,uVar8,local_native_cached_woman)\n'+body+'\nend')
        for index in (10,20,30,40,50):
            events=[]
            quest=lua.table_from({'AddLineToConversation':lambda q,ident,key,speaker,listener,flag:events.append((ident,key,speaker,listener,flag)),
                'IsActiveThreadTerminating':lambda q:self.fail('Unexpected termination check inside ranged line sequence')})
            self.assertEqual(run(quest,'man',index,173,'woman'),73)
            self.assertEqual(events,[(173,'TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_'+str(index),'man','woman',False),
                (173,'TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_'+str(index),'woman','man',False)])

    def test_changed_prefix_rejects(self):
        fn,source,data,manifest=self.inputs()
        class Changed:
            def bytes_at(self,address,size):
                return bytes(size) if address==0x12d8a48 else data.bytes_at(address,size)
        result,evidence=recover_affair_man_ranged_lines(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
