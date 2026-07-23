#!/usr/bin/env python3
"""dialogue_pipeline.py - end-to-end CUSTOM DIALOGUE staging for Fable: TLC.

One workflow: WAV + line text + speaker  ->  staged modified copies of every
artifact a spoken line needs:

    text.big     the line text (Content/SpeechBank/Speaker/Identifier)
    <bank>.lut   the voice audio (Xbox ADPCM clip at index <N>)
    dialogue.big the LIPSYNC mouth-weight curve (entry <stem>_<N>)
    <bank>snds.bin  (ADD only) the name->index join table in data\\Defs

THE CROSS-FILE JOIN (settled 2026-07-20, empirical - see docs/DIALOGUE_PIPELINE.md):
  text.big stores only the bank NAME (SpeechBank="ScriptDialogue.lug"); the index
  <N> lives in data\\Defs\\<bank>snds.bin:
      u32 count ; count x { u32 crc, u32 soundID }   sorted ascending by crc
      crc = CRC-32 (poly 0xEDB88320, seed 0, no final xor) of "SND_" + entryName
  soundID <N> == .lut clip header Index == dialogue.big LIPSYNC entry id
  (entry name Dialogue_<N> / ScriptDialogue_<N> / ScriptDialogue2_<N>).

Modes:
  REPLACE (donor line)  - v1, lowest risk: swap a retail line's text + audio +
      lipsync in place. snds.bin untouched (name->N mapping already exists).
  ADD (new line)        - stages a brand-new text entry + snds.bin pair + .lut
      clip at index max+1 + LIPSYNC entry. Structurally clean; engine acceptance
      [unproven] until a live test (see doc).

ALL WRITES GO TO STAGED COPIES under --out; retail files are opened read-only.

Usage:
  # inspect the full join for a line
  python tools/dialogue_pipeline.py resolve TEXT_QST_088_EAT_PIES_INTRO_40

  # replace a donor line end-to-end
  python tools/dialogue_pipeline.py stage --wav my.wav --text "New line." \
      --speaker "DEMON DOOR" --replace TEXT_QST_088_EAT_PIES_INTRO_40 \
      --out work/dialogue_pipeline/my_line

  # add a brand-new line
  python tools/dialogue_pipeline.py stage --wav my.wav --text "New line." \
      --speaker "DEMON DOOR" --add TEXT_MY_NEW_LINE --bank ScriptDialogue2.lug \
      --out work/dialogue_pipeline/my_new_line

  # synthesize a test WAV (two-tone sweep 'voice')
  python tools/dialogue_pipeline.py makewav out.wav --seconds 2.5
"""
import argparse, math, os, shutil, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb
import parse_lut
import lut_write
import xbadpcm
import big_write
import text_build
import lipsync_build

DEFAULT_INSTALL = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"

# SpeechBank name -> (lut file, snds.bin file, lipsync sub-bank, lipsync entry stem)
BANKS = {
    'Dialogue.lug':        ('Dialogue.lut',        'dialoguesnds.bin',
                            'LIPSYNC_ENGLISH_MAIN',     'Dialogue'),
    'Dialogue2.lug':       ('Dialogue2.lut',       'dialoguesnds2.bin',
                            'LIPSYNC_ENGLISH_MAIN_2',   'Dialogue2'),
    'ScriptDialogue.lug':  ('ScriptDialogue.lut',  'scriptdialoguesnds.bin',
                            'LIPSYNC_ENGLISH_SCRIPT',   'ScriptDialogue'),
    'ScriptDialogue2.lug': ('ScriptDialogue2.lut', 'scriptdialoguesnds2.bin',
                            'LIPSYNC_ENGLISH_SCRIPT_2', 'ScriptDialogue2'),
}
AVG_BPS = (22050 * 36) // 64          # 12403, retail-exact (LUG_AUDIO_FORMAT.md 8)


def crc0(s):
    """Fable seed-0 reflected CRC-32 (poly 0xEDB88320, no final xor) - the same
    hash the def field tags use (appearance_survey.crc0)."""
    crc = 0
    for ch in s.encode():
        crc ^= ch
        for _ in range(8):
            crc = (crc >> 1) ^ (0xEDB88320 if crc & 1 else 0)
    return crc


