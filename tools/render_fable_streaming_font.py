#!/usr/bin/env python3
"""Render Unicode text with a retail Fable streaming font from fonts.big.

The PC streaming-font sub-bank stores one metadata entry per named font and a
shared ``GlyphData`` entry:

  metadata := ASCIIZ source-name
              u32 height, u32 weight, u8 italic, u32 max-height
              u32 bank-offsets[1025]
              1024 variable glyph banks

  glyph-bank := u16 compressed-and-size
                LZO1X({u16 first, u16 count, u32 glyph-index[count]})

  GlyphData := u32 glyph-count, adjustment-count, binary-data-start,
                   compressed-metric-size, metric-chunk-size
               LZO1X metric chunks + chunk offsets + adjustments
               packed 32x32/64x64 alpha blocks

The packed metadata and pixel lookup are ported from EgoCore's
``StreamingFontParser.h``. Fable's in-tree LZO1X decoder handles the compressed
blocks, so this tool has no native codec dependency.
"""

import argparse
import os
import re
import struct
import sys

from PIL import Image, ImageFilter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from lionhead_lz import decompress  # noqa: E402
from parse_bigb import parse_footer, parse_header, parse_toc  # noqa: E402


METRIC = struct.Struct("<bbBBI")
GLYPH_BANK_HEADER = struct.Struct("<HH")
PIXEL_HEADER = struct.Struct("<IIIII")
GLYPH_INDEX_MASK = 0x00FFFFFF
MEMORY_OFFSET_MASK = 0x003FFFFF
INVALID_MEMORY_OFFSET = MEMORY_OFFSET_MASK


def _checked_slice(raw, offset, size, description):
    if offset < 0 or size < 0 or offset + size > len(raw):
        raise ValueError(
            "%s exceeds its payload: offset=%d size=%d payload=%d" %
            (description, offset, size, len(raw)))
    return raw[offset:offset + size]


def _parse_metadata(raw):
    try:
        zero = raw.index(b"\0")
    except ValueError as error:
        raise ValueError("streaming-font source name is not terminated") from error

    cursor = zero + 1
    if cursor + 13 + 1025 * 4 > len(raw):
        raise ValueError("streaming-font metadata header is truncated")
    height, weight = struct.unpack_from("<II", raw, cursor)
    cursor += 8
    italic = bool(raw[cursor])
    cursor += 1
    (max_height,) = struct.unpack_from("<I", raw, cursor)
    cursor += 4
    offsets = struct.unpack_from("<1025I", raw, cursor)
    cursor += 1025 * 4

    if offsets[0] != 0:
        raise ValueError("first streaming glyph-bank offset is not zero")
    if any(end < start for start, end in zip(offsets, offsets[1:])):
        raise ValueError("streaming glyph-bank offsets are not monotonic")
    if cursor + offsets[-1] != len(raw):
        raise ValueError(
            "streaming glyph-bank table does not consume the metadata payload")

    banks = []
    for bank_index, (start, end) in enumerate(zip(offsets, offsets[1:])):
        if start == end:
            banks.append({"first": 64, "indices": ()})
            continue

        block = _checked_slice(
            raw, cursor + start, end - start,
            "streaming glyph bank %d" % bank_index)
        if len(block) < 2:
            raise ValueError(
                "streaming glyph bank %d has no block header" % bank_index)
        (block_header,) = struct.unpack_from("<H", block)
        payload = block[2:]
        declared_size = block_header & 0x7FFF
        if declared_size != len(payload):
            raise ValueError(
                "streaming glyph bank %d size differs: %d vs %d" %
                (bank_index, declared_size, len(payload)))
        decoded = (
            decompress(payload)
            if block_header & 0x8000
            else payload
        )
        if len(decoded) < GLYPH_BANK_HEADER.size:
            raise ValueError(
                "streaming glyph bank %d is truncated" % bank_index)
        first, count = GLYPH_BANK_HEADER.unpack_from(decoded)
        if first > 64 or count > 64 or first + count > 64:
            raise ValueError(
                "streaming glyph bank %d has invalid range %d..%d" %
                (bank_index, first, first + count))
        expected_size = GLYPH_BANK_HEADER.size + count * 4
        if len(decoded) != expected_size:
            raise ValueError(
                "streaming glyph bank %d decoded size differs: %d vs %d" %
                (bank_index, len(decoded), expected_size))
        indices = tuple(
            value & GLYPH_INDEX_MASK
            for value in struct.unpack_from(
                "<%dI" % count, decoded, GLYPH_BANK_HEADER.size)
        )
        banks.append({"first": first, "indices": indices})

    return {
        "face": raw[:zero].decode("latin1"),
        "height": height,
        "weight": weight,
        "italic": italic,
        "max_height": max_height,
        "glyph_banks": banks,
    }


