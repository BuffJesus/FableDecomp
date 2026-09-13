import copy
import unittest
from dataclasses import replace
from types import SimpleNamespace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_scalars as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_post_attack_scalars import recover_post_attack_scalars
from tools.script_recovery.native_post_attack_logbook import recover_post_attack_logbook
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackLogbookTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackScalarTests().inputs()
        return fn, recover_post_attack_scalars(fn, source, RData(), manifest)[0], manifest

    def test_actual_recovered_call_uses_numeric_overload_once(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_logbook(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('CSubtitleRenderer::SetText', recovered)
        calls = [s.strip() for s in strip_declarations(recovered) if 'GSI->AddLogbookStoryEntry(' in s]
        self.assertEqual(calls, ['GSI->AddLogbookStoryEntry(20);'])
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Logbook', '{\n' + calls[0] + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'AddLogbookStoryEntry': lambda _q, value: events.append(value)})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [20])
        self.assertIs(type(events[0]), int)

    def test_changed_native_callee_caller_source_and_contract_reject(self):
        fn, source, manifest = self.inputs()
        data = RData()
        for changed_address in (0xDBEB20, 0xCBE87F):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed_address else raw
            result, evidence = recover_post_attack_logbook(fn, source, SimpleNamespace(bytes_at=read), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        changed = copy.deepcopy(manifest)
        changed['AddLogbookStoryEntry']['parameters'][0]['type'] = 'string'
        self.assertEqual(recover_post_attack_logbook(fn, source, data, changed)[1][0]['status'], 'rejected')
        self.assertEqual(recover_post_attack_logbook(fn, source + '\n', data, manifest)[1][0]['status'], 'rejected')

    def test_numeric_overload_does_not_consume_stale_string_and_string_overload_survives(self):
        _, _, manifest = self.inputs()
        params = manifest['AddLogbookStoryEntry']['parameters']
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        lifter.temps['old'] = '"unrelated"'
        lifter.order.append('old')
        self.assertEqual(lifter.place_args(params, ['20']), ['20'])
        self.assertEqual(lifter.place_args(params, ['0xffffffff']), ['-1'])
        self.assertEqual(lifter.place_args(params, ['"TEXT_QST_LOG_STORY_20"']), ['"TEXT_QST_LOG_STORY_20"'])
        self.assertEqual(lifter.todo, [])

    def test_decoder_cannot_substitute_register_value_or_target(self):
        fn, source, manifest = self.inputs()
        for field, value in (('ecx', ('constant', 21)), ('target', ('constant', 0xCBE960))):
            def decode(*args, **kwargs):
                return replace(read_call_window(*args, **kwargs), **{field: value})
            with patch('tools.script_recovery.native_post_attack_logbook.read_call_window', side_effect=decode):
                result, evidence = recover_post_attack_logbook(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