# ------------------------------------------------------------------ snds.bin

def load_snds(path):
    b = open(path, 'rb').read()
    n = struct.unpack_from('<I', b)[0]
    pairs = [struct.unpack_from('<II', b, 4 + 8 * i) for i in range(n)]
    if len(b) != 4 + 8 * n:
        raise ValueError('%s: size %d != 4+8*%d' % (path, len(b), n))
    return pairs                     # [(crc, soundID)] sorted ascending by crc


def save_snds(path, pairs):
    pairs = sorted(pairs)            # keep the binary-search sort invariant
    out = struct.pack('<I', len(pairs))
    out += b''.join(struct.pack('<II', c, i) for c, i in pairs)
    with open(path, 'wb') as f:
        f.write(out)


def snds_lookup(pairs, entry_name):
    key = crc0('SND_' + entry_name)
    d = dict(pairs)
    return d.get(key), key


# ------------------------------------------------------------------ audio

def prep_pcm(wav_blob):
    """Any PCM16 RIFF -> mono 22050 Hz PCM16 RIFF (downmix + linear resample)."""
    fmt, data = xbadpcm.parse_wav(wav_blob)
    if fmt['tag'] != 1 or fmt['bits'] != 16:
        raise SystemExit('input WAV must be PCM16 (got tag %#x, %d-bit); '
                         'convert first: ffmpeg -i in.wav -c:a pcm_s16le out.wav'
                         % (fmt['tag'], fmt['bits']))
    n = len(data) // 2
    samples = struct.unpack('<%dh' % n, data[:n * 2])
    ch = fmt['channels']
    if ch > 1:                                        # downmix
        frames = n // ch
        samples = [sum(samples[i * ch + c] for c in range(ch)) // ch
                   for i in range(frames)]
    rate = fmt['rate']
    if rate != 22050:                                 # linear resample
        ratio = rate / 22050.0
        out_n = int(len(samples) / ratio)
        rs = []
        for i in range(out_n):
            x = i * ratio
            j = int(x)
            fr = x - j
            a = samples[j]
            b2 = samples[min(j + 1, len(samples) - 1)]
            rs.append(int(a + (b2 - a) * fr))
        samples = rs
    return xbadpcm.build_pcm_riff(list(samples), 1, 22050)


def encode_clip(wav_path):
    """WAV file -> (xbox-adpcm RIFF bytes, duration seconds)."""
    pcm = prep_pcm(open(wav_path, 'rb').read())
    riff = xbadpcm.encode_wav(pcm)
    fmt, data = xbadpcm.parse_wav(riff)
    dur = len(data) / float(AVG_BPS)
    return riff, dur


# ------------------------------------------------------------------ dialogue.big helpers

def record_info_offset(b, rec_off):
    """Return absolute offset of the info blob inside a BIGB TOC record."""
    r = parse_bigb.Reader(b)
    r.p = rec_off
    for _ in range(6):
        r.u32()                       # magic,id,type,size,offset,crc
    r.lpstr()                         # name
    r.u32()                           # timestamp
    for _ in range(r.u32()):          # deps
        r.lpstr()
    infosize = r.u32()
    return r.p, infosize


def patch_lipsync_info(buf, sub_name, entry_name, info_blob):
    """In-place patch of a LIPSYNC entry's TOC info blob (same size only) -
    covers the big_write limitation that TOC records are copied verbatim."""
    buf = bytearray(buf)
    _m, _v, foot_off, _fs = parse_bigb.parse_header(buf)
    subs, _ = parse_bigb.parse_footer(bytes(buf), foot_off)
    sub = next(s for s in subs if s['name'] == sub_name)
    entries, _st, _e, err = parse_bigb.parse_toc(bytes(buf), sub, foot_off)
    if err:
        raise ValueError(err)
    e = next(x for x in entries if x['name'] == entry_name)
    off, size = record_info_offset(bytes(buf), e['rec_off'])
    if size != len(info_blob):
        raise ValueError('info size mismatch %d != %d' % (size, len(info_blob)))
    buf[off:off + size] = info_blob
    return bytes(buf)


