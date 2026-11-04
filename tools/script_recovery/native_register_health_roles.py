"""Native caller roles for a health result consumed by a saved x87 predicate.

Caller supplies a checked registered-entry/frame context and predicate proof.
Normal child ABI, expected GSI interface, valid objects and disjoint heap remain
explicit contracts. This does not reconstruct resource acquisition or ownership.
"""
from dataclasses import asdict
import hashlib

from capstone import x86 as x
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_receiver_reaching import _reaching


def prove_roles(frame, query_site, predicate, image, *, getter_target, this_definition, health_slot, call_window=None):
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
    window_report=None
    if call_window is not None:
        require(isinstance(call_window,dict) and set(call_window)=={'start','nativeSha256'},'call window schema')
        window=int(call_window['start'],16);require(start<=window<getter.address and window in by,'call window bounds')
        prefix=ins[by[window]:query_index+1];require(len(prefix)==9 and [i.mnemonic for i in prefix]==['mov','mov','lea','push','lea','call','mov','push','call'],'fresh load prefix shape')
        body=b''.join(bytes(i.bytes) for i in prefix);require(hashlib.sha256(body).hexdigest()==call_window['nativeSha256'],'call window native pin')
        def mem(op,base,disp):return op.type==x.X86_OP_MEM and op.mem.base==base and op.mem.disp==disp and not op.mem.index and not op.mem.segment
        first,table=prefix[:2];require(first.operands[0].type==x.X86_OP_REG and first.operands[0].size==4 and mem(first.operands[1],x.X86_REG_ESI,4),'fresh interface load')
        require(table.operands[0].type==x.X86_OP_REG and table.operands[0].size==4 and mem(table.operands[1],first.operands[0].reg,0),'fresh table load')
        require(mem(query.operands[0],table.operands[0].reg,health_slot) and prefix[5].address==getter.address,'fresh prefix target/getter')
        nodes=set(range(by[window],query_index+1));incoming=[(ins[p].address,ins[k].address) for k in nodes for p in frame['preds'][k] if p not in nodes]
        require(nodes<=frame['reachable'] and incoming and all(b==window for a,b in incoming),'call window single entry')
        start=window;window_report=dict(start=hex(window),endExclusive=hex(query.address+query.size),nativeSha256=call_window['nativeSha256'],incoming=[dict(source=hex(a),target=hex(b)) for a,b in incoming],freshInterfaceAndTableLoads=True,earlierHeapAliasEqualityAssumed=False,limits='Single-entry fresh native load/call window with global owner reaching proof. Normal getter preservation of callee-saved table bits/caller memory and current expected GSI/resource types remain conditions; no earlier parent/entity heap equality or disjointness inferred.')
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
    a = int(predicate['start'],16)
    addresses = [int(value,16) for value in predicate['nodes']]
    require(len(addresses)==len(set(addresses))==11 and a==min(addresses), 'predicate node inventory')
    require(all(address in by for address in addresses), 'predicate node address')
    nodes = {by[address] for address in addresses}
    require(nodes <= frame['reachable'], 'predicate unreachable node')
    incoming = [(ins[p].address,ins[k].address) for k in nodes for p in frame['preds'][k] if p not in nodes]
    require(incoming == [(query_site,a)], 'predicate incoming edges')
    depth = depths[by[a]]
    require(all(depths[k] == depth for k in nodes), 'predicate stack depth')
    require(frame['contracts'][hex(query_site)]['derivedNormalPurge'] == 4
            and frame['contracts'][hex(getter.address)]['derivedNormalPurge'] == 4, 'query/getter purge')
    cleanup = [ins[k] for k in sorted(nodes) if ins[k].mnemonic == 'call']
    require(len(cleanup) == 1, 'cleanup count')
    destroy = cleanup[0]
    require(frame['contracts'][hex(destroy.address)]['derivedNormalPurge'] == 0, 'cleanup purge')
    require(records[getter.address]['pushedStack'] == [predicate['cleanupOffset']-depth]
            and records[destroy.address]['ecxStack'] == predicate['cleanupOffset']-depth, 'exported object coordinates')
    require(records[getter.address]['ecxStack'] == creation.ecx[1]-depth, 'exported resource coordinates')
    require(predicate['flagRegister']=='BL' and predicate['preservedParent']=='EBX', 'saved flag identity')
    consumers=[k for k in nodes if ins[k].mnemonic=='test' and all(op.type==x.X86_OP_REG and op.reg==x.X86_REG_BL for op in ins[k].operands)]
    require(len(consumers)==1, 'saved flag consumer')
    definitions=_reaching(ins,frame['preds'],consumers[0],x.X86_REG_BL,x)
    stores={k for k in nodes if ins[k].mnemonic in ('mov','xor') and ins[k].operands[0].type==x.X86_OP_REG and ins[k].operands[0].reg==x.X86_REG_BL}
    require(len(stores)==2 and definitions==stores, 'saved flag reaching definitions')

    require(0 <= predicate['cleanupOffset'] and predicate['cleanupOffset']+12 <= depth, 'Thing outside frame')
    result=dict(query=hex(query_site), getter=hex(getter.address), cleanup=hex(destroy.address),
                frameDepth=depth, objectEntryOffset=predicate['cleanupOffset']-depth,
                resourceEntryOffset=creation.ecx[1]-depth, objectBytes=12,
                getterSetup=asdict(creation), querySetup=asdict(setup),
                incoming=[dict(source=hex(p),target=hex(q)) for p,q in incoming],
                purges=dict(getter=4,query=4,cleanup=0),
                limits='Normal checked child ABI; valid expected GSI interface and 12-byte Thing output; disjoint heaps and preserved caller locals; acquisition/ownership/reentry/exceptions remain outside this role proof.')

    if window_report is not None:result['boundedCallWindow']=window_report
    return result
