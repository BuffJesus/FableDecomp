import unittest
from pathlib import Path

from tools.script_recovery.audit_quest_info_bar_render_chain import audit


class QuestInfoBarRenderChainAuditTests(unittest.TestCase):
    def test_retail_storage_and_forge_bridge_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root)
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["nativeFunctions"], 4)

    def test_exporter_is_read_only_and_address_complete(self):
        root = Path(__file__).resolve().parents[2]
        text = (root / "tools/script_recovery/export_quest_info_bar_render_chain.ps1").read_text(encoding="utf-8-sig")
        self.assertIn('"-process", "Fable.exe", "-readOnly", "-noanalysis"', text)
        for address in ("0x0064F7FC", "0x00644DF8", "0x00656B1B", "0x006463FF"):
            self.assertIn(f'"{address}"', text)


if __name__ == "__main__":
    unittest.main()
