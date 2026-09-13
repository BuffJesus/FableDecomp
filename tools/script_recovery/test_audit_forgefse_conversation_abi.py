import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_conversation_abi import audit


class ConversationAbiAuditTests(unittest.TestCase):
    def make_tree(self, root: Path, native_call: str) -> tuple[Path, Path]:
        folder = root / "FableScriptExtender"
        folder.mkdir()
        (folder / "GameInterface.h").write_text(
            "typedef void(__thiscall* tAddLineToConversation)(CGameScriptInterfaceBase* This, "
            "int conversationID, const CCharString* pLine, bool bUnknown, "
            "const CScriptThing* pSpeaker, const CScriptThing* pListener);", encoding="utf-8")
        (folder / "LuaQuestState.cpp").write_text(native_call, encoding="utf-8")
        pdb = root / "names.tsv"
        pdb.write_text(
            "00890750\t?AddLineToConversation@CGameScriptInterface@@UBEXJABVCCharString@@_NABVCScriptThing@@2@Z\n",
            encoding="utf-8")
        return root, pdb

    def test_accepts_exact_native_operand_order(self):
        with tempfile.TemporaryDirectory() as tmp:
            root, pdb = self.make_tree(Path(tmp),
                "AddLineToConversation_API(m_pGameInterface, conversationID, fsKey, "
                "bShowSubtitle, pSpeaker, pListener);")
            self.assertTrue(audit(root, pdb)["ok"])

    def test_rejects_speaker_before_bool(self):
        with tempfile.TemporaryDirectory() as tmp:
            root, pdb = self.make_tree(Path(tmp),
                "AddLineToConversation_API(m_pGameInterface, conversationID, fsKey, "
                "pSpeaker, bShowSubtitle, pListener);")
            result = audit(root, pdb)
            self.assertFalse(result["ok"])
            self.assertFalse(result["checks"]["luaBoolForwardedInNativeSlot"])


if __name__ == "__main__":
    unittest.main()
