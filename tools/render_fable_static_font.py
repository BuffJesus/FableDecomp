#!/usr/bin/env python3
"""Render text with a retail Fable static-font atlas from fonts.big.

This is the small, non-streaming font format used by FONT_ENGLISH_MAIN.
Its payload layout is ported from EgoCore's FontParser.h:

  ASCIIZ face name
  u32 height, u32 weight, u8 italic
  u32 maxHeight, textureWidth, textureHeight, minChar, maxChar, bankCount
  bankCount * {u32 bankIndex, u32 unknown, u32 glyphCount,
               glyphCount * <ffffhhh>}
  u32 tgaSize, TGA bytes

Example:
  python tools/render_fable_static_font.py fonts.big ENG_ARIAL_24 \
      "Press Left Mouse Button To Continue" prompt.png \
      --canvas 640 480 --position 320 240 --align center
"""

import argparse
import io
import os
import struct
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from parse_bigb import parse_footer, parse_header, parse_toc  # noqa: E402


GLYPH = struct.Struct("<ffffhhh")


def load_font(bank_path, font_name):
    data = open(bank_path, "rb").read()
    _, _, footer_offset, _ = parse_header(data)
    subbanks, _ = parse_footer(data, footer_offset)
    static_bank = next(
        (s for s in subbanks if s["name"] == "FONT_ENGLISH_MAIN"), None)
    if static_bank is None:
        raise ValueError("FONT_ENGLISH_MAIN is absent")
    entries, _, _, error = parse_toc(data, static_bank, footer_offset)
    if error:
        raise ValueError(error)
    entry = next((e for e in entries if e["name"] == font_name), None)
    if entry is None:
        raise ValueError("font entry is absent: %s" % font_name)

    raw = data[entry["offset"]:entry["offset"] + entry["size"]]
    zero = raw.index(b"\0")
    offset = zero + 1
    face_name = raw[:zero].decode("latin1")
    height, weight = struct.unpack_from("<II", raw, offset)
    offset += 8
    italic = bool(raw[offset])
    offset += 1
    max_height, texture_width, texture_height, min_char, max_char, bank_count = (
        struct.unpack_from("<IIIIII", raw, offset)
    )
    offset += 24

    glyphs = []
    for _ in range(bank_count):
        _, _, glyph_count = struct.unpack_from("<III", raw, offset)
        offset += 12
        for _ in range(glyph_count):
            glyphs.append(GLYPH.unpack_from(raw, offset))
            offset += GLYPH.size

    (tga_size,) = struct.unpack_from("<I", raw, offset)
    offset += 4
    atlas = Image.open(
        io.BytesIO(raw[offset:offset + tga_size])).convert("RGBA")
    if atlas.size != (texture_width, texture_height):
        raise ValueError(
            "atlas dimensions differ from metadata: %r vs %dx%d" %
            (atlas.size, texture_width, texture_height))
    expected = max_char - min_char + 1
    if len(glyphs) != expected:
        raise ValueError(
            "glyph count differs from character range: %d vs %d" %
            (len(glyphs), expected))
    return {
        "face": face_name,
        "height": height,
        "weight": weight,
        "italic": italic,
        "max_height": max_height,
        "min_char": min_char,
        "max_char": max_char,
        "glyphs": glyphs,
        "atlas": atlas,
    }


def text_advance(font, text):
    total = 0
    for character in text:
        code = ord(character)
        if font["min_char"] <= code <= font["max_char"]:
            total += font["glyphs"][code - font["min_char"]][6]
    return total


def render_line(font, text, canvas_size, position, alignment):
    canvas = Image.new("RGBA", canvas_size, (0, 0, 0, 0))
    atlas = font["atlas"]
    atlas_width, atlas_height = atlas.size
    x = float(position[0])
    if alignment == "center":
        x -= text_advance(font, text) * 0.5
    elif alignment == "right":
        x -= text_advance(font, text)
    y = int(round(position[1]))

    for character in text:
        code = ord(character)
        if not font["min_char"] <= code <= font["max_char"]:
            continue
        left, top, right, bottom, x_offset, _, advance = (
            font["glyphs"][code - font["min_char"]]
        )
        # Retail UVs use the inclusive 0..(dimension-1) coordinate range.
        crop_left = int(round(left * (atlas_width - 1)))
        crop_top = int(round(top * (atlas_height - 1)))
        crop_right = int(round(right * (atlas_width - 1))) + 1
        crop_bottom = int(round(bottom * (atlas_height - 1))) + 1
        glyph = atlas.crop(
            (crop_left, crop_top, crop_right, crop_bottom))
        canvas.alpha_composite(
            glyph,
            (int(round(x + x_offset)), y))
        x += advance
    return canvas


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("bank")
    parser.add_argument("font")
    parser.add_argument("text")
    parser.add_argument("output")
    parser.add_argument(
        "--canvas", nargs=2, type=int, metavar=("WIDTH", "HEIGHT"),
        default=(640, 480))
    parser.add_argument(
        "--position", nargs=2, type=float, metavar=("X", "Y"),
        default=(0, 0))
    parser.add_argument(
        "--align", choices=("left", "center", "right"), default="left")
    args = parser.parse_args()

    font = load_font(args.bank, args.font)
    image = render_line(
        font,
        args.text,
        tuple(args.canvas),
        tuple(args.position),
        args.align)
    image.save(args.output)
    print(
        "FABLE_STATIC_FONT_RENDER PASS font=%s face=%s height=%d "
        "range=%d..%d output=%s" %
        (
            args.font,
            font["face"],
            font["height"],
            font["min_char"],
            font["max_char"],
            args.output,
        )
    )


if __name__ == "__main__":
    main()
