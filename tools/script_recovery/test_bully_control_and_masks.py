import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_control_flow import recover_flow,RETRY_PHASE
from tools.script_recovery.bully_predicate_masks import recover_masks
from tools.script_recovery.bully_initial_candidate import DRAFT
from tools.script_recovery.bully_subdual_retry_native import execute as retry_native,termination_edge
from tools.script_recovery.bully_masks_native import execute as masks_native

class BullyControlMaskTests(unittest.TestCase):
    def test_original_entry_and_tail_termination_results_control_the_loop(self):
        for tail in (False,True):
            for cancel in (False,True):
                lua=LuaRuntime();quest=lua.table();events=[]
                quest.NewScriptFrame=lambda _:events.append(('frame',))
                quest.IsActiveThreadTerminating=lambda _:events.append(('term',cancel)) or cancel
                source='local quest=...; '+('quest:NewScriptFrame(); ' if tail else '')+'local alive=not quest:IsActiveThreadTerminating(); return alive'
                self.assertEqual((lua.execute(source,quest),events),termination_edge(tail,cancel))

    def test_original_subdual_retry_edges_and_cancellation(self):
        for failures in (0,1,3):
            for cancel in (1,2,3,4,999):
                lua=LuaRuntime();lua.execute(RETRY_PHASE);q=lua.table();r=lua.table();me=lua.table();events=[];state={'tries':0,'queries':0}
                r.PrepareResource=lambda _,c:events.append(('prepare','self'))
                def acquire(_,c,actor,priority):
                    self.assertEqual((c,priority),(7,4));self.assertTrue(lua.eval('rawequal')(actor,me))
                    state['tries']+=1;ok=state['tries']>failures;events.append(('acquire','self',ok));return ok
                r.TryAcquire=acquire;q.NewScriptFrame=lambda _:events.append(('frame',))
                def term(_):state['queries']+=1;ok=state['queries']>=cancel;events.append(('term',ok));return ok
                q.IsActiveThreadTerminating=term
                result=lua.globals().BullyReacquireAfterSubdual(q,r,7,me)
                self.assertEqual((result,events),retry_native(failures,cancel))

    def test_native_masks_short_circuit_reverse_cleanup_and_preserved_foreign_bits(self):
        hero='SCRIPT_NAME_HERO';teddy='OBJECT_TEDDY_BEAR_UNGIVEABLE'
        for kind in ('talk','hit'):
            for answers in itertools.product((False,True),repeat=3):
                for mask in (0,0x100):
                    result,after,events=masks_native(kind,answers,mask)
                    self.assertEqual(after,mask)
                    self.assertEqual(result,answers[0] and answers[1] if kind=='talk' else answers[0] or (answers[1] and not answers[2]))
                    keys=[e[1] for e in events if e[0]=='string.new']
                    self.assertEqual([e[1] for e in events if e[0]=='string.destroy'],list(reversed(keys)))
                    if kind=='talk':
                        self.assertEqual(keys,[hero,teddy] if answers[0] else [hero])
                        self.assertEqual(sum(e[0]=='hero' for e in events),int(answers[0]))
                    else:self.assertEqual(len(keys),1 if answers[0] else (3 if answers[1] else 2))

    def test_source_loops_restore_executable_edges_and_masks_become_scopes(self):
        source,report=recover_flow(DRAFT.read_text());source,masks=recover_masks(source)
        LuaRuntime().execute('return function()\n'+source+'\nend')
        self.assertNotIn('extraout_AL_06',source);self.assertNotIn('extraout_AL_57',source)
        self.assertIn('while alive do',source);self.assertIn('goto acquireBullyAfterSubdued',source)
        self.assertIn('::acquireBullyAfterSubdued::\n    while not cVar4 do',source)
        self.assertNotIn('uVar16 = uStack_120 |',source)
        self.assertIn('resources:BullyTalkedWithTeddy(me)',source)
        self.assertIn('resources:IsHitByHeroExceptAbility(me, 14)',source)
        self.assertIn('nil --[[missing]]',source) # Remaining true gaps stay visible.
        with self.assertRaises(ValueError):recover_flow(DRAFT.read_text().replace('extraout_AL_57','extraout_AL_99'))
        with self.assertRaises(ValueError):recover_masks(source.replace('resources:BullyTalkedWithTeddy(me)','unknown()'))
