"""Verify both immediate hero-name query scopes in the retail wife's Main."""
import hashlib
import json
from pathlib import Path


def verify(data):
    witness=json.loads(Path(__file__).with_name('native_affair_wife_talk_witness.json').read_text())
    if (witness['literal'],witness['slot'],witness['receiver'])!=(0x125D1C8,0x6C,'edi'):
        raise ValueError('Wife talk receiver or identity changed')
    if data.string_at(witness['literal'])!='SCRIPT_NAME_HERO':
        raise ValueError('Wife talk literal changed')
    if [(b['address'],b['size'],b['query']) for b in witness['blocks']]!=[
            (0xDB2E9C,39,0xDB2EB5),(0xDB3970,39,0xDB3989)]:
        raise ValueError('Wife talk scope coverage changed')
    for block in witness['blocks']:
        raw=data.bytes_at(block['address'],block['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=block['sha256']:
            raise ValueError('Wife talk native instructions changed')
    return witness
