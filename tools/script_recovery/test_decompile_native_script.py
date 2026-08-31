import unittest

from tools.script_recovery.decompile_native_script import parse_discovery


class NativeDecompileOrchestratorTests(unittest.TestCase):
    def test_parse_headless_log(self):
        log = """
INFO x> DISCOVERY_BEGIN
INFO x> {"anchor":"A","allocatorCandidate":"0x1234"} (GhidraScript)
INFO x> DISCOVERY_END count=1
"""
        self.assertEqual(parse_discovery(log), [{"anchor": "A", "allocatorCandidate": "0x1234"}])

    def test_ignores_json_outside_discovery_section(self):
        self.assertEqual(parse_discovery('{"allocatorCandidate":"bad"}'), [])


if __name__ == "__main__":
    unittest.main()
