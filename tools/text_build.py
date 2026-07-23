#!/usr/bin/env python3
r"""text.big WRITER/editor for Fable: TLC localized-text BIGB banks.

Pairs with parse_bigb.py (container read) + big_write.py (container rebuild).
Payload codec is a byte-exact port of EgoCore CTextParser::Parse/Recompile
(EgoCore-master\EgoCore\Text\TextParser.h); validated round-trip on all
28,912 payload-bearing retail text.big entries (see docs/TEXT_BIG_WRITE.md).

Text-bank facts this tool relies on (all verified against retail text.big):
  * Type-0 payload = UTF-16LE content \0\0 + 3 length-prefixed strings
    (SpeechBank, Speaker, Identifier) + u32 tagCount + tags{i32 pos, cstr}.
  * Type-0 TOC Info blob = u32 LE = index of Speaker in the NarratorList
    (26,807/26,807 retail entries match). Identifier == entry Name on all.
  * Type-1 (group) payload = u32 count + count*u32 member IDs; Info empty.
  * Type-2 (NarratorList) TOC entry has Size=0/no payload blob; the narrator
    names live in its TOC Info blob: u32 count + count NUL-terminated names.
    Its TOC Offset field == the sub-bank TOC offset (retail compiler wrote
    "current file position" for the empty payload); we keep that invariant.
  * CRC / Timestamp / deps are all 0 in retail text.big.

Usage:
  python text_build.py list <text.big> [--filter SUBSTR] [--limit N]
  python text_build.py get <text.big> <NAME>
  python text_build.py narrators <text.big>
  python text_build.py set <text.big> <NAME> -o OUT [--text S] [--speaker S]
                            [--speechbank S]
  python text_build.py add <text.big> <NAME> --text S -o OUT [--speaker S]
                            [--speechbank S] [--id N]
  python text_build.py roundtrip <text.big>       # no-edit rebuild oracle
  python text_build.py selftest <text.big> <workdir>   # full (a)/(b)/(c) suite
NEVER point -o at the retail file; work on a copy (selftest copies for you).
"""
import argparse
import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import parse_bigb
import big_write

# ---------------------------------------------------------------- payload codec

def parse_text_payload(pl):
    """Type-0 payload -> dict. Byte-exact port of CTextParser::Parse type-0."""
    i = 0
    while pl[i:i + 2] != b'\x00\x00':
        i += 2
    content = pl[:i].decode('utf-16-le')
    i += 2
    strs = []
    for _ in range(3):
        n = struct.unpack_from('<I', pl, i)[0]
        i += 4
        strs.append(pl[i:i + n].decode('latin1'))
        i += n
    tag_count = struct.unpack_from('<I', pl, i)[0]
    i += 4
    tags = []
    for _ in range(tag_count):
        pos = struct.unpack_from('<i', pl, i)[0]
        i += 4
        e = pl.index(b'\x00', i)
        tags.append((pos, pl[i:e].decode('latin1')))
        i = e + 1
    if i != len(pl):
        raise ValueError('type-0 payload has %d slack bytes' % (len(pl) - i))
    return dict(content=content, speechbank=strs[0], speaker=strs[1],
                identifier=strs[2], tags=tags)

def build_text_payload(content, speechbank='', speaker='', identifier='',
                       tags=()):
    """dict fields -> type-0 payload bytes (CTextParser::Recompile)."""
    out = content.encode('utf-16-le') + b'\x00\x00'
    for s in (speechbank, speaker, identifier):
        e = s.encode('latin1')
        out += struct.pack('<I', len(e)) + e
    out += struct.pack('<I', len(tags))
    for pos, name in tags:
        out += struct.pack('<i', pos) + name.encode('latin1') + b'\x00'
    return out

def parse_group_payload(pl):
    n = struct.unpack_from('<I', pl, 0)[0]
    if 4 + 4 * n != len(pl):
        raise ValueError('group payload size mismatch')
    return list(struct.unpack_from('<%dI' % n, pl, 4))

def build_group_payload(ids):
    return struct.pack('<I', len(ids)) + struct.pack('<%dI' % len(ids), *ids)

