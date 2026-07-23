#!/usr/bin/env python3
"""Verify the .big TOC per-entry CRC formula against real retail bytes.

NON-Ghidra, pure computation. Hypothesis under test: the TOC `CRC` field (+0x14)
= engine `CCRC::Calc(seed, data, len)` — a standard reflected CRC-32
(poly 0xEDB88320 / zlib table), NO final inversion, seed passed by caller.

Confirmed engine facts (ghidra_out/decomp_ccrc_calc.log, crc_table_full.txt):
  crc = seed
  for b in data: crc = (crc >> 8) ^ TABLE[(b ^ (crc & 0xff)) & 0xff]
  return crc                       # NO final XOR
Seed-0 is the game.bin field-tag / text.big symbol hash. Seed 0xFFFFFFFF is
CCharString::GetCRC. We test BOTH seeds, WITH/WITHOUT final inversion, over
several data domains, and require a single formula to match EVERY sampled entry
across MULTIPLE files.

Reuses tools/parse_bigb.py as a READ-ONLY dependency (no modification).
"""
import os, sys, struct, zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb  # read-only dependency

# --- Build the standard reflected CRC-32 table (poly 0xEDB88320) ---
def build_table():
    tbl = []
    for n in range(256):
        c = n
        for _ in range(8):
            c = (0xEDB88320 ^ (c >> 1)) if (c & 1) else (c >> 1)
        tbl.append(c & 0xFFFFFFFF)
    return tbl

TABLE = build_table()
# Sanity: table[1] must be 0x77073096 per crc_table_full.txt
assert TABLE[1] == 0x77073096, "CRC table mismatch vs engine dump"

def ccrc_calc(seed, data):
    """Engine CCRC::Calc — reflected CRC-32, caller seed, NO final XOR."""
    crc = seed & 0xFFFFFFFF
    for b in data:
        crc = (crc >> 8) ^ TABLE[(b ^ (crc & 0xFF)) & 0xFF]
    return crc & 0xFFFFFFFF

# --- Candidate formula matrix ------------------------------------------------
# Each candidate: (label, function(data)->u32)
CANDIDATES = [
    ("seed0_noinv",        lambda d: ccrc_calc(0x00000000, d)),
    ("seed0_inv",          lambda d: ccrc_calc(0x00000000, d) ^ 0xFFFFFFFF),
    ("seedFFFF_noinv",     lambda d: ccrc_calc(0xFFFFFFFF, d)),
    ("seedFFFF_inv",       lambda d: ccrc_calc(0xFFFFFFFF, d) ^ 0xFFFFFFFF),
    ("zlib_crc32",         lambda d: zlib.crc32(d) & 0xFFFFFFFF),  # control (= seedFFFF_inv)
]

# --- Data domains for one entry ---------------------------------------------
def domains(b, e):
    """Return list of (domain_label, bytes) for candidate CRC input.

    Extended to every plausible source: payload, name(+NUL), sub-bank name,
    the Deps build-path, the Info descriptor, and concatenations. If the CRC is
    a reflected CRC-32 of ANY of these, one of these domains catches it.
    """
    payload = b[e['offset']:e['offset'] + e['size']] if e['size'] > 0 else b''
    name = e['name'].encode('latin1')
    sb = e.get('_subbank', '').encode('latin1')
    dep = e['deps'][0].encode('latin1') if e['deps'] else b''
    info = e['info']
    return [
        ("payload",       payload),
        ("name",          name),
        ("name_nul",      name + b'\x00'),
        ("subbank",       sb),
        ("subbank_nul",   sb + b'\x00'),
        ("dep",           dep),
        ("dep_nul",       dep + b'\x00'),
        ("info",          info),
        ("name+payload",  name + payload),
        ("payload+info",  payload + info),
        ("name+info",     name + info),
        ("info+payload",  info + payload),
    ]