def load_lipsync(dialogue_path):
    b = open(dialogue_path, 'rb').read()
    _m, _v, foot_off, _fs = parse_bigb.parse_header(b)
    subs, _ = parse_bigb.parse_footer(b, foot_off)
    out = {}
    for s in subs:
        entries, _st, _e, err = parse_bigb.parse_toc(b, s, foot_off)
        if err:
            raise ValueError('%s: %s' % (s['name'], err))
        out[s['name']] = {e['id']: e for e in entries}
    return b, out


# ------------------------------------------------------------------ resolve

def resolve(install, entry_name, quiet=False):
    """Full join for one text entry. Returns dict or None."""
    tb = text_build.TextBank(os.path.join(install, r'data\lang\English\text.big'))
    if entry_name not in tb.by_name:
        raise SystemExit('text entry %r not found in text.big' % entry_name)
    d = tb.decode(entry_name)
    sb = d['speechbank']
    if sb not in BANKS:
        raise SystemExit('entry %r has SpeechBank %r - not a voice bank' % (entry_name, sb))
    lut_name, snds_name, lip_sub, stem = BANKS[sb]
    pairs = load_snds(os.path.join(install, r'data\Defs', snds_name))
    n, key = snds_lookup(pairs, entry_name)
    if n is None:
        raise SystemExit('no snds.bin pair for %r (crc0(SND_%s)=%08x) - line has no audio'
                         % (entry_name, entry_name, key))
    _b, lips = load_lipsync(os.path.join(install, r'data\lang\English\dialogue.big'))
    le = lips[lip_sub].get(n)
    lut_path = os.path.join(install, r'data\lang\English', lut_name)
    clips, _stop, _toc = parse_lut.walk(open(lut_path, 'rb').read())
    clip = next((c for c in clips if c['index'] == n), None)
    res = dict(entry=entry_name, speechbank=sb, speaker=d['speaker'],
               text=d['content'], n=n, crc=key, lut=lut_name,
               lip_sub=lip_sub, lip_name='%s_%d' % (stem, n),
               lip_dur=struct.unpack('<f', le['info'])[0] if le and le['infosize'] == 4 else None,
               clip_span=clip['riff_span'] if clip else None)
    if not quiet:
        print('entry      : %s' % entry_name)
        print('text       : %r' % d['content'])
        print('speaker    : %r   speechbank: %r' % (d['speaker'], sb))
        print('join       : crc0("SND_%s") = %08X -> %s soundID <N> = %d'
              % (entry_name, key, snds_name, n))
        print('audio      : %s clip Index=%d  (riff span %s B)' % (lut_name, n, res['clip_span']))
        print('lipsync    : dialogue.big %s entry %r  dur=%.3fs'
              % (lip_sub, res['lip_name'], res['lip_dur'] or -1))
    return res


# ------------------------------------------------------------------ staging

