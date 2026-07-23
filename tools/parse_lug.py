#!/usr/bin/env python3
"""parse_lug.py — parser/extractor for Fable TLC `data\\Sound\\*.lug` audio segment banks.

Container: `LiOnHeAd` + `LHFileSegmentBankInfo` (SFX/ambience banks; music is loose .ogg,
dialogue voice lives in the .lut banks — see parse_lut.py / LUT_AUDIOBANK_FORMAT.md).

Layout (all little-endian; evidence in docs/LUG_AUDIO_FORMAT.md):
  0x00  char[8]   "LiOnHeAd"
  0x08  block stream, each block = char[32] NUL-padded struct name + u32 size + payload[size]:
        LHFileSegmentBankInfo   payload = bank title/description string field (size 0x208)
        LHAudioWaveData         payload = back-to-back complete RIFF/WAVE files
        LHAudioBankSampleTable  payload = u16 count, u16 unk, then count * 652-byte records:
            +0    char[260] source path (dev machine, NUL-padded)
            +260  u32 id (1-based)     +264 u32 id2 (== id in retail)
            +268  u32 riffSize         +272 u32 riffOffset (into LHAudioWaveData payload)
            +276  u32 unk0             +280 u32 unk1 (0x10000)
            +284..291 zero
            +292  u16 fmtTag, u16 channels
            +296  u32 sampleRate       +300 u32 avgBytesPerSec
            +304  u16 blockAlign, u16 bitsPerSample
            +308  u32 zero, +312 u32 -1, +316 u32 -1
            +320  char[256] group/category name ("Arena", "Balverine", ...)
            +576  76 bytes playback params (priority/flags/volume-pitch variation,
                  f32 minDist @+616, f32 maxDist @+620, ... see doc).
        LHAudioBankCriteiaInfo  (engine's own typo) payload = u32 count, then count *
            { u32 len; char[len] tagString; u32 n; u32 sampleId[n] } — event/criteria
            tags (e.g. "SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS") -> sample ids.

Codec: embedded RIFFs are WAVE_FORMAT_XBOX_ADPCM (0x0069) — same codec as the .lut
dialogue banks; ffmpeg decodes them as adpcm_ima_xbox.

Usage:
  python parse_lug.py BANK.lug                 # summary (blocks, counts, codec stats)
  python parse_lug.py BANK.lug --list          # per-entry table
  python parse_lug.py BANK.lug --criteria      # dump criteria/event tag map
  python parse_lug.py BANK.lug --extract N out.wav   # extract sample id N (1-based)
  python parse_lug.py BANK.lug --extract-all OUTDIR  # extract every sample
"""
import os
import struct
import sys

SAMPLE_REC_SIZE = 652
PATH_FIELD = 260


class LugError(Exception):
    pass


def read_blocks(data):
    """Yield (name, payload_offset, payload_size) for each 32-byte-named block."""
    if data[:8] != b"LiOnHeAd":
        raise LugError("not a LiOnHeAd file")
    off = 8
    blocks = []
    while off + 36 <= len(data):
        name = data[off:off + 32].split(b"\0")[0].decode("latin1")
        size = struct.unpack_from("<I", data, off + 32)[0]
        payload = off + 36
        if not name or payload + size > len(data):
            raise LugError(f"bad block at {off:#x}: name={name!r} size={size:#x}")
        blocks.append((name, payload, size))
        off = payload + size
    if off != len(data):
        raise LugError(f"trailing slack: walk ended at {off:#x}, file is {len(data):#x}")
    return blocks


class Sample:
    __slots__ = ("id", "id2", "path", "group", "riff_off", "riff_size", "fmt_tag",
                 "channels", "sample_rate", "avg_bps", "block_align", "bits",
                 "min_dist", "max_dist")

    @property
    def name(self):
        return os.path.basename(self.path.replace("\\", "/")) or f"sample_{self.id}"

    @property
    def duration(self):
        return self.riff_size / self.avg_bps if self.avg_bps else 0.0


def parse_sample_table(data, off, size):
    count, unk = struct.unpack_from("<HH", data, off)
    base = off + 4
    if 4 + count * SAMPLE_REC_SIZE != size:
        raise LugError(f"sample table size mismatch: count={count} size={size:#x}")
    samples = []
    for i in range(count):
        e = base + i * SAMPLE_REC_SIZE
        s = Sample()
        s.path = data[e:e + PATH_FIELD].split(b"\0")[0].decode("latin1")
        (s.id, s.id2, s.riff_size, s.riff_off, _unk0, _unk1) = struct.unpack_from(
            "<6I", data, e + PATH_FIELD)
        (s.fmt_tag, s.channels, s.sample_rate, s.avg_bps, s.block_align,
         s.bits) = struct.unpack_from("<HHIIHH", data, e + PATH_FIELD + 32)
        s.group = data[e + 320:e + 576].split(b"\0")[0].decode("latin1")
        s.min_dist, s.max_dist = struct.unpack_from("<2f", data, e + 616)
        samples.append(s)
    return samples, unk


def parse_criteria(data, off, size):
    (count,) = struct.unpack_from("<I", data, off)
    p = off + 4
    out = []
    for _ in range(count):
        (slen,) = struct.unpack_from("<I", data, p)
        tag = data[p + 4:p + 4 + slen].decode("latin1")
        p += 4 + slen
        (n,) = struct.unpack_from("<I", data, p)
        ids = struct.unpack_from(f"<{n}I", data, p + 4)
        p += 4 + 4 * n
        out.append((tag, list(ids)))
    if p != off + size:
        raise LugError(f"criteria walk ended at {p:#x}, expected {off+size:#x}")
    return out


