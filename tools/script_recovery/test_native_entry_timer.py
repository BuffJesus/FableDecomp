import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_entry_timer import recover_entry_timer
from tools.script_recovery import test_native_presented_items as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations


class EntryTimerTests(unittest.TestCase):
    def inputs(self):
        return fixtures.PresentedItemComparisonTests().inputs('0x00DB4F70')[:3]

    def recover(self, fn, source, manifest, data=None, state=None):
        return recover_entry_timer(fn, source, data or RData(), manifest,
                                   {'0x108': ('WatchTimer', 'Int')} if state is None else state,
                                   strip_declarations)

    def test_actual_first_call_emits_dynamic_watch_timer_read(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = self.recover(fn, source, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['callSite'], '0xdb4f85')
        self.assertEqual(recovered.count('GSI->GetTimer();'), source.count('GSI->GetTimer();') - 1)
        statement = strip_declarations(recovered)[0]
        lifter = Lifter(manifest, {'0x108': ('WatchTimer', 'Int')}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Main', '{\n' + statement + '\nreturn iVar3;\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        for timer in (-1, 0, 39):
            def state(_q, key):
                events.append(('state', key))
                return timer
            def query(_q, argument):
                events.append(('timer', argument))
                return 12
            quest = lua.table_from({'GetStateInt': state, 'GetTimer': query})
            self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), 12)
            self.assertEqual(events[-2:], [('state', 'WatchTimer'), ('timer', timer)])
        # Exercise the production integration on the entire actual native body.
        body = '\n'.join(lifter.lift('Main', source, native_function=fn))
        self.assertEqual(lifter.entry_timer_evidence[0]['status'], 'recovered')
        self.assertIn('iVar3 = quest:GetTimer(quest:GetStateInt("WatchTimer"))', body)

    def test_source_contract_and_field_must_agree(self):
        fn, source, manifest = self.inputs()
        for changed in (
                dict(fn, decompile=fn['decompile'].replace('iVar3 =', 'iVar8 =', 1)),
                dict(fn, decompile=fn['decompile'].replace('CScriptBase *param_1', 'Other *param_1'))):
            self.assertEqual(self.recover(changed, source, manifest)[1][0]['status'], 'rejected')
        self.assertEqual(self.recover(fn, source, manifest, state={})[1][0]['status'], 'rejected')
        self.assertEqual(self.recover(fn, source, {})[1][0]['status'], 'rejected')

    def test_header_comment_is_not_replaced_instead_of_executable_statement(self):
        fn, source, manifest = self.inputs()
        comment = '/* iVar3 = GSI->GetTimer(); */\n'
        recovered, evidence = self.recover(fn, comment + source, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertTrue(recovered.startswith(comment))
        self.assertIn('GetTimer(*(int *)(this + 0x108))', strip_declarations(recovered)[0])

    def test_changed_native_target_and_reentry_reject(self):
        fn, source, manifest = self.inputs()
        start = int(fn['address'], 16)
        prefix = RData().bytes_at(start, 0xDB4F8B - start)
        changed_target = prefix[:-4] + bytes.fromhex('6c010000') + b'\xc3'
        reentry = prefix + b'\xeb' + (-(len(prefix) + 2)).to_bytes(1, 'little', signed=True)
        for raw in (changed_target, reentry):
            with self.subTest(raw=raw.hex()):
                data = SimpleNamespace(bytes_at=lambda address, size: raw[address-start:address-start+size])
                result, evidence = self.recover(dict(fn, size=len(raw)), source, manifest, data)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')
