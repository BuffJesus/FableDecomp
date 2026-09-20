#!/usr/bin/env python3
"""table_read.py -- stitch the voiced manifest lines into one listenable WAV, in story order.

    python tools/oakvale_reborn/table_read.py [--out work/oakvale_reborn/table_read.wav] [--gap 0.9]

Reads manifest `lines` in file order (which is beat order), takes each vo: true
line's work/oakvale_reborn/vo/<key>.wav, and writes one 22050 Hz mono PCM16 WAV
with `--gap` seconds of silence between lines and a longer pause at each `# beat`
comment boundary. Subtitle-only lines are skipped but printed, so the printed
script reads as the full scene. Pure Python (xbadpcm RIFF helpers); no ffmpeg.
"""
from __future__ import annotations

import argparse
import pathlib
import struct
import sys

import yaml  # noqa: F401

REPO = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / 'tools'))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import xbadpcm  # noqa: E402
import oakvale_manifest  # noqa: E402

MANIFEST = REPO / 'refs/script_recovery/authored/OakvaleReborn/manifest/intro.yaml'
VO_DIR = REPO / 'work/oakvale_reborn/vo'
RATE = 22050


def beats_in_order(manifest: pathlib.Path) -> list[tuple[str | None, dict]]:
    """(beat comment, line) pairs in file order; the comment is the `# beat ...` line above the block."""
    m = oakvale_manifest.load(manifest)
    by_key = {ln['key']: ln for ln in m.get('lines') or []}
    out, beat = [], None
    for raw in manifest.read_text(encoding='utf-8').splitlines():
        s = raw.strip()
        if s.startswith('# beat'):
            beat = s[2:]
        elif s.startswith('- { key:'):
            key = s.split('key:')[1].split(',')[0].strip()
            if key in by_key:
                out.append((beat, by_key[key]))
                beat = None
    # title lines are not in the file; append them by title and category
    seen = {ln['key'] for _, ln in out}
    last = None
    for ln in m['lines']:
        if ln['key'] in seen or not ln.get('title'):
            continue
        b = ln.get('beat')
        out.append((b if b != last else None, ln))
        last = b
    return out


def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--manifest', type=pathlib.Path, default=MANIFEST)
    a.add_argument('--out', type=pathlib.Path, default=REPO / 'work/oakvale_reborn/table_read.wav')
    a.add_argument('--gap', type=float, default=0.9)
    a.add_argument('--beat-gap', type=float, default=1.8)
    args = a.parse_args()
    samples: list[int] = []
    total_lines = 0
    for beat, ln in beats_in_order(args.manifest):
        if beat:
            print(f'\n{beat}')
            samples += [0] * int(RATE * args.beat_gap)
        who = ln.get('voice') or ln.get('speaker') or '?'
        if not ln.get('vo'):
            print(f'  [{who:8s}] ({ln["key"]}) {ln["text"]}   <subtitle only>')
            continue
        wav = VO_DIR / f'{ln["key"]}.wav'
        if not wav.exists():
            print(f'  [{who:8s}] ({ln["key"]}) MISSING {wav.name}')
            continue
        fmt, data = xbadpcm.parse_wav(wav.read_bytes())
        n = len(data) // 2
        samples += list(struct.unpack(f'<{n}h', data[:n * 2]))
        samples += [0] * int(RATE * args.gap)
        total_lines += 1
        print(f'  [{who:8s}] {ln["text"]}   ({n / RATE:.1f}s)')
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_bytes(xbadpcm.build_pcm_riff(samples, 1, RATE))
    print(f'\n{total_lines} lines, {len(samples) / RATE:.1f}s -> {args.out}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
