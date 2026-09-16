"""Verify both resource-derived health Things and their immediate destruction."""
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_villager_control_resource import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(data,temporaries=None):
    control=verify_control(data)
    if data.bytes_at(0x122DEDC,4)!=bytes(4):raise ValueError('Villager health zero threshold changed')
    rows=temporaries if temporaries is not None else json.loads(Path(__file__).with_name('native_villager_health_temporaries_witness.json').read_text())
    getters={e['site']:e for e in control['events'] if e['name']=='get_thing'}
    if len(rows)!=len(getters) or {r['create'] for r in rows}!=set(getters):raise ValueError('Villager health temporary coverage changed')
    start,size=control['address'],control['size'];decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(start,size),start));at={i.address:i for i in instructions};events={}
    for row in rows:
        create=row['create'];identity=tuple(row['output'])
        if getters[create]['setup']['stack_arguments']!=[row['output']]:raise ValueError('Villager health output changed')
        query=read_call_window(data,start,size,row['query'],{create:(4,'thing')},argument_count=1)
        destroy=read_call_window(data,start,size,row['destroy'],{},argument_count=0)
        if query is None or destroy is None or json.loads(json.dumps(asdict(query)))!=row['querySetup'] or json.loads(json.dumps(asdict(destroy)))!=row['destroySetup']:
            raise ValueError('Villager health setup changed')
        if (not at[row['query']].op_str.endswith('+ 0x420]') or query.ecx!=('memory',('address',('register','esi'),4))
            or query.stack_arguments!=(('result',create,'thing'),) or destroy.ecx!=identity or destroy.target!=('constant',0x4AA840)):
            raise ValueError('Villager health receiver/destructor changed')
        for key,operation in [('create','start'),('query','use'),('destroy','end')]:events[row[key]]=(operation,identity)
    if not check_single_resource_lifetime(instructions,events):raise ValueError('Villager health temporary lifetime changed')
    return dict(status='verified',temporaries=rows,controlSha256=control['sha256'])
