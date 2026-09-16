import itertools
import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_new_oakvale import RAW,build
from tools.script_recovery.native_oakvale_main_tail import execute
from tools.script_recovery.readable_oakvale_main import lower
from tools.script_recovery.readable_new_oakvale_main_bindings import lower as bindings
from tools.script_recovery.native_oakvale_objective import lower as objective


class ReadableOakvaleMainTests(unittest.TestCase):
    def test_final_main_tail_order_matches_original_instructions(self):
        with tempfile.TemporaryDirectory() as directory:
            report=build(Path(directory));source=(Path(directory)/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text()
            self.assertTrue(report['syntax']['ok']);self.assertEqual(report['mainBody']['bindingCount'],16)
            self.assertNotIn('function RegisterMain(',source)
            row=next(row for row in report['functions'] if row['function']=='RegisterMain')
            self.assertEqual(row['implementationFunction'],'LuaQuestHost::RegisterMain')
            for attack,terminate in itertools.product((False,True),repeat=2):
                lua=LuaRuntime();lua.execute(source);q,r=lua.table(),lua.table();events=[];names=[]
                def event(*args):events.append(args)
                q.AddEntityBinding=lambda _,name,path:names.append((name,path))
                q.FinalizeEntityBindings=lambda _:(event('bindings.base.finalize'),event('bindings.game.finalize'))
                q.GetStateBool=lambda _,key:(event('attack.over',attack),attack)[1]
                q.IsActiveThreadTerminating=lambda _:(event('term',terminate),terminate)[1]
                q.DeactivateQuest=lambda _,name,zero:event('deactivate',name)
                q.WithRetailResources=lambda _,body:body(r);r.SetInitialOakvaleObjective=lambda _:event('objective')
                q.CreateThread=lambda _,name:event('thread',name);lua.globals().DoMission=lambda _:event('mission')
                lua.globals().Main(q)
                self.assertEqual([name for name,_ in names],report['mainBindings']['bindingNames'])
                self.assertTrue(all(path=='NewOakValeIntro/Entities/'+name for name,path in names))
                self.assertEqual(events,execute(attack,terminate))

    def test_changed_intermediate_main_rejected(self):
        source=(RAW/'FSE/NewOakValeIntro/NewOakValeIntro.lua').read_text();source,_=bindings(source);source,_=objective(source)
        with self.assertRaisesRegex(ValueError,'Main draft changed'):lower(source.replace('FinalizeEntityBindings()','OtherBindings()'))


if __name__=='__main__':unittest.main()
