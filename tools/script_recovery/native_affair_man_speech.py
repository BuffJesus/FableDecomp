"""Expose native speech scheduling requirements before resource-aware lowering."""
import hashlib
import json
from pathlib import Path


def map_affair_man_speech(function,rdata,manifest):
    w=json.loads(Path(__file__).with_name('native_affair_man_speech_mapping_witness.json').read_text())
    if str(function.get('address','')).lower()!=w['address'].lower(): return []
    if hashlib.sha256(function.get('decompile','').encode()).hexdigest()!=w['sourceSha256']:
        return [{'status':'rejected','reason':'speech source changed'}]
    for region in w['regions']:
        raw=rdata.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            return [{'status':'rejected','reason':'speech native evidence changed'}]
    if manifest.get('Speak')!=w['contract']:
        return [{'status':'rejected','reason':'speech host contract changed'}]
    return [dict(w,status='mapped',issues=[
        f"native speech 0x{call['address']:08x}: me:Speak adds a blocking wait; explicit resource speech/lifetime required"
        for call in w['calls']])]
