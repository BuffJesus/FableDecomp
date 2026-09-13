import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_home as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_home,recover_affair_man_question_entry
from tools.script_recovery.lift_native_lua import Lifter,known_callee_aliases,load_entity_parent_state


class AffairManQuestionEntryTests(unittest.TestCase):
    def inputs(self):
        fn,source,data,manifest=fixtures.AffairManHomeTests().inputs()
        source,_=recover_affair_man_home(fn,source,data,manifest)
        return fn,source,data,manifest

    def entry(self):
        fn,source,data,manifest=self.inputs()
        result,evidence=recover_affair_man_question_entry(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        self.assertNotIn('LAB_00db1243',result)
        snippet=evidence[0]['edits'][0]['new']
        end=snippet.index(';',snippet.index('GSI->GiveHeroYesNoQuestion'))+1
        snippet=snippet[:end]
        lifter=Lifter(manifest,{},'quest',True,'',data,native_gotos=True,live_termination=True,callee_names=known_callee_aliases(fn),parent_state=load_entity_parent_state('NOVI_AffairMan'))
        body='\n'.join(lifter.lift('Entry','{\n'+snippet+'\nreturn 1;\nLAB_00db1d7c:\nreturn -99;\n}',parameters={'fVar3':'number','fVar18':'number'}))
        self.assertEqual(lifter.todo,[])
        return body

    def test_health_gate_speech_completion_and_cancellation(self):
        lua=LuaRuntime()
        run=lua.execute('return function(quest,me,fVar3,fVar18)\n'+self.entry()+'\nend')
        for health in (-1,0,0.25,100,float('nan')):
            for stop_at in (0,1,2):
                with self.subTest(health=health,stop_at=stop_at):
                    events=[]
                    tasks=iter((True,False))
                    checks=[]
                    def terminating(q):
                        checks.append(True)
                        events.append('termination')
                        return len(checks)==stop_at
                    quest=lua.table_from({'GetHero':lambda q:events.append('hero') or 'hero',
                        'NewScriptFrame':lambda *args:events.append('frame') or True,
                        'IsActiveThreadTerminating':terminating,
                        'PauseAllNonScriptedEntities':lambda q,flag:events.append(('pause',flag)),
                        'EndMovieSequence':lambda q:events.append('end'),
                        'SetStateBool':lambda q,key,flag:events.append(('state',key,flag)),
                        'GiveHeroYesNoQuestion':lambda q,*args:events.append(('question',args))})
                    me=lua.table_from({'Speak':lambda actor,hero,key,*args:events.append(('speak',hero,key)),
                        'IsPerformingScriptTask':lambda actor:events.append('task') or next(tasks)})
                    cancelled=health>0 and stop_at!=0
                    self.assertEqual(run(quest,me,0,health),-99 if cancelled else 1)
                    expected=[]
                    if health>0:
                        expected=['hero',('speak','hero','TEXT_QST_048_AFFAIRMAN_INTRO'),'task','frame','termination']
                        if stop_at!=1: expected+=['task','termination']
                    if cancelled: expected += [('pause',False),'end']
                    else: expected += [('state','HeroDiscoveredInfidelity',True),('question',('TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL','TEXT_OBJECT_HERO_ANSWER_YES','TEXT_OBJECT_HERO_ANSWER_NO','',True))]
                    self.assertEqual(events,expected)

    def test_changed_native_completion_branch_rejects(self):
        fn,source,data,manifest=self.inputs()
        self.assertEqual(data.bytes_at(0xdb123d,2),b'\x0f\x85')
        class Changed:
            def bytes_at(self,address,size):
                raw=data.bytes_at(address,size)
                if address==0xdb09e0:
                    raw=bytearray(raw)
                    raw[0xdb123e-address]=0x84
                    return bytes(raw)
                return raw
        result,evidence=recover_affair_man_question_entry(fn,source,Changed(),manifest)
        self.assertEqual(result,source)
        self.assertEqual(evidence[0]['status'],'rejected')