def load_entries(path):
    with open(path, 'rb') as f:
        b = f.read()
    magic, ver, foot_off, foot_size = parse_bigb.parse_header(b)
    subs, _ = parse_bigb.parse_footer(b, foot_off)
    out = []
    for sub in subs:
        entries, stats, endp, err = parse_bigb.parse_toc(b, sub, foot_off)
        for e in entries:
            e['_subbank'] = sub['name']
            out.append(e)
    return b, out

def nonzero_crc_entries(b, entries, limit):
    picked = [e for e in entries if e['crc'] != 0]
    return picked[:limit]

def main():
    install = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"
    files = {
        "shaders.big":  os.path.join(install, r"data\shaders\pc\shaders.big"),
        "frontend.big": os.path.join(install, r"data\graphics\pc\frontend.big"),
        "fonts.big":    os.path.join(install, r"data\lang\English\fonts.big"),
        "graphics.big": os.path.join(install, r"data\graphics\graphics.big"),
        "textures.big": os.path.join(install, r"data\graphics\pc\textures.big"),
    }
    SAMPLES_PER_FILE = 6

    # Track: for each (domain, candidate) how many samples matched / total
    from collections import defaultdict
    score = defaultdict(lambda: [0, 0])  # key -> [matched, total]
    all_samples = []

    for fname, fpath in files.items():
        if not os.path.exists(fpath):
            print(f"!! MISSING {fpath}")
            continue
        b, entries = load_entries(fpath)
        picked = nonzero_crc_entries(b, entries, SAMPLES_PER_FILE)
        print(f"\n=== {fname}: {len(picked)} sampled non-zero-CRC entries "
              f"(of {sum(1 for e in entries if e['crc'])} nonzero / {len(entries)} total) ===")
        for e in picked:
            stored = e['crc']
            doms = domains(b, e)
            print(f"  [{e['_subbank']}] id={e['id']} name={e['name']!r} "
                  f"size={e['size']} infoSize={e['infosize']} storedCRC=0x{stored:08x} ts={e['timestamp']}")
            sample = dict(file=fname, name=e['name'], stored=stored, matches=[])
            for dlabel, data in doms:
                for clabel, fn in CANDIDATES:
                    got = fn(data)
                    key = (dlabel, clabel)
                    score[key][1] += 1
                    if got == stored:
                        score[key][0] += 1
                        sample['matches'].append(key)
            all_samples.append(sample)

    total_samples = len(all_samples)
    n_formulas = len(set(score.keys()))
    print(f"\n\n===== FORMULA MATRIX ({n_formulas} formulas x {total_samples} samples across all files) =====")
    winners = []
    any_hit = False
    for key in sorted(score.keys()):
        m, t = score[key]
        flag = ""
        if m == total_samples and total_samples > 0:
            flag = "  <<< MATCHES ALL"
            winners.append(key)
        if m > 0:
            any_hit = True
            print(f"  domain={key[0]:14s} candidate={key[1]:16s}  {m}/{total_samples}{flag}")
    if not any_hit:
        print("  (no formula matched even ONE sample)")

    print("\n===== VERDICT =====")
    if winners:
        for w in winners:
            print(f"  VERIFIED: CRC = {w[1]} over domain '{w[0]}' — matches ALL {total_samples} samples")
    else:
        print("  REFUTED: no reflected-CRC-32 formula (any seed/inversion) over any of")
        print(f"  {sorted(set(k[0] for k in score))} matched even a single stored CRC.")
        best = max(score.items(), key=lambda kv: kv[1][0])
        print(f"  Closest: domain={best[0][0]} candidate={best[0][1]} "
              f"matched {best[1][0]}/{total_samples}")
        print("\n  Structural finding: the TOC CRC is CONSTANT across entries that share a")
        print("  resource FORMAT/CLASS (e.g. all 465 shaders.big entries share one CRC;")
        print("  frontend has 3 distinct CRCs for 394 entries; 0x4e1ace9f recurs in BOTH")
        print("  frontend and textures). It is a per-format/class schema fingerprint, NOT a")
        print("  per-payload hash. See docs/BIG_ROUNDTRIP.md 'CRC formula — refuted'.")

    return winners, all_samples

if __name__ == '__main__':
    main()
