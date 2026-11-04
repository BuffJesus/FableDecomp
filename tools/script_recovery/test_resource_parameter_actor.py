from pathlib import Path
import sys
import unittest
from lupa import LuaRuntime

W = Path(__file__).resolve().parent
sys.path.insert(0, str(W.parents[1]))
from tools.script_recovery.test_lift_native_lua import NoRData
from tools.script_recovery import lift_native_lua as candidate

def lift(operand, parameters):
    lifter = candidate.Lifter({}, {}, 'quest', False, 'TraderEscort', NoRData())
    body = '\n'.join(lifter.lift('Main', '{\nresource = RESOURCE_NewResource();\n'
        f'acquired = RESOURCE_TryAcquire(resource, {operand}, 4);\nreturn acquired;\n}}',
        parameters=parameters))
    return body, lifter.todo

class ResourceParameterTests(unittest.TestCase):
    def test_known_actor_identity_and_failed_acquisition(self):
        for spelling in ('&native_arg_Me', '(CScriptThing *)&native_arg_Me', '(CScriptThing_bv *)&native_arg_Me'):
            body, todo = lift(spelling, {'native_arg_Me': 'thing'})
            self.assertFalse(todo)
            for available in (False, True):
                lua = LuaRuntime()
                run = lua.execute('''
local actor, resource, calls = {}, {}, 0
function RESOURCE_NewResource() return resource end
function RESOURCE_TryAcquire(r, a, p)
    assert(r == resource and a == actor and p == 4)
    calls = calls + 1
    return AVAILABLE
end
return function()
    local result = (function(native_arg_Me)
BODY
    end)(actor)
    assert(calls == 1)
    return result
end
'''.replace('AVAILABLE', 'true' if available else 'false').replace('BODY', body))
                self.assertEqual(run(), available)

    def test_untyped_or_non_actor_parameter_stays_unresolved(self):
        for params in ({}, {'native_arg_Me': 'number'}, {'native_arg_Me': 'string'}):
            body, todo = lift('&native_arg_Me', params)
            self.assertTrue(todo)
            self.assertIn('-- TODO(native):', body)

    def test_unknown_local_address_stays_unresolved(self):
        body, todo = lift('&local_actor', {'native_arg_Me': 'thing'})
        self.assertTrue(todo)
        self.assertIn('-- TODO(native):', body)

if __name__ == '__main__':
    unittest.main()
