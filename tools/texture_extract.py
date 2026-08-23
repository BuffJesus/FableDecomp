#!/usr/bin/env python3
"""Extract a texture entry from a .big to a PNG (for round-trip editing in an
external image editor, then re-import with texture_build/card_studio).

Usage:
  python tools/texture_extract.py <big> --id <N>       <out.png>
  python tools/texture_extract.py <big> --entry <NAME> <out.png>

Decodes MIP 0 and crops to the entry's real width/height. The alloc padding
(pow2) is dropped so the editor shows exactly the visible texel grid.
"""
import sys, os
sys.path.insert(0, os.path.dirname(__file__))
import texture_build as tb


# Quest-start backdrop atlas (must match card_studio.QUEST_START_LAYOUT).
_QS_LAYOUT = {4722: (0, 0, 256, 256), 4723: (256, 0, 256, 256), 4724: (512, 0, 256, 256),
              4725: (0, 256, 256, 256), 4726: (256, 256, 256, 256), 4727: (512, 256, 256, 256),
              4728: (0, 512, 256, 128), 4729: (256, 512, 256, 128), 4730: (512, 512, 256, 128)}


def extract_backdrop(big, out):
    """Reassemble the 9 quest-start tiles into one 768x640 PNG (for whole-background editing)."""
    from PIL import Image
    buf, parsed = tb.load_big(big)
    by = {e["id"]: e for s, es in parsed for e in es}
    canvas = Image.new("RGB", (768, 640), (0, 0, 0))
    for tid, (x, y, w, h) in _QS_LAYOUT.items():
        e = by[tid]
        info, mips = tb.decode_entry(e["info"], buf[e["offset"]:e["offset"] + e["size"]])
        canvas.paste(Image.fromarray(mips[0][:info["height"], :info["width"]]).convert("RGB"), (x, y))
    canvas.save(out)
    print("reassembled quest-start backdrop (768x640) -> %s" % out)
    return 0


def main(argv):
    if len(argv) >= 3 and argv[1] == "--backdrop":
        return extract_backdrop(argv[0], argv[2])
    if len(argv) < 4:
        print(__doc__); return 2
    big = argv[0]
    key_flag, key_val, out = argv[1], argv[2], argv[3]
    buf, parsed = tb.load_big(big)
    want_id = int(key_val, 0) if key_flag == "--id" else None
    want_name = key_val if key_flag == "--entry" else None
    hit = None
    for s, es in parsed:
        for e in es:
            if (want_id is not None and e["id"] == want_id) or \
               (want_name and (e["name"] == want_name or e["name"].endswith(want_name))):
                hit = e; break
        if hit:
            break
    if not hit:
        sys.stderr.write("entry not found: %s %s\n" % (key_flag, key_val)); return 1
    info, mips = tb.decode_entry(hit["info"], buf[hit["offset"]:hit["offset"] + hit["size"]])
    rgba = mips[0][:info["height"], :info["width"]]
    tb.save_png(rgba, out)
    print("extracted %s (id=%d) %dx%d %s -> %s"
          % (hit["name"], hit["id"], info["width"], info["height"], info["fmt_name"], out))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
