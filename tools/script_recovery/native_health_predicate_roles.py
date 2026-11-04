"""Native caller roles for a health result consumed by a saved x87 predicate.

Caller supplies a checked registered-entry/frame context and predicate proof.
Normal child ABI, expected GSI interface, valid objects and disjoint heap remain
explicit contracts. This does not reconstruct resource acquisition or ownership.
"""
from dataclasses import asdict

from capstone import x86 as x
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_receiver_reaching import _reaching


def prove_roles(frame, query_site, predicate, image, *, getter_target, this_definition, health_slot):
    def require(ok, why):
        if not ok:
            raise ValueError('health predicate roles: ' + why)

    ins, by, depths, records = (frame[k] for k in ('ins','by','depths','calls'))
    start, end = frame['start'], frame['end']
    query_index = by[query_site]
    query = ins[query_index]
    require(query.mnemonic == 'call' and query.address + query.size == int(predicate['start'],16), 'query/predicate adjacency')
    previous = [i for i in ins[:query_index] if i.mnemonic == 'call']
    require(bool(previous), 'getter absent')
    getter = previous[-1]
    require(getter.operands[0].type == x.X86_OP_IMM and getter.operands[0].imm == getter_target, 'getter identity')
    require([i.mnemonic for i in ins[by[getter.address]+1:query_index]] == ['mov','push'], 'intervening query setup')
    receiver = ('memory',('address',('register','esi'),4))
    target = ('memory',('address',('memory',('address',receiver,0)),health_slot))
    setup = read_call_window(image,start,end-start,query_site,{getter.address:(4,'health_actor')},argument_count=1)
    require(setup is not None and setup.ecx == receiver and setup.target == target
            and setup.stack_arguments == (('result',getter.address,'health_actor'),), 'query receiver/actor')
    creation = read_call_window(image,start,end-start,getter.address,argument_count=1)
    require(creation is not None and creation.target == ('constant',getter_target)
            and creation.ecx[0] == 'stack' and len(creation.stack_arguments) == 1
            and creation.stack_arguments[0][0] == 'stack', 'getter output/resource')
    require(creation.stack_arguments[0][1] == predicate['cleanupOffset'], 'created/destroyed Thing mismatch')
    for address in (getter.address,query_site):
        definitions = _reaching(ins,frame['preds'],by[address],x.X86_REG_ESI,x)
        require(-1 not in definitions and {ins[k].address for k in definitions} == {this_definition}, 'owner origin')
    a,b = int(predicate['start'],16),int(predicate['endExclusive'],16)
    nodes = {k for k in frame['reachable'] if a <= ins[k].address < b}
    incoming = [(ins[p].address,ins[k].address) for k in nodes for p in frame['preds'][k] if p not in nodes]
    require(incoming == [(query_site,a)], 'predicate incoming edges')
    depth = depths[by[a]]
    require(all(depths[k] == depth for k in nodes), 'predicate stack depth')
    require(frame['contracts'][hex(query_site)]['derivedNormalPurge'] == 4
            and frame['contracts'][hex(getter.address)]['derivedNormalPurge'] == 4, 'query/getter purge')
    cleanup = [i for i in ins if a <= i.address < b and i.mnemonic == 'call']
    require(len(cleanup) == 1, 'cleanup count')
    destroy = cleanup[0]
    require(frame['contracts'][hex(destroy.address)]['derivedNormalPurge'] == 0, 'cleanup purge')
    require(records[getter.address]['pushedStack'] == [predicate['cleanupOffset']-depth]
            and records[destroy.address]['ecxStack'] == predicate['cleanupOffset']-depth, 'exported object coordinates')
    require(records[getter.address]['ecxStack'] == creation.ecx[1]-depth, 'exported resource coordinates')
    require(not predicate['cleanupOffset'] <= predicate['flagOffset'] < predicate['cleanupOffset']+12, '12-byte Thing overlaps saved flag')
    require(0 <= predicate['cleanupOffset'] and predicate['cleanupOffset']+12 <= depth, 'Thing outside frame')
    return dict(query=hex(query_site), getter=hex(getter.address), cleanup=hex(destroy.address),
                frameDepth=depth, objectEntryOffset=predicate['cleanupOffset']-depth,
                resourceEntryOffset=creation.ecx[1]-depth, objectBytes=12,
                getterSetup=asdict(creation), querySetup=asdict(setup),
                incoming=[dict(source=hex(p),target=hex(q)) for p,q in incoming],
                purges=dict(getter=4,query=4,cleanup=0),
                limits='Normal checked child ABI; valid expected GSI interface and 12-byte Thing output; disjoint heaps and preserved caller locals; acquisition/ownership/reentry/exceptions remain outside this role proof.')
