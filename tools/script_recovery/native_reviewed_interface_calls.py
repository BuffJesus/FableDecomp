"""Restore explicitly reviewed interface call heads with lost receiver identity."""
import hashlib
from collections import Counter


def prepare(text, fn, row, rdata, entries):
    """Validate against original call sites before optionally deferring edits.

    Keeping explicit receiver uses through lowering preserves the evidence used
    to separate reused pointer/string/scalar stack slots. Only exact reviewed
    call spellings may be replaced later; a changed spelling fails closed.
    """
    restored = restore(text, fn, row, rdata, entries)
    if row is None or 'deferUntilLowered' not in row:
        return restored, ()
    if row['deferUntilLowered'] is not True:
        raise ValueError('reviewed interface calls: invalid deferred phase')
    pending, original = Counter(), Counter()
    for head in row['heads']:
        receiver = head.get('printedReceiver')
        if receiver is None:
            raise ValueError('reviewed interface calls: deferred receiver missing')
        for site in head['sites']:
            args = row['arguments'][hex(int(site, 16))]
            before = head['old'] + '(' + ','.join([receiver, *args]) + ')'
            after = 'GSI->' + row['method'] + '(' + ','.join(args) + ')'
            original[before, after] += 1
            if 'counterRenamedReceiver' in head:
                if head['counterRenamedReceiver'] is not True or 'receiverRegister' not in head:
                    raise ValueError('reviewed interface calls: invalid counter receiver phase')
                name = receiver.removeprefix('(void *)')
                counter = 'ctr_' + (name.split('_', 1)[1] if '_' in name else name)
                lowered_head = head['old'].replace('*(int *)' + name, '*' + name)
                before = lowered_head + '(' + ','.join([counter, *args]) + ')'
            pending[before, after] += 1
    edits = tuple((before, after, count) for (before, after), count in pending.items())
    original_edits = tuple((before, after, count) for (before, after), count in original.items())
    if apply_deferred(text, original_edits) != restored:
        raise ValueError('reviewed interface calls: deferred edits differ from reviewed edits')
    return text, edits


def apply_deferred(text, edits):
    for before, after, count in edits:
        if text.count(before) != count:
            raise ValueError('reviewed interface calls: deferred call inventory changed')
        text = text.replace(before, after)
    return text


