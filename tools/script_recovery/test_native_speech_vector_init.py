import json
import unittest

from tools.script_recovery.lift_native_lua import (ROOT, RData, load_manifest,
    load_slots, load_thing_tables, annotate)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_speech_vectors import recover_vectors, recover_init_vector_construction


class SpeechVectorInitTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DAADD0')
        manifest, slots, data = load_manifest(), load_slots(), RData()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=False)
        return fn, source, data, manifest

    def test_only_immutable_construction_tail_is_replaced(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_init_vector_construction(fn, source, data, recover_vectors(data.bytes_at))
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual((evidence[0]['vectorCount'], evidence[0]['keyCount']), (8, 42))
        prefix = source[:source.index(evidence[0]['old'])]
        self.assertEqual(result, prefix + 'return;\n}\n')
        self.assertNotIn('std__vector__Reallocate', result)
        self.assertIn('GSI->SetTimer', result)

    def test_missing_or_changed_materialized_tables_reject(self):
        fn, source, data, manifest = self.inputs()
        altered = recover_vectors(data.bytes_at)
        altered['0xd8'] = list(reversed(altered['0xd8']))
        for tables in ({}, altered):
            result, evidence = recover_init_vector_construction(fn, source, data, tables)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
