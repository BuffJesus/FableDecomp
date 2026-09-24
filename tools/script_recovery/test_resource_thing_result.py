import unittest

from tools.script_recovery.native_evidence_lowering import fold_resource_objects


class ResourceThingResultTests(unittest.TestCase):
    def test_void_pointer_cast_preserves_hidden_thing_and_result_alias(self):
        source = '    result = (void *)Resource::GetScriptThing((Resource *)resource,(int)hidden);\n'
        self.assertEqual(fold_resource_objects(source, {'Resource::GetScriptThing': 0x7E7490}),
                         '    hidden = RESOURCE_ScriptThing(resource);\n    result = hidden;\n')

    def test_addressed_hidden_slot_and_typed_result(self):
        source = '    result = (CScriptThing *)Resource::GetScriptThing((Resource *)&resource,(int)&hidden);\n'
        self.assertEqual(fold_resource_objects(source, {'Resource::GetScriptThing': 0x7E7490}),
                         '    hidden = RESOURCE_ScriptThing(resource);\n    result = hidden;\n')

    def test_unproven_callee_and_nonpointer_cast_are_not_rewritten(self):
        source = '    result = (void *)Resource::GetScriptThing(resource,hidden);\n'
        self.assertEqual(fold_resource_objects(source, {'Resource::GetScriptThing': 0x1234}), source)
        source = '    result = (int)Resource::GetScriptThing(resource,hidden);\n'
        self.assertEqual(fold_resource_objects(source, {'Resource::GetScriptThing': 0x7E7490}), source)


if __name__ == '__main__':
    unittest.main()