def parse_narrators(info):
    """Type-2 TOC Info blob -> list of narrator names (index = Info u32 of
    type-0 entries)."""
    n = struct.unpack_from('<I', info, 0)[0]
    names = info[4:].split(b'\x00')
    if names and names[-1] == b'':
        names.pop()
    if len(names) != n:
        raise ValueError('narrator count %d != %d' % (len(names), n))
    return [s.decode('latin1') for s in names]

# ---------------------------------------------------------------- bank wrapper

class TextBank:
    def __init__(self, path):
        self.path = path
        with open(path, 'rb') as f:
            self.buf = f.read()
        magic, self.ver, self.foot_off, _ = parse_bigb.parse_header(self.buf)
        if magic != b'BIGB':
            raise ValueError('not a BIGB container: %s' % path)
        self.subs, _ = parse_bigb.parse_footer(self.buf, self.foot_off)
        text_subs = [s for s in self.subs if s['name'].startswith('TEXT_')]
        if not text_subs:
            raise ValueError('no TEXT_* sub-bank in %s' % path)
        self.sub = text_subs[0]
        self.entries, self.stats, _, err = parse_bigb.parse_toc(
            self.buf, self.sub, self.foot_off)
        if err:
            raise ValueError('TOC parse error: %s' % err)
        self.by_name = {e['name']: e for e in self.entries}
        self.by_id = {e['id']: e for e in self.entries}
        nar = [e for e in self.entries if e['type'] == 2]
        self.narrator_entry = nar[0] if nar else None
        self.narrators = (parse_narrators(nar[0]['info']) if nar else [])
        self._edits = {}          # (sub, name) -> payload bytes
        self._adds = []           # big_write add dicts
        self._info_patch = {}     # name -> new 4-byte info (same length only)

    def payload(self, e):
        return self.buf[e['offset']:e['offset'] + e['size']]

    def decode(self, name):
        e = self.by_name[name]
        if e['type'] == 0:
            d = parse_text_payload(self.payload(e))
            d['speaker_index'] = struct.unpack('<I', e['info'])[0]
        elif e['type'] == 1:
            d = dict(members=parse_group_payload(self.payload(e)))
        else:
            d = dict(narrators=self.narrators)
        d.update(id=e['id'], type=e['type'], name=e['name'])
        return d

    def speaker_index(self, speaker):
        try:
            return self.narrators.index(speaker)
        except ValueError:
            raise SystemExit('speaker %r is not in the NarratorList (%d names;'
                             ' see `narrators` cmd). Adding new narrators is'
                             ' not supported yet.' % (speaker, len(self.narrators)))

    def set_text(self, name, text=None, speaker=None, speechbank=None,
                 tags=None):
        """Stage an edit of an existing type-0 entry (None = keep field)."""
        e = self.by_name[name]
        if e['type'] != 0:
            raise SystemExit('%s is type %d, not a string entry' % (name, e['type']))
        d = parse_text_payload(self.payload(e))
        if text is not None:
            d['content'] = text
        if speechbank is not None:
            d['speechbank'] = speechbank
        if tags is not None:
            d['tags'] = tags
        if speaker is not None:
            d['speaker'] = speaker
            self._info_patch[name] = struct.pack('<I', self.speaker_index(speaker))
        self._edits[(self.sub['name'], name)] = build_text_payload(
            d['content'], d['speechbank'], d['speaker'], d['identifier'],
            d['tags'])

    def add_text(self, name, text, speaker='', speechbank='', tags=(),
                 eid=None):
        """Stage a NEW type-0 entry. Identifier is set = name (retail
        invariant); Info = 4-byte narrator index of speaker."""
        if name in self.by_name:
            raise SystemExit('entry %r already exists' % name)
        if eid is not None and eid in self.by_id:
            raise SystemExit('id %d already in use' % eid)
        info = struct.pack('<I', self.speaker_index(speaker))
        self._adds.append(dict(
            sub=self.sub['name'], name=name, type=0, id=eid, info=info,
            payload=build_text_payload(text, speechbank, speaker, name, tags)))

    def add_group(self, name, member_ids, eid=None):
        if name in self.by_name:
            raise SystemExit('entry %r already exists' % name)
        for m in member_ids:
            if m not in self.by_id or self.by_id[m]['type'] != 0:
                raise SystemExit('group member id %d is not a type-0 entry' % m)
        self._adds.append(dict(
            sub=self.sub['name'], name=name, type=1, id=eid, info=b'',
            payload=build_group_payload(member_ids)))

    # ------------------------------------------------------------- rebuild

    def rebuild(self):
        out = bytearray(big_write.rebuild(self.buf, edits=self._edits,
                                          adds=self._adds, toc_align=1))
        self._fixups(out)
        return bytes(out)

    def _fixups(self, out):
        """Post-pass on the rebuilt container:
        1. same-length TOC Info patches (speaker-index changes);
        2. retail invariant: the zero-size NarratorList entry's TOC Offset
           field == the sub-bank TOC offset (the retail compiler wrote the
           end-of-payload file position there). big_write keeps the ORIGINAL
           value verbatim, so re-point it when the payload region moved."""
        magic, ver, foot_off, _ = parse_bigb.parse_header(out)
        subs, _ = parse_bigb.parse_footer(out, foot_off)
        sub = next(s for s in subs if s['name'] == self.sub['name'])
        entries, _stats, toc_end, err = parse_bigb.parse_toc(out, sub, foot_off)
        if err:
            raise AssertionError('rebuilt TOC does not parse: %s' % err)
        rec_end = {e['rec_off']: (entries[i + 1]['rec_off']
                                  if i + 1 < len(entries) else toc_end)
                   for i, e in enumerate(entries)}
        for e in entries:
            if e['name'] in self._info_patch:
                blob = self._info_patch[e['name']]
                assert len(blob) == e['infosize'], 'info patch must keep length'
                pos = rec_end[e['rec_off']] - e['infosize']
                out[pos:pos + e['infosize']] = blob
            if (e['type'] == 2 and e['size'] == 0
                    and e['offset'] == self.sub['offset']
                    and e['offset'] != sub['offset']):
                struct.pack_into('<I', out, e['rec_off'] + 16, sub['offset'])

    def save(self, out_path):
        data = self.rebuild()
        with open(out_path, 'wb') as f:
            f.write(data)
        return data

