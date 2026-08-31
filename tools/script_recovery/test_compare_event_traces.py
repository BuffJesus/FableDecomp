import unittest

from tools.script_recovery.compare_event_traces import compare


class EventTraceComparisonTests(unittest.TestCase):
    def test_normalizes_runtime_only_fields(self):
        expected = [{"event": "api-call", "name": "A", "timestamp": 1}]
        actual = [{"event": "api-call", "name": "A", "timestamp": 99}]
        self.assertTrue(compare(expected, actual)["matched"])

    def test_reports_first_semantic_difference(self):
        result = compare([{"name": "A"}, {"name": "B"}], [{"name": "A"}, {"name": "C"}])
        self.assertFalse(result["matched"])
        self.assertEqual(result["firstDifference"], 1)

    def test_subsequence_allows_native_telemetry_between_operations(self):
        expected = [{"name": "A"}, {"name": "C"}]
        actual = [{"name": "A"}, {"name": "telemetry"}, {"name": "C"}]
        self.assertTrue(compare(expected, actual, mode="subsequence")["matched"])


if __name__ == "__main__":
    unittest.main()
