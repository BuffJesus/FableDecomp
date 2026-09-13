import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_cutscenes_threads import audit


class NewOakValeCutsceneThreadAuditTests(unittest.TestCase):
    def test_cutscene_resources_actors_and_threads_resolve_in_lua(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["cutsceneRecords"], 18)
        self.assertEqual(result["namedCutscenes"], 9)
        self.assertEqual(result["actorKeys"], 13)
        self.assertEqual(result["spawnedThreads"], 4)


if __name__ == "__main__":
    unittest.main()
