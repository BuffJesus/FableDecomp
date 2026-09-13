import unittest
import json

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_literal_loops import fold_literal_comparisons
from tools.script_recovery.lift_native_lua import ROOT, lift_cluster, strip_declarations
from tools.script_recovery.test_lift_native_lua import make


def native_compare(left, right, count):
    return (f'iVar1 = {count};\nbVar1 = true;\npcVar1 = "{left}";\npcVar2 = "{right}";\n'
            'do {\nif (iVar1 == 0) break;\niVar1 = iVar1 + -1;\n'
            'bVar1 = *pcVar1 == *pcVar2;\npcVar1 = pcVar1 + 1;\npcVar2 = pcVar2 + 1;\n'
            '} while (bVar1);')


class LiteralComparisonTests(unittest.TestCase):
    def test_actual_theresa_loop_folds_only_with_pointer_lifetime_proof(self):
        from tools.script_recovery.native_literal_loops import _COMPARE
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        source = next(f['decompile'] for f in unit['functions'] if f['address'] == '0x00DB97A0')
        match = next(m for m in _COMPARE.finditer(source) if m['a'] == 'OBJECT_CHOCOLATE_BOX_UNGIVEABLE')
        self.assertEqual(fold_literal_comparisons(source)[1], 0)
        folded, count = fold_literal_comparisons(source, strip_declarations)
        self.assertEqual(count, 1)
        replacement = folded[match.start():match.start() + len('iVar12 = 31;\nbVar4 = false;')]
        self.assertEqual(replacement, 'iVar12 = 31;\nbVar4 = false;')
        body = make().lift('Main', '{\n' + replacement + '\nreturn iVar12,bVar4;\n}')
        self.assertEqual(LuaRuntime().execute('return function()\n' + '\n'.join(body) + '\nend')(), (31, False))
        for pointer in (match['left'], match['right']):
            changed = source[:match.end()] + '\nuse(' + pointer + ');\n' + source[match.end():]
            self.assertEqual(fold_literal_comparisons(changed, strip_declarations), (changed, 0))

    def test_beardy_baldy_recovers_both_exported_literal_comparisons(self):
        report = lift_cluster('V_BeardyBaldy')
        recovered = {role: body['literalByteComparisons'] for role, body in report['functions'].items()
                     if body.get('literalByteComparisons')}
        self.assertEqual(recovered, {'Main': 1, 'Thread:WatchForQuestCardConditions': 1})
        self.assertNotIn('*"BowerstoneSlumsWarehouses"', report['lua'])

    def test_comparison_and_remaining_count_execute_in_lua(self):
        for left, right, count, equal, remaining in (
                ('BowerstoneSlumsWarehouses', '', 26, False, 25),
                ('ABC', 'AXC', 3, False, 1), ('ABC', 'ABC', 4, True, 0),
                ('ABC', 'AXC', 1, True, 0), ('ABC', 'ABCD', 4, False, 0),
                ('', '', 0, True, 0), ('', '', 1, True, 0)):
            with self.subTest(left=left, right=right, count=count):
                source = ('void Main()\n{\nchar *pcVar1;\nchar *pcVar2;\n' +
                          native_compare(left, right, count) + '\n'
                          'GSI->SetTimer(42,iVar1);\nif (bVar1) {\nGSI->SetTimer(42,100);\n}\n}')
                folded, changed = fold_literal_comparisons(source)
                self.assertEqual(changed, 1)
                body = make().lift('Main', folded)
                lua = LuaRuntime()
                events = []
                quest = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
                lua.execute('return function(Quest)\n' + '\n'.join(body) + '\nend')(quest)
                self.assertEqual(events, [remaining] + ([100] if equal else []))

    def test_reads_beyond_literal_nul_and_unsigned_counts_are_not_folded(self):
        for source in (native_compare('', '', 2), native_compare('ABC', 'ABC', 5),
                       native_compare('A', '', '0xffffffff')):
            self.assertEqual(fold_literal_comparisons(source), (source, 0))

    def test_live_pointer_side_effects_and_changed_loop_shapes_are_not_removed(self):
        loop = native_compare('A', '', 2)
        for source in (loop + '\niVar2 = *pcVar1;',
                       'SavedPointer = pcVar2;\n' + loop,
                       loop.replace('pcVar2 = pcVar2 + 1', 'pcVar2 = pcVar2 + 2'),
                       loop.replace('bVar1 = *pcVar1', 'bVar2 = *pcVar1'),
                       loop.replace('pcVar2', 'pcVar1'),
                       loop.replace('"A"', '"\\x41"'),
                       '/*\n' + loop + '\n*/'):
            self.assertEqual(fold_literal_comparisons(source), (source, 0))


if __name__ == '__main__':
    unittest.main()