def stage(args):
    install = args.install
    out_dir = args.out
    os.makedirs(out_dir, exist_ok=True)
    riff, dur = encode_clip(args.wav)
    print('[audio] %s -> Xbox ADPCM %d B, %.3f s' % (args.wav, len(riff), dur))

    text_src = os.path.join(install, r'data\lang\English\text.big')
    dlg_src = os.path.join(install, r'data\lang\English\dialogue.big')
    tb = text_build.TextBank(text_src)
    if args.speaker not in tb.narrators:
        raise SystemExit('speaker %r not in the 379-name NarratorList; pick one of '
                         'the existing names (text_build.py narrators)' % args.speaker)

    if args.replace:
        entry = args.replace
        res = resolve(install, entry, quiet=True)
        sb = res['speechbank']
        n = res['n']
        mode = 'REPLACE'
    else:
        entry = args.add
        sb = args.bank
        if sb not in BANKS:
            raise SystemExit('--bank must be one of %s' % ', '.join(BANKS))
        if entry in tb.by_name:
            raise SystemExit('entry %r already exists; use --replace' % entry)
        mode = 'ADD'
    lut_name, snds_name, lip_sub, stem = BANKS[sb]
    lut_src = os.path.join(install, r'data\lang\English', lut_name)
    snds_src = os.path.join(install, r'data\Defs', snds_name)

    # ---------- 1. text.big
    if mode == 'REPLACE':
        tb.set_text(entry, text=args.text, speaker=args.speaker)
    else:
        tb.add_text(entry, args.text, speaker=args.speaker, speechbank=sb)
    text_out = os.path.join(out_dir, 'text.big')
    tb.save(text_out)
    print('[text.big] %s %r speaker=%r -> %s' % (mode, entry, args.speaker, text_out))

    # ---------- 2. snds.bin (join table)
    snds_out = os.path.join(out_dir, snds_name)
    pairs = load_snds(snds_src)
    if mode == 'ADD':
        n = max(i for _c, i in pairs) + 1
        key = crc0('SND_' + entry)
        if any(c == key for c, _i in pairs):
            raise SystemExit('crc collision for %r in %s' % (entry, snds_name))
        pairs.append((key, n))
        save_snds(snds_out, pairs)
        print('[%s] ADD pair {crc0(SND_%s)=%08X -> %d} -> %s'
              % (snds_name, entry, key, n, snds_out))
    else:
        shutil.copyfile(snds_src, snds_out)
        print('[%s] unchanged (join %s -> %d already retail); copied for deploy set'
              % (snds_name, entry, n))

    # ---------- 3. .lut audio bank
    lut_buf = open(lut_src, 'rb').read()
    clips, _stop, _toc = parse_lut.walk(lut_buf)
    if mode == 'REPLACE':
        hdr, _c = lut_write.get_record_header(lut_buf, clips, n)
        new_lut = lut_write.rebuild(lut_buf, replace=(n, lut_write.make_record(hdr, riff)))
    else:
        max_idx = max(c['index'] for c in clips)
        if n != max_idx + 1:
            # keep snds/lut/lipsync agreeing: new N must be max of all three + 1
            n = max(n, max_idx + 1)
        hdr, _c = lut_write.get_record_header(lut_buf, clips, clips[-1]['index'])
        hdr = bytearray(hdr)
        struct.pack_into('<I', hdr, 0, n)             # Index field
        new_lut = lut_write.rebuild(lut_buf, add=(n, lut_write.make_record(bytes(hdr), riff)))
    lut_out = os.path.join(out_dir, lut_name)
    with open(lut_out, 'wb') as f:
        f.write(new_lut)
    print('[%s] %s clip Index=%d (%d B RIFF) -> %s' % (lut_name, mode, n, len(riff), lut_out))

    # ---------- 4. dialogue.big lipsync
    dlg_buf, lips = load_lipsync(dlg_src)
    ls = lipsync_build.synth_curve(dur)
    lip_name = '%s_%d' % (stem, n)
    if mode == 'REPLACE':
        donor = lips[lip_sub][n]
        if donor['size'] > 0:
            dls = lipsync_build.LipSync.decode(
                dlg_buf[donor['offset']:donor['offset'] + donor['size']])
            ls.visemes = dls.visemes                   # keep the donor viseme dictionary
        new_dlg = big_write.rebuild(dlg_buf, edits={(lip_sub, lip_name): ls.encode()})
        new_dlg = patch_lipsync_info(new_dlg, lip_sub, lip_name, ls.info_blob())
    else:
        donor = max(lips[lip_sub].values(), key=lambda e: e['size'])
        dls = lipsync_build.LipSync.decode(
            dlg_buf[donor['offset']:donor['offset'] + donor['size']])
        ls.visemes = dls.visemes
        new_dlg = big_write.rebuild(dlg_buf, adds=[dict(
            sub=lip_sub, name=lip_name, payload=ls.encode(), type=donor['type'],
            id=n, deps=donor['deps'], info=ls.info_blob())])
    dlg_out = os.path.join(out_dir, 'dialogue.big')
    with open(dlg_out, 'wb') as f:
        f.write(new_dlg)
    print('[dialogue.big] %s %s (%d frames @43fps, dur %.3fs) -> %s'
          % (mode, lip_name, len(ls.frames), ls.duration, dlg_out))

    # ---------- 5. validation
    validate(out_dir, install, entry, sb, n, args.text, args.speaker, riff, mode)

    # ---------- 6. deploy checklist
    print('\n================ DEPLOY CHECKLIST (manual, backup first!) ================')
    print('Staged copies in: %s' % os.path.abspath(out_dir))
    print('  1. BACK UP the retail originals:')
    print(r'       data\lang\English\text.big')
    print(r'       data\lang\English\%s' % lut_name)
    print(r'       data\lang\English\dialogue.big')
    if mode == 'ADD':
        print(r'       data\Defs\%s' % snds_name)
    print('  2. Copy the staged files over the retail ones (game NOT running):')
    print('       text.big, %s, dialogue.big%s' % (lut_name, (', ' + snds_name) if mode == 'ADD' else ''))
    if mode == 'ADD':
        print(r'       (%s goes to data\Defs, the rest to data\lang\English)' % snds_name)
    print('  3. Launch the game and trigger the line:')
    if mode == 'REPLACE':
        print('       %s is a retail line - trigger its retail context.' % entry)
    else:
        print('       NEW line %s: a quest script / FSE Lua call must reference it' % entry)
        print('       (no retail trigger exists). Engine acceptance of grown banks is')
        print('       [unproven] - test REPLACE first if this is your first deploy.')
    print('  4. Roll back by restoring the backups.')
    print('==========================================================================')


