"""Verify the first inlined position query's lookup ownership."""
import hashlib
import json
import re
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_first_barrel_actors(source, evidence):
    """Use the first successful self-resource binding; keep wrapper cleanup visible."""
    if not evidence or evidence[0].get('status') != 'mapped':
        return source
    mapping = evidence[0]
    if hashlib.sha256(source.encode()).hexdigest() != mapping['actorAnnotatedSha256']:
        mapping['actorLoweringStatus'] = 'rejected: annotated source changed'
        return source
    pattern = (r'pCVar7\s*=\s*\(CScriptThing\s*\*\)\s*'
               r'CScriptGameResourceObjectScriptedThingBase::\s*'
               r'_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ'
               r'\s*\(\(CScriptGameResourceObjectScriptedThingBase\s*\*\)&ppuStack_1f4\);')
    if len(re.findall(pattern, source)) != 4:
        mapping['actorLoweringStatus'] = 'rejected: actor query shape changed'
        return source
    mapping['actorLoweringStatus'] = 'recovered first two queries; native wrapper cleanup unresolved'
    return re.sub(pattern, 'pCVar7 = me;', source, count=2)


def map_barrel_position(function, rdata):
    witness = json.loads(Path(__file__).with_name('native_barrel_position_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return []
    def reject(reason):
        return [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(function.get('decompile', '').encode()).hexdigest() != witness['sourceSha256']:
        return reject('position source changed')
    for item in [witness] + witness['profiledCallees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native position ownership evidence changed')
    if rdata.string_at(int(witness['scriptNameAddress'], 16)) != witness['scriptName']:
        return reject('position lookup name changed')
    if rdata.bytes_at(0x1260F0C + 0x120, 4) != (0x8A7D60).to_bytes(4, 'little'):
        return reject('position lookup binding changed')
    if rdata.bytes_at(0x1260F0C + 0x20, 4) != (0x89B5B0).to_bytes(4, 'little'):
        return reject('position actor acquisition binding changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    vtable = ('memory', ('address', receiver, 0))
    acquisitions = []
    for site in witness['actorAcquisition']['sites']:
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                 int(site, 16), argument_count=3)
        if (setup is None or setup.ecx != receiver
                or setup.target != ('memory', ('address', vtable, 0x20))
                or setup.stack_arguments != (('register', 'edi'), ('stack', 20), ('constant', 4))):
            return reject('position actor acquisition operands changed')
        acquisitions.append(asdict(setup))
    lookup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                              int(witness['lookupSite'], 16), argument_count=2)
    if (lookup is None or lookup.ecx != receiver
            or lookup.target != ('memory', ('address', vtable, 0x120))
            or lookup.stack_arguments != (('stack', witness['wrapperOffset']), ('stack', 160))
            or witness['implementationOffset'] != witness['wrapperOffset'] + 4):
        return reject('position hidden-output relationship changed')
    consumers = witness['vectorConsumers']
    actor_site = int(consumers['repeatActorSite'], 16)
    actor = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                            actor_site, argument_count=1)
    if (actor is None or actor.target != ('constant', 0x7E7490)
            or actor.ecx != ('stack', consumers['actorResourceOffset'])
            or actor.stack_arguments != (('stack', 232),)):
        return reject('position consumer actor resource changed')
    distance = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                               int(consumers['repeatDistanceSite'], 16),
                               {actor_site: (4, 'controlled_actor')}, argument_count=1)
    if (distance is None or distance.target != ('constant', 0xCBE45C)
            or distance.ecx != ('result', actor_site, 'controlled_actor')
            or distance.edx != ('stack', consumers['frameVectorOffset'])
            or distance.stack_arguments != (('constant', int(consumers['thresholdBits'], 16)),)):
        return reject('position consumer vector or threshold changed')
    return [dict(witness, status='mapped', loweringStatus='unresolved', lookupSetup=asdict(lookup),
                 acquisitionSetups=acquisitions,
                 repeatActorSetup=asdict(actor), repeatDistanceSetup=asdict(distance))]


def recover_first_barrel_vector(source, evidence, rdata):
    if not evidence or evidence[0].get('status') != 'mapped':
        return source
    mapping = evidence[0]
    witness = json.loads(Path(__file__).with_name('native_barrel_first_vector_witness.json').read_text())
    def reject(reason):
        mapping['vectorLoweringStatus'] = 'rejected: ' + reason
        return source
    if not mapping.get('actorLoweringStatus', '').startswith('recovered'):
        return reject('first actor queries are not proven')
    if hashlib.sha256(source.encode()).hexdigest() != witness['annotatedSha256']:
        return reject('first position annotated source changed')
    setup = read_call_window(rdata, int(mapping['address'], 16), mapping['size'],
                             int(witness['moveSite'], 16), argument_count=5)
    if (setup is None or setup.target != ('constant', 0x7E72F0) or setup.ecx != ('stack', 20)
            or setup.stack_arguments != (('stack', 184), ('register', 'ebp'), ('constant', 1),
                                        ('register', 'ebp'), ('register', 'ebp'))):
        return reject('first movement operands changed')
    if any(source.count(e['old']) != e['count'] for e in witness['edits']):
        return reject('first position source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    mapping.update(vectorLoweringStatus='recovered first snapshot, two distances and movement operands',
        loweringStatus='first position lowered; native wrapper cleanup and later positions remain unresolved',
        moveSetup=asdict(setup), runtimeRequirement='proposed RetailThingPosition global binding')
    return source


