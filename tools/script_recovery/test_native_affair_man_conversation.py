import re
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_mask as fixtures
from tools.script_recovery.native_affair_wife_mask import recover_affair_man_mask
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_conversation
from tools.script_recovery.lift_native_lua import Lifter


class AffairManConversationTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest,_=fixtures.AffairManMaskTests().inputs()
        source,_=recover_affair_man_mask(fn,source,data)
        return fn,source,data,manifest

    def test_both_paths_preserve_id_owner_flags_and_participant(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_conversation(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        snippets=re.findall(r'uVar8 = GSI->AddNewConversation\(me,false,false\);\s*uStack_128 = uVar8;\s*native_arg_man_conversation_id = uVar8;\s*GSI->AddPersonToConversation\(uVar8,local_native_cached_woman\);',result)
        self.assertEqual(len(snippets),2)
        for snippet in snippets:
            lifter=Lifter(manifest,{},'quest',True,'',data)
            body='\n'.join(lifter.lift('Conversation','{\nuVar9 = GSI->GetHero();\n'+snippet+'\nreturn uVar8;\n}',parameters={'local_native_cached_woman':'thing'}))
            self.assertEqual(lifter.todo,[])
            lua,events=LuaRuntime(),[]
            quest=lua.table_from({'GetHero':lambda q:'hero',
                'AddNewConversation':lambda q,actor,a,b:events.append(('new',actor,a,b)) or 173,
                'AddPersonToConversation':lambda q,ident,actor:events.append(('person',ident,actor))})
            run=lua.execute('return function(quest,me,local_native_cached_woman)\n'+body+'\nend')
            self.assertEqual(run(quest,'husband','woman'),173)
            self.assertEqual(events,[('new','husband',False,False),('person',173,'woman')])

    def test_saved_id_initializes_and_updates_for_both_creation_paths(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_conversation(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertIn('native_arg_man_conversation_id = 0;',result)
        self.assertEqual(result.count('native_arg_man_conversation_id = uVar8;'),2)
        query=re.search(r'cVar5 = GSI->IsConversationActive\(native_arg_man_conversation_id\);',result)[0]
        creations=re.findall(r'uVar8 = GSI->AddNewConversation\(me,false,false\);\s*uStack_128 = uVar8;\s*native_arg_man_conversation_id = uVar8;',result)
        snippet='native_arg_man_conversation_id = 0;\n'+query+'\n'+creations[0]+'\n'+query+'\n'+creations[1]+'\n'+query
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('SavedID','{\n'+snippet+'\n}'))
        self.assertEqual(lifter.todo,[])
        lua,events,ids=LuaRuntime(),[],iter((91,92))
        quest=lua.table_from({'IsConversationActive':lambda q,ident:events.append(('query',ident)) or False,
            'AddNewConversation':lambda q,actor,a,b:next(ids)})
        lua.execute('return function(quest,me)\n'+body+'\nend')(quest,'husband')
        self.assertEqual(events,[('query',0),('query',91),('query',92)])

    def test_changed_source_rejects(self):
        fn,source,data,manifest=self.inputs()
        source+='\n// changed'
        result,evidence=recover_affair_man_conversation(fn,source,data,manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
