"""Build a local, read-only compatibility corpus from an extracted forum mirror.

python tools/audit_fmp_corpus.py EXTRACTED OUTPUT --forge-tools PATH
No packages are installed. Nested FMPs are materialized by SHA256 only.
RAR inspection needs rarfile and a tar executable with RAR support; 7z needs py7zr.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import sqlite3
import struct
import subprocess
import urllib.parse
import zipfile
import zlib


def semantic_snapshot(data):
    """Independently compare entry metadata and payloads, ignoring physical offsets."""
    def u32(pos):
        return struct.unpack_from('<I', data, pos)[0]

    pos = u32(8)
    count = u32(pos)
    pos += 4
    banks = []
    for _ in range(count):
        end = pos
        while end < len(data) and data[end] not in (0, 255):
            end += 1
        name = data[pos:end]
        bank_id, n, entry_pos, _, block = struct.unpack_from('<5I', data, end + 1)
        pos = end + 21
        entries = []
        if n:
            entry_pos += 4 + u32(entry_pos) * 8
        for _ in range(n):
            magic, ident, kind, length, offset, dev_kind, name_len = struct.unpack_from('<7I', data, entry_pos)
            entry_pos += 28
            entry_name = data[entry_pos:entry_pos + name_len]
            entry_pos += name_len
            crc, sources = struct.unpack_from('<2I', data, entry_pos)
            entry_pos += 8
            source_names = []
            for _ in range(sources):
                size = u32(entry_pos)
                entry_pos += 4
                source_names.append(data[entry_pos:entry_pos + size])
                entry_pos += size
            sub_len = u32(entry_pos)
            entry_pos += 4
            subheader = data[entry_pos:entry_pos + sub_len]
            entry_pos += sub_len
            if offset + length > len(data):
                raise ValueError('payload outside file')
            entries.append((magic, ident, kind, dev_kind, entry_name, crc, source_names,
                            subheader, length, hashlib.sha256(data[offset:offset+length]).hexdigest()))
        banks.append((name, bank_id, block, entries))
    return data[:8], u32(12), banks


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('extracted', type=Path)
    ap.add_argument('output', type=Path)
    ap.add_argument('--forge-tools', type=Path, required=True)
    ap.add_argument('--roundtrip', action='store_true', help='Rewrite accepted packages and compare bytes')
    args = ap.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    nested = args.output / 'zip_fmps'
    nested.mkdir(exist_ok=True)
    normalized = args.output / 'normalized_fmps'
    normalized.mkdir(exist_ok=True)
    packages, origins, errors = {}, [], []

    def add(data, origin, actual):
        digest = hashlib.sha256(data).hexdigest()
        if actual is None:
            actual = nested / (digest + '.fmp')
            if not actual.exists():
                actual.write_bytes(data)
        packages.setdefault(digest, {'sha256': digest, 'size': len(data),
                                     'local_path': str(actual), 'origins': []})
        packages[digest]['origins'].append(origin)
        origins.append((origin, digest))

    for p in sorted(args.extracted.rglob('*')):
        if not p.is_file():
            continue
        origin = p.relative_to(args.extracted).as_posix()
        if p.suffix.lower() == '.fmp':
            add(p.read_bytes(), origin, p)
        elif p.suffix.lower() == '.zip':
            try:
                with zipfile.ZipFile(p) as z:
                    for info in z.infolist():
                        if info.filename.lower().endswith('.fmp'):
                            if info.file_size > 256 * 1024 * 1024:
                                raise ValueError('FMP exceeds 256 MiB inspection limit')
                            add(z.read(info), origin + '!' + info.filename, None)
            except Exception as ex:
                errors.append({'archive': origin, 'error': str(ex)})
        elif p.suffix.lower() == '.rar':
            try:
                import rarfile
                with rarfile.RarFile(p) as r:
                    for info in r.infolist():
                        if info.filename.lower().endswith('.fmp'):
                            if info.file_size > 256 * 1024 * 1024:
                                raise ValueError('FMP exceeds 256 MiB inspection limit')
                            proc = subprocess.run(['tar', '-xOf', str(p), info.filename],
                                                  capture_output=True, timeout=60)
                            if proc.returncode or len(proc.stdout) != info.file_size:
                                raise ValueError(proc.stderr.decode(errors='replace'))
                            add(proc.stdout, origin + '!' + info.filename, None)
            except Exception as ex:
                errors.append({'archive': origin, 'error': str(ex)})
        elif p.suffix.lower() == '.7z':
            try:
                import py7zr
                from py7zr.io import BytesIOFactory
                with py7zr.SevenZipFile(p) as z:
                    targets = [i.filename for i in z.list() if i.filename.lower().endswith('.fmp')]
                    if targets:
                        factory = BytesIOFactory(limit=256 * 1024 * 1024)
                        z.extract(targets=targets, factory=factory)
                        for name in targets:
                            data = factory.get(name)
                            data.seek(0)
                            add(data.read(), origin + '!' + name, None)
            except Exception as ex:
                errors.append({'archive': origin, 'error': str(ex)})

    print(f'Catalogued {len(origins)} occurrences; linking local forum pages', flush=True)

    # Preserve local page references, not guessed authorship or dependency claims.
    by_name = collections.defaultdict(set)
    for origin, digest in origins:
        by_name[origin.split('!')[0].rsplit('/', 1)[-1]].add(digest)
    refs = collections.defaultdict(set)
    for p in sorted(args.extracted.rglob('*')):
        if p.suffix.lower() not in ('.html', '.php') or not p.is_file():
            continue
        content = p.read_text(encoding='utf-8', errors='replace')
        for href in re.findall(r'''(?:href|src)\s*=\s*["']([^"']+)["']''', content, re.I):
            base = urllib.parse.urlsplit(href.replace('&amp;', '&')).path.rsplit('/', 1)[-1]
            for digest in by_name.get(base, ()):
                refs[digest].add(p.relative_to(args.extracted).as_posix())

    db = sqlite3.connect(args.output / 'corpus.sqlite3')
    db.executescript('''
        DROP TABLE IF EXISTS packages; DROP TABLE IF EXISTS origins;
        DROP TABLE IF EXISTS page_refs; DROP TABLE IF EXISTS entries;
        CREATE TABLE packages(sha256 TEXT PRIMARY KEY, size INTEGER, local_path TEXT,
                              content_type INTEGER, version INTEGER, error TEXT);
        CREATE TABLE origins(path TEXT, sha256 TEXT);
        CREATE TABLE page_refs(path TEXT, sha256 TEXT);
        CREATE TABLE entries(sha256 TEXT, bank TEXT, name TEXT, definition TEXT, length INTEGER);
        CREATE INDEX entries_identity ON entries(bank,name);
    ''')
    bank_packages, bank_entries, dialects = collections.Counter(), collections.Counter(), collections.Counter()
    failures = []
    for i, (digest, record) in enumerate(sorted(packages.items()), 1):
        try:
            read_path = record['local_path']
            data = Path(read_path).read_bytes()
            record['header_hex'] = data[:16].hex()
            if data[:4] == bytes.fromhex('39300000'):
                decoder = zlib.decompressobj()
                inflated = decoder.decompress(data[4:], 256 * 1024 * 1024 + 1)
                if len(inflated) > 256 * 1024 * 1024 or not decoder.eof or decoder.unused_data:
                    raise ValueError('Incomplete, oversized, or trailing-data zlib wrapper')
                if inflated[:4] not in (b'BIGB', b'B\0\0\0'):
                    raise ValueError('Inflated wrapper does not contain a BIG archive')
                read_path = str(normalized / (digest + '.fmp'))
                Path(read_path).write_bytes(inflated)
                record['normalization'] = 'u32 12345 + zlib -> BIG'
                record['normalized_path'] = read_path
                record['normalized_sha256'] = hashlib.sha256(inflated).hexdigest()
            proc = subprocess.run([str(args.forge_tools), 'fmp', 'list', read_path, '--json'],
                                  capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=30)
            if proc.returncode:
                raise ValueError((proc.stderr or proc.stdout).strip())
            result = json.loads(proc.stdout)
            record['inspection'] = result
            if args.roundtrip:
                rewritten = args.output / 'roundtrip.fmp'
                rewrite = subprocess.run([str(args.forge_tools), 'fmp', '_rewrite', read_path, str(rewritten)],
                                         capture_output=True, timeout=30)
                if rewrite.returncode:
                    record['roundtrip_error'] = rewrite.stderr.decode(errors='replace')
                else:
                    source_bytes, rewritten_bytes = Path(read_path).read_bytes(), rewritten.read_bytes()
                    record['roundtrip_byte_equal'] = source_bytes == rewritten_bytes
                    record['roundtrip_semantic_equal'] = semantic_snapshot(source_bytes) == semantic_snapshot(rewritten_bytes)
                rewritten.unlink(missing_ok=True)
            dialects[result['content_type']] += 1
            for bank in result['banks']:
                bank_packages[bank['bank']] += 1
                bank_entries[bank['bank']] += bank['count']
                db.executemany('INSERT INTO entries VALUES(?,?,?,?,?)',
                               [(digest, bank['bank'], e['name'], e['definition'], e['length'])
                                for e in bank['entries']])
            db.execute('INSERT INTO packages VALUES(?,?,?,?,?,NULL)',
                       (digest, record['size'], record['local_path'], result['content_type'], result['version']))
        except Exception as ex:
            record['error'] = str(ex)
            failures.append({'sha256': digest, 'origins': record['origins'], 'error': str(ex)})
            db.execute('INSERT INTO packages VALUES(?,?,?,NULL,NULL,?)',
                       (digest, record['size'], record['local_path'], str(ex)))
        if i % 100 == 0:
            print(f'Inspected {i}/{len(packages)} unique packages', flush=True)
    db.executemany('INSERT INTO origins VALUES(?,?)', origins)
    db.executemany('INSERT INTO page_refs VALUES(?,?)',
                   [(p, digest) for digest, pages in refs.items() for p in sorted(pages)])
    db.commit()
    collisions = db.execute('''SELECT bank,name,COUNT(DISTINCT sha256) AS n FROM entries
                              WHERE bank NOT LIKE '%Link%' AND bank NOT IN ('Settings','names')
                              GROUP BY bank,name HAVING n>1 ORDER BY n DESC LIMIT 50''').fetchall()
    summary = {'occurrences': len(origins), 'direct_fmps': sum('!' not in o for o, _ in origins),
               'nested_fmps': sum('!' in o for o, _ in origins), 'unique_packages': len(packages),
               'duplicate_occurrences': len(origins)-len(packages),
               'reader_pass': len(packages)-len(failures), 'reader_fail': failures,
               'normalized_wrappers': sum('normalization' in r for r in packages.values()),
               'roundtrip_byte_equal': sum(r.get('roundtrip_byte_equal') is True for r in packages.values()),
               'roundtrip_byte_different': sum(r.get('roundtrip_byte_equal') is False for r in packages.values()),
               'roundtrip_errors': [r['sha256'] for r in packages.values() if 'roundtrip_error' in r],
               'roundtrip_semantic_equal': sum(r.get('roundtrip_semantic_equal') is True for r in packages.values()),
               'roundtrip_semantic_different': [r['sha256'] for r in packages.values() if r.get('roundtrip_semantic_equal') is False],
               'forge_tools_sha256': hashlib.sha256(args.forge_tools.read_bytes()).hexdigest(),
               'content_types': dict(dialects), 'packages_per_bank': dict(bank_packages),
               'entries_per_bank': dict(bank_entries), 'nested_archive_errors': errors,
               'packages_with_page_refs': len(refs), 'page_references': sum(map(len, refs.values())),
               'frequent_shared_record_names': collisions,
               'limits': ['One nested archive layer inspected; archives within archives are not traversed',
                          'Reader acceptance does not establish successful install or playability',
                          'Page links are filename matches, not verified attribution or instructions']}
    (args.output / 'summary.json').write_text(json.dumps(summary, indent=2), encoding='utf-8')
    (args.output / 'packages.json').write_text(json.dumps(list(packages.values()), indent=2), encoding='utf-8')
    db.close()
    print(json.dumps({k: v for k, v in summary.items() if k not in ('reader_fail','unscanned_nested_archives','frequent_shared_record_names')}, indent=2))


if __name__ == '__main__':
    main()
