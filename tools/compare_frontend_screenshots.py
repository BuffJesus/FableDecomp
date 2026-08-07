"""Compare two frontend captures with premultiplied-alpha metrics.

The retail frontend mixes opaque sprite layers with transparent text and
helper art. Comparing straight RGB values makes transparent pixels look like
large errors, so this tool compares premultiplied RGB and alpha separately.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

import numpy as np
from PIL import Image


def load_rgba(path: Path) -> np.ndarray:
    return np.asarray(Image.open(path).convert("RGBA"), dtype=np.float32) / 255.0


def compare(reference: np.ndarray, reconstruction: np.ndarray) -> dict:
    if reference.shape != reconstruction.shape:
        raise ValueError(
            "capture dimensions differ: %sx%s vs %sx%s"
            % (
                reference.shape[1],
                reference.shape[0],
                reconstruction.shape[1],
                reconstruction.shape[0],
            )
        )

    reference_alpha = reference[:, :, 3:4]
    reconstruction_alpha = reconstruction[:, :, 3:4]
    coverage = np.maximum(reference_alpha, reconstruction_alpha)
    premultiplied_delta = np.abs(
        reference[:, :, :3] * reference_alpha
        - reconstruction[:, :, :3] * reconstruction_alpha
    )
    alpha_delta = np.abs(reference_alpha - reconstruction_alpha)
    covered = float(coverage.sum())
    pixel_count = reference.shape[0] * reference.shape[1]
    denominator = max(3.0 * covered, 1.0)
    rgb_mae = float(premultiplied_delta.sum() / denominator * 255.0)
    alpha_mae = float(alpha_delta.mean() * 255.0)
    changed = np.maximum(
        premultiplied_delta.max(axis=2), alpha_delta[:, :, 0]
    ) > (2.0 / 255.0)
    return {
        "width": int(reference.shape[1]),
        "height": int(reference.shape[0]),
        "rgb_mae": round(rgb_mae, 4),
        "alpha_mae": round(alpha_mae, 4),
        "changed_pixels": int(changed.sum()),
        "changed_fraction": round(float(changed.mean()), 6),
        "covered_pixels": round(covered, 2),
        "pixel_count": pixel_count,
        "premultiplied_rgb_delta": premultiplied_delta,
        "alpha_delta": alpha_delta[:, :, 0],
    }


def save_heatmap(result: dict, path: Path) -> None:
    rgb_delta = result["premultiplied_rgb_delta"].max(axis=2)
    alpha_delta = result["alpha_delta"]
    # Red emphasizes visible RGB disagreement; green identifies alpha-only
    # disagreement. Keep the output opaque so it is easy to inspect anywhere.
    heatmap = np.zeros((*rgb_delta.shape, 4), dtype=np.uint8)
    heatmap[:, :, 0] = np.clip(rgb_delta * 3.0 * 255.0, 0, 255)
    heatmap[:, :, 1] = np.clip(alpha_delta * 3.0 * 255.0, 0, 255)
    heatmap[:, :, 2] = np.clip((rgb_delta + alpha_delta) * 255.0, 0, 255)
    heatmap[:, :, 3] = 255
    path.parent.mkdir(parents=True, exist_ok=True)
    Image.fromarray(heatmap).save(path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("reference", type=Path)
    parser.add_argument("reconstruction", type=Path)
    parser.add_argument("--output", type=Path, help="write an RGBA heatmap")
    parser.add_argument("--json", type=Path, help="write metrics as JSON")
    args = parser.parse_args()

    result = compare(load_rgba(args.reference), load_rgba(args.reconstruction))
    if args.output:
        save_heatmap(result, args.output)
    serializable = {
        key: value
        for key, value in result.items()
        if key not in ("premultiplied_rgb_delta", "alpha_delta")
    }
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(serializable, indent=2) + "\n")
    print(json.dumps(serializable, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
