import os
import sys
import unittest

import numpy as np


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from render_fable_frontend_animation import stitch_frame  # noqa: E402


class FrontendAnimationRenderTests(unittest.TestCase):
    def test_stitches_cropped_edge_tiles_without_seams(self):
        dimensions = (
            (256, 256),
            (256, 256),
            (256, 128),
            (224, 256),
            (224, 256),
            (224, 128),
        )
        tiles = [
            np.full((height, width, 4), index + 1, dtype=np.uint8)
            for index, (height, width) in enumerate(dimensions)
        ]
        frame = stitch_frame(tiles)
        self.assertEqual((480, 640, 4), frame.shape)
        self.assertTrue(np.all(frame[0, 0] == 1))
        self.assertTrue(np.all(frame[0, 639] == 3))
        self.assertTrue(np.all(frame[479, 0] == 4))
        self.assertTrue(np.all(frame[479, 639] == 6))


if __name__ == "__main__":
    unittest.main()