def _parse_pixel_data(raw):
    if len(raw) < PIXEL_HEADER.size:
        raise ValueError("streaming GlyphData header is truncated")
    (
        glyph_count,
        adjustment_count,
        binary_data_start,
        compressed_size,
        chunk_size,
    ) = PIXEL_HEADER.unpack_from(raw)
    if glyph_count == 0 or chunk_size == 0:
        raise ValueError("streaming GlyphData has an empty metric table")

    compressed_start = PIXEL_HEADER.size
    compressed = _checked_slice(
        raw, compressed_start, compressed_size,
        "compressed streaming-glyph metrics")
    cursor = compressed_start + compressed_size
    index_count = (glyph_count + chunk_size - 1) // chunk_size + 1
    index_bytes = index_count * 4
    indices_raw = _checked_slice(
        raw, cursor, index_bytes, "streaming-glyph chunk indices")
    indices = struct.unpack("<%dI" % index_count, indices_raw)
    cursor += index_bytes

    if indices[0] != 0 or indices[-1] != compressed_size:
        raise ValueError(
            "streaming-glyph chunk indices do not span compressed metrics")
    if any(end < start for start, end in zip(indices, indices[1:])):
        raise ValueError("streaming-glyph chunk indices are not monotonic")
    if binary_data_start < cursor or binary_data_start > len(raw):
        raise ValueError("streaming GlyphData binary-data offset is invalid")

    metrics = []
    for chunk_index, (start, end) in enumerate(zip(indices, indices[1:])):
        first_glyph = chunk_index * chunk_size
        expected_count = min(chunk_size, glyph_count - first_glyph)
        expected_size = expected_count * METRIC.size
        decoded = decompress(
            _checked_slice(
                compressed, start, end - start,
                "streaming-glyph metric chunk %d" % chunk_index))
        if len(decoded) != expected_size:
            raise ValueError(
                "streaming-glyph metric chunk %d size differs: %d vs %d" %
                (chunk_index, len(decoded), expected_size))
        metrics.extend(METRIC.iter_unpack(decoded))
    if len(metrics) != glyph_count:
        raise ValueError(
            "streaming glyph metric count differs: %d vs %d" %
            (len(metrics), glyph_count))

    return {
        "glyph_count": glyph_count,
        "adjustment_count": adjustment_count,
        "adjustments": raw[cursor:binary_data_start],
        "metrics": metrics,
        "pixels": raw[binary_data_start:],
        "chunk_size": chunk_size,
    }


def load_font(bank_path, font_name, subbank_name="STREAMING_FONT_ENGLISH_PC"):
    with open(bank_path, "rb") as stream:
        data = stream.read()
    _, _, footer_offset, _ = parse_header(data)
    subbanks, _ = parse_footer(data, footer_offset)
    subbank = next(
        (item for item in subbanks if item["name"] == subbank_name), None)
    if subbank is None:
        raise ValueError("streaming font sub-bank is absent: %s" % subbank_name)
    entries, _, _, error = parse_toc(data, subbank, footer_offset)
    if error:
        raise ValueError(error)

    metadata_entry = next(
        (
            entry for entry in entries
            if entry["name"] == font_name and entry["type"] in (0, 1)
        ),
        None)
    glyph_data_entry = next(
        (
            entry for entry in entries
            if entry["name"] == "GlyphData" and entry["type"] == 2
        ),
        None)
    if metadata_entry is None:
        raise ValueError("streaming font entry is absent: %s" % font_name)
    if glyph_data_entry is None:
        raise ValueError("streaming GlyphData entry is absent")

    metadata_raw = _checked_slice(
        data,
        metadata_entry["offset"],
        metadata_entry["size"],
        "streaming font metadata")
    glyph_data_raw = _checked_slice(
        data,
        glyph_data_entry["offset"],
        glyph_data_entry["size"],
        "streaming GlyphData")
    font = _parse_metadata(metadata_raw)
    font.update(_parse_pixel_data(glyph_data_raw))
    font["name"] = font_name
    font["subbank"] = subbank_name
    return font


