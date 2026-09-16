"""Verify every controlled Thing query and its immediate native lifetime."""
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_barrel_man_resources import verify as verify_resource
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_bounded_switch import resolve
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
from tools.script_recovery.native_barrel_man_position_snapshots import verify as verify_snapshots


def verify(function,data,witness=None):
    resource=verify_resource(function,data)
    snapshots=verify_snapshots(data)
    witness=witness or json.loads(Path(__file__).with_name('native_barrel_man_temporary_things_witness.json').read_text())
    getters={e['site']:e for e in resource['events'] if e['name']=='get_thing'}
    things=witness['temporaries'];start,size=resource['address'],resource['size']
    if len(things)!=len(getters) or {t['create'] for t in things}!=set(getters):
        raise ValueError('Barrel temporary Thing coverage changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(start,size),start));at={i.address:i for i in instructions}
    events={}
    for thing in things:
        identity=tuple(thing['output']);create=thing['create']
        if getters[create]['setup']['stack_arguments']!=[thing['output']]:
            raise ValueError('Barrel temporary output changed')
        query=read_call_window(data,start,size,thing['query'],{create:(4,'thing')},argument_count=1)
        destroy=read_call_window(data,start,size,thing['destroy'],{},argument_count=0)
        if query is None or destroy is None:
            raise ValueError('Barrel temporary setup unavailable')
        if (json.loads(json.dumps(asdict(query)))!=thing['querySetup']
                or json.loads(json.dumps(asdict(destroy)))!=thing['destroySetup']):
            raise ValueError('Barrel temporary operands changed')
        if destroy.ecx!=identity or destroy.target!=('constant',0x4AA840):
            raise ValueError('Barrel temporary destructor changed')
        result=('result',create,'thing')
        if thing['kind']=='distance':
            if (query.target!=('constant',0xCBE45C) or query.ecx!=result
                    or query.edx[0]!='stack' or query.stack_arguments!=(('constant',0x40000000),)):
                raise ValueError('Barrel temporary distance receiver/threshold changed')
            expected = snapshots['snapshots'][0 if thing['query'] < 0xDB5800 else 1]
            if query.edx != ('stack', expected['snapshotStack']):
                raise ValueError('Barrel temporary distance snapshot identity changed')
        elif thing['kind']=='health':
            if (not at[thing['query']].op_str.endswith('+ 0x420]')
                    or query.ecx!=('memory',('address',('register','esi'),4))
                    or query.stack_arguments!=(result,)):
                raise ValueError('Barrel temporary health receiver changed')
        else:raise ValueError('Unknown Barrel temporary query')
        for operation,key in (('start','create'),('use','query'),('end','destroy')):
            site=thing[key]
            if site in events:raise ValueError('Overlapping Barrel temporary events')
            events[site]=(operation,identity)
    if not check_single_resource_lifetime(instructions,events,indirect_branches=resolve(instructions,data)):
        raise ValueError('Barrel temporary lifetime CFG changed')
    return dict(witness,status='verified',owningResourceSha256=resource['nativeSha256'],
                positionSnapshots=snapshots,
                remaining='Compose retained marker, movie and controlled-Thing ownership into Lua lowering.')
