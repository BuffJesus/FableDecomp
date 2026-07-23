#!/usr/bin/env python3
"""xbadpcm.py — pure-Python Xbox IMA ADPCM (WAVE_FORMAT_XBOX_ADPCM, fmt tag 0x0069)
encoder + decoder for Fable TLC audio banks (.lug SFX banks, .lut dialogue banks).

ffmpeg DECODES this codec (adpcm_ima_xbox) but has NO encoder for it — this module is
the encode side. Decode semantics were pinned byte-exact against ffmpeg 8.1 on retail
clips (mono 22050/44100 and stereo 22050, village.lug; see docs/LUG_AUDIO_FORMAT.md §8):

  Block = 36*ch bytes:
    ch x { i16 predictor (an EXACT source sample), u8 stepIndex, u8 reserved=0 }
    then 8 groups x ch x u32 data words (4 bytes = 8 nibbles, LOW nibble first)
  Decode emits 64 samples per block per channel: the header predictor sample, then the
  first 63 nibble expansions. The 64th nibble is decoded state-wise but its sample is
  DISCARDED (ffmpeg trims to 64). Blocks are independent (state re-read from header):
  retail blocks do NOT continue predictor/step state across block boundaries.
  Nibble expansion is the "precise" IMA variant: diff = ((2*(n&7)+1)*step)>>3.

Encoding convention used here (matches decode; retail-compatible): per block, header
predictor = exact source sample s0 with the current step index; 63 nibbles encode
s1..s63; the spare 64th nibble encodes the NEXT block's first sample (keeps the ADPCM
state sane; its output sample is discarded by the decoder). Input is zero-padded to a
multiple of 64 samples (retail data sizes are exact multiples of blockAlign).

CLI:
  python xbadpcm.py encode in_pcm.wav out_xadpcm.wav   # PCM16 -> Xbox ADPCM RIFF
  python xbadpcm.py decode in_xadpcm.wav out_pcm.wav   # (validation aid; ffmpeg-equivalent)
"""
import struct
import sys

STEP_TABLE = [
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230,
    253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963,
    1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327,
    3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
    11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
    32767]
INDEX_TABLE = [-1, -1, -1, -1, 2, 4, 6, 8]

SAMPLES_PER_BLOCK = 64
BLOCK_ALIGN_PER_CH = 36


def _clamp16(v):
    return -32768 if v < -32768 else (32767 if v > 32767 else v)


def _expand(nibble, pred, idx):
    """Precise IMA nibble expansion (ffmpeg adpcm_ima_expand_nibble, shift=3)."""
    step = STEP_TABLE[idx]
    diff = ((2 * (nibble & 7) + 1) * step) >> 3
    pred = _clamp16(pred - diff) if nibble & 8 else _clamp16(pred + diff)
    idx = idx + INDEX_TABLE[nibble & 7]
    return pred, (0 if idx < 0 else (88 if idx > 88 else idx))


def _compress(sample, pred, idx):
    """Choose the nibble whose expansion lands closest to `sample`; return
    (nibble, new_pred, new_idx) with new state = decoder state."""
    step = STEP_TABLE[idx]
    delta = sample - pred
    nib = 8 if delta < 0 else 0
    if delta < 0:
        delta = -delta
    # standard IMA quantize: bit-per-term greedy == nearest for this quantizer
    if delta >= step:
        nib |= 4
        delta -= step
    if delta >= step >> 1:
        nib |= 2
        delta -= step >> 1
    if delta >= step >> 2:
        nib |= 1
    pred, idx = _expand(nib, pred, idx)
    return nib, pred, idx


def encode(samples, channels):
    """samples: interleaved list/sequence of int16. Returns Xbox ADPCM data bytes.
    Zero-pads to a whole number of 64-sample blocks."""
    if channels not in (1, 2):
        raise ValueError("channels must be 1 or 2")
    n = len(samples) // channels
    nblocks = (n + SAMPLES_PER_BLOCK - 1) // SAMPLES_PER_BLOCK
    if nblocks == 0:
        nblocks = 1
    # de-interleave + pad
    chans = []
    for c in range(channels):
        s = list(samples[c::channels])
        s += [0] * (nblocks * SAMPLES_PER_BLOCK + 1 - len(s))  # +1 for spare nibble
        chans.append(s)
    idxs = [0] * channels
    out = bytearray()
    for b in range(nblocks):
        base = b * SAMPLES_PER_BLOCK
        # headers: exact first sample + current step index
        preds = []
        for c in range(channels):
            p = _clamp16(chans[c][base])
            out += struct.pack("<hBB", p, idxs[c], 0)
            preds.append(p)
        # nibbles: s1..s63 + spare nibble = next block's s0
        nibs = [[] for _ in range(channels)]
        for c in range(channels):
            p, i = preds[c], idxs[c]
            for k in range(1, SAMPLES_PER_BLOCK + 1):
                nib, p, i = _compress(chans[c][base + k], p, i)
                nibs[c].append(nib)
            idxs[c] = i
        # pack: 8 groups x ch x 4 bytes, low nibble first
        for g in range(8):
            for c in range(channels):
                for by in range(4):
                    lo = nibs[c][g * 8 + by * 2]
                    hi = nibs[c][g * 8 + by * 2 + 1]
                    out.append(lo | (hi << 4))
    return bytes(out)


