"""The autopilot driver's entry points exist and are callable (2026-09-22).

`--harvest-save` was added with the helper's `def` line missing: the body was spliced onto the end of the
previous function as unreachable code, the module still imported, `--help` still listed the flag, and the
failure only surfaced at the END of a forty-minute run as
`NameError: name 'harvest_save' is not defined` -- discarding the graduation autosave the flag existed to
keep. Argparse showing an option proves nothing about the code path behind it.
"""
import importlib.util
import inspect
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
AUTOPILOT = ROOT / 'tools/script_recovery/autopilot.py'


def load():
    spec = importlib.util.spec_from_file_location('autopilot_under_test', AUTOPILOT)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class DriverEntryPointTests(unittest.TestCase):
    def setUp(self):
        self.mod = load()

    def test_save_helpers_exist(self):
        for name in ('stage_save', 'restore_save', 'harvest_save'):
            with self.subTest(helper=name):
                self.assertTrue(callable(getattr(self.mod, name, None)), f'{name} is not defined')

    def test_harvest_save_signature(self):
        params = list(inspect.signature(self.mod.harvest_save).parameters)
        self.assertEqual(params[:2], ['profile', 'source'])

if __name__ == '__main__':
    unittest.main()
