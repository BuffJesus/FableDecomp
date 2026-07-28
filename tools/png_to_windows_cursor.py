#!/usr/bin/env python3
"""Convert a retail RGBA pointer PNG into a single-image Windows cursor."""

import argparse
import struct

from PIL import Image


def build_cursor(image, hotspot=(0, 0)):
    rgba = image.convert("RGBA")
    width, height = rgba.size
    if not 1 <= width <= 256 or not 1 <= height <= 256:
        raise ValueError("cursor dimensions must be in the 1..256 range")
    if not 0 <= hotspot[0] < width or not 0 <= hotspot[1] < height:
        raise ValueError("cursor hotspot lies outside the image")

    xor = bytearray()
    for y in range(height - 1, -1, -1):
        for red, green, blue, alpha in (
                rgba.getpixel((x, y)) for x in range(width)):
            xor.extend((blue, green, red, alpha))

    mask_stride = ((width + 31) // 32) * 4
    and_mask = bytes(mask_stride * height)
    bitmap_info = struct.pack(
        "<IIIHHIIIIII",
        40,
        width,
        height * 2,
        1,
        32,
        0,
        len(xor) + len(and_mask),
        0,
        0,
        0,
        0,
    )
    image_data = bitmap_info + bytes(xor) + and_mask
    directory = struct.pack("<HHH", 0, 2, 1)
    entry = struct.pack(
        "<BBBBHHII",
        0 if width == 256 else width,
        0 if height == 256 else height,
        0,
        0,
        hotspot[0],
        hotspot[1],
        len(image_data),
        len(directory) + 16,
    )
    return directory + entry + image_data


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input")
    parser.add_argument("output")
    parser.add_argument(
        "--hotspot", nargs=2, type=int, default=(0, 0),
        metavar=("X", "Y"))
    args = parser.parse_args()

    image = Image.open(args.input)
    payload = build_cursor(image, tuple(args.hotspot))
    with open(args.output, "wb") as stream:
        stream.write(payload)
    print(
        "FABLE_WINDOWS_CURSOR PASS size=%dx%d hotspot=%d,%d output=%s" %
        (
            image.width,
            image.height,
            args.hotspot[0],
            args.hotspot[1],
            args.output,
        ))


if __name__ == "__main__":
    main()
