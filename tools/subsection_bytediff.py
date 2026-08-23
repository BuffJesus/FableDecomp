#!/usr/bin/env python3
"""Flatten tools/subsection_oracle.py JSONL into the TSV consumed by
tools/subsection_bytediff.cpp (the classified retail-vs-port byte diff).

Columns: map, recordId, collectionType, objectCount, B floats, retail tableHex
Only records that actually carry a subsection table are emitted.

  python tools/subsection_bytediff.py <oracle.jsonl> <out.tsv>
"""
import json, sys, collections

def main():
    src, dst = sys.argv[1], sys.argv[2]
    n = 0
    per = collections.Counter()
    with open(src, encoding='utf-8') as f, open(dst, 'w', encoding='utf-8') as o:
        for i, line in enumerate(f):
            r = json.loads(line)
            per[(r['map'], bool(r.get('subsectionsPresent')))] += 1
            if not r.get('subsectionsPresent'):
                continue
            flat = []
            for v in r['B']:
                flat.extend(v[:4])
            assert len(flat) == 4 * r['objectCount']
            o.write('%s\t%d\t%d\t%d\t%s\t%s\n' % (
                r['map'], i, r['collectionType'], r['objectCount'],
                ' '.join('%r' % x for x in flat), r['tableHex']))
            n += 1
    for k in sorted(per, key=str):
        sys.stderr.write('%-22s tables=%-5s %d\n' % (k[0], k[1], per[k]))
    sys.stderr.write('%d rows written\n' % n)

main()
