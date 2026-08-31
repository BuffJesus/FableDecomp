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
                "script": "V_Seed", "allocatorAddress": "0x00123456",
                "vtableAddress": "0x00200000", "evidenceAnchors": ["ANCHOR"]
            }), encoding="utf-8")
            result = build(catalog, root / "queue.json", root / "queue.tsv", clusters)
            row = json.loads((root / "queue.json").read_text())["queue"][0]
            self.assertEqual(row["evidence"], "native-decompile")
            self.assertEqual(row["stage"], "extract-operation-ir")
            self.assertEqual(row["allocatorAddress"], "0x00123456")
            self.assertEqual(result["anchoredClusters"], 1)

            native_ir = root / "native_ir"
            native_ir.mkdir()
            (native_ir / "V_Seed.json").write_text(json.dumps({
                "schema": "fable-native-script-operation-ir/0.1", "script": "V_Seed"
            }), encoding="utf-8")
            result = build(catalog, root / "queue.json", root / "queue.tsv", clusters, native_ir)
            row = json.loads((root / "queue.json").read_text())["queue"][0]
            self.assertEqual(row["stage"], "compare-runtime-trace")
            self.assertEqual(row["evidence"], "native-operation-ir")
            self.assertEqual(result["nativeOperationIr"], 1)

    def test_unanchored_structural_cluster_has_distinct_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog = root / "catalog.json"
            catalog.write_text(json.dumps({"scripts": [{
                "name": "V_Known", "kind": "village", "section": "S_VK",
                "allocatorAddress": "0x00123456"}], "seedCorrelations": []}), encoding="utf-8")
            clusters = root / "clusters"
            clusters.mkdir()
            (clusters / "V_Known.json").write_text(json.dumps({
                "script": "V_Known", "allocatorAddress": "0x00123456",
                "vtableAddress": "0x00200000", "evidenceAnchors": []}), encoding="utf-8")
            native_ir = root / "native_ir"
            native_ir.mkdir()
            (native_ir / "V_Known.json").write_text(json.dumps({
                "schema": "fable-native-script-operation-ir/0.1", "script": "V_Known"
            }), encoding="utf-8")
            result = build(catalog, root / "queue.json", root / "queue.tsv", clusters, native_ir)
            row = json.loads((root / "queue.json").read_text())["queue"][0]
            self.assertEqual(row["evidence"], "native-operation-ir-address-resolved")
            self.assertTrue(row["nativeCluster"].endswith("V_Known.json"))
            self.assertEqual(result["clusters"], 1)
            self.assertEqual(result["anchoredClusters"], 0)


if __name__ == "__main__":
    unittest.main()
