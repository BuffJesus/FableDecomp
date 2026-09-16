"""Pin the inline cleanup and native destructors used to replace it."""
import hashlib
import json
from pathlib import Path


def verify(data):
    w=json.loads(Path(__file__).with_name('native_barrel_man_inline_cleanup_witness.json').read_text())
    if w['order']!=[48,36,20] or [(p['address'],p['size']) for p in w['profiles']]!=[
            (0xDB694C,183),(0x4AA840,66),(0x7E74D0,66),(0x99A2E0,7),(0x99A430,56)]:
        raise ValueError('Barrel inline cleanup coverage changed')
    for profile in w['profiles']:
        raw=data.bytes_at(profile['address'],profile['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=profile['sha256']:
            raise ValueError('Barrel inline cleanup/destructor changed')
    return w
