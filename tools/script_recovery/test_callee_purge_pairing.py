import unittest
from unittest import mock

from tools.script_recovery import convert_quest_unit as cqu

LABEL = 'CScriptThing::?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ'
ISALIVE = 'CScriptThing::?IsAlive@CScriptThing@@UBE_NXZ'


class CalleePurgeAlignTests(unittest.TestCase):
    def test_overlong_record_keeps_only_the_callees_own_push(self):
        # MakeTraderComment 0x00E022DF: GetDataString (ret 4) recorded with AddLineToConversation's early pushes
        site = {'target': '0x004AA900', 'ecxStack': -24, 'edxStack': -56,
                'pushedStack': [None, -24, None, None, None, None, -56]}
        with mock.patch.object(cqu, 'callee_stack_words', return_value=1):
            self.assertEqual(cqu._align(site, ['(CScriptThing *)(auStack_24 + 4)', '(int)&local_40']),
                             [(-24, None), (-56, None)])

    def test_zero_purge_receiver_only_call_drops_stray_pushes(self):
        site = {'target': '0x004AB130', 'ecxStack': -48, 'pushedStack': [None, None]}
        with mock.patch.object(cqu, 'callee_stack_words', return_value=0):
            self.assertEqual(cqu._align(site, ['(CScriptThing *)&xStack_30']), [(-48, None)])

    def test_unproven_purge_stays_unaligned(self):
        site = {'target': '0x00123456', 'ecxStack': -24, 'pushedStack': [None, -24, -56]}
        with mock.patch.object(cqu, 'callee_stack_words', return_value=None):
            self.assertIsNone(cqu._align(site, ['a', 'b']))

    def test_printed_arity_must_match_the_purge(self):
        site = {'target': '0x004AA900', 'ecxStack': -24, 'pushedStack': [None, -24, -56]}
        with mock.patch.object(cqu, 'callee_stack_words', return_value=1):
            self.assertIsNone(cqu._align(site, ['a', 'b', 'c']))


class CodePointerPairingTests(unittest.TestCase):
    FN = {'callOrder': ['0x1', '0x2'],
          'calls': [{'site': '0x1', 'target': '0x004AB130', 'ecxStack': -48, 'pushedStack': [None, None],
                     'currentName': ISALIVE},
                    {'site': '0x2', 'target': '0x004AB130', 'ecxStack': -48, 'currentName': ISALIVE}]}

    def _pointer(self, va):
        return {0x01238db8: 0x004AB130}.get(va)

    def test_ptr_and_indexed_heads_pair_and_respell(self):
        text = ('  local_30 = &PTR__scalar_deleting_destructor__01238c8c;\n'
                '  cVar2 = (*(code *)PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)();\n'
                '  cVar2 = (*(code *)local_30[0x4b])();\n')
        with mock.patch.object(cqu, '_code_pointer', side_effect=self._pointer), \
                mock.patch.object(cqu, 'callee_stack_words', return_value=0):
            self.assertIsNotNone(cqu._text_order_sites(text, self.FN))
            out = cqu.respell_code_pointer_calls(text, self.FN)
        self.assertEqual(out.count('CScriptThing::_IsAlive_CScriptThing__UBE_NXZ((CScriptThing *)&xStack_30)'), 2)

    def test_vtable_head_on_a_loaded_vtable_local_pairs_with_the_direct_site(self):
        text = ('  local_30._0_4_ = &PTR__scalar_deleting_destructor__01238c8c;\n'
                '  cVar2 = (*(code *)PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8)();\n'
                '  cVar2 = (**(code **)(local_30._0_4_ + 300))();\n')
        with mock.patch.object(cqu, '_code_pointer', side_effect=self._pointer), \
                mock.patch.object(cqu, 'callee_stack_words', return_value=0):
            self.assertIsNotNone(cqu._text_order_sites(text, self.FN))

    def test_pointer_to_another_target_is_not_paired(self):
        text = '  cVar2 = (*(code *)PTR__Other_01238db8)();\n  cVar2 = (*(code *)PTR__Other_01238db8)();\n'
        with mock.patch.object(cqu, '_code_pointer', return_value=0x00999999):
            self.assertIsNone(cqu._text_order_sites(text, self.FN))


class CallerCleanupTests(unittest.TestCase):
    class Ins:
        def __init__(self, mnemonic, op_str=''):
            self.mnemonic, self.op_str = mnemonic, op_str

    def test_cdecl_add_esp_after_call(self):
        insns = [self.Ins('call', '0xbfe9bc'), self.Ins('add', 'esp, 4')]
        self.assertEqual(cqu.caller_cleanup_words(insns, 0), 1)

    def test_no_cleanup(self):
        insns = [self.Ins('call', '0x99eae0'), self.Ins('mov', 'eax, esi')]
        self.assertEqual(cqu.caller_cleanup_words(insns, 0), 0)


if __name__ == '__main__':
    unittest.main()