def decode(data, channels):
    """ffmpeg-equivalent decode (validation aid). Returns interleaved int16 list."""
    ba = BLOCK_ALIGN_PER_CH * channels
    out = []
    for boff in range(0, len(data) - ba + 1, ba):
        preds, idxs = [], []
        for c in range(channels):
            p, i, _r = struct.unpack_from("<hBB", data, boff + 4 * c)
            preds.append(p)
            idxs.append(min(88, i))
        chans = [[preds[c]] for c in range(channels)]
        pos = boff + 4 * channels
        for g in range(8):
            for c in range(channels):
                for by in data[pos:pos + 4]:
                    for nib in (by & 0xF, by >> 4):
                        preds[c], idxs[c] = _expand(nib, preds[c], idxs[c])
                        chans[c].append(preds[c])
                pos += 4
        for s in range(SAMPLES_PER_BLOCK):  # header + 63 nibbles; 64th discarded
            for c in range(channels):
                out.append(chans[c][s])
    return out


# ---------------------------------------------------------------- RIFF helpers

def parse_wav(blob):
    """Parse a RIFF/WAVE blob -> (fmt_dict, data_bytes). Accepts extra chunks."""
    if blob[:4] != b"RIFF" or blob[8:12] != b"WAVE":
        raise ValueError("not a RIFF/WAVE")
    off, fmt, data = 12, None, None
    while off + 8 <= len(blob):
        cid = blob[off:off + 4]
        csz = struct.unpack_from("<I", blob, off + 4)[0]
        body = blob[off + 8:off + 8 + csz]
        if cid == b"fmt ":
            fmt = body
        elif cid == b"data":
            data = body
        off += 8 + csz + (csz & 1)
    if fmt is None or data is None:
        raise ValueError("missing fmt/data chunk")
    tag, ch, rate, abps, ba, bits = struct.unpack_from("<HHIIHH", fmt, 0)
    return {"tag": tag, "channels": ch, "rate": rate, "avg_bps": abps,
            "block_align": ba, "bits": bits}, data


def build_pcm_riff(samples, channels, rate):
    """Minimal PCM16 RIFF (fmt size 16 + data), retail .lug PCM convention."""
    data = struct.pack(f"<{len(samples)}h", *samples)
    fmt = struct.pack("<HHIIHH", 1, channels, rate, rate * 2 * channels,
                      2 * channels, 16)
    return _riff(fmt, data)


def build_xadpcm_riff(adpcm_data, channels, rate):
    """Xbox ADPCM RIFF, byte-layout identical to retail clips (fmt size 20,
    cbSize=2, extra=0x0040 samples/block)."""
    ba = BLOCK_ALIGN_PER_CH * channels
    abps = (rate * ba) // SAMPLES_PER_BLOCK  # floor, matches retail (12403 @ 22050 mono)
    fmt = struct.pack("<HHIIHHHH", 0x0069, channels, rate, abps, ba, 4, 2, 0x0040)
    return _riff(fmt, adpcm_data)


def _riff(fmt_body, data_body):
    inner = (b"WAVE" + b"fmt " + struct.pack("<I", len(fmt_body)) + fmt_body +
             b"data" + struct.pack("<I", len(data_body)) + data_body)
    return b"RIFF" + struct.pack("<I", len(inner)) + inner


def encode_wav(pcm_blob):
    """PCM16 RIFF bytes -> Xbox ADPCM RIFF bytes (retail-layout)."""
    fmt, data = parse_wav(pcm_blob)
    if fmt["tag"] != 1 or fmt["bits"] != 16:
        raise ValueError(f"input must be PCM16 (got tag {fmt['tag']:#x}, {fmt['bits']}-bit)")
    samples = struct.unpack(f"<{len(data) // 2}h", data[:len(data) // 2 * 2])
    return build_xadpcm_riff(encode(samples, fmt["channels"]),
                             fmt["channels"], fmt["rate"])


def decode_wav(xadpcm_blob):
    """Xbox ADPCM RIFF bytes -> PCM16 RIFF bytes."""
    fmt, data = parse_wav(xadpcm_blob)
    if fmt["tag"] != 0x0069:
        raise ValueError(f"not Xbox ADPCM (tag {fmt['tag']:#x})")
    return build_pcm_riff(decode(data, fmt["channels"]),
                          fmt["channels"], fmt["rate"])


def main(argv):
    if len(argv) != 4 or argv[1] not in ("encode", "decode"):
        print(__doc__)
        return 1
    blob = open(argv[2], "rb").read()
    out = encode_wav(blob) if argv[1] == "encode" else decode_wav(blob)
    open(argv[3], "wb").write(out)
    print(f"wrote {argv[3]} ({len(out)} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
