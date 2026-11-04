import subprocess
import sys
import unittest


class ReadableProfileIsolationTests(unittest.TestCase):
    def test_importing_unit_builder_preserves_book_trader_readability(self):
        script = '''
from tools.script_recovery.test_readable_book_trader import ReadableBookTraderTests
from tools.script_recovery.readable_book_trader_conditions import readable_book_conditions
ReadableBookTraderTests.setUpClass()
before = ReadableBookTraderTests.after
from tools.script_recovery import build_readable_unit
ReadableBookTraderTests.setUpClass()
after = ReadableBookTraderTests.after
assert before == after, 'builder import changed the independent readable pass'
readable_book_conditions(after)
'''
        result = subprocess.run([sys.executable, '-c', script], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_explicit_converter_names_do_not_affect_later_default_calls(self):
        from tools.script_recovery.readable_lua import readable_source
        from tools.script_recovery.build_readable_unit import CONVERTER_GENERATED
        source = '''function Probe(quest)
    local __native_condition_1
    __native_condition_1 = quest:IsActiveThreadTerminating()
    return __native_condition_1
end
'''
        before, _ = readable_source(source)
        converted, _ = readable_source(source, generated=CONVERTER_GENERATED)
        after, _ = readable_source(source)
        self.assertEqual(before, source)
        self.assertNotEqual(converted, source)
        self.assertEqual(after, before)
