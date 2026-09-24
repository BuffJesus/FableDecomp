import unittest

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, RData, load_manifest


class NoRData(RData):
    def __init__(self):
        self.ok = False


class ResourceMovementOperandsTests(unittest.TestCase):
    def test_complete_native_operands_keep_position_radius_and_both_flags(self):
        lifter = Lifter(load_manifest(), {}, 'quest', True, 'Test', NoRData(), execution_entity=True)
        lifter.accessor_kinds = True
        source = '''{
CScriptGameResourceObjectScriptedThingBase::_MoveToPosition_CScriptGameResourceObjectScriptedThingBase__UAEXABVC3DVector__MW4EScriptEntityMoveType___N2_Z(resource,(int)&position,0x3f800000,1,0,1);
}'''
        body = '\n'.join(lifter.lift('Main', source))
        self.assertIn('me:MoveToPosition(position, 1.0, 1, false, true)', body)
        self.assertFalse(any('missing' in todo for todo in lifter.todo))
        lua = LuaRuntime(unpack_returned_tuples=True)
        function = lua.execute('return function(me, position)\n' + body + '\nend')
        calls = []
        position = lua.table_from({'x': 2, 'y': 3, 'z': 4})
        def move(me, point, radius, kind, first, second):
            calls.append((point['x'], point['y'], point['z'], radius, kind, first, second))
        function(lua.table_from({'MoveToPosition': move}), position)
        self.assertEqual(calls, [(2, 3, 4, 1.0, 1, False, True)])


if __name__ == '__main__':
    unittest.main()
