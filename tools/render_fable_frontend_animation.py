#!/usr/bin/env python3
"""Decode and stitch a retail frontend background animation theme.

Each frontend frame is six independently-addressed textures laid out in a
3x2 grid at x=0/256/512 and y=0/256. The right and bottom edge tiles carry
their real cropped dimensions (128 and 224 pixels). This tool decodes all
42 retail entries from one frontend.big read and writes:

* four opaque 640x480 theme frames stacked into a 640x1920 PNG;
* three alpha-bearing sunbeam frames stacked into a 640x1440 PNG.

The compiled press-start screen selects the misspelled ``FORREST`` theme,
while ``UI_FRONTEND_MAIN_MENU`` selects ``COASTAL``.  ``--theme`` keeps that
choice explicit and leaves the press-start theme as the compatible default.
"""

import argparse
import os
import sys

import numpy as np
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from texture_build import decode_entry, find_entry, load_big  # noqa: E402


FRAME_WIDTH = 640
FRAME_HEIGHT = 480
TILE_X = (0, 256, 512)
TILE_Y = (0, 256)


def _decode_named(buf, parsed, name):
    hits = find_entry(parsed, name)
    if not hits:
        raise ValueError("frontend texture is absent: %s" % name)
    _, entry = hits[0]
    payload = buf[entry["offset"]:entry["offset"] + entry["size"]]
    descriptor, mips = decode_entry(entry["info"], payload, max_mips=1)
    return mips[0][:descriptor["height"], :descriptor["width"]]


def stitch_frame(tiles):
    if len(tiles) != 6:
        raise ValueError("a frontend animation frame requires six tiles")
    frame = np.zeros((FRAME_HEIGHT, FRAME_WIDTH, 4), dtype=np.uint8)
    for tile_index, tile in enumerate(tiles):
        row, column = divmod(tile_index, 3)
        x = TILE_X[column]
        y = TILE_Y[row]
        expected_width = min(256, FRAME_WIDTH - x)
        expected_height = min(256, FRAME_HEIGHT - y)
        if (
            tile.ndim != 3 or
            tile.shape[2] != 4 or
            tile.shape[0] < expected_height or
            tile.shape[1] < expected_width or
            tile.shape[0] > 256 or
            tile.shape[1] > 256
        ):
            raise ValueError(
                "tile %d dimensions cannot cover %dx%d: %r" %
                (
                    tile_index + 1,
                    expected_height,
                    expected_width,
                    tile.shape,
                ))
        frame[
            y:y + expected_height,
            x:x + expected_width,
        ] = tile[:expected_height, :expected_width]
    return frame


def build_sheet(buf, parsed, prefix, frame_count):
    frames = []
    for frame_number in range(1, frame_count + 1):
        tiles = [
            _decode_named(
                buf,
                parsed,
                "%s_%d_%d" % (prefix, frame_number, tile_number))
            for tile_number in range(1, 7)
        ]
        frames.append(stitch_frame(tiles))
    return np.concatenate(frames, axis=0)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("bank")
    parser.add_argument("forest_output")
    parser.add_argument("sunbeam_output")
    parser.add_argument(
        "--theme",
        choices=("forest", "coastal", "spooky"),
        default="forest")
    args = parser.parse_args()

    buf, parsed = load_big(args.bank)
    theme_prefix = {
        "forest": "FORREST",
        "coastal": "COASTAL",
        "spooky": "SPOOKY",
    }[args.theme]
    forest = build_sheet(buf, parsed, theme_prefix, 4)
    sunbeam = build_sheet(
        buf, parsed, "%s_SUNBEAM" % theme_prefix, 3)
    Image.fromarray(forest).save(args.forest_output)
    Image.fromarray(sunbeam).save(args.sunbeam_output)
    print(
        "FABLE_FRONTEND_ANIMATION PASS theme=%s forest=%dx%d frames=4 "
        "sunbeam=%dx%d frames=3 alpha=%d..%d" %
        (
            args.theme,
            forest.shape[1],
            forest.shape[0],
            sunbeam.shape[1],
            sunbeam.shape[0],
            int(sunbeam[..., 3].min()),
            int(sunbeam[..., 3].max()),
        )
    )


if __name__ == "__main__":
    main()
