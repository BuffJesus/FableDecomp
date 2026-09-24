import json
from pathlib import Path
import tempfile
import unittest

from tools.script_recovery.ghidra_typing_spec import signature_types, proven_bool_returns


class SignatureTypesTests(unittest.TestCase):
    def test_bool_return_requires_signature_and_all_native_tails(self):
        class Image:
            def __init__(self, raw):
                self.raw = raw

            def bytes_at(self, address, size):
                return self.raw

        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory, 'translation_unit.json')
            fn = {'address': '0x1000', 'decompile': 'public: bool __thiscall NScript::CTest::Helper(void)',
                  'bodyRanges': [{'start': '0x1000', 'endExclusive': '0x1008'}]}
            path.write_text(json.dumps({'functions': [fn]}))
            # Both literal outcomes prove a one-byte bool, not a full EAX integer.
            self.assertEqual(proven_bool_returns(directory, Image(bytes.fromhex('32c0c20400b001c3'))),
                             {'0x1000': 'NScript::CTest::Helper'})
            # A second unproven return must prevent inference for the whole function.
            self.assertEqual(proven_bool_returns(directory, Image(bytes.fromhex('32c0c2040090c3'))), {})
            self.assertEqual(proven_bool_returns(directory, Image(bytes.fromhex('b801000000c3'))), {})
            fn['decompile'] = 'public: int __thiscall NScript::CTest::Helper(void)'
            path.write_text(json.dumps({'functions': [fn]}))
            self.assertEqual(proven_bool_returns(directory, Image(bytes.fromhex('32c0c3'))), {})

    def test_wrapped_const_reference_preserves_parameter_type(self):
        source = '''/* [bsim sim=0.577 <- ego_r]
 public: bool __thiscall NScript::CQ_TestScript::Comment(class CCharString const
 &,class CScriptThing const &,enum NScript::CQ_TestScript::EComment) */'''
        with tempfile.TemporaryDirectory() as directory:
            Path(directory, 'translation_unit.json').write_text(json.dumps({
                'functions': [{'address': '0x00E01900', 'decompile': source}]}))
            self.assertEqual(signature_types(directory)['0x00e01900'],
                             ['CCharString *', 'CScriptThing *', None])

    def test_multiline_pointer_and_primitive_remain_distinct(self):
        source = '''/* [bsim sim=1 <- ego_r]
 public: void __thiscall NScript::CQ_TestScript::Work(class C3DVector
 *,float,bool,unsigned long) */'''
        with tempfile.TemporaryDirectory() as directory:
            Path(directory, 'translation_unit.json').write_text(json.dumps({
                'functions': [{'address': '0x10', 'decompile': source}]}))
            self.assertEqual(signature_types(directory)['0x10'],
                             ['C3DVector *', 'float', 'bool', 'int'])


if __name__ == '__main__':
    unittest.main()
