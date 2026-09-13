import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_question_cleanup as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_question_cleanup,recover_affair_man_home
from tools.script_recovery.lift_native_lua import Lifter,known_callee_aliases,thing_signatures,load_thing_tables


class AffairManHomeTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManQuestionCleanupTests().inputs()
        source,_=recover_affair_man_question_cleanup(fn,source,data,manifest)
        return fn,source,data,manifest

    def lifted_loop(self, include_gate=False):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_home(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        start=result.index('native_arg_man_home_position =')
        end=result.index('goto LAB_00db1c71;',start)+len('goto LAB_00db1c71;')
        snippet=result[start:end]
        if include_gate:
            start=result.rindex('fVar25 = 0.1;',0,start)
            end=result.index('cVar5 = GSI->IsConversationActive',end)
            snippet='pCVar1 = (CScriptThing *)(this + 8);\n'+result[start:end]+'\nreturn 0;'
        lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True,live_termination=True,callee_names=known_callee_aliases(fn),thing_sigs=thing_signatures(load_thing_tables(manifest,{})[0]))
        body='\n'.join(lifter.lift('Home','{\n'+snippet+'\nLAB_00db1c71:\nreturn 1;\nLAB_00db1d7c:\nreturn -99;\n}'))
        self.assertEqual(lifter.todo,[])
        return body

    def test_initial_gate_and_later_home_snapshot_are_distinct(self):
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me)\n'+self.lifted_loop(include_gate=True)+'\nend')
        for outside in (False,True):
            events=[]
            homes=iter((11,22))
            distances=iter((outside,True,False))
            def home(actor):
                value=next(homes)
                events.append(('home',value))
                return lua.table_from({'x':value,'y':0,'z':0})
            def distance(actor,pos,radius):
                events.append(('distance',pos['x'],radius))
                return next(distances)
            me=lua.table_from({'GetHomePos':home,'IsDistanceFromPositionOver':distance,
                'MoveToPosition':lambda actor,pos,*args:events.append(('move',pos['x'],args)),
                'IsPerformingScriptTask':lambda actor:False})
            quest=lua.table_from({'NewScriptFrame':lambda *args:True,
                'IsActiveThreadTerminating':lambda q:False})
            self.assertEqual(run(quest,me),1 if outside else 0)
            expected=[('home',11),('distance',11,0.1)]
            if outside:
                expected += [('home',22),('distance',22,2),('move',22,(0,0,False,True)),('distance',22,2)]
            self.assertEqual(events,expected)

    def test_retry_uses_one_snapshot_and_native_flags(self):
        lua=LuaRuntime()
        body=self.lifted_loop()
        events=[]
        distances=iter((True,True,False))
        tasks=iter((True,False,False))
        home=lua.table_from({'x':17,'y':-4,'z':8})
        def distance(actor,pos,radius):
            events.append(('distance',pos['x'],radius))
            return next(distances)
        me=lua.table_from({'GetHomePos':lambda actor:events.append('home') or home,
            'IsDistanceFromPositionOver':distance,
            'MoveToPosition':lambda actor,pos,*args:events.append(('move',pos['x'],args)),
            'IsPerformingScriptTask':lambda actor:events.append('task') or next(tasks)})
        quest=lua.table_from({'NewScriptFrame':lambda *args:events.append('frame') or True,
            'IsActiveThreadTerminating':lambda q:events.append('termination') or False})
        self.assertEqual(lua.execute('return function(quest,me)\n'+body+'\nend')(quest,me),1)
        self.assertEqual(events,['home',('distance',17,2),'frame','termination',('move',17,(0,0,False,True)),
            'task','frame','termination','task','termination',('distance',17,2),
            'frame','termination',('move',17,(0,0,False,True)),'task','termination',('distance',17,2),'termination'])

    def test_cancellation_before_movement_or_during_task_wait(self):
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me)\n'+self.lifted_loop()+'\nend')
        for stop_at in (1,2,3):
            events=[]
            checks=[]
            def terminate(q):
                checks.append(True)
                events.append('termination')
                return len(checks)==stop_at
            me=lua.table_from({'GetHomePos':lambda actor:lua.table_from({'x':9,'y':0,'z':0}),
                'IsDistanceFromPositionOver':lambda *args:events.append('distance') or True,
                'MoveToPosition':lambda *args:events.append('move'),
                'IsPerformingScriptTask':lambda actor:events.append('task') or len(checks)<2})
            quest=lua.table_from({'NewScriptFrame':lambda *args:events.append('frame') or True,
                'IsActiveThreadTerminating':terminate})
            self.assertEqual(run(quest,me),-99)
            expected=['distance','frame','termination']
            if stop_at>=2:
                expected+=['move','task','frame','termination']
            if stop_at==3:
                expected+=['task','termination']
            self.assertEqual(events,expected)

    def test_already_within_retry_radius_skips_frames_and_movement(self):
        lua=LuaRuntime()
        events=[]
        me=lua.table_from({'GetHomePos':lambda actor:events.append('home') or lua.table(),
            'IsDistanceFromPositionOver':lambda actor,pos,radius:events.append(('distance',radius)) or False})
        quest=lua.table_from({'IsActiveThreadTerminating':lambda q:events.append('termination') or False})
        run=lua.execute('return function(quest,me)\n'+self.lifted_loop()+'\nend')
        self.assertEqual(run(quest,me),1)
        self.assertEqual(events,['home',('distance',2),'termination'])

    def test_changed_native_movement_flag_rejects(self):
        fn,source,data,manifest=self.inputs()
        self.assertEqual(data.bytes_at(0xdb175b,2),b'\x6a\x01')
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb175c-address]=0
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_home(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
