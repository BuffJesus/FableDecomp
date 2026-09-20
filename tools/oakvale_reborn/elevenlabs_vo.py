#!/usr/bin/env python3
"""elevenlabs_vo.py -- generate the Oakvale Reborn voice lines with ElevenLabs.

    python tools/oakvale_reborn/elevenlabs_vo.py --dry-run          # lines, voices, character cost
    python tools/oakvale_reborn/elevenlabs_vo.py                    # generate every vo: true line
    python tools/oakvale_reborn/elevenlabs_vo.py --key TEXT_OVR_OFFER_010 --force

Reads manifest `lines` (vo: true only) and `voices` (eleven_voice_id per voice),
asks the ElevenLabs text-to-speech endpoint for raw PCM at 22050 Hz mono
(`output_format=pcm_22050` -- the retail speech rate, so dialogue_pipeline's
prep_pcm has nothing to resample), wraps it in a PCM16 RIFF, normalises it, and
writes work/oakvale_reborn/vo/<key>.wav, which build_custom_intro.py `text`
consumes.

Post-processing (in this order): trim leading/trailing silence below -50 dBFS
keeping 150 ms of room, peak-normalise to -1 dBFS, then bring the RMS to about
-20 dBFS (a rough loudness match to retail ScriptDialogue clips) without
letting the peak exceed -1 dBFS.

Per voice the manifest may set `model` (default eleven_v3) and `settings`
(stability / similarity_boost / style / use_speaker_boost); per line, `direction`
is an inline performance tag prepended for v3 models only ("[quiet, amused]").

Cache: work/oakvale_reborn/vo_cache/<sha256(text|voice_id|model|settings)>.pcm
holds the raw synthesis, so re-running with unchanged text/voice costs nothing;
--force ignores it. The API key comes from ELEVENLABS_API_KEY only.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import pathlib
import struct
import sys

import yaml  # noqa: F401

REPO = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / 'tools'))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import xbadpcm  # noqa: E402
import oakvale_manifest  # noqa: E402

DEFAULT_MANIFEST = REPO / 'refs/script_recovery/authored/OakvaleReborn/manifest/intro.yaml'
OUT_DIR = REPO / 'work/oakvale_reborn/vo'
CACHE_DIR = REPO / 'work/oakvale_reborn/vo_cache'
RATE = 22050
API = 'https://api.elevenlabs.io/v1/text-to-speech/{voice_id}?output_format=pcm_22050'
MODEL = 'eleven_v3'
SETTINGS = {'stability': 0.0, 'similarity_boost': 0.8, 'style': 0.0, 'use_speaker_boost': True}  # v3 Creative


def voice_plan(voice: dict, ln: dict, default_model: str) -> tuple[str, dict, str]:
    """(model, settings, prompt text) for a line: the voice's overrides, the line's direction tag for v3."""
    model = voice.get('model') or default_model
    settings = dict(SETTINGS)
    settings.update(voice.get('settings') or {})
    text = ln['text']
    if ln.get('direction') and model.startswith('eleven_v3'):
        text = f"[{ln['direction'].strip('[]')}] {text}"
    return model, settings, text


def cache_key(text: str, voice_id: str, model: str, settings: dict) -> str:
    blob = '|'.join([text, voice_id, model, json.dumps(settings, sort_keys=True)])
    return hashlib.sha256(blob.encode('utf-8')).hexdigest()


def synthesize(text: str, voice_id: str, api_key: str, model: str, settings: dict) -> bytes:
    import requests
    r = requests.post(API.format(voice_id=voice_id),
                      headers={'xi-api-key': api_key, 'accept': 'application/octet-stream',
                               'content-type': 'application/json'},
                      json={'text': text, 'model_id': model, 'voice_settings': settings}, timeout=120)
    if r.status_code != 200:
        raise SystemExit(f'ElevenLabs {r.status_code}: {r.text[:300]}')
    return r.content


# --- post-processing on int16 samples ---------------------------------------------

def db_to_lin(db: float) -> float:
    return 10 ** (db / 20.0)


def trim_silence(samples: list[int], floor_db: float = -50.0, keep_ms: int = 150) -> list[int]:
    thr = db_to_lin(floor_db) * 32767
    first = next((i for i, s in enumerate(samples) if abs(s) > thr), None)
    if first is None:
        return samples
    last = next(i for i in range(len(samples) - 1, -1, -1) if abs(samples[i]) > thr)
    pad = RATE * keep_ms // 1000
    return samples[max(0, first - pad):min(len(samples), last + 1 + pad)]


def normalise(samples: list[int], peak_db: float = -1.0, rms_db: float = -20.0) -> list[int]:
    if not samples:
        return samples
    peak = max(abs(s) for s in samples) or 1
    rms = math.sqrt(sum(s * s for s in samples) / len(samples)) or 1.0
    gain = min(db_to_lin(peak_db) * 32767 / peak, db_to_lin(rms_db) * 32767 / rms)
    return [max(-32768, min(32767, int(round(s * gain)))) for s in samples]


def pcm_bytes_to_samples(blob: bytes) -> list[int]:
    n = len(blob) // 2
    return list(struct.unpack(f'<{n}h', blob[:n * 2]))


def stats(samples: list[int]) -> str:
    if not samples:
        return 'empty'
    peak = max(abs(s) for s in samples) / 32767
    rms = math.sqrt(sum(s * s for s in samples) / len(samples)) / 32767
    return f'{len(samples) / RATE:.2f}s peak {20 * math.log10(peak or 1e-9):.1f} dBFS rms {20 * math.log10(rms or 1e-9):.1f} dBFS'


# --- main ---------------------------------------------------------------------------

def main() -> int:
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--manifest', type=pathlib.Path, default=DEFAULT_MANIFEST)
    a.add_argument('--out', type=pathlib.Path, default=OUT_DIR)
    a.add_argument('--key', action='append', help='only these line keys (repeatable)')
    a.add_argument('--titles', action='store_true', help='only the hero-title lines')
    a.add_argument('--no-titles', action='store_true', help='skip the hero-title lines')
    a.add_argument('--dry-run', action='store_true', help='list what would be generated and the character cost')
    a.add_argument('--force', action='store_true', help='ignore the synthesis cache')
    a.add_argument('--model', default=MODEL)
    args = a.parse_args()

    m = oakvale_manifest.load(args.manifest)
    voices = m.get('voices') or {}
    lines = [ln for ln in (m.get('lines') or []) if ln.get('vo')]
    if args.key:
        lines = [ln for ln in lines if ln['key'] in set(args.key)]
    if args.titles:
        lines = [ln for ln in lines if ln.get('title')]
    if args.no_titles:
        lines = [ln for ln in lines if not ln.get('title')]
    if not lines:
        print('no vo: true lines selected')
        return 0

    problems = 0
    chars = 0
    for ln in lines:
        voice = voices.get(ln.get('voice') or '', {})
        vid = voice.get('eleven_voice_id') or ''
        chars += len(ln['text'])
        status = 'ok' if vid else 'NO eleven_voice_id'
        if not vid:
            problems += 1
        model, settings, prompt = voice_plan(voice, ln, args.model)
        cached = (CACHE_DIR / f'{cache_key(prompt, vid, model, settings)}.pcm').exists() if vid else False
        print(f'{ln["key"]:32s} {ln.get("voice", "?"):10s} {len(ln["text"]):4d} chars  {"cached" if cached else "new   "}  {model:22s} {status}'
              + (f'  dir=[{ln["direction"]}]' if ln.get('direction') else ''))
    print(f'{len(lines)} line(s), {chars} characters{" (dry run)" if args.dry_run else ""}')
    if args.dry_run:
        return 1 if problems else 0
    if problems:
        raise SystemExit('fill in eleven_voice_id for every voice first')
    api_key = os.environ.get('ELEVENLABS_API_KEY', '')

    args.out.mkdir(parents=True, exist_ok=True)
    CACHE_DIR.mkdir(parents=True, exist_ok=True)
    for ln in lines:
        voice = voices[ln['voice']]
        vid = voice['eleven_voice_id']
        model, settings, prompt = voice_plan(voice, ln, args.model)
        cache = CACHE_DIR / f'{cache_key(prompt, vid, model, settings)}.pcm'
        if cache.exists() and not args.force:
            raw = cache.read_bytes()
        else:
            if not api_key:
                raise SystemExit('ELEVENLABS_API_KEY is not set')
            raw = synthesize(prompt, vid, api_key, model, settings)
            cache.write_bytes(raw)
        samples = normalise(trim_silence(pcm_bytes_to_samples(raw)))
        wav = args.out / f'{ln["key"]}.wav'
        wav.write_bytes(xbadpcm.build_pcm_riff(samples, 1, RATE))
        print(f'{ln["key"]:32s} -> {wav.name}  {stats(samples)}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