def glyph_metric(font, character):
    codepoint = ord(character)
    bank_index, local_index = divmod(codepoint, 64)
    if bank_index >= len(font["glyph_banks"]):
        return None
    bank = font["glyph_banks"][bank_index]
    relative_index = local_index - bank["first"]
    if relative_index < 0 or relative_index >= len(bank["indices"]):
        return None
    one_based_index = bank["indices"][relative_index]
    if one_based_index == 0:
        return None
    metric_index = one_based_index - 1
    if metric_index >= len(font["metrics"]):
        raise ValueError(
            "glyph U+%04X references metric %d beyond %d metrics" %
            (codepoint, metric_index, len(font["metrics"])))
    return font["metrics"][metric_index]


def metric_advance(metric):
    return (metric[4] >> 23) & 0xFF


def _static_ascii_metric(static_font, character):
    if static_font is None:
        return None
    code = ord(character)
    if not static_font["min_char"] <= code <= static_font["max_char"]:
        return None
    return static_font["glyphs"][code - static_font["min_char"]]


def text_advance(font, text, scale=1.0, static_ascii_font=None):
    total = 0
    for character in text:
        if character in "\r\n":
            continue
        static_metric = _static_ascii_metric(
            static_ascii_font, character)
        if static_metric is not None:
            total += static_metric[6]
        else:
            metric = glyph_metric(font, character)
            if metric is not None:
                total += metric_advance(metric)
    return total * scale


def _wrap_paragraph(
        font, paragraph, width, scale=1.0, static_ascii_font=None):
    if (
        width is None or
        width <= 0 or
        text_advance(
            font, paragraph, scale, static_ascii_font) <= width
    ):
        return [paragraph]

    lines = []
    line = ""
    pending_space = ""
    for token in re.findall(r"\S+|[ \t]+", paragraph):
        if token.isspace():
            pending_space += token
            continue
        candidate = line + pending_space + token
        if (
            line and
            text_advance(
                font, candidate, scale, static_ascii_font) > width
        ):
            lines.append(line.rstrip())
            line = token
        else:
            line = candidate
        pending_space = ""
    lines.append(line.rstrip())
    return lines


def wrap_text(
        font, text, width, scale=1.0, static_ascii_font=None):
    lines = []
    for paragraph in text.replace("\r\n", "\n").replace("\r", "\n").split("\n"):
        lines.extend(_wrap_paragraph(
            font, paragraph, width, scale, static_ascii_font))
    return lines


def _glyph_image(font, metric):
    offset_x, offset_y, width, height, packed = metric
    del offset_x, offset_y
    pitch = 64 if packed & 0x80000000 else 32
    width = min(width, pitch)
    height = min(height, pitch)
    memory_offset = packed & MEMORY_OFFSET_MASK
    if (
        width == 0 or
        height == 0 or
        memory_offset == INVALID_MEMORY_OFFSET
    ):
        return None

    source = memory_offset * 1024
    required_end = source + (height - 1) * pitch + width
    if required_end > len(font["pixels"]):
        raise ValueError(
            "glyph alpha block exceeds GlyphData: %d > %d" %
            (required_end, len(font["pixels"])))
    alpha_bytes = b"".join(
        font["pixels"][source + row * pitch:source + row * pitch + width]
        for row in range(height)
    )
    alpha = Image.frombytes("L", (width, height), alpha_bytes)
    glyph = Image.new("RGBA", (width, height), (255, 255, 255, 0))
    glyph.putalpha(alpha)
    return glyph


def _static_glyph_image(font, metric):
    left, top, right, bottom = metric[:4]
    atlas = font["atlas"]
    atlas_width, atlas_height = atlas.size
    crop_left = int(round(left * (atlas_width - 1)))
    crop_top = int(round(top * (atlas_height - 1)))
    crop_right = int(round(right * (atlas_width - 1))) + 1
    crop_bottom = int(round(bottom * (atlas_height - 1))) + 1
    return atlas.crop((crop_left, crop_top, crop_right, crop_bottom))


