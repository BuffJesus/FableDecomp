import json
import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_random import fold_retail_random, RAND_THUNK
from tools.script_recovery.convert_new_oakvale import convert, ROOT


class RetailRandomTests(unittest.TestCase):
    def test_villager_random_condition_retains_short_circuit_draw_timing(self):
        from tools.script_recovery.lift_native_lua import RData, load_slots, load_manifest, load_thing_tables, annotate, Lifter
        from tools.script_recovery.native_self_wrapper import fold_self_wrapper_arguments
        from tools.script_recovery.native_random import fold_retail_random
        import re
        fn = next(f for f in json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())['functions']
                  if f['address'] == '0x00DADF80')
        slots, rdata = load_slots(), RData()
        things, returning = load_thing_tables(load_manifest(), slots)
        source = fold_self_wrapper_arguments(annotate(fn['decompile'], slots, things, returning, entity=True))[0]
        recovered, evidence = fold_retail_random(source, fn, rdata.bytes_at)
        self.assertEqual(evidence[0]['status'], 'recovered')
        header = re.search(r'if \(\(iVar8 == 0\) && \(iVar8 = GSI->RetailRandModulo\(100\), iVar8 == 0\)\) \{', recovered)[0]
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Main', '{\niVar8 = native_arg_timer;\n' + header + '\nreturn 1;\n}\nreturn 0;\n}',
                                     parameters={'native_arg_timer': 'number'}))
        for timer, remainder, expected, count in ((1, 0, 0, 0), (0, 7, 0, 1), (0, 0, 1, 1)):
            lua, draws = LuaRuntime(), []
            def random(_q, modulus):
                draws.append(modulus)
                return remainder
            q = lua.table_from({'RetailRandModulo': random})
            self.assertEqual(lua.execute('return function(quest,native_arg_timer)\n' + body + '\nend')(q, timer), expected)
            self.assertEqual(draws, [100] * count)
        changed, rejected = fold_retail_random(source + '\nuse(iVar8);', fn, rdata.bytes_at)
        self.assertEqual(changed, source + '\nuse(iVar8);')
        self.assertEqual(rejected[0]['status'], 'rejected')
        changed_fn = dict(fn, decompile=fn['decompile'] + '\n/* changed */')
        self.assertEqual(fold_retail_random(source, changed_fn, rdata.bytes_at)[1][0]['reason'],
                         'sourceSha256 changed')
        def changed_bytes(address, size):
            raw = rdata.bytes_at(address, size)
            return bytes([raw[0] ^ 1]) + raw[1:] if address == 0x00DADF80 else raw
        unchanged, rejected = fold_retail_random(source, fn, changed_bytes)
        self.assertEqual(unchanged, source)
        self.assertEqual(rejected[0]['reason'], 'bytesSha256 changed')

    def test_exact_target_and_dead_temporary_required(self):
        source = 'int iVar2;\niVar2 = rand();\nuVar3 = iVar2 % count;'
        fn = {'calls': [{'currentName': 'MSVCR71.DLL::rand', 'site': '0x1000', 'target': hex(RAND_THUNK)}]}
        raw = b'\xe8' + (RAND_THUNK - 0x1005).to_bytes(4, 'little', signed=True)
        result, evidence = fold_retail_random(source, fn, lambda *_: raw)
        self.assertIn('uVar3 = GSI->RetailRandModulo(count);', result)
        self.assertEqual(len(evidence), 1)
        for candidate in (source + '\nuse(iVar2);', '/* ' + source + ' */',
                          source.replace('rand();', 'rand(); /* retain */'),
                          source + '\nx = rand();'):
            self.assertEqual(fold_retail_random(candidate, fn, lambda *_: raw), (candidate, []))
        self.assertEqual(fold_retail_random(source, fn, lambda *_: b'\x90' * 5), (source, []))
        fn['calls'][0]['target'] = '0x1234'
        self.assertEqual(fold_retail_random(source, fn, lambda *_: raw), (source, []))

    def test_generated_villager_helper_execution_matches_port_effects(self):
        with tempfile.TemporaryDirectory() as directory:
            report = convert(Path(directory))
            row = next(r for r in report['functions'] if r['owner'] == 'NOVI_Villager'
                       and r['function'] == 'GetVillagerSpeechIndex')
            self.assertEqual(row['todo'], [])
            self.assertEqual(len(row['randomEvidence']), 1)
            generated = (Path(directory) / row['path']).read_text()
        generated = generated[generated.index('function GetVillagerSpeechIndex('):]
        port = (ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua').read_text()
        # Execute the unchanged port helper, extracted at its next top-level declaration.
        port = port[port.index('local function get_villager_speech_index('):]
        port = port[:port.index('\nlocal function pick_reputation_table')]

        def run(source, name, sequence, cancel, initially_cancelled=False, cancel_query=None):
            lua, events, values = LuaRuntime(), [], iter(sequence)
            state = {'lastVillagerSpeechIdx': 1}
            terminated = [initially_cancelled]
            queries = [0]
            def terminating(_q):
                queries[0] += 1
                return terminated[0] or queries[0] == cancel_query
            def draw(_q, count):
                value = next(values)
                self.assertGreater(count, 0)
                self.assertLess(value, count)
                events.append(('draw', count, value))
                return value
            def frame(_q, me):
                events.append(('frame', me))
                terminated[0] = cancel
                return not cancel
            def write(_q, key, value):
                state[key] = value
                events.append(('write', key, value))
            q = lua.table_from({'RetailRandModulo': draw, 'NewScriptFrame': frame,
                                'IsActiveThreadTerminating': terminating,
                                'GetStateInt': lambda _q, key: state[key], 'SetStateInt': write})
            lua.execute('F = {lastVillagerSpeechIdx="lastVillagerSpeechIdx", '
                        'get=function(q,k) return q:GetStateInt(k) end, '
                        'set=function(q,k,v) q:SetStateInt(k,v) end}; '
                        'NOVI = {frame=function(q,me) return q:NewScriptFrame(me) end}')
            result = lua.execute(source + '\nreturn ' + name)(q, 'villager', 6)
            self.assertIsNone(lua.globals().uVar3)
            return result, events, state

        for sequence, cancel in (([4], False), ([1, 1, 0], False), ([1], True)):
            with self.subTest(sequence=sequence, cancel=cancel):
                native = run(generated, 'GetVillagerSpeechIndex', sequence, cancel)
                working = run(port, 'get_villager_speech_index', sequence, cancel)
                self.assertEqual(native[1:], working[1:])
                if cancel:
                    # Retail's sentinel is zero; the handwritten port uses nil.
                    self.assertEqual(native[0], 0)
                    self.assertIsNone(working[0])
                else:
                    self.assertEqual(native[0], working[0])
        result, events, state = run(generated, 'GetVillagerSpeechIndex', [], False, True)
        self.assertEqual(result, 0)
        self.assertEqual(events, [])
        self.assertEqual(state['lastVillagerSpeechIdx'], 1)
        for query, expected in ((2, 1), (3, 4)):
            result, events, state = run(generated, 'GetVillagerSpeechIndex', [4], False,
                                        cancel_query=query)
            self.assertEqual(result, 0)
            self.assertEqual(state['lastVillagerSpeechIdx'], expected)
            self.assertEqual(sum(e[0] == 'write' for e in events), int(query == 3))


if __name__ == '__main__':
    unittest.main()
