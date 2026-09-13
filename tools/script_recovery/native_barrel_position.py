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
