import copy
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import Lifter, converter_signatures


class MovementSignatureTests(unittest.TestCase):
    def manifest(self):
        return {'MoveToPosition': {'scope':'Entity', 'returnType':'void', 'blocking':True,
            'parameters':[{'name':name,'type':kind} for name,kind in (
                ('pMe','CScriptThing*'),('position','sol::table'),('radius','float'),('moveType','int'))]}}

    def test_overlay_is_narrow_immutable_and_idempotent(self):
        manifest=self.manifest()
        original=copy.deepcopy(manifest)
        updated=converter_signatures(manifest)
        self.assertEqual(manifest,original)
        self.assertFalse(updated['MoveToPosition']['blocking'])
        self.assertEqual(len(updated['MoveToPosition']['parameters']),6)
        self.assertEqual(converter_signatures(updated),updated)
        manifest['MoveToPosition']['parameters'][3]['name']='customMovement'
        self.assertEqual(converter_signatures(manifest)['MoveToPosition'],manifest['MoveToPosition'])

    def test_flags_and_movement_arguments_survive_without_added_wait(self):
        lua=LuaRuntime()
        for flags in ((False,False),(False,True),(True,False),(True,True)):
            args=','.join('true' if x else 'false' for x in flags)
            lifter=Lifter(converter_signatures(self.manifest()),{},'quest',True,'',None)
            body='\n'.join(lifter.lift('Move','{\nGSI->MoveToPosition(native_arg_position,0.25,2,'+args+');\n}',parameters={'native_arg_position':'vector'}))
            self.assertEqual(lifter.todo,[])
            events=[]
            me=lua.table_from({'MoveToPosition':lambda actor,pos,*values:events.append((pos['x'],pos['y'],pos['z'],values))})
            position=lua.table_from({'x':12,'y':-3,'z':7})
            lua.execute('return function(quest,me,native_arg_position)\n'+body+'\nend')(lua.table(),me,position)
            self.assertEqual(events,[(12,-3,7,(0.25,2,*flags))])

    def test_omitted_optional_arguments_use_host_defaults(self):
        lifter=Lifter(converter_signatures(self.manifest()),{},'quest',True,'',None)
        body='\n'.join(lifter.lift('Move','{\nGSI->MoveToPosition(native_arg_position,2.0);\n}',parameters={'native_arg_position':'vector'}))
        self.assertEqual(lifter.todo,[])
        lua=LuaRuntime()
        events=[]
        me=lua.table_from({'MoveToPosition':lambda actor,pos,*args:events.append(args)})
        lua.execute('return function(quest,me,native_arg_position)\n'+body+'\nend')(lua.table(),me,lua.table())
        self.assertEqual(events,[(2,)])
