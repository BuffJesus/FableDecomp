import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, RData, converter_signatures, load_manifest


class NativeRegionUnloadTests(unittest.TestCase):
    def test_native_result_controls_branch_and_forwards_region(self):
        lifter=Lifter(load_manifest(),{},'quest',False,'',RData())
        source='''{
        bool bVar2;
        bVar2 = false;
        bVar2 = GSI->MsgIsRegionUnloaded("HeroGuildComplexInside");
        if (bVar2) {
            return 1;
        }
        return 0;
        }'''
        body='\n'.join(lifter.lift('Watch',source))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        run=lua.execute('return function(quest)\n'+body+'\nend')
        for answer in (False,True):
            seen=[]
            q=lua.table_from({'MsgIsRegionUnloaded':lambda _,name:seen.append(name) or answer})
            self.assertEqual(run(q),int(answer))
            self.assertEqual(seen,['HeroGuildComplexInside'])

    def test_existing_manifest_signature_takes_precedence(self):
        supplied={'MsgIsRegionUnloaded':{'scope':'Quest','returnType':'bool','parameters':[{'name':'region','type':'std::string'}]}}
        self.assertEqual(converter_signatures(supplied)['MsgIsRegionUnloaded'],supplied['MsgIsRegionUnloaded'])

    def test_missing_region_remains_an_operand_diagnostic(self):
        lifter=Lifter(load_manifest(),{},'quest',False,'',RData())
        lifter.lift('Watch','{\nbVar2 = GSI->MsgIsRegionUnloaded();\n}')
        self.assertTrue(any('missing regionName' in note for note in lifter.todo),lifter.todo)
