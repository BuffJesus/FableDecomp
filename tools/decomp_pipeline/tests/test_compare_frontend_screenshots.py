import unittest

import numpy as np

from tools.compare_frontend_screenshots import compare


class CompareFrontendScreenshotsTests(unittest.TestCase):
    def test_identical_captures_are_clean(self):
        image = np.array([[[1.0, 0.5, 0.0, 1.0]]], dtype=np.float32)
        result = compare(image, image.copy())
        self.assertEqual(result["rgb_mae"], 0.0)
        self.assertEqual(result["alpha_mae"], 0.0)
        self.assertEqual(result["changed_pixels"], 0)

    def test_transparent_rgb_is_not_counted_as_visible_error(self):
        reference = np.array([[[1.0, 0.0, 0.0, 0.0]]], dtype=np.float32)
        reconstruction = np.array([[[0.0, 1.0, 0.0, 0.0]]], dtype=np.float32)
        result = compare(reference, reconstruction)
        self.assertEqual(result["rgb_mae"], 0.0)
        self.assertEqual(result["alpha_mae"], 0.0)
        self.assertEqual(result["changed_pixels"], 0)

    def test_alpha_difference_is_reported_separately(self):
        reference = np.array([[[1.0, 1.0, 1.0, 1.0]]], dtype=np.float32)
        reconstruction = np.array([[[1.0, 1.0, 1.0, 0.5]]], dtype=np.float32)
        result = compare(reference, reconstruction)
        self.assertGreater(result["rgb_mae"], 0.0)
        self.assertGreater(result["alpha_mae"], 0.0)
        self.assertEqual(result["changed_pixels"], 1)

    def test_dimensions_must_match(self):
        reference = np.zeros((1, 1, 4), dtype=np.float32)
        reconstruction = np.zeros((2, 1, 4), dtype=np.float32)
        with self.assertRaises(ValueError):
            compare(reference, reconstruction)


if __name__ == "__main__":
    unittest.main()
