import json
import unittest
from pathlib import Path

class NewOakvaleUpstreamCompatibilityTests(unittest.TestCase):
    def test_original_upstream_fse_is_not_yet_compatible(self):
        p = Path('refs/script_recovery/lifted/NewOakValeIntro/UPSTREAM_FSE_COMPATIBILITY.json')
        d = json.loads(p.read_text(encoding='utf-8'))
        self.assertFalse(d['compatibleWithoutPorting'])
        self.assertIn('GetStateFloat', d['missingQuestBindings'])
        self.assertIn('IsDistanceFromPositionOver', d['missingEntityBindings'])

if __name__ == '__main__': unittest.main()
