"""Recover the native key for BordelloLady's paid cutscene dialogue map."""
import hashlib


def restore(source, fn, row, rdata):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady cutscene map {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    require(rdata.bytes_at(0xE3FB8F, 0x40) == bytes.fromhex(row['nativeSetup']),
            'native map setup changed')
    require(rdata.string_at(0x012E43D4) == '$SEXTALK', 'native map key changed')
    calls = {c['site'].lower(): c['target'].lower() for c in fn['calls']}
    require(calls.get('0x00e3fbc3') == '0x009ac700'
            and calls.get('0x00e3fbca') == '0x0099efb0', 'native map assignment changed')
    old = 'resources:SetString(resources:MemberStringMap("csargs"), pCVar7, pCVar12)'
    require(source.count(old) == 1
            and source.count('pCVar12 = ("TEXT_CS_B13_SEX_" .. pCVar11)') == 1,
            'lifted map assignment changed')
    return source.replace(old,
        'resources:SetString(resources:MemberStringMap("csargs"), "$SEXTALK", pCVar12)', 1)
