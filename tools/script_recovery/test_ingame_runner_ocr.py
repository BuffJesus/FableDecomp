"""A real target label missed by full-screen OCR in Guardian Sister run gs4."""
import os
from pathlib import Path
import tempfile
import unittest

from PIL import Image
from tools.script_recovery.ingame_runner import target_labels, norm


@unittest.skipUnless(os.name == 'nt', 'requires Windows.Media.Ocr')
class TargetLabelTests(unittest.TestCase):
    def test_outlined_maze_label_is_recognized(self):
        with tempfile.TemporaryDirectory() as directory:
            png = Path(directory) / 'screen.png'
            frame = Image.new('RGB', (1024, 768), 'white')
            with Image.open(Path(__file__).parent / 'testdata/runner/maze_target.png') as crop:
                frame.paste(crop, (350, 0))
            frame.save(png)
            self.assertIn('maze', [norm(t) for t in target_labels(png, 'Maze')])


if __name__ == '__main__':
    unittest.main()
