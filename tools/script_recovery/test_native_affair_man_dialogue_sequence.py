import re
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_ranged_lines as fixtures
from tools.script_recovery.native_affair_man_conversation import recover_affair_man_ranged_lines
from tools.script_recovery.lift_native_lua import ROOT,Lifter,load_entity_state,known_callee_aliases


class AffairManDialogueSequenceTests(unittest.TestCase):
    def build(self):
        fn,source,data,manifest=fixtures.AffairManRangedLinesTests().inputs()
        source,evidence=recover_affair_man_ranged_lines(fn,source,data,manifest)
        self.assertEqual(evidence[0]['status'],'recovered')
        starts=[m.start() for m in re.finditer(r'uVar8 = GSI->AddNewConversation\(me,false,false\);',source)]
        self.assertEqual(len(starts),2)
        near_end=source.index('*(undefined1 *)(*(int *)(param_1 + 0x14) + 0x71) = 1;',starts[0])
        ranged_last='GSI->AddLineToConversation(uVar8,native_arg_woman_ranged_key,false,local_native_cached_woman,me);'
        ranged_end=source.index(ranged_last,starts[1])+len(ranged_last)
        lua=LuaRuntime()
        runners={}
        for mode,start,end in (('near',starts[0],near_end),('ranged',starts[1],ranged_end)):
            lifter=Lifter(manifest,load_entity_state('NOVI_AffairMan'),'quest',True,'',data,
                state_receiver='me',live_termination=True,native_gotos=True,callee_names=known_callee_aliases(fn))
            body='\n'.join(lifter.lift('Sequence','{\n'+source[start:end]+'\nreturn true;\nLAB_00db1d7c:\nreturn false;\n}',parameters={'local_native_cached_woman':'thing'}))
            self.assertEqual(lifter.todo,[])
            runners[mode]=lua.execute('return function(quest,me,local_native_cached_woman)\n'+body+'\nend')
        return lua,runners

    def test_shared_counter_repeats_across_near_and_ranged_modes(self):
        lua,runners=self.build()
        state={'BadgerIndex':0}
        events=[]
        me=lua.table_from({'GetStateInt':lambda actor,key:state[key],
            'SetStateInt':lambda actor,key,value:state.__setitem__(key,value)})
        ident=[90]
        def create(q,actor,a,b):
            self.assertFalse(a)
            self.assertFalse(b)
            ident[0]+=1
            events.append(('new',ident[0]))
            return ident[0]
        quest=lua.table_from({'AddNewConversation':create,
            'AddPersonToConversation':lambda q,i,actor:events.append(('person',i,actor)),
            'IsActiveThreadTerminating':lambda q:events.append(('termination',)) or False,
            'AddLineToConversation':lambda q,i,key,speaker,listener,flag:events.append(('line',i,key,flag))})
        for iteration in range(12):
            events.clear()
            mode='near' if iteration%2==0 else 'ranged'
            prior=state['BadgerIndex']
            expected=10 if prior==50 else prior+10
            self.assertTrue(runners[mode](quest,me,'woman'))
            self.assertEqual(state['BadgerIndex'],expected)
            lines=[event for event in events if event[0]=='line']
            man_prefix='TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_' if mode=='near' else 'TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_'
            woman_prefix='TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_' if mode=='near' else 'TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_'
            keys=[woman_prefix,man_prefix] if mode=='near' and expected==10 else [man_prefix,woman_prefix]
            self.assertEqual([line[2] for line in lines],[key+str(expected) for key in keys])
            self.assertEqual([line[1] for line in lines],[ident[0],ident[0]])
            self.assertEqual([line[3] for line in lines],[False,False])
            self.assertEqual(events[:2],[('new',ident[0]),('person',ident[0],'woman')])
            self.assertEqual(events.count(('termination',)),int(prior==50)+int(mode=='near'))

    def test_wrap_cancellation_leaves_increment_and_queues_no_lines(self):
        lua,runners=self.build()
        for mode,run in runners.items():
            state={'BadgerIndex':50}
            events=[]
            me=lua.table_from({'GetStateInt':lambda actor,key:state[key],
                'SetStateInt':lambda actor,key,value:state.__setitem__(key,value)})
            quest=lua.table_from({'AddNewConversation':lambda q,actor,a,b:events.append('new') or 173,
                'AddPersonToConversation':lambda q,i,actor:events.append('person'),
                'IsActiveThreadTerminating':lambda q:events.append('termination') or True,
                'AddLineToConversation':lambda *args:events.append('line')})
            self.assertFalse(run(quest,me,'woman'))
            self.assertEqual(state['BadgerIndex'],60)
            self.assertEqual(events,['new','person','termination'])

    def test_line_sequence_matches_working_port_helpers(self):
        lua,runners=self.build()
        port=(ROOT/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_AffairMan.lua').read_text(encoding='utf8')
        constants='\n'.join(re.findall(r'^local (?:TEXT_MAN_DIRTY_WORDS_NEAR|TEXT_WOMAN_DIRTY_WORDS_NEAR|TEXT_MAN_SCRMSG_DIRTY_WORDS|TEXT_WOMAN_DIRTY_WORDS|BADGER_STEP|BADGER_MAX|BADGER_RESET)\s*=.*$',port,re.M))
        next_start=port.index('local function next_badger_index()')
        next_end=port.index('\nend',next_start)+len('\nend')
        near_start=port.index('local function near_conversation(')
        ranged_end=port.index('\nend',port.index('local function ranged_conversation('))+len('\nend')
        helpers=lua.execute(constants+'\nlocal BadgerIndex=0\nlocal last_conversation\nlocal SaidFirstRangedComment\nlocal F={set=function() end}\n'+port[next_start:next_end]+'\n'+port[near_start:ranged_end]+'\nreturn {near=near_conversation,ranged=ranged_conversation}')
        state={'BadgerIndex':0}
        me=lua.table_from({'GetStateInt':lambda actor,key:state[key],
            'SetStateInt':lambda actor,key,value:state.__setitem__(key,value)})
        traces=[[],[]]
        quests=[]
        for trace in traces:
            quests.append(lua.table_from({'AddNewConversation':lambda q,actor,a,b:173,
                'AddPersonToConversation':lambda *args:None,
                'IsActiveThreadTerminating':lambda q:False,
                'AddLineToConversation':lambda q,i,key,speaker,listener,*flags,trace=trace:trace.append((i,key,speaker,listener))}))
        # Compare actor identity inside Lua, avoiding Python proxy identity rules.
        for quest in quests:
            lua.eval('function(q,man) local f=q.AddLineToConversation; q.AddLineToConversation=function(self,i,k,s,l,...) f(self,i,k,s==man and "man" or s,l==man and "man" or l,...) end end')(quest,me)
        for iteration in range(12):
            mode='near' if iteration%2==0 else 'ranged'
            self.assertTrue(runners[mode](quests[0],me,'woman'))
            helpers[mode](quests[1],me,'woman')
        self.assertEqual(traces[0],traces[1])
