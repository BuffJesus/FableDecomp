import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_reconstructed_speech_propagation import audit


class ReconstructedSpeechPropagationTests(unittest.TestCase):
    def test_accepts_guarded_and_returned_speech(self):
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            (folder / "Entity.lua").write_text(
                "if me:Speak(hero, key) == false then return false end\n"
                "return speak_if_alive(quest, me, key)\n", encoding="utf-8")
            result = audit(folder)
            self.assertTrue(result["ok"])
            self.assertEqual(result["callsChecked"], 2)

    def test_rejects_discarded_speech_result(self):
        with tempfile.TemporaryDirectory() as tmp:
            folder = Path(tmp)
            (folder / "Entity.lua").write_text("speak_if_alive(quest, me, key)\n", encoding="utf-8")
            result = audit(folder)
            self.assertFalse(result["ok"])
            self.assertEqual(result["violations"][0]["line"], 1)


if __name__ == "__main__":
    unittest.main()