def recover_barrel_teleport_choice(source, evidence, rdata, manifest):
    if not evidence or evidence[0].get('status') != 'mapped':
        return source
    mapping = evidence[0]
    w = json.loads(Path(__file__).with_name('native_barrel_teleport_choice_witness.json').read_text())
    def reject(reason):
        mapping['teleportChoiceStatus'] = 'rejected: ' + reason
        return source
    if (hashlib.sha256(source.encode()).hexdigest() != w['annotatedSha256']
            or mapping.get('bytesSha256') != w['nativeSha256'] or source.count(w['old']) != 1):
        return reject('teleport choice correspondence changed')
    for address, name in w['strings'].items():
        if rdata.string_at(int(address, 16)) != name:
            return reject('teleport marker name changed')
    for name, result, types in (('IsCameraPosOnScreen', 'bool', ['sol::table']),
            ('EntityTeleportToThing', 'void', ['CScriptThing*', 'CScriptThing*', 'sol::optional<bool>'])):
        spec = manifest.get(name, {})
        if (spec.get('scope') != 'Quest' or spec.get('returnType') != result
                or [p.get('type') for p in spec.get('parameters', [])] != types):
            return reject(name + ' contract changed')
    mapping.update(teleportChoiceStatus='recovered primary-offscreen/alternate-onscreen choice',
                   teleportSites=w['nativeSites'])
    return source.replace(w['old'], w['new'])


def recover_barrel_return_position(source, evidence, rdata):
    if not evidence or evidence[0].get('status') != 'mapped':
        return source
    mapping = evidence[0]
    w = json.loads(Path(__file__).with_name('native_barrel_return_position_witness.json').read_text())
    def reject(reason):
        mapping['returnPositionStatus'] = 'rejected: ' + reason
        return source
    if hashlib.sha256(source.encode()).hexdigest() != w['annotatedSha256']:
        return reject('return position annotated source changed')
    start, size = int(mapping['address'], 16), mapping['size']
    raw = rdata.bytes_at(start, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != w['nativeSha256']:
        return reject('return position native bytes changed')
    if rdata.string_at(int(w['string']['address'], 16)) != w['string']['value']:
        return reject('ManStart name changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    vtable = ('memory', ('address', receiver, 0))
    lookup = read_call_window(rdata, start, size, int(w['lookupSite'], 16), argument_count=2)
    if (lookup is None or lookup.ecx != receiver
            or lookup.target != ('memory', ('address', vtable, 0x120))
            or lookup.stack_arguments != (('stack', 36), ('stack', 60))
            or rdata.bytes_at(0xDB5915, 4) != bytes.fromhex('8b4c2428')):
        return reject('ManStart implementation does not belong to cached lookup')
    acquisitions = []
    for site in w['acquisitionSites']:
        setup = read_call_window(rdata, start, size, int(site, 16), argument_count=3)
        if (setup is None or setup.ecx != receiver
                or setup.target != ('memory', ('address', vtable, 0x20))
                or setup.stack_arguments != (('register', 'edi'), ('stack', 20), ('constant', 4))):
            return reject('return-position actor acquisition changed')
        acquisitions.append(asdict(setup))
    getter_site = int(w['getters'][1], 16)
    getter = read_call_window(rdata, start, size, getter_site, argument_count=1)
    distance = read_call_window(rdata, start, size, int(w['distances'][1], 16),
                               {getter_site: (4, 'controlled_actor')}, argument_count=1)
    move = read_call_window(rdata, start, size, int(w['movementSite'], 16), argument_count=5)
    if (getter is None or getter.target != ('constant', 0x7E7490) or getter.ecx != ('stack', 20)
            or getter.stack_arguments != (('stack', 244),)
            or distance is None or distance.target != ('constant', 0xCBE45C)
            or distance.ecx != ('result', getter_site, 'controlled_actor')
            or distance.edx != ('stack', 196) or distance.stack_arguments != (('constant', 0x40000000),)
            or move is None or move.target != ('constant', 0x7E72F0) or move.ecx != ('stack', 20)
            or move.stack_arguments != (('stack', 196), ('register', 'ebp'), ('constant', 1),
                                       ('register', 'ebp'), ('register', 'ebp'))):
        return reject('return position actor/vector/movement consumers changed')
    if any(source.count(e['old']) != e['count'] for e in w['edits']):
        return reject('return position source correspondence changed')
    for edit in w['edits']:
        source = source.replace(edit['old'], edit['new'])
    mapping.update(returnPositionStatus='recovered cached ManStart snapshot, two distances and movement',
        returnLookup=asdict(lookup), returnAcquisitions=acquisitions,
        returnGetter=asdict(getter), returnDistance=asdict(distance), returnMove=asdict(move),
        firstReturnConsumerEvidence='whole-function native hash and inspected DB5935..DB5965 stack instructions; not call-window IR')
    return source
