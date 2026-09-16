"""Prove one retained presented-item CString across both native polling sites."""
from collections import deque
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_IMM,X86_OP_REG,X86_OP_MEM,X86_REG_ECX,X86_REG_ESP
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def prove(data=None,*,omit_cleanup=False):
    data=data or RData();_,w=recover(data);c=Cs(CS_ARCH_X86,CS_MODE_32);c.detail=True
    ins=list(c.disasm(data.bytes_at(w['mainAddress'],w['mainSize']),w['mainAddress']))
    positions={i.address:n for n,i in enumerate(ins)};queue=deque([(0,False,None)]);seen=set();visited=set()
    while queue:
        index,live,selected=queue.popleft()
        if (index,live,selected) in seen:continue
        seen.add((index,live,selected));i=ins[index]
        if any(i.reg_name(r) in ('ecx','cx','cl','ch') for r in i.regs_access()[1]):selected=None
        if i.mnemonic=='lea' and len(i.operands)==2 and i.operands[0].type==X86_OP_REG and i.operands[0].reg==X86_REG_ECX:
            mem=i.operands[1]
            if mem.type==X86_OP_MEM and mem.mem.base==X86_REG_ESP and not mem.mem.index:selected=mem.mem.disp
        if i.address==0xdbb60e:
            if live:raise ValueError('Presented output constructed twice')
            live=True;visited.add(i.address)
        if i.address in (0xdbb9f2,0xdbba2e):
            if not live:raise ValueError('Presented poll outside output lifetime')
            visited.add(i.address)
        if i.address==0xdbbd87 or i.address==0xdbccdd and selected==40:
            if not live:raise ValueError('Presented output destroyed outside lifetime')
            if not omit_cleanup:live=False
            visited.add(i.address)
        if i.address==0xdbccdd and selected is None:raise ValueError('Shared string cleanup receiver unknown')
        if i.mnemonic=='call':selected=None
        if i.mnemonic.startswith('ret'):
            if live:raise ValueError('Presented output survives native return')
            continue
        nexts=[index+1]
        if i.mnemonic.startswith('j'):
            if not i.operands or i.operands[0].type!=X86_OP_IMM:raise ValueError('Unproved branch')
            target=positions[i.operands[0].imm]
            nexts=[target] if i.mnemonic=='jmp' else nexts+[target]
        for n in nexts:
            if n>=len(ins):raise ValueError('Unterminated CFG')
            queue.append((n,live,selected))
    if visited!={0xdbb60e,0xdbb9f2,0xdbba2e,0xdbbd87,0xdbccdd}:raise ValueError('Incomplete output proof')
    return {'nativeCtor':0xdbb60e,'stackOffset':40,'polls':[0xdbb9f2,0xdbba2e],
        'normalCleanup':0xdbbd87,'sharedCleanup':0xdbccdd,'cfgStatesChecked':len(seen),
        'lifetime':'One default CString spans talk/teddy predicates, both item polls and optional movie; never recreated per poll.',
        'limits':'Other strings at the shared cleanup call are distinct scopes; no engine exception-flow claim.'}

if __name__=='__main__':print(prove())
