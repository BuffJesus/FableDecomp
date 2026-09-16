import copy
import json
import unittest
from pathlib import Path
from tools.script_recovery import test_native_barrel_man_resources as resource_tests
from tools.script_recovery.native_barrel_man_temporary_things import verify


class BarrelTemporaryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        resource_tests.BarrelResourceTests.setUpClass()
        cls.function=resource_tests.BarrelResourceTests.function;cls.data=resource_tests.BarrelResourceTests.data
        cls.witness=json.loads(Path(__file__).with_name('native_barrel_man_temporary_things_witness.json').read_text())

    def test_all_queries_and_lifetimes(self):
        result=verify(self.function,self.data)
        self.assertEqual([t['kind'] for t in result['temporaries']].count('distance'),4)
        self.assertEqual([t['kind'] for t in result['temporaries']].count('health'),7)

    def test_omission_wrong_output_and_early_destruction_reject(self):
        for index in range(11):
            for mutation in ('omit','output','destroy'):
                changed=copy.deepcopy(self.witness)
                if mutation=='omit':changed['temporaries'].pop(index)
                elif mutation=='output':changed['temporaries'][index]['output'][1]+=4
                else:changed['temporaries'][index]['destroy']=changed['temporaries'][index]['query']
                with self.subTest(index=index,mutation=mutation),self.assertRaises(ValueError):
                    verify(self.function,self.data,changed)
