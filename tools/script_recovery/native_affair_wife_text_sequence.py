"""Recover the numbered argument sequence for the pinned installed text bank."""
import hashlib
import json
from pathlib import Path

TEXT_BANK = Path(r'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\text.big')


def recover_affair_wife_text_sequence(function, source, rdata, text_bank=TEXT_BANK):
    w = json.loads(Path(__file__).with_name('native_affair_wife_text_sequence_witness.json').read_text())
    if str(function.get('address', '')).lower() != w['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != w[key]:
            return reject(key + ' changed')
    try:
        bank_hash = hashlib.sha256(Path(text_bank).read_bytes()).hexdigest()
    except OSError:
        return reject('reviewed text bank unavailable')
    if bank_hash != w['textBankSha256']:
        return reject('reviewed text bank changed')
    for region in w['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native text sequence changed')
    if any(source.count(e['old']) != 1 for e in w['edits']):
        return reject('text sequence source correspondence changed')
    for edit in w['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(w, status='recovered', textBankScope='pinned installed English bank')]
