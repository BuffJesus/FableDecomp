#!/usr/bin/env python3
"""Sample Fable TLC's authored environment/RSA colour lookup table.

Retail CEnvironmentLookupTexture::LookupFloatColour samples one exact texel at
(column, row), reads BGRA memory, and converts every channel with
(byte + 0.5) / 255.0. Time-of-day/theme blending happens after these samples
are placed in CBlendedEnvironmentTheme.
"""

import argparse
import json
from collections import OrderedDict

from PIL import Image


# Exact row indices decoded from the retail ENVIRONMENT definition.
LOOKUP_ROWS = OrderedDict((
    ("diffuse", 0),
    ("ambient", 1),
    ("cloud", 2),
    ("backlight", 3),
    ("reflection", 4),
    ("mist_effect", 5),
    ("fog_colour", 6),
    ("fog_alpha", 7),
    ("sun", 8),
    ("sunlight_attenuator", 8),
    ("moon", 9),
    ("stars", 10),
    ("sun_flare", 11),
    ("lens_flare", 12),
    ("sky_gradient_top", 13),
    ("sky_gradient_top_alpha", 14),
    ("sky_gradient_bottom", 15),
    ("sky_gradient_bottom_alpha", 16),
    ("water", 17),
    ("sea", 18),
    ("glow_threshold", 19),
    ("glow_bloom", 20),
))


def engine_float_channel(value):
    """Apply the retail byte-to-float conversion (which maps 255 above 1)."""
    return (value + 0.5) / 255.0


def sample_lookup(image, column, row):
    """Return byte and engine-float RGBA values for one lookup texel."""
    rgba = image.convert("RGBA").getpixel((column, row))
    return {
        "rgba8": list(rgba),
        "rgba_float": [engine_float_channel(value) for value in rgba],
    }


def build_report(path, columns):
    with Image.open(path) as source:
        image = source.convert("RGBA")
        width, height = image.size
        if height <= max(LOOKUP_ROWS.values()):
            raise ValueError(
                f"lookup is only {height} rows; retail requires row 20"
            )
        reports = []
        for label, column in columns:
            if not 0 <= column < width:
                raise ValueError(
                    f"column {column} for {label!r} is outside 0..{width - 1}"
                )
            reports.append({
                "label": label,
                "column": column,
                "rows": {
                    name: {
                        "row": row,
                        **sample_lookup(image, column, row),
                    }
                    for name, row in LOOKUP_ROWS.items()
                },
            })
    return {
        "path": str(path),
        "dimensions": [width, height],
        "sample_formula": "(channel_byte + 0.5) / 255.0",
        "columns": reports,
    }


def _parse_column(value):
    if "=" in value:
        label, text = value.split("=", 1)
        return label, int(text, 0)
    column = int(value, 0)
    return str(column), column


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("lookup", help="path to lighting_colours.tga")
    parser.add_argument(
        "--column", action="append", required=True, type=_parse_column,
        help="column or LABEL=column; may be repeated",
    )
    parser.add_argument("--json", metavar="PATH", help="write the full report")
    args = parser.parse_args()

    report = build_report(args.lookup, args.column)
    print(
        f'{report["dimensions"][0]}x{report["dimensions"][1]} '
        f'{report["path"]}'
    )
    for column in report["columns"]:
        print(f'{column["label"]}: column {column["column"]}')
        for name, sample in column["rows"].items():
            print(
                f'  row {sample["row"]:2d} {name:26s} '
                f'{sample["rgba8"]} -> '
                + ", ".join(
                    f"{value:.9g}" for value in sample["rgba_float"]
                )
            )
    if args.json:
        with open(args.json, "w", encoding="utf-8") as output:
            json.dump(report, output, indent=2)
            output.write("\n")
        print(f'wrote {len(report["columns"])} columns -> {args.json}')


if __name__ == "__main__":
    main()