def validate(out_dir, install, entry, sb, n, text, speaker, riff, mode):
    print('\n[validate]')
    lut_name, snds_name, lip_sub, stem = BANKS[sb]
    ok = True

    tb2 = text_build.TextBank(os.path.join(out_dir, 'text.big'))
    d = tb2.decode(entry)
    a = d['content'] == text and d['speaker'] == speaker and d['speechbank'] == sb
    ok &= a
    print('  text.big   : entry decodes back (text/speaker/speechbank) .... %s' % ('OK' if a else 'FAIL'))

    pairs = load_snds(os.path.join(out_dir, snds_name))
    got, _k = snds_lookup(pairs, entry)
    a = got == n and [c for c, _i in pairs] == sorted(c for c, _i in pairs)
    ok &= a
    print('  %s: SND_%s -> %s (want %d), crc-sorted .... %s'
          % (snds_name, entry, got, n, 'OK' if a else 'FAIL'))

    new_lut = open(os.path.join(out_dir, lut_name), 'rb').read()
    clips, stop, toc = parse_lut.walk(new_lut)
    clip = next((c for c in clips if c['index'] == n), None)
    a = clip is not None and new_lut[clip['riff_off']:clip['riff_off'] + clip['riff_span']] == riff
    ok &= a
    print('  %s: re-walk clean (%d clips), clip %d byte-exact to encoded RIFF .... %s'
          % (lut_name, len(clips), n, 'OK' if a else 'FAIL'))
    # stable fixpoint: rebuilding the staged bank reproduces it byte-exact
    a = lut_write.rebuild(new_lut) == new_lut
    ok &= a
    print('  %s: IDENTITY fixpoint on staged bank .... %s' % (lut_name, 'OK' if a else 'FAIL'))
    # untouched-clip audit vs retail
    orig_lut = open(os.path.join(install, r'data\lang\English', lut_name), 'rb').read()
    changed, identical, sok, notes = lut_write.diff_clips(orig_lut, new_lut)
    if mode == 'ADD':
        # every retail clip byte-identical; exactly one extra index (n) in the new bank
        oc = {c['index'] for c in parse_lut.walk(orig_lut)[0]}
        nc = {c['index'] for c in clips}
        a = changed == [] and len(identical) == len(oc) and nc - oc == {n}
    else:
        a = sok and changed == [n]
    ok &= a
    print('  %s: clip diff vs retail changed=%s identical=%d .... %s'
          % (lut_name, changed, len(identical), 'OK' if a else 'FAIL'))

    dlg_out = os.path.join(out_dir, 'dialogue.big')
    b2, lips2 = load_lipsync(dlg_out)
    le = lips2[lip_sub][n]
    ls2 = lipsync_build.LipSync.decode(b2[le['offset']:le['offset'] + le['size']])
    fmt, data = xbadpcm.parse_wav(riff)
    dur = len(data) / float(AVG_BPS)
    info_dur = struct.unpack('<f', le['info'])[0]
    a = abs(ls2.duration - dur) < 0.05 and abs(info_dur - ls2.duration) < 0.001
    ok &= a
    print('  dialogue.big: %s_%d decodes, curve dur %.3f ~ audio %.3f, Info dur %.3f .... %s'
          % (stem, n, ls2.duration, dur, info_dur, 'OK' if a else 'FAIL'))
    # untouched-entry audit
    b1, lips1 = load_lipsync(os.path.join(install, r'data\lang\English\dialogue.big'))
    bad = 0
    for subn, es in lips1.items():
        for eid, e in es.items():
            if subn == lip_sub and eid == n:
                continue
            e2 = lips2[subn].get(eid)
            if e2 is None or b1[e['offset']:e['offset'] + e['size']] != b2[e2['offset']:e2['offset'] + e2['size']]:
                bad += 1
    a = bad == 0
    ok &= a
    print('  dialogue.big: all other LIPSYNC payloads byte-identical (%d bad) .... %s'
          % (bad, 'OK' if a else 'FAIL'))

    print('  VERDICT: %s' % ('ALL CHECKS PASSED' if ok else '*** FAILURES ABOVE ***'))
    if not ok:
        raise SystemExit(1)


