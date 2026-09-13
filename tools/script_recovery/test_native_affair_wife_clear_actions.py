import re
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_animation_operands as fixtures
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter, converter_signatures


class AffairWifeClearActionsTests(unittest.TestCase):
    def test_native_resource_and_emitted_clear_order(self):
        fn,source,data,manifest=fixtures.AffairWifeAnimationOperandsTests().inputs()
        # Both native wrappers read resource16; actions and commands are separate
        # vtable operations. Do not merge them or accept the guessed extra args.
        for site,target,slot in ((0xdb3a16,0x7e7400,0x54),(0xdb3a1f,0x7e7360,0x28)):
            setup=read_call_window(data,0xdb2b10,4890,site,argument_count=0)
            self.assertEqual(setup.ecx,('stack',16))
            self.assertEqual(setup.target,('constant',target))
            self.assertEqual(data.bytes_at(target,13),bytes.fromhex('8b490885c974058b01ff60')+bytes([slot])+b'\xc3')
        match=re.search(r'CScriptGameResourceObjectScriptedThingBase::\s*_ClearAllActions[\s\S]*?;',source)
        self.assertIsNotNone(match)
        rest=source[match.end():]
        next_call=re.search(r'CScriptGameResourceObjectScriptedThingBase::\s*_ClearCommands[\s\S]*?;',rest)
        self.assertIsNotNone(next_call)
        lifter=Lifter(manifest,{},'quest',True,'',data)
        body='\n'.join(lifter.lift('ClearActions','{\n'+match[0]+'\n'+next_call[0]+'\n}'))
        self.assertEqual(lifter.todo,[])
        lua,events=LuaRuntime(),[]
        me=lua.table_from({'ClearAllActions':lambda actor:events.append('actions'),
                           'ClearCommands':lambda actor:events.append('commands')})
        lua.execute('return function(quest,me)\n'+body+'\nend')(lua.table(),me)
        self.assertEqual(events,['actions','commands'])

    def test_existing_contract_is_preserved(self):
        custom={'scope':'Entity','returnType':'bool','parameters':[]}
        manifest={'ClearAllActions':custom}
        self.assertEqual(converter_signatures(manifest)['ClearAllActions'],custom)
        self.assertEqual(manifest,{'ClearAllActions':custom})
