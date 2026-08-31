import json
import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_conversion_queue import build


class ConversionQueueTests(unittest.TestCase):
    def test_seed_is_first_but_not_marked_verified(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog = root / "catalog.json"
            catalog.write_text(json.dumps({
                "scripts": [
                    {"name": "Q_Other", "kind": "quest", "section": "S_QO", "allocatorAddress": "0x00123456"},
                    {"name": "V_Seed", "kind": "village", "section": "S_VS", "allocatorAddress": None},
                ],
                "seedCorrelations": [{"nativeName": "V_Seed", "package": "Seed", "status": "matched"}],
            }), encoding="utf-8")
            result = build(catalog, root / "queue.json", root / "queue.tsv")
            queue = json.loads((root / "queue.json").read_text())["queue"]
            self.assertEqual(queue[0]["name"], "V_Seed")
            self.assertEqual(queue[0]["evidence"], "registry-fact")
            self.assertEqual(result["seeded"], 1)

    def test_anchored_cluster_advances_seed(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog = root / "catalog.json"
            catalog.write_text(json.dumps({
                "scripts": [{"name": "V_Seed", "kind": "village", "section": "S_VS", "allocatorAddress": None}],
                "seedCorrelations": [{"nativeName": "V_Seed", "package": "Seed", "status": "matched"}],
            }), encoding="utf-8")
            clusters = root / "clusters"
            clusters.mkdir()
            (clusters / "V_Seed.json").write_text(json.dumps({
                "script": "V_Seed", "allocatorAddress": "0x00123456", "evidenceAnchors": ["ANCHOR"]
            }), encoding="utf-8")
            result = build(catalog, root / "queue.json", root / "queue.tsv", clusters)
            row = json.loads((root / "queue.json").read_text())["queue"][0]
            self.assertEqual(row["evidence"], "native-decompile")
            self.assertEqual(row["stage"], "extract-operation-ir")
            self.assertEqual(row["allocatorAddress"], "0x00123456")
            self.assertEqual(result["anchoredClusters"], 1)


if __name__ == "__main__":
    unittest.main()
