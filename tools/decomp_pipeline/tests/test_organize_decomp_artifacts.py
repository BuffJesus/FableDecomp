import os
import sys
import tempfile
import unittest
from pathlib import Path


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from organize_decomp_artifacts import (  # noqa: E402
    Move,
    apply_moves,
    plan_address_files,
    try_get_mtime,
)


class OrganizeDecompArtifactsTests(unittest.TestCase):
    def test_missing_concurrently_moved_file_has_no_mtime(self):
        with tempfile.TemporaryDirectory() as temporary:
            missing = Path(temporary) / "already-moved.json"
            self.assertIsNone(try_get_mtime(missing))

    def test_source_planning_can_exclude_score_objects(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            source = root / "0x00123456_candidate.cpp"
            score = root / "_score" / "00123456.obj"
            score.parent.mkdir()
            source.write_text("candidate", encoding="utf-8")
            score.write_bytes(b"score")

            moves = []
            plan_address_files(
                moves,
                "compiled-source",
                root,
                suffixes=frozenset({".cpp"}),
            )

            self.assertEqual(1, len(moves))
            self.assertEqual(source, Path(moves[0].source))

    def test_differing_collision_preserves_old_and_promotes_new(self):
        with tempfile.TemporaryDirectory() as temporary:
            workspace = Path(temporary)
            source = workspace / "code" / "0x00123456_candidate.cpp"
            destination = (
                workspace / "code" / "00" / "12" /
                "0x00123456_candidate.cpp"
            )
            source.parent.mkdir()
            destination.parent.mkdir(parents=True)
            source.write_text("new candidate", encoding="utf-8")
            destination.write_text("old candidate", encoding="utf-8")

            result = apply_moves(
                [
                    Move(
                        "agent-source",
                        str(source),
                        str(destination),
                    )
                ],
                workspace,
            )

            self.assertFalse(source.exists())
            self.assertEqual(
                "new candidate",
                destination.read_text(encoding="utf-8"),
            )
            archives = list(
                destination.parent.glob(
                    destination.name + ".collision-*"))
            self.assertEqual(1, len(archives))
            self.assertEqual(
                "old candidate",
                archives[0].read_text(encoding="utf-8"),
            )
            self.assertEqual(1, result["moved"])
            self.assertEqual(1, result["collisions"])
            self.assertEqual(1, result["archived_collisions"])
            self.assertEqual(0, result["unresolved_collisions"])

    def test_curated_collision_keeps_canonical_and_archives_incoming(self):
        with tempfile.TemporaryDirectory() as temporary:
            workspace = Path(temporary)
            source = workspace / "compiled" / "00123456_candidate.cpp"
            destination = (
                workspace / "compiled" / "00" / "12" /
                "00123456_candidate.cpp"
            )
            source.parent.mkdir()
            destination.parent.mkdir(parents=True)
            source.write_text("incoming candidate", encoding="utf-8")
            destination.write_text("canonical source", encoding="utf-8")

            result = apply_moves(
                [
                    Move(
                        "compiled-source",
                        str(source),
                        str(destination),
                    )
                ],
                workspace,
            )

            self.assertFalse(source.exists())
            self.assertEqual(
                "canonical source",
                destination.read_text(encoding="utf-8"),
            )
            archives = list(
                destination.parent.glob(
                    destination.name + ".incoming-*"))
            self.assertEqual(1, len(archives))
            self.assertEqual(
                "incoming candidate",
                archives[0].read_text(encoding="utf-8"),
            )
            self.assertEqual(0, result["moved"])
            self.assertEqual(1, result["collisions"])
            self.assertEqual(1, result["archived_collisions"])
            self.assertEqual(0, result["unresolved_collisions"])


if __name__ == "__main__":
    unittest.main()