# --------------------------------------------------------------------- CLI

def _fmt(d):
    for k in ('name', 'id', 'type'):
        print('%-13s %s' % (k + ':', d[k]))
    if d['type'] == 0:
        print('%-13s %r' % ('text:', d['content']))
        for k in ('speechbank', 'speaker', 'speaker_index', 'identifier'):
            print('%-13s %r' % (k + ':', d[k]))
        for pos, t in d['tags']:
            print('  tag @%d: %s' % (pos, t))
    elif d['type'] == 1:
        print('%-13s %s' % ('members:', d['members']))
    else:
        print('%-13s %d narrators' % ('narrators:', len(d['narrators'])))

def selftest(big_path, workdir):
    """Validation suite from docs/TEXT_BIG_WRITE.md, run against a COPY."""
    import shutil
    os.makedirs(workdir, exist_ok=True)
    copy = os.path.join(workdir, 'text_selftest_copy.big')
    shutil.copyfile(big_path, copy)
    src = TextBank(copy)

    # decoder sanity: every payload round-trips byte-exact through the codec
    for e in src.entries:
        pl = src.payload(e)
        if e['type'] == 0:
            d = parse_text_payload(pl)
            assert build_text_payload(d['content'], d['speechbank'],
                                      d['speaker'], d['identifier'],
                                      d['tags']) == pl
        elif e['type'] == 1:
            assert build_group_payload(parse_group_payload(pl)) == pl
    print('codec: %d payloads re-encode byte-exact' % (len(src.entries) - 1))

    # (a) no-edit rebuild is byte-identical
    assert src.rebuild() == src.buf
    print('(a) no-edit rebuild: byte-identical OK')

    def others_identical(nb, skip):
        for e in src.entries:
            ne = nb.by_name[e['name']]
            same_toc = (e['id'], e['type'], e['crc'], e['timestamp'],
                        e['deps'], e['info']) == \
                       (ne['id'], ne['type'], ne['crc'], ne['timestamp'],
                        ne['deps'], ne['info'])
            assert same_toc and (e['name'] in skip
                                 or src.payload(e) == nb.payload(ne)), e['name']
        assert nb.narrator_entry['offset'] == nb.sub['offset']

    # (b) modify one string, decode back, everything else byte-identical
    target = next(e['name'] for e in src.entries if e['type'] == 0)
    b = TextBank(copy)
    b.set_text(target, text='FORGE SELFTEST Æ– edited')
    out = os.path.join(workdir, 'text_selftest_edit.big')
    b.save(out)
    nb = TextBank(out)
    assert nb.decode(target)['content'] == 'FORGE SELFTEST Æ– edited'
    others_identical(nb, {target})
    print('(b) edit %s: decoded back OK, all other entries byte-identical' % target)

    # (c) add a new entry, decode back
    b = TextBank(copy)
    b.add_text('TEXT_FORGE_SELFTEST_NEW', 'New entry — hello Albion.',
               speaker='NONE')
    out = os.path.join(workdir, 'text_selftest_add.big')
    b.save(out)
    nb = TextBank(out)
    d = nb.decode('TEXT_FORGE_SELFTEST_NEW')
    assert d['content'] == 'New entry — hello Albion.'
    assert nb.narrators[d['speaker_index']] == 'NONE'
    assert d['identifier'] == 'TEXT_FORGE_SELFTEST_NEW'
    assert nb.sub['entry_count'] == src.sub['entry_count'] + 1
    assert dict(nb.stats)[0] == dict(src.stats)[0] + 1
    others_identical(nb, set())
    print('(c) add TEXT_FORGE_SELFTEST_NEW id=%d: decoded back OK, '
          'stats/entry_count bumped, retail entries byte-identical' % d['id'])
    print('TEXT SELFTEST: ALL ASSERTIONS PASSED')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sp = ap.add_subparsers(dest='cmd', required=True)
    for c in ('list', 'get', 'narrators', 'set', 'add', 'roundtrip', 'selftest'):
        p = sp.add_parser(c)
        p.add_argument('big')
        if c == 'selftest':
            p.add_argument('workdir')
        if c in ('get', 'set', 'add'):
            p.add_argument('name')
        if c == 'list':
            p.add_argument('--filter', default='')
            p.add_argument('--limit', type=int, default=50)
        if c in ('set', 'add'):
            p.add_argument('--text', default=None, required=(c == 'add'))
            p.add_argument('--speaker', default=None)
            p.add_argument('--speechbank', default=None)
            p.add_argument('-o', '--out', required=True)
        if c == 'add':
            p.add_argument('--id', type=int, default=None)
    a = ap.parse_args(argv)

    if a.cmd == 'selftest':
        return selftest(a.big, a.workdir)
    bank = TextBank(a.big)
    if a.cmd == 'list':
        n = 0
        for e in bank.entries:
            if a.filter.lower() in e['name'].lower():
                extra = ''
                if e['type'] == 0:
                    extra = repr(parse_text_payload(bank.payload(e))['content'][:60])
                print('%6d t%d %-55s %s' % (e['id'], e['type'], e['name'], extra))
                n += 1
                if n >= a.limit:
                    print('... (limit %d reached)' % a.limit)
                    break
    elif a.cmd == 'get':
        _fmt(bank.decode(a.name))
    elif a.cmd == 'narrators':
        for i, s in enumerate(bank.narrators):
            print('%4d %s' % (i, s))
    elif a.cmd == 'set':
        if os.path.abspath(a.out) == os.path.abspath(a.big):
            raise SystemExit('refusing to overwrite the input bank')
        bank.set_text(a.name, text=a.text, speaker=a.speaker,
                      speechbank=a.speechbank)
        bank.save(a.out)
        print('edited %r -> %s' % (a.name, a.out))
    elif a.cmd == 'add':
        if os.path.abspath(a.out) == os.path.abspath(a.big):
            raise SystemExit('refusing to overwrite the input bank')
        bank.add_text(a.name, a.text, speaker=a.speaker or '',
                      speechbank=a.speechbank or '', eid=a.id)
        bank.save(a.out)
        nb = TextBank(a.out)
        print('added %r id=%d -> %s' % (a.name, nb.by_name[a.name]['id'], a.out))
    elif a.cmd == 'roundtrip':
        data = bank.rebuild()
        same = (data == bank.buf)
        print('no-edit rebuild: %d -> %d bytes, byte-identical=%s'
              % (len(bank.buf), len(data), same))
        if not same:
            raise SystemExit(1)

if __name__ == '__main__':
    main()
