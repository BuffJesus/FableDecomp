"""Ordered x87 predicate retained in BL across a normal callee-saved cleanup.

Consumes a registered, checked normal-ABI frame. Traverses actual CFG edges,
so unrelated code between a conditional arm and its join is not interpreted.
Caller must separately prove producer identity, object ownership and liveness.
"""
import hashlib,math,struct
from capstone import x86 as x

def prove(frame,query_site,read_bytes,cleanup_target,*,positive_exit,false_exit):
    def require(ok,why):
        if not ok:raise ValueError('register saved x87 predicate: '+why)
    ins,by,preds=frame['ins'],frame['by'],frame['preds']
    require(query_site in by and ins[by[query_site]].mnemonic=='call','query identity')
    query=ins[by[query_site]];start=query.address+query.size
    require(start in by and positive_exit!=false_exit and all(a in by for a in [positive_exit,false_exit]),'scope endpoints')
    successors={k:[] for k in preds}
    for k,parents in preds.items():
        for parent in parents:successors[parent].append(k)
    nodes=set();exits=set();pending=[by[start]]
    while pending:
        k=pending.pop()
        require(k in frame['reachable'],'unreachable predicate node')
        if ins[k].address in [positive_exit,false_exit]:exits.add(ins[k].address);continue
        if k in nodes:continue
        nodes.add(k);require(len(nodes)<=11,'predicate extent')
        require(bool(successors[k]),'terminal node inside predicate')
        pending.extend(successors[k])
    require(len(nodes)==11 and exits=={positive_exit,false_exit},'predicate extent/exits')
    code=[ins[k] for k in sorted(nodes)]
    require([i.mnemonic for i in code]==['fcomp','fnstsw','test','jne','mov','jmp','xor','lea','call','test','je'],'instruction sequence')
    compare,status,mask,branch,yes,jump,no,receiver,cleanup,test,exit=code
    def reg(op,value):return op.type==x.X86_OP_REG and op.reg==value
    def imm(op,value):return op.type==x.X86_OP_IMM and op.imm==value
    operand=compare.operands[0]
    require(operand.type==x.X86_OP_MEM and operand.size==4 and not operand.mem.base and not operand.mem.index and not operand.mem.segment,'threshold operand')
    constant=read_bytes(operand.mem.disp,4)
    require(constant is not None and len(constant)==4,'threshold unavailable')
    threshold=struct.unpack('<f',constant)[0];require(math.isfinite(threshold),'nonfinite threshold')
    require(reg(status.operands[0],x.X86_REG_AX),'status register')
    require(reg(mask.operands[0],x.X86_REG_AH) and imm(mask.operands[1],0x41),'ordered status mask')
    require(imm(branch.operands[0],no.address),'negative status arm')
    require(reg(yes.operands[0],x.X86_REG_BL) and imm(yes.operands[1],1),'positive flag')
    require(imm(jump.operands[0],receiver.address),'positive join')
    require(all(reg(op,x.X86_REG_BL) for op in no.operands),'negative flag')
    require(reg(receiver.operands[0],x.X86_REG_ECX),'cleanup receiver register')
    local=receiver.operands[1]
    require(local.type==x.X86_OP_MEM and local.mem.base==x.X86_REG_ESP and not local.mem.index and not local.mem.segment and 0<=local.mem.disp<4096,'cleanup receiver local')
    require(imm(cleanup.operands[0],cleanup_target),'cleanup identity')
    require(all(reg(op,x.X86_REG_BL) for op in test.operands),'saved flag consumer')
    require(imm(exit.operands[0],false_exit) and exit.address+exit.size==positive_exit,'decision exits')
    incoming=[(ins[p].address,ins[k].address) for k in nodes for p in preds[k] if p not in nodes]
    require(incoming==[(query_site,start)],'external predicate entry')
    # Require the exact local graph, not just its sorted instruction inventory.
    expected={compare.address:[status.address],status.address:[mask.address],mask.address:[branch.address],
              branch.address:[yes.address,no.address],yes.address:[jump.address],jump.address:[receiver.address],
              no.address:[receiver.address],receiver.address:[cleanup.address],cleanup.address:[test.address],
              test.address:[exit.address],exit.address:[positive_exit,false_exit]}
    for i in code:require({ins[k].address for k in successors[by[i.address]]}==set(expected[i.address]),'predicate graph edges')
    depths={frame['depths'][k] for k in nodes};require(len(depths)==1,'predicate stack depth')
    depth=next(iter(depths));require(local.mem.disp+12<=depth,'Thing outside frame')
    contract=frame['contracts'].get(hex(cleanup.address))
    require(contract is not None and contract['derivedNormalPurge']==0,'cleanup purge')
    body=b''.join(bytes(i.bytes) for i in code)
    return dict(start=hex(start),positiveExit=hex(positive_exit),falseExit=hex(false_exit),
        nativeNodeSha256=hashlib.sha256(body).hexdigest(),nodes=[hex(i.address) for i in code],
        thresholdAddress=hex(operand.mem.disp),thresholdBytes=constant.hex(),threshold=threshold,
        flagRegister='BL',preservedParent='EBX',cleanupTarget=hex(cleanup_target),cleanup=hex(cleanup.address),
        cleanupOffset=local.mem.disp,frameDepth=depth,relation='ordered-greater',unordered=False,
        incoming=[dict(source=hex(a),target=hex(b)) for a,b in incoming],
        limits='Checked normal child ABI,callee-saved EBX preservation,valid12-byte Thing anddisjoint caller locals;no heap/lifecycle/reentry/exception proof.')
