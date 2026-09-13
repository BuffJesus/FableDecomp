"""Map both native position branches without guessing the missing-actor fallback."""
import hashlib
import json
from pathlib import Path


def map_affair_wife_position(function, rdata):
    w=json.loads(Path(__file__).with_name('native_affair_wife_position_witness.json').read_text())
    if str(function.get('address','')).lower()!=w['address'].lower():
        return []
    if hashlib.sha256(function.get('decompile','').encode()).hexdigest()!=w['sourceSha256']:
        return [{'status':'rejected','reason':'position source changed'}]
    for region in w['regions']:
        raw=rdata.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            return [{'status':'rejected','reason':'position native evidence changed'}]
    return [dict(w,status='mapped')]
