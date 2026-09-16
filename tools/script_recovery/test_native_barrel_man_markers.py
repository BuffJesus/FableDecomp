import copy
import json
import unittest
from pathlib import Path
from tools.script_recovery import test_native_barrel_man_resources as resource_tests
from tools.script_recovery.native_barrel_man_markers import verify


class BarrelMarkerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        resource_tests.BarrelResourceTests.setUpClass()
        cls.function=resource_tests.BarrelResourceTests.function;cls.data=resource_tests.BarrelResourceTests.data
        cls.witness=json.loads(Path(__file__).with_name('native_barrel_man_markers_witness.json').read_text())

    def test_all_lookups_and_cleanup_required(self):
        report=verify(self.function,self.data)
        self.assertEqual(len(report['markers']),7);self.assertEqual(len(report['ends']),12)
        for key in ('markers','ends'):
            for index in range(len(self.witness[key])):
                changed=copy.deepcopy(self.witness);changed[key].pop(index)
                with self.subTest(key=key,index=index),self.assertRaises(ValueError):verify(self.function,self.data,changed)

    def test_selected_destructors_and_name_string_lifetime(self):
        for site in self.witness['selections']:
            changed=copy.deepcopy(self.witness);del changed['selections'][site]
            with self.assertRaises(ValueError):verify(self.function,self.data,changed)
        for index in range(7):
            changed=copy.deepcopy(self.witness);changed['markers'][index]['string']['destroy']=changed['markers'][index]['create']
            with self.assertRaises(ValueError):verify(self.function,self.data,changed)