# ------------------------------------------------------------------ makewav

def makewav(path, seconds):
    """Synthetic 'voice-like' test clip: warbling two-tone with syllable envelope."""
    rate = 22050
    n = int(rate * seconds)
    samples = []
    for i in range(n):
        t = i / float(rate)
        syll = 0.5 * (1 - math.cos(2 * math.pi * 3.1 * t))          # ~3 syllables/s
        f = 160 + 60 * math.sin(2 * math.pi * 0.9 * t)              # pitch wander
        v = (math.sin(2 * math.pi * f * t) * 0.6 +
             math.sin(2 * math.pi * f * 2.7 * t) * 0.3)
        samples.append(int(20000 * syll * v * 0.9))
    with open(path, 'wb') as fh:
        fh.write(xbadpcm.build_pcm_riff(samples, 1, rate))
    print('wrote %s (%.2f s PCM16 mono 22050 Hz)' % (path, seconds))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sub = ap.add_subparsers(dest='cmd', required=True)

    p = sub.add_parser('resolve', help='print the full text->lut->lipsync join for a line')
    p.add_argument('entry')
    p.add_argument('--install', default=DEFAULT_INSTALL)

    p = sub.add_parser('stage', help='stage all artifacts for a custom line')
    p.add_argument('--wav', required=True, help='input WAV (PCM16; any rate/channels)')
    p.add_argument('--text', required=True, help='the line text')
    p.add_argument('--speaker', required=True, help='NarratorList speaker name')
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument('--replace', metavar='TEXT_ENTRY', help='donor line to replace')
    g.add_argument('--add', metavar='TEXT_ENTRY', help='brand-new entry name')
    p.add_argument('--bank', help='SpeechBank for --add (e.g. ScriptDialogue2.lug)')
    p.add_argument('--out', required=True, help='staging directory (created)')
    p.add_argument('--install', default=DEFAULT_INSTALL)

    p = sub.add_parser('makewav', help='synthesize a test WAV')
    p.add_argument('path')
    p.add_argument('--seconds', type=float, default=2.0)

    args = ap.parse_args()
    if args.cmd == 'resolve':
        resolve(args.install, args.entry)
    elif args.cmd == 'makewav':
        makewav(args.path, args.seconds)
    else:
        if args.add and not args.bank:
            raise SystemExit('--add requires --bank')
        stage(args)


if __name__ == '__main__':
    main()
