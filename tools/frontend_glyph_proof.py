#!/usr/bin/env python3
"""Proof + metrics dump for a runtime glyph renderer.

Loads the retail Fable static font, renders real save-profile names with it (proof
the atlas+metrics pipeline produces correct text), and dumps the ASCII glyph
metrics (atlas UV rect + advance) that a C++ DrawString will consume when the
glyph atlas is baked into g_OptionsTexture's free region.
"""
import os, sys
from PIL import Image
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from render_fable_static_font import load_font, render_line, add_outline

FONT_BANK = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\fonts.big"
FONT_NAME = "ENG_ARIAL_12"
OUT = sys.argv[1] if len(sys.argv) > 1 else \
    r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\60e53032-daf0-4b7f-b8fd-c49d9c4b10b6\scratchpad\glyph_proof.png"

font = load_font(FONT_BANK, FONT_NAME)
aw, ah = font["atlas"].size
print(f"font={font['face']!r} height={font['height']} maxH={font['max_height']} "
      f"atlas={aw}x{ah} chars={font['min_char']}..{font['max_char']}")

# Render real save names (from the user's Saves dir) as a proof image.
names = ["Cornelio", "Default", "AutoSave", "112121212", "Empty Slot"]
canvas = Image.new("RGBA", (320, 24 * len(names) + 8), (16, 16, 32, 255))
for i, nm in enumerate(names):
    line = render_line(font, nm, canvas.size, (8, 6 + i * 24), "left", 1.0)
    canvas.alpha_composite(add_outline(line, 1))
canvas.save(OUT)
print(f"proof render -> {OUT}")

# Dump ASCII glyph metrics (what the C++ table needs).
print("\nASCII glyph metrics (char: uv_left,uv_top,uv_right,uv_bottom  xoff  advance):")
sample = "AZaz09 .Cornelio"
for ch in sample:
    code = ord(ch)
    if not font["min_char"] <= code <= font["max_char"]:
        continue
    l, t, r, b, xo, _, adv = font["glyphs"][code - font["min_char"]]
    print(f"  {ch!r:5} {l:.4f},{t:.4f},{r:.4f},{b:.4f}  xoff={xo:2d} adv={adv}")