class LugBank:
    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        self.blocks = read_blocks(self.data)
        bmap = {n: (o, s) for n, o, s in self.blocks}
        self.title = ""
        if "LHFileSegmentBankInfo" in bmap:
            o, s = bmap["LHFileSegmentBankInfo"]
            self.title = self.data[o:o + s].split(b"\0")[0].decode("latin1")
        self.wave_off, self.wave_size = bmap["LHAudioWaveData"]
        to, ts = bmap["LHAudioBankSampleTable"]
        self.samples, self.table_unk = parse_sample_table(self.data, to, ts)
        self.criteria = []
        if "LHAudioBankCriteiaInfo" in bmap:
            co, cs = bmap["LHAudioBankCriteiaInfo"]
            self.criteria = parse_criteria(self.data, co, cs)

    def riff_bytes(self, sample):
        if not sample.riff_size:
            raise LugError(f"sample {sample.id} is an empty placeholder slot")
        a = self.wave_off + sample.riff_off
        blob = self.data[a:a + sample.riff_size]
        if blob[:4] != b"RIFF":
            raise LugError(f"sample {sample.id}: no RIFF at wave+{sample.riff_off:#x}")
        rsz = struct.unpack_from("<I", blob, 4)[0]
        if rsz + 8 != sample.riff_size:
            raise LugError(f"sample {sample.id}: RIFF size {rsz+8:#x} != table {sample.riff_size:#x}")
        return blob

    def validate(self):
        """Walk the wave payload independently and cross-check against the table."""
        offs = []
        off = self.wave_off
        end = self.wave_off + self.wave_size
        while off < end:
            if self.data[off:off + 4] != b"RIFF":
                raise LugError(f"wave walk: no RIFF at {off:#x}")
            rsz = struct.unpack_from("<I", self.data, off + 4)[0]
            offs.append((off - self.wave_off, rsz + 8))
            off += 8 + rsz
        if off != end:
            raise LugError("wave walk overran block")
        # Multiple table entries may alias the same RIFF (dedup) -> compare as sets.
        # Entries with riff_size == 0 are empty placeholder slots -> skip.
        table = set((s.riff_off, s.riff_size) for s in self.samples if s.riff_size)
        walk = set(offs)
        if table != walk:
            raise LugError(
                f"table/walk disagree: {sorted(table - walk)[:3]} vs {sorted(walk - table)[:3]}")
        return len(offs)


def fmt_name(tag):
    return {0x0001: "PCM", 0x0069: "XBOX_ADPCM", 0x0011: "IMA_ADPCM"}.get(tag, f"{tag:#06x}")


def main(argv):
    if len(argv) < 2 or argv[1] in ("-h", "--help"):
        print(__doc__)
        return 1
    bank = LugBank(argv[1])
    args = argv[2:]

    if not args:
        n = bank.validate()
        print(f"file: {argv[1]}  ({len(bank.data):,} bytes)")
        print(f"title: {bank.title!r}")
        for name, o, s in bank.blocks:
            print(f"  block {name:<28} payload @ {o:#x}  size {s:#x}")
        uniq = len(set((s.riff_off, s.riff_size) for s in bank.samples))
        print(f"samples: {len(bank.samples)}  (wave walk: {n} RIFFs, "
              f"{len(bank.samples) - uniq} aliased entries, cross-check OK)")
        fmts = {}
        for s in bank.samples:
            key = (fmt_name(s.fmt_tag), s.channels, s.sample_rate, s.bits)
            fmts[key] = fmts.get(key, 0) + 1
        for (f, ch, sr, bits), c in sorted(fmts.items()):
            print(f"  codec {f} {ch}ch {sr}Hz {bits}-bit: {c} clips")
        dur = sum(s.duration for s in bank.samples)
        print(f"total audio: {dur:.1f}s   criteria tags: {len(bank.criteria)}")
    elif args[0] == "--list":
        for s in bank.samples:
            print(f"{s.id:5d}  {s.riff_off:#10x}  {s.riff_size:8d}  "
                  f"{fmt_name(s.fmt_tag)} {s.channels}ch {s.sample_rate}Hz  "
                  f"{s.duration:6.2f}s  [{s.group}]  {s.path}")
    elif args[0] == "--criteria":
        for tag, ids in bank.criteria:
            print(f"{tag}  -> {ids}")
    elif args[0] == "--extract":
        want = int(args[1])
        out = args[2] if len(args) > 2 else None
        for s in bank.samples:
            if s.id == want:
                dest = out or s.name
                open(dest, "wb").write(bank.riff_bytes(s))
                print(f"wrote {dest} ({s.riff_size} bytes, {fmt_name(s.fmt_tag)}, "
                      f"{s.sample_rate}Hz, {s.duration:.2f}s) from {s.path}")
                return 0
        print(f"no sample with id {want}", file=sys.stderr)
        return 1
    elif args[0] == "--extract-all":
        outdir = args[1]
        os.makedirs(outdir, exist_ok=True)
        wrote = 0
        for s in bank.samples:
            if not s.riff_size:
                continue
            dest = os.path.join(outdir, f"{s.id:05d}_{s.name}")
            open(dest, "wb").write(bank.riff_bytes(s))
            wrote += 1
        print(f"wrote {wrote} files to {outdir} "
              f"({len(bank.samples) - wrote} empty slots skipped)")
    else:
        print(f"unknown option {args[0]}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
