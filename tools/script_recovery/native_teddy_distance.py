"""Connect reviewed cached native entity lookups to their distance checks."""
import hashlib
import json
import struct
from dataclasses import asdict
from collections import Counter
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_teddy_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_teddy_distance_witness.json')


def recover_theresa_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_theresa_distance_witness.json')


def recover_barrel_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_barrel_distance_witness.json')


def recover_post_attack_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_post_attack_distance_witness.json')


def recover_affair_man_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_affair_man_distance_witness.json')


def recover_affair_man_woman_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_affair_man_woman_distance_witness.json')


def recover_affair_wife_distance(function, source, rdata):
    return _recover_distance(function, source, rdata, 'native_affair_wife_distance_witness.json')


def _recover_distance(function, source, rdata, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    data = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if data is None or hashlib.sha256(data).hexdigest() != witness['bytesSha256']:
        return reject('native body changed')
    for item in witness.get('profiledCallees', []):
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native distance callee changed')
    if rdata.string_at(int(witness['scriptNameAddress'], 16)) != witness['scriptName']:
        return reject('native script name changed')
    lookup_setup = read_call_window(rdata, int(witness['address'], 16), witness['size'], int(witness['lookupSite'], 16))
    checks = witness.get('distanceChecks', [witness])
    distance_setups = []
    for check in checks:
        known_calls = {int(site, 16): tuple(profile) for site, profile in check['knownSetupCalls'].items()}
        distance_setups.append(read_call_window(rdata, int(witness['address'], 16), witness['size'], int(check['distanceSite'], 16), known_calls))
    if lookup_setup is None or any(setup is None for setup in distance_setups):
        return reject('native call setup unresolved')
    for check, distance_setup in zip(checks, distance_setups):
        if (len(lookup_setup.stack_arguments) != 2 or lookup_setup.stack_arguments[0][0] != 'stack'
            or distance_setup.edx != lookup_setup.stack_arguments[0]
            or distance_setup.target != ('constant', 0xCBE2FF)
            or len(distance_setup.stack_arguments) != 1
            or distance_setup.stack_arguments[0][0] != 'constant'):
            return reject('native cached-wrapper argument relationship changed')
        if 'heroSite' in check and distance_setup.ecx != ('result', int(check['heroSite'], 16), 'hero'):
            return reject('native distance hero result changed')
        if 'receiverRegister' in check and distance_setup.ecx != ('register', check['receiverRegister']):
            return reject('native distance receiver changed')
        distance_word = distance_setup.stack_arguments[0][1]
        native_distance = struct.unpack('<f', struct.pack('<I', distance_word))[0]
        if native_distance != witness['distance']:
            return reject('native distance threshold changed')
    # DAF106..DAF110 pushes the string at original ESP+0x10 and an out wrapper
    # at original ESP+0x28. DAFAB1 addresses that wrapper as ESP+0x2c after
    # pushing float 0x41200000; ECX is entity+8, EDX is the cached Bully wrapper.
    # Theresa's DB9844 output is original ESP+0x2c; DBA3F3 takes ESP+0x30
    # after pushing 2.0, with the hero in ECX and the outro marker in EDX.
    lookup = witness.get('lookupText', 'GSI->GetThingWithScriptName();')
    distances = Counter(check.get('originalDistance', witness['originalDistance']) for check in checks)
    slot = witness['outputSlot']
    if (source.count(lookup) != witness['lookupCount'] or slot in source
            or any(source.count(distance) != count for distance, count in distances.items())):
        return reject('native lookup/distance shape changed')
    occurrence = witness.get('lookupOccurrence', 0)
    if not isinstance(occurrence, int) or not 0 <= occurrence < witness['lookupCount']:
        return reject('native lookup occurrence invalid')
    parts = source.split(lookup)
    prefix = lookup.join(parts[:occurrence + 1])
    suffix = lookup.join(parts[occurrence + 1:])
    source = prefix + 'GSI->GetThingWithScriptName(&' + slot + ',' + json.dumps(witness['scriptName']) + ');' + suffix
    for distance in distances:
        source = source.replace(distance, 'IsDistanceBetweenThingsUnder(' + witness['distanceReceiver'] + ',' + slot + ',' + witness['distanceOperand'] + ')')
    return source, [dict(witness, status='recovered', outputSlot=slot,
                         lookupSetup=asdict(lookup_setup), distanceSetup=asdict(distance_setups[0]),
                         distanceSetups=[asdict(setup) for setup in distance_setups])]
