import unittest

from tools.update_readme_progress import (
    replace_snapshot_row,
    replace_summary_denominator,
    verified_public_counts,
)


class VerifiedPublicCountsTests(unittest.TestCase):
    def setUp(self):
        self.status = {
            "lift_matching": 16,
            "lift_functional": 2,
        }
        self.parity = {
            "matching": 2689,
            "relocation_matching": 2029,
        }
        self.coverage = {
            "verified_lifted": 4736,
            "candidate_retail_matches": 2689,
        }

    def test_includes_lifted_functions_in_public_totals(self):
        self.assertEqual(
            verified_public_counts(
                self.status, self.parity, self.coverage),
            (4736, 2705))

    def test_rejects_stale_coverage_total(self):
        self.coverage["verified_lifted"] = 4718
        with self.assertRaisesRegex(
                RuntimeError, "verified-function sources disagree"):
            verified_public_counts(
                self.status, self.parity, self.coverage)

    def test_rejects_stale_candidate_exact_total(self):
        self.coverage["candidate_retail_matches"] = 2688
        with self.assertRaisesRegex(
                RuntimeError, "exact-match sources disagree"):
            verified_public_counts(
                self.status, self.parity, self.coverage)

    def test_updates_catalog_denominator_in_summary_prose(self):
        original = (
            "functional-or-matching coverage is **9.55%** of the "
            "49,552-function catalog."
        )
        self.assertEqual(
            replace_summary_denominator(original, 49568),
            (
                "functional-or-matching coverage is **9.55%** of the "
                "49,568-function catalog."
            ),
        )

    def test_updates_two_column_snapshot_row(self):
        original = (
            "| Snapshot | Current result |\n"
            "|---|---:|\n"
            "| Verified functional or matching reconstruction | "
            "**4,984 · 10.05%** |\n"
        )
        self.assertEqual(
            replace_snapshot_row(
                original,
                "Verified functional or matching reconstruction",
                "**5,205 · 10.50%**",
            ),
            (
                "| Snapshot | Current result |\n"
                "|---|---:|\n"
                "| Verified functional or matching reconstruction | "
                "**5,205 · 10.50%** |\n"
            ),
        )

    def test_rejects_missing_snapshot_row(self):
        with self.assertRaisesRegex(
                RuntimeError, "snapshot row not found exactly once"):
            replace_snapshot_row(
                "| Snapshot | Current result |\n",
                "Byte-identical reconstruction",
                "**2,900 · 5.85%**",
            )


if __name__ == "__main__":
    unittest.main()
