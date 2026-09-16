import copy
import json
import unittest
from pathlib import Path
from tools.script_recovery import test_native_barrel_man_resources as resource_tests
from tools.script_recovery.native_barrel_man_movies import verify


class BarrelMovieTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        resource_tests.BarrelResourceTests.setUpClass()
        cls.function=resource_tests.BarrelResourceTests.function;cls.data=resource_tests.BarrelResourceTests.data
        cls.witness=json.loads(Path(__file__).with_name('native_barrel_man_movies_witness.json').read_text())

    def test_all_movie_and_pause_events_required(self):
        report=verify(self.function,self.data)
        self.assertEqual(len(report['events']),12);self.assertEqual(len(report['pauses']),17)
        for key in ('events','pauses'):
            for index in range(len(self.witness[key])):
                changed=copy.deepcopy(self.witness);changed[key].pop(index)
                with self.subTest(key=key,index=index),self.assertRaises(ValueError):verify(self.function,self.data,changed)

    def test_shared_destructor_receiver_selections_required(self):
        for site in self.witness['selections']:
            changed=copy.deepcopy(self.witness);del changed['selections'][site]
            with self.subTest(site=site),self.assertRaises(ValueError):verify(self.function,self.data,changed)

    def test_movie_class_scope_and_pause_identity_required(self):
        for index in range(4):
            changed=copy.deepcopy(self.witness);changed['classStrings'][index]['destroy']=changed['classStrings'][index]['use']
            with self.subTest(index=index),self.assertRaises(ValueError):verify(self.function,self.data,changed)
        changed=copy.deepcopy(self.witness);changed['pauses'][0]['identity']=['stack',88]
        with self.assertRaises(ValueError):verify(self.function,self.data,changed)
