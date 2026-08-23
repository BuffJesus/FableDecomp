#!/usr/bin/env python3
"""Generate Fable game textures from existing ones with OpenAI image models.

This is the MIDDLE step of the texture pipeline; it takes/produces plain PNGs:

    1. extract an existing texture ->  texture_build.py decode <big> <entry> ref.png
    2. generate a new texture     ->  ai_texture.py "<prompt>" --ref ref.png --out new.png
    3. encode to game format      ->  texture_build.py build new.png out_prefix --format dxt3

Modes:
  * with --ref IMAGE...  -> image EDIT (re-imagine existing texture(s), keeping shape/theme)
  * without --ref        -> pure generation from the prompt

SECURITY: the OpenAI API key is read from the OPENAI_API_KEY environment variable
ONLY. It is never written to disk, never logged, never committed. Rotate any key
that has been shared in plaintext.

Examples:
  set OPENAI_API_KEY=sk-...            (PowerShell: $env:OPENAI_API_KEY="sk-...")
  python tools/ai_texture.py "a glowing purple quest orb, game UI icon, centered" \\
      --ref work/orb_core.png --out work/orb_custom.png --final-size 128 --transparent
  python tools/ai_texture.py "weathered oak quest card parchment" --n 3 \\
      --out work/card.png --final-size 256
"""
import argparse
import base64
import os
import sys


def eprint(*a):
    print(*a, file=sys.stderr)


def load_client():
    key = os.environ.get("OPENAI_API_KEY")
    if not key:
        eprint("error: OPENAI_API_KEY is not set. Export your (rotated) key first;")
        eprint("       this tool never stores or hard-codes it.")
        sys.exit(2)
    try:
        from openai import OpenAI
    except ImportError:
        eprint("error: the 'openai' package is required (pip install openai).")
        sys.exit(2)
    return OpenAI()  # picks up OPENAI_API_KEY from the environment


def downscale(png_bytes, size):
    """Resize a PNG to size x size, preserving alpha, high-quality."""
    from io import BytesIO
    from PIL import Image
    img = Image.open(BytesIO(png_bytes)).convert("RGBA")
    if size:
        img = img.resize((size, size), Image.LANCZOS)
    out = BytesIO()
    img.save(out, "PNG")
    return out.getvalue()


def main(argv=None):
    ap = argparse.ArgumentParser(
        description="Generate Fable textures from existing ones via OpenAI images.")
    ap.add_argument("prompt", help="what the new texture should look like")
    ap.add_argument("--ref", nargs="*", default=[],
                    help="existing texture PNG(s) to base the result on (edit mode)")
    ap.add_argument("--out", required=True,
                    help="output PNG path; with --n>1, _1/_2/... are appended")
    ap.add_argument("--model", default="gpt-image-1",
                    help="OpenAI image model (default gpt-image-1)")
    ap.add_argument("--size", default="1024x1024",
                    help="generation size the API renders at (1024x1024|1536x1024|"
                         "1024x1536|auto)")
    ap.add_argument("--final-size", type=int, default=None,
                    help="downscale each result to NxN (power of 2 for game textures)")
    ap.add_argument("--n", type=int, default=1, help="number of variations")
    ap.add_argument("--transparent", action="store_true",
                    help="request a transparent background (sprites/orbs/icons)")
    ap.add_argument("--quality", default="high",
                    help="gpt-image-1 quality: low|medium|high|auto")
    args = ap.parse_args(argv)

    client = load_client()

    kwargs = dict(model=args.model, prompt=args.prompt, size=args.size, n=args.n)
    # gpt-image-1 extras (dall-e models reject these, so gate on model name).
    if args.model.startswith("gpt-image"):
        kwargs["quality"] = args.quality
        if args.transparent:
            kwargs["background"] = "transparent"

    try:
        if args.ref:
            for p in args.ref:
                if not os.path.isfile(p):
                    eprint(f"error: --ref not found: {p}")
                    sys.exit(2)
            files = [open(p, "rb") for p in args.ref]
            try:
                resp = client.images.edit(image=files, **kwargs)
            finally:
                for f in files:
                    f.close()
            eprint(f"edited from {len(args.ref)} reference(s) -> {args.n} result(s)")
        else:
            resp = client.images.generate(**kwargs)
            eprint(f"generated {args.n} result(s) from prompt")
    except Exception as e:  # surface the API error without leaking the key
        eprint(f"OpenAI request failed: {type(e).__name__}: {e}")
        sys.exit(1)

    root, ext = os.path.splitext(args.out)
    ext = ext or ".png"
    written = []
    for i, item in enumerate(resp.data):
        b64 = getattr(item, "b64_json", None)
        if not b64:
            eprint("error: response had no image data (b64_json missing).")
            sys.exit(1)
        png = base64.b64decode(b64)
        if args.final_size:
            png = downscale(png, args.final_size)
        path = args.out if args.n == 1 else f"{root}_{i + 1}{ext}"
        with open(path, "wb") as fh:
            fh.write(png)
        written.append(path)

    for p in written:
        print(p)
    dims = f"{args.final_size}x{args.final_size}" if args.final_size else "(source)"
    eprint(f"wrote {len(written)} PNG(s) at {dims}. Next: "
           f"texture_build.py build <png> <out_prefix> --format dxt3")
    return 0


if __name__ == "__main__":
    sys.exit(main())
