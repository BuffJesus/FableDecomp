"""Recover the two BordelloLady question calls from native argument setup."""
import hashlib


CALLS = (
    'quest:GiveHeroYesNoQuestion(uVar10, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_PAID_QUESTION")',
    'quest:GiveHeroYesNoQuestion(uVar10, pppuVar20, "TEXT_OBJECT_HERO_ANSWER_NO", "PARTY_FREE_QUESTION")',
)
RECOVERED = 'quest:GiveHeroYesNoQuestion(uVar10, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)'


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady questions {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end - start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    calls = {c['site'].lower(): c for c in fn['indirectCalls']}
    require(row['questionSites'] == ['0x00e3f1d3', '0x00e3f9cb'], 'question sites changed')
    for site in row['questionSites']:
        require(calls.get(site, {}).get('slot') == '0x1c8', f'question call changed at {site}')
    require(set(row['nativeArgumentWindows']) == {'0x00e3f159', '0x00e3f957'},
            'argument windows changed')
    for address, encoded in row['nativeArgumentWindows'].items():
        expected = bytes.fromhex(encoded)
        require(rdata.bytes_at(int(address, 16), len(expected)) == expected,
                f'argument setup changed at {address}')
    # Empty CCharString constructors use a NUL address; string_at deliberately
    # returns None there. Check the byte itself instead.
    require(rdata.bytes_at(0x0122D70E, 1) == b'\0', 'empty button string changed')
    for address, expected in {
        0x012C216C: 'TEXT_OBJECT_HERO_ANSWER_NO',
        0x012C2188: 'TEXT_OBJECT_HERO_ANSWER_YES',
        0x012E42E8: 'PARTY_PAID_QUESTION',
        0x012E43F8: 'PARTY_FREE_QUESTION',
    }.items():
        require(rdata.string_at(address) == expected, f'question string changed at {address:08x}')
    for old in CALLS:
        require(source.count(old) == 1, 'lifted question call changed')
    for old in CALLS:
        source = source.replace(old, RECOVERED, 1)
    return source
