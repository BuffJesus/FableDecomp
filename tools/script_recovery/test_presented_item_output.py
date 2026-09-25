import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.annotate_interface_slots import load_thing_slots
from tools.script_recovery.test_lift_native_lua import make


class PresentedItemOutputTests(unittest.TestCase):
    def lifter(self):
        lifter = make(entity=True)
        lifter.manifest = dict(lifter.manifest, MsgIsPresentedWithItem={
            'scope': 'Entity', 'returnType': 'bool', 'parameters': []})
        lifter.thing_sigs = {name: sig for name, sig in load_thing_slots().values()}
        return lifter

    def body(self, target):
        lifter = self.lifter()
        lifter.locals.add('xStack_178')
        lifter.kinds['xStack_178'] = 'string'
        lifter.thing_call(target, 'MsgIsPresentedWithItem', 'me', '&xStack_178')
        return '\n'.join(lifter.out)

    def test_message_copies_the_binding_output_only_on_success(self):
        for accepted in (True, False):
            with self.subTest(accepted=accepted):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.globals().accepted = accepted
                result = lua.execute('''
                    local xStack_178 = "old"
                    g_PresentedItemName = "stale"
                    local calls = 0
                    local me = {MsgIsPresentedWithItem = function()
                        calls = calls + 1
                        if accepted then g_PresentedItemName = "item" end
                        return accepted
                    end}
                ''' + self.body('presented') + '\nreturn xStack_178, presented, calls')
                self.assertEqual(result, ('item' if accepted else 'old', accepted, 1))

    def test_ignored_boolean_still_copies_the_output(self):
        lua = LuaRuntime()
        result = lua.execute('''
            local xStack_178 = "old"
            local me = {MsgIsPresentedWithItem = function()
                g_PresentedItemName = "item"
                return true
            end}
        ''' + self.body(None) + '\nreturn xStack_178')
        self.assertEqual(result, 'item')

    def test_false_message_preserves_a_staged_string_constructor(self):
        body = '\n'.join(self.lifter().lift('Main', '{\n'
            'CCharString::CCharString(&xStack_178,"old",-1);\n'
            'bVar3 = CScriptThing::MsgIsPresentedWithItem((CScriptThing *)(this + 8), &xStack_178);\n}'))
        lua = LuaRuntime()
        result = lua.execute('local me = {MsgIsPresentedWithItem = function() return false end}\n'
                             + body + '\nreturn xStack_178')
        self.assertEqual(result, 'old')
