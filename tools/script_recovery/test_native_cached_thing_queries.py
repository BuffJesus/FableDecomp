import re
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_man_distance as fixtures
from tools.script_recovery.lift_native_lua import RData, Lifter, load_slots, load_thing_tables, thing_signatures
from tools.script_recovery.native_teddy_distance import recover_affair_man_distance, recover_affair_man_woman_distance
from tools.script_recovery.native_random import fold_retail_random
from tools.script_recovery.native_cached_thing_queries import recover_cached_thing_queries
from tools.script_recovery.native_call_setup_ir import read_call_window


class CachedThingQueryTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.AffairManDistanceTests().inputs()
        data = RData()
        source, _ = recover_affair_man_distance(fn, source, data)
        source, _ = recover_affair_man_woman_distance(fn, source, data)
        source, _ = fold_retail_random(source, fn, data.bytes_at)
        return fn, source, manifest

    def test_actual_queries_keep_distinct_cached_receivers_and_nil_safety(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_cached_thing_queries(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        lookups = [re.search(r'GSI->GetThingWithScriptName\(&' + q['slot'] + r'[^;]+;', recovered)[0]
                   for q in evidence[0]['queries']]
        queries = [q['new'] for q in evidence[0]['queries']]
        code = '{\n' + '\n'.join(lookups) + '\nbVar4 = ' + queries[0] + ';\nif (bVar4) {\nreturn 1;\n}\n'
        code += 'bVar4 = ' + queries[1] + ';\nif (bVar4) {\nreturn 2;\n}\nreturn 0;\n}'
        things, _ = load_thing_tables(manifest, load_slots())
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Queries', code))
        self.assertEqual(lifter.todo, [])
        for woman, wife, expected, calls in ((True, True, 1, ['woman']),
                                            (False, True, 2, ['woman', 'wife']),
                                            (None, True, 2, ['wife']),
                                            (None, None, 0, [])):
            lua, events = LuaRuntime(), []
            def entity(name, alive):
                return None if alive is None else lua.table_from({'IsAlive': lambda *_: events.append(name) or alive})
            handles = {'NOVI_AffairWoman': entity('woman', woman), 'NOVI_AffairWife': entity('wife', wife)}
            quest = lua.table_from({'GetThingWithScriptName': lambda _q, name: handles[name]})
            self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), expected)
            self.assertEqual(events, calls)

    def test_changed_query_receiver_rejects_all_edits(self):
        fn, source, _ = self.inputs()
        def decode(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, ecx=('stack', 32)) if args[3] == 0xDB15BB else setup
        with patch('tools.script_recovery.native_cached_thing_queries.read_call_window', side_effect=decode):
            result, evidence = recover_cached_thing_queries(fn, source, RData())
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        result, evidence = recover_cached_thing_queries(fn, source + '\n', RData())
        self.assertEqual(result, source + '\n')
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
