import unittest
from tools.script_recovery.native_dispatch_scaffolding import prune_dispatch_loads


class DispatchScaffoldingTests(unittest.TestCase):
    def test_reviewed_barrel_cleanup_preserves_other_uses_of_reused_names(self):
        from tools.script_recovery import test_native_barrel_hits as fixtures
        from tools.script_recovery.native_book_trader_hits import recover_barrel_hits
        from tools.script_recovery.native_dispatch_scaffolding import recover_barrel_dispatch
        from tools.script_recovery.lift_native_lua import RData
        fn, source, manifest = fixtures.BarrelHitTests().inputs()
        source, _ = recover_barrel_hits(fn, source, RData(), manifest)
        result, evidence = recover_barrel_dispatch(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('ppuVar18 = ppuStack_1c8;', result)
        self.assertIn('((uint)ppuVar18 | 3)', result)
        self.assertIn('((uint)ppuVar18 | 7)', result)
        self.assertIn('ppuStack_1d4 = *(undefined ***)(param_1 + 0xc);', result)
        self.assertNotIn('puVar14 = *', result)
        self.assertNotIn('ppuVar18 = *(undefined ***)(param_1 + 4)', result)
        self.assertEqual(source.count('GSI->PauseAllNonScriptedEntities'),
                         result.count('GSI->PauseAllNonScriptedEntities'))
        changed = source + '\nconsume(ppuVar18);'
        result, evidence = recover_barrel_dispatch(fn, changed, RData())
        self.assertEqual(result, changed)
        self.assertEqual(evidence[0]['status'], 'rejected')

    def test_only_unused_dispatch_chain_is_removed(self):
        source = ['local ppuVar2 = *(param_1 + 4)', 'quest:PauseAllNonScriptedEntities(true)',
                  'local puVar19 = *ppuVar2', 'quest:EndMovieSequence()']
        output, evidence = prune_dispatch_loads(source, entity=True)
        self.assertEqual(output, [source[1], source[3]])
        self.assertEqual([e['original'] for e in evidence], [source[0], source[2]])
        for suffix in ('consume(puVar19)', '-- TODO(native): consume(ppuVar2)', 'ppuVar2 = somethingElse'):
            live = source + [suffix]
            self.assertEqual(prune_dispatch_loads(live, entity=True), (live, []))

    def test_other_pointer_reads_and_wrong_owner_are_preserved(self):
        for source, entity in ((['local value = *(param_1 + 8)'], True),
                               (['local value = *(param_1 + 4)'], False),
                               (['local value = *actor'], True)):
            self.assertEqual(prune_dispatch_loads(source, entity=entity), (source, []))
        self.assertEqual(prune_dispatch_loads(['local value = *(param_1 + 0x40)'], entity=False)[0], [])