def render_text(
        font,
        text,
        canvas_size,
        position,
        alignment,
        wrap_width=None,
        line_height=None,
        scale=1.0,
        static_ascii_font=None):
    canvas = Image.new("RGBA", canvas_size, (0, 0, 0, 0))
    lines = wrap_text(
        font, text, wrap_width, scale, static_ascii_font)
    if line_height is None:
        line_height = font["max_height"]
    y = float(position[1])

    for line in lines:
        x = float(position[0])
        advance = text_advance(
            font, line, scale, static_ascii_font)
        if alignment == "center":
            x -= advance * 0.5
        elif alignment == "right":
            x -= advance

        for character in line:
            static_metric = _static_ascii_metric(
                static_ascii_font, character)
            if static_metric is not None:
                glyph = _static_glyph_image(
                    static_ascii_font, static_metric)
                offset_x = static_metric[4]
                offset_y = 0
                advance = static_metric[6]
            else:
                metric = glyph_metric(font, character)
                if metric is None:
                    continue
                glyph = _glyph_image(font, metric)
                offset_x = metric[0]
                offset_y = metric[1]
                advance = metric_advance(metric)
            if glyph is not None:
                if scale != 1.0:
                    glyph = glyph.resize(
                        (
                            max(1, int(round(glyph.width * scale))),
                            max(1, int(round(glyph.height * scale))),
                        ),
                        Image.Resampling.BILINEAR)
                canvas.alpha_composite(
                    glyph,
                    (
                        int(round(x + offset_x * scale)),
                        int(round(y + offset_y * scale)),
                    ))
            x += advance * scale
        y += line_height * scale
    return canvas


def add_outline(image, pixels):
    if pixels <= 0:
        return image
    expanded_alpha = image.getchannel("A").filter(
        ImageFilter.MaxFilter(pixels * 2 + 1))
    outlined = Image.new("RGBA", image.size, (0, 0, 0, 0))
    outlined.putalpha(expanded_alpha)
    outlined.alpha_composite(image)
    return outlined


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("bank")
    parser.add_argument("font")
    parser.add_argument("text")
    parser.add_argument("output")
    parser.add_argument(
        "--subbank", default="STREAMING_FONT_ENGLISH_PC")
    parser.add_argument(
        "--canvas", nargs=2, type=int, metavar=("WIDTH", "HEIGHT"),
        default=(640, 480))
    parser.add_argument(
        "--position", nargs=2, type=float, metavar=("X", "Y"),
        default=(0, 0))
    parser.add_argument(
        "--align", choices=("left", "center", "right"), default="left")
    parser.add_argument("--wrap-width", type=float)
    parser.add_argument("--line-height", type=float)
    parser.add_argument(
        "--scale", type=float, default=1.0,
        help="Scale retail glyph geometry, advances, and line height")
    parser.add_argument(
        "--prefer-static-ascii", action="store_true",
        help=(
            "Use the matching retail static atlas for ASCII while retaining "
            "streaming-font fallback for Unicode glyphs"))
    parser.add_argument(
        "--outline-pixels", type=int, default=0,
        help="Add a black outline in 640x480 design-space pixels")
    parser.add_argument("--require-all-glyphs", action="store_true")
    args = parser.parse_args()

    font = load_font(args.bank, args.font, args.subbank)
    static_ascii_font = None
    if args.prefer_static_ascii:
        from render_fable_static_font import load_font as load_static_font
        static_ascii_font = load_static_font(args.bank, args.font)
    missing = sorted(
        {
            ord(character)
            for character in args.text
            if character not in "\r\n" and glyph_metric(font, character) is None
        })
    if args.require_all_glyphs and missing:
        raise ValueError(
            "font lacks required codepoints: %s" %
            ", ".join("U+%04X" % codepoint for codepoint in missing))
    resolved_count = sum(
        1
        for character in args.text
        if character in "\r\n" or glyph_metric(font, character) is not None
    )
    image = render_text(
        font,
        args.text,
        tuple(args.canvas),
        tuple(args.position),
        args.align,
        args.wrap_width,
        args.line_height,
        args.scale,
        static_ascii_font)
    image = add_outline(image, args.outline_pixels)
    image.save(args.output)
    print(
        "FABLE_STREAMING_FONT_RENDER PASS font=%s face=%s height=%d "
        "metrics=%d text-glyphs=%d missing=%d output=%s" %
        (
            args.font,
            font["face"],
            font["height"],
            len(font["metrics"]),
            resolved_count,
            len(missing),
            args.output,
        )
    )


if __name__ == "__main__":
    main()
