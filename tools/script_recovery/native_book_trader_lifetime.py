"""Verify BookTrader's complete controlled-resource and temporary-Thing lifetimes."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_post_attack_resources import map_book_trader_resources
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def map_book_trader_lifetime(function, rdata):
    base = map_book_trader_resources(function, rdata)
    if not base or base[0].get('status') != 'mapped':
        return base
    witness = json.loads(Path(__file__).with_name('native_book_trader_lifetime_witness.json').read_text())
    def reject(reason):
        return [{'status': 'rejected', 'reason': reason}]
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('BookTrader lifetime bytes changed')
    for event in witness['setups']:
        known = {int(site): tuple(value) for site, value in event['knownCalls'].items()}
        actual = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                 event['site'], known, argument_count=event['argumentCount'])
        if actual is None or json.loads(json.dumps(asdict(actual))) != event['setup']:
            return reject('BookTrader lifetime operands changed at ' + hex(event['site']))
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(rdata.bytes_at(int(witness['address'], 16), witness['size']),
                                       int(witness['address'], 16)))
    resource_events = {event['site']: (event['operation'], tuple(event['identity']))
                       for event in witness['resourceEvents']}
    # Inventory every known resource helper call in the pinned body, including
    # uses that would not by themselves affect the single-live-resource check.
    targets = {0x99A380, 0x99A430, 0xCD23B9, 0xCD2770, 0x7E7490, 0x7E7390,
               0x7E7450, 0x7E72F0, 0x7E73D0, 0x7E74D0, 0x7E7400, 0x7E7360}
    expected_sites = {ins.address for ins in instructions if ins.mnemonic == 'call'
                      and ins.op_str.startswith('0x') and int(ins.op_str, 16) in targets}
    expected_sites.update(int(event['site'], 16) for event in base[0]['events'] if event['name'] == 'acquire')
    if set(resource_events) != expected_sites or len(resource_events) != len(witness['resourceEvents']):
        return reject('BookTrader resource event coverage changed')
    selections = {int(site): tuple(identity) for site, identity in witness['selections'].items()}
    if not check_single_resource_lifetime(instructions, resource_events,
                                          receiver_register='ecx', selections=selections):
        return reject('BookTrader controlled-resource lifetime CFG changed')
    thing_events = {event['site']: (event['operation'], tuple(event['identity']))
                    for event in witness['thingEvents']}
    getters = {event['site'] for event in witness['resourceEvents'] if event['name'] == 'get_thing'}
    if {site for site, (operation, _) in thing_events.items() if operation == 'start'} != getters:
        return reject('BookTrader temporary getter coverage changed')
    if not check_single_resource_lifetime(instructions, thing_events):
        return reject('BookTrader temporary-Thing lifetime CFG changed')
    return [dict(witness, status='mapped',
        lifetimeStatus='one constructor, all mapped uses, four destruction exits verified',
        temporaryThingStatus='eight getter/query/destruction lifetimes verified',
        loweringStatus='explicit resource candidate not yet generated')]
