#!/usr/bin/env python3
"""Aggregate tools/subsection_bytediff.cpp's per-record TSV per map / class."""
import sys, collections, csv

CLS = {0: 'COUNT_WRONG (split algorithm)',
       1: 'COUNT_OK, INT LANES WRONG (order/permutation/tree)',
       2: 'INT LANES OK, FLOAT LANES WRONG (sphere centre/radius)',
       3: 'FULL MATCH 0x00-0x4B'}

rows = list(csv.DictReader(open(sys.argv[1]), delimiter='\t'))
for r in rows:
    for k in ('collectionType','tMaskR1','tMaskAny','objectCount','retailElems','portElems','class','bestT',
              'countLane','startLane','childLane','floatBytesBad','floatBytesTot',
              'tailNonZero','tailTot'):
        r[k] = int(r[k])

maps = sorted({r['map'] for r in rows})
print('=== per-map classification (best over T=1..4 and the R sweep) ===')
for mp in maps + ['ALL']:
    sub = rows if mp == 'ALL' else [r for r in rows if r['map'] == mp]
    c = collections.Counter(r['class'] for r in sub)
    print('\n%s  (%d retail subsection tables)' % (mp, len(sub)))
    for k in (3,2,1,0):
        print('   %-52s %5d  %6.2f%%' % (CLS[k], c[k], 100.0*c[k]/len(sub)))
    fb = sum(r['floatBytesBad'] for r in sub); ft = sum(r['floatBytesTot'] for r in sub)
    print('   float-lane bytes wrong: %d/%d (%.2f%%)' % (fb, ft, 100.0*fb/ft))
    tn = sum(r['tailNonZero'] for r in sub); tt = sum(r['tailTot'] for r in sub)
    print('   EXCLUDED tail 0x4C..0x4F: %d/%d retail bytes non-zero (%.2f%%)'
          % (tn, tt, 100.0*tn/tt))

print('\n=== integer-lane match (count+startIndex+childOffset) by objectCount ===')
buckets = [(1,4),(5,8),(9,16),(17,24),(25,32),(33,10**9)]
print('  %-10s %7s %7s %7s' % ('objects','tables','int-ok','rate'))
for lo,hi in buckets:
    sub = [r for r in rows if lo <= r['objectCount'] <= hi]
    if not sub: continue
    ok = sum(1 for r in sub if r['class'] >= 2)
    print('  %-10s %7d %7d %6.1f%%' % ('%d-%d'%(lo,min(hi,999)), len(sub), ok, 100.0*ok/len(sub)))

print('\n=== which integer lane fails, among COUNT_OK-but-int-wrong records ===')
bad = [r for r in rows if r['class'] == 1]
print('  records: %d' % len(bad))
if bad:
    print('  count[] ok   : %d' % sum(r['countLane'] for r in bad))
    print('  startIndex ok: %d' % sum(r['startLane'] for r in bad))
    print('  childOffset ok: %d' % sum(r['childLane'] for r in bad))

print('\n=== leaf threshold T needed, by collectionType (T is a per-MESH constant) ===')
h = collections.defaultdict(collections.Counter)
for r in rows:
    if r['class'] >= 2: h[r['collectionType']][r['bestT']] += 1
for ct in sorted(h):
    tot = sum(h[ct].values())
    print('  collectionType %-4d n=%-5d %s' % (ct, tot,
          ' '.join('T=%d:%d' % (t, h[ct][t]) for t in sorted(h[ct]))))

print()
print('=== T-consistency: is the leaf threshold a free per-record knob? ===')
import collections as _c
print('  tMaskR1 histogram (bit0=T1,bit1=T2,bit2=T3,bit3=T4), R=1:')
for m, v in sorted(_c.Counter(r['tMaskR1'] for r in rows).items()):
    print('    mask=%-3d T set=%-16s %5d records' % (m, str([t for t in (1,2,3,4) if m & (1<<(t-1))]), v))
for T in (1,2,3,4):
    ok = sum(1 for r in rows if r['tMaskR1'] & (1 << (T-1)))
    oka = sum(1 for r in rows if r['tMaskAny'] & (1 << (T-1)))
    print('  ONE global T=%d for every record: int lanes %d/%d (%.1f%%) at R=1; %d/%d (%.1f%%) allowing per-record R'
          % (T, ok, len(rows), 100.0*ok/len(rows), oka, len(rows), 100.0*oka/len(rows)))
byct = _c.defaultdict(lambda: 0xF); cnt = _c.Counter()
for r in rows:
    byct[r['collectionType']] &= r['tMaskR1']; cnt[r['collectionType']] += 1
print('  per-collectionType intersection (one T must serve the whole type):')
for ct in sorted(byct):
    m = byct[ct]
    print('    collectionType %-4d n=%-5d common T = %s'
          % (ct, cnt[ct], str([t for t in (1,2,3,4) if m & (1<<(t-1))]) if m else 'NONE'))

print('\n=== COUNT_WRONG detail (retail vs port element count, T=1..4,R sweep all failed) ===')
cw = [r for r in rows if r['class'] == 0]
if cw:
    c = collections.Counter((r['objectCount'], r['retailElems'], r['portElems']) for r in cw)
    for k, v in c.most_common(30):
        print('   objects=%-4d retailElems=%-4d portElems=%-4d  x%d' % (k[0],k[1],k[2],v))
else:
    print('   none')
