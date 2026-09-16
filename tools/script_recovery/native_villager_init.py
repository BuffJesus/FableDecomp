"""Pin the complete Villager Init, including bound actor identity and state reset."""
import hashlib


def verify(data):
    address,size=0xDADF00,82
    digest='a306c925fe51364685ddd94123ffc6e17c24499b64409bdd681fe800e412bf15'
    raw=data.bytes_at(address,size)
    if raw is None or hashlib.sha256(raw).hexdigest()!=digest:raise ValueError('Villager Init bytes changed')
    return dict(address=address,size=size,sha256=digest,status='verified-native-init',
                calls=['damageable(false)','killable(false,false)','combo(false)','fresh hero','ally(bound actor,hero)'],
                state='HeroDidHitMe byte1c reset before first API call',
                limits='Game API internals and host scheduling are separate integration gates')