def restore(text, fn, row, rdata, entries):
    if row is None:
        return text
    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed interface calls {fn["address"]}: {why}')
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'source changed')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32, x86
    decoder = Cs(CS_ARCH_X86, CS_MODE_32); decoder.detail = True
    ins = list(decoder.disasm(raw, start))
    require(ins and ins[-1].address + ins[-1].size == end, 'incomplete disassembly')
    by_site = {i.address:i for i in ins}
    require(len(row['nativeInstructions']) >= 4, 'too few instruction anchors')
    for site, encoded in row['nativeInstructions'].items():
        native = by_site.get(int(site,16))
        require(native is not None and native.bytes.hex() == encoded, 'instruction changed at '+site)
    from tools.script_recovery.annotate_interface_slots import load_slots
    from tools.script_recovery.convert_quest_unit import callee_stack_words
    slot, table, target = (int(row[k],16) for k in ('slot','table','target'))
    require(table == 0x01260f0c and load_slots().get(slot) == row['method'], 'interface slot method changed')
    require(rdata.bytes_at(table+slot,4) == target.to_bytes(4,'little'), 'interface target changed')
    encoded = bytes.fromhex(row['calleeBytes'])
    require(len(encoded) >= 16 and rdata.bytes_at(target,len(encoded)) == encoded, 'callee bytes changed')
    purge = row['purgeWords']
    require(isinstance(purge,int) and not isinstance(purge,bool) and 0 < purge <= 16,
            'invalid callee purge')
    require(callee_stack_words(target) == purge, 'callee purge changed')
    require(isinstance(row['heads'],list) and row['heads'], 'missing reviewed heads')
    seen=set(); receiver_rewrites=[]
    for head in row['heads']:
        old=head['old'];sites={int(s,16) for s in head['sites']}
        receiver = head.get('printedReceiver')
        receiver_register = None
        if receiver is not None:
            import re
            spelling = re.fullmatch(r'\(void \*\)(\w+)', receiver) if isinstance(receiver,str) else None
            require(spelling is not None and '*(int *)'+spelling[1] in old,
                    'printed receiver does not match reviewed head')
            offset_text = head.get('receiverStackOffset')
            register_text = head.get('receiverRegister')
            if 'receiverRegister' in head:
                require(offset_text is None and register_text in ('eax','ebx','ecx','edx','esi','edi','ebp'),
                        'invalid reviewed receiver register')
                receiver_register = getattr(x86, 'X86_REG_' + register_text.upper())
            else:
                require(isinstance(offset_text,str) and re.fullmatch(r'0x[0-9a-f]+',offset_text),
                        'missing reviewed receiver stack offset')
                receiver_offset = int(offset_text,16)
        require(sites and len(sites)==len(head['sites']) and not (sites & seen), 'duplicated or missing sites')
        seen.update(sites)
        matching=[]
        for a,b,args,call,key,vtable in entries or []:
            begin=a-len(old)-1
            if text[begin:a]==old+'(':
                require(vtable and int(key,16)==slot and int(call['slot'],16)==slot, 'printed call slot changed')
                site = int(call['site'],16)
                expected = row['arguments'].get(hex(site))
                printed = [receiver,*expected] if receiver is not None and isinstance(expected,list) else expected
                require(isinstance(expected,list) and len(expected)==purge and args==printed,
                        'printed immediate arguments changed')
                if receiver is not None:
                    receiver_rewrites.append((begin,b,'GSI->'+row['method']+'('+','.join(expected)))
                native = by_site.get(site)
                require(native is not None and native.mnemonic=='call' and native.operands
                        and native.operands[0].type==x86.X86_OP_MEM
                        and native.operands[0].mem.disp==slot, 'native call slot changed')
                index = next(n for n,i in enumerate(ins) if i.address==site)
                if receiver_register is not None:
                    # A saved register receiver: load its vtable, push the bool,
                    # move receiver to ECX, optionally spill the saved register.
                    # Native body and anchors pin provenance beyond this prefix.
                    finish = index
                    last = ins[finish-1]
                    if last.mnemonic=='mov' and last.operands[0].type==x86.X86_OP_MEM:
                        dst,src=last.operands
                        require(dst.size==4 and dst.mem.base==x86.X86_REG_ESP and not dst.mem.index
                                and src.type==x86.X86_OP_REG and src.reg==receiver_register,
                                'native receiver spill changed')
                        finish -= 1
                    setup=ins[finish-3:finish]
                    require(len(setup)==3 and [i.mnemonic for i in setup]==['mov','push','mov'],
                            'native register receiver setup changed')
                    table_load,_,load=setup
                    require(load.operands[0].type==x86.X86_OP_REG and load.operands[0].reg==x86.X86_REG_ECX
                            and load.operands[1].type==x86.X86_OP_REG and load.operands[1].reg==receiver_register,
                            'native receiver register move changed')
                    require(table_load.operands[0].type==x86.X86_OP_REG
                            and table_load.operands[0].reg==native.operands[0].mem.base
                            and table_load.operands[1].type==x86.X86_OP_MEM and table_load.operands[1].size==4
                            and table_load.operands[1].mem.base==receiver_register
                            and not table_load.operands[1].mem.index and table_load.operands[1].mem.disp==0
                            and not native.operands[0].mem.index,
                            'native register receiver vtable load changed')
                elif receiver is not None:
                    setup=ins[index-3:index]
                    require(len(setup)==3 and setup[0].mnemonic=='mov' and setup[1].mnemonic=='mov'
                            and setup[2].mnemonic=='push', 'native receiver setup changed')
                    load,table_load=setup[:2]
                    require(load.operands[0].type==x86.X86_OP_REG and load.operands[0].reg==x86.X86_REG_ECX
                            and load.operands[1].type==x86.X86_OP_MEM and load.operands[1].size==4
                            and load.operands[1].mem.base==x86.X86_REG_ESP and not load.operands[1].mem.index
                            and load.operands[1].mem.disp==receiver_offset, 'native receiver stack reload changed')
                    require(table_load.operands[0].type==x86.X86_OP_REG
                            and table_load.operands[0].reg==native.operands[0].mem.base
                            and table_load.operands[1].type==x86.X86_OP_MEM and table_load.operands[1].size==4
                            and table_load.operands[1].mem.base==x86.X86_REG_ECX
                            and not table_load.operands[1].mem.index and table_load.operands[1].mem.disp==0,
                            'native receiver vtable load changed')
                pushed=[]
                for instruction in reversed(ins[max(0,index-8):index]):
                    if instruction.mnemonic=='push':
                        require(instruction.operands[0].type==x86.X86_OP_IMM,
                                'native immediate push changed')
                        pushed.append(instruction.operands[0].imm)
                        if len(pushed)==purge:break
                    require(instruction.mnemonic in ('mov','lea','push'),
                            'native argument setup changed')
                def immediate(value):
                    return {'false':0,'true':1}[value] if value in ('false','true') else int(value,0)
                require(len(pushed)==purge and pushed==[immediate(v) for v in expected],
                        'native immediate arguments changed')
                matching.append(site)
        require(set(matching)==sites and len(matching)==len(sites) and text.count(old)==len(sites), 'printed head/site inventory changed')
    for begin,end,replacement in sorted(receiver_rewrites,reverse=True):
        text=text[:begin]+replacement+text[end:]
    for head in row['heads']:
        if head.get('printedReceiver') is None:
            text=text.replace(head['old'],'GSI->'+row['method'])
    return text
