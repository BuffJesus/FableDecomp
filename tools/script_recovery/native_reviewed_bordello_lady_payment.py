"""Recover the negated per-entity price passed to GiveHeroGold."""
import hashlib


def restore(source, fn, row, rdata, fields):
    if row is None:
        return source

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed BordelloLady payment {fn["address"]}: {why}')

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    require(fields.get('0x20') == ['GoldRequired', 'Int'], 'entity price field changed')
    require(rdata.bytes_at(0xE3FB6B, 17) == bytes.fromhex('8b45208b4d048b11f7d850ff92f8010000'),
            'native price operand changed')
    call = next((c for c in fn['indirectCalls'] if c['site'].lower() == '0x00e3fb76'), None)
    require(call is not None and call['slot'] == '0x1f8', 'payment call changed')
    old = 'quest:GiveHeroGold(fVar18)'
    require(source.count(old) == 1 and source.count('quest:GiveHeroGold(') == 1
            and source.count('__native_entity_state:GetStateInt("GoldRequired") <= iVar9') == 1,
            'lifted payment operand changed')
    return source.replace(old, 'quest:GiveHeroGold(-__native_entity_state:GetStateInt("GoldRequired"))', 1)
