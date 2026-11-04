"""Restore reviewed stack-backed call arguments from registration and cleanup evidence."""
import hashlib
import re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86


def restore(source, todo, fn, unit, rd):
    row = unit.get('reviewedStackCallArguments', {}).get(str(fn['address']).lower())
    if row is None:
        return source, todo
    def require(ok, why):
        if not ok:
            raise ValueError('Reviewed stack call argument: ' + why)
    source = source.replace('\r\n', '\n').replace('\r', '\n')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rd.bytes_at(start, end-start)
    for key, data in [('nativeSha256', raw), ('sourceSha256', fn['decompile'].encode()), ('luaSha256', source.encode())]:
        require(data is not None and hashlib.sha256(data).hexdigest() == row[key], key+' changed')
    cs = Cs(CS_ARCH_X86, CS_MODE_32); cs.detail = True
    ins = list(cs.disasm(raw, start)); index = {i.address:i for i in ins}
    require(ins and ins[-1].address+ins[-1].size == end, 'incomplete body')
    def reg(op, name):
        return op.type == x86.X86_OP_REG and op.reg == getattr(x86, 'X86_REG_'+name.upper())
    def mem(op, base, disp):
        return op.type == x86.X86_OP_MEM and op.size == 4 and op.mem.base == getattr(x86, 'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp == disp
    require(ins[0].mnemonic == 'sub' and reg(ins[0].operands[0], 'esp') and ins[0].operands[1].type == x86.X86_OP_IMM, 'frame prologue')
    frame = ins[0].operands[1].imm
    for i in ins[1:]:
        if i.mnemonic == 'call': break
        if i.mnemonic == 'push':
            require(i.operands[0].type == x86.X86_OP_REG, 'ambiguous prologue'); frame += 4
        else:
            require(i.mnemonic in ['mov','lea','xor'] and x86.X86_REG_ESP not in i.regs_access()[1], 'ambiguous prologue')
    require(frame == row['frameBytes'], 'frame changed')
    require(set(row['windows']) == {'registration','cache','talk','cleanup'}, 'window inventory')
    for window in row['windows'].values():
        data = rd.bytes_at(window['start'], window['end']-window['start'])
        require(data is not None and hashlib.sha256(data).hexdigest() == window['sha256'], 'native window changed')
    require(len(row['timers']) == 2, 'timer inventory')
    names = []
    for timer in row['timers']:
        call, copy, store, setter = (index[timer[k]] for k in ['registerCall','returnCopy','store','setCall'])
        require(call.mnemonic == 'call' and mem(call.operands[0], 'edx', row['registerSlot']), 'registration call')
        require(copy.mnemonic == 'mov' and reg(copy.operands[0], timer['register']) and reg(copy.operands[1], 'eax'), 'return identity')
        middle = [i for i in ins if copy.address < i.address < store.address]
        require([i.mnemonic for i in middle] == ['mov','push','push'] and middle[1].operands[0].type == x86.X86_OP_IMM and middle[1].operands[0].imm == 0 and reg(middle[2].operands[0],timer['register']), 'timer setup arguments')
        require(store.mnemonic == 'mov' and mem(store.operands[0], 'esp', frame+timer['entrySlot']+8) and reg(store.operands[1], timer['register']), 'registration stack slot')
        require(setter.mnemonic == 'call' and mem(setter.operands[0], 'eax', row['setSlot']), 'timer setup call')
        name = timer['local']; names.append(name)
        require(name == f'i_stk_{-timer["entrySlot"]:x}', 'stack local identity')
        require(re.search(r'(?m)^    local [^\n]*\b'+re.escape(name)+r'\b',source), 'missing local')
        require(source.count(timer['initialization']) == 1 and timer['initialization'].endswith(f'quest:SetTimer({name}, 0)'), 'Lua registration identity')
    require(len(set(names)) == 2, 'distinct timer slots')
    cache = index[row['cacheLoad']]
    require(cache.mnemonic == 'mov' and reg(cache.operands[0], 'edi') and mem(cache.operands[1], 'esp', frame+row['timers'][0]['entrySlot']), 'cached first timer')
    load, push1, call1, push2, call2 = (index[row[k]] for k in ['cleanupLoad','firstPush','firstCall','secondPush','secondCall'])
    require(load.mnemonic == 'mov' and reg(load.operands[0], 'edx') and mem(load.operands[1], 'esp',frame+row['timers'][1]['entrySlot']), 'second timer cleanup load')
    require(push1.mnemonic == push2.mnemonic == 'push' and reg(push1.operands[0], 'edx') and reg(push2.operands[0], 'edi'), 'cleanup argument order')
    require(all(i.mnemonic == 'call' and mem(i.operands[0], 'eax',row['cleanupSlot']) for i in [call1,call2]), 'cleanup calls')
    branch = index[row['terminationBranch']]
    require(branch.mnemonic == 'jne' and branch.operands[0].type == x86.X86_OP_IMM and branch.operands[0].imm == row['windows']['cleanup']['start'], 'termination edge')
    api = row['api']
    target = rd.bytes_at(api['table']+row['cleanupSlot'], 4)
    require(target is not None and int.from_bytes(target,'little') == api['start'], 'cleanup vtable target')
    body = rd.bytes_at(api['start'], api['end']-api['start'])
    require(body is not None and hashlib.sha256(body).hexdigest() == api['sha256'], 'cleanup API changed')
    ret = next((i for i in cs.disasm(body,api['start']) if i.address == api['return']), None)
    require(ret is not None and ret.mnemonic == 'ret' and ret.operands[0].imm == 4, 'cleanup stack purge')
    old = row['block']
    require(source.count(old) == 1, 'ambiguous cleanup block')
    duplicate = f'quest:DeregisterTimer({names[0]})'
    require(old.count(duplicate) == 2 and old.count('quest:DeregisterTimer(') == 2 and 'me:IsTalkedToByHero()' in old and 'quest:IsActiveThreadTerminating()' in old and 'return\n' in old, 'cleanup block scope')
    return source.replace(old,old.replace(duplicate,f'quest:DeregisterTimer({names[1]})',1)), todo
