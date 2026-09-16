"""Reviewed native vector layout overlays, separate from port serialization types."""
import hashlib
import re


def recover_barrel_spawn_health(function, source, rdata, manifest):
    if str(function.get('address', '')).lower() != '0x00dbe890':
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(function.get('decompile', '').encode()).hexdigest() != 'e2df608e6c0cf8097d480cd0ddc92e516ffeeeda5ad530d5294d5a0d329d2523':
        return reject('WatchBarrels source changed')
    raw = rdata.bytes_at(0xDBEA4B, 0x6D)
    if raw is None or hashlib.sha256(raw).hexdigest() != '42a68d9f21e969cc97175cbbec2200c99b6c20017c271079373ede6939afff6a':
        return reject('beetle spawn/health native operands changed')
    spec = manifest.get('EntitySetMaxHealth', {})
    if (spec.get('scope') != 'Quest' or spec.get('returnType') != 'void'
            or [p.get('type') for p in spec.get('parameters', [])] !=
            ['const std::shared_ptr<CScriptThing>&', 'float', 'bool']):
        return reject('EntitySetMaxHealth contract changed')
    pattern = r'GSI->EntitySetMaxHealth\s*\(&puStack_28,0x40000000,1\)'
    if len(re.findall(pattern, source)) != 1:
        return reject('beetle health source correspondence changed')
    # Native DBEAA2..DBEAAE: set-current=true, float bits 0x40000000,
    # and the spawned Thing at baseline stack+34. Only this verified call
    # is normalized; integer literals elsewhere are not guessed as floats.
    return re.sub(pattern, 'GSI->EntitySetMaxHealth(&puStack_28,2.0,1)', source), [
        {'status': 'recovered', 'callSite': '0x00DBEAAE', 'maxHealth': 2.0,
         'setCurrentToMax': True, 'nativeSha256': hashlib.sha256(raw).hexdigest()}]


def new_oakvale_vectors(rdata):
    # DB7DC6 calls bound Thing GetPos; DB7DCC..DB7DDC copies its three
    # 32-bit components to parent+76/+7A/+7E. The inventory also identifies
    # this as BarrelBrokenPos. The persistence manifest's 'string' describes
    # a port representation and must not be mistaken for the native type.
    raw = rdata.bytes_at(0xDB7DB0, 49)
    if raw is None or hashlib.sha256(raw).hexdigest() != 'e062e2b8e8e68b63ba358c61f3df7755e32d9e408332b5cf145cc421cb0d0df8':
        raise ValueError('native barrel position copy changed')
    vectors = {'0x76': 'BarrelBrokenPos'}
    components = {hex(0x76 + 4 * i): ('BarrelBrokenPos_' + axis, 'Float') for i, axis in enumerate('xyz')}
    home = rdata.bytes_at(0xDB5260, 0xA7)
    if home is None or hashlib.sha256(home).hexdigest() != '700d34947b365ed19535d6db434e5bb016919eb233688bdde9d17f0b10c855e0':
        raise ValueError('native warehouse home position copy changed')
    vectors['0x85'] = 'WarehouseMeetPoint'
    components.update({hex(0x85 + 4 * i): ('WarehouseMeetPoint_' + axis, 'Float') for i, axis in enumerate('xyz')})
    evidence = {'address': '0x00DB7DB0', 'size': 49, 'nativeSha256': hashlib.sha256(raw).hexdigest(),
                'nativeType': 'C3DVector', 'components': components,
                'representation': 'three Float state keys; native offsets retained',
                'persistence': 'transient; not added to OnPersist'}
    evidence['warehouseMeetPoint'] = {'address': '0x00DB5260', 'size': 0xA7,
        'nativeSha256': hashlib.sha256(home).hexdigest(),
        'copy': 'GetHomePos return vector copied to parent+85/+89/+8D at DB52D0..DB52E6',
        'persistence': 'transient; reset by BarrelMan Init'}
    return vectors, components, evidence
