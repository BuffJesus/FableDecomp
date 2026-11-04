"""Rejoin reviewed movie object members split by Ghidra stack depth drift."""
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET


def restore(text, fn, row, rdata):
    if row is None:
        return text

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed movie members {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    source = fn['decompile']
    require(hashlib.sha256(source.encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    by_site = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = by_site.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')

    indirect = {c['site'].lower(): c for c in fn['indirectCalls']}
    direct = {c['site'].lower(): c for c in fn['calls']}
    if 'sharedBaseDestructors' in row:
        from tools.script_recovery.native_movie_phi_cleanup import fold
        pairs = row['sharedBaseDestructors']
        require(bool(pairs), 'empty shared destructor evidence')
        witnessed = {int(address, 16) for address in row['nativeInstructions']}
        total = 0
        for pair in pairs:
            obj, member = pair['object'], pair['member']
            require(re.fullmatch(r'(?:[A-Za-z]+Stack_|local_)[0-9a-f]+', obj)
                    and re.fullmatch(r'(?:[A-Za-z]+Stack_|local_)[0-9a-f]+', member)
                    and int(obj.rsplit('_', 1)[1], 16) - int(member.rsplit('_', 1)[1], 16) == 8,
                    'shared movie member offset changed')
            require(pair['startSites'] and pair['releaseSites'], 'shared movie lifetime is incomplete')
            for site in pair['startSites']:
                call = indirect.get(site.lower())
                require(int(site, 16) in witnessed and call is not None
                        and call.get('slot') == '0x5c8' and call['depth'] == pair['frameBytes'] + 8
                        and call['pushedStack'][0] == pair['entrySlot'],
                        'shared movie start stack changed')
            for site in pair['releaseSites']:
                call = direct.get(site.lower())
                require(int(site, 16) in witnessed and call is not None
                        and int(call['target'], 16) == 0x6e7ab0
                        and call['depth'] == pair['frameBytes']
                        and call['ecxStack'] == pair['entrySlot'] + 8,
                        'shared movie release stack changed')
            total += len(pair['releaseSites'])
        require(row['baseDestructorSites'], 'missing base destructor joins')
        for site in row['baseDestructorSites']:
            call = direct.get(site.lower())
            require(int(site, 16) in witnessed and call is not None
                    and int(call['target'], 16) == 0x99a430, 'shared base destructor changed')
        labels = {}
        for call in fn['calls']:
            if call.get('currentName'):
                labels.setdefault(call['currentName'], set()).add(int(call['target'], 16))
        result = fold(text, {name: next(iter(targets)) for name, targets in labels.items() if len(targets) == 1})
        base_labels = [name for name, targets in labels.items() if targets == {0x99a430}]
        removed_bases = sum(len(re.findall(re.escape(name) + r'\s*\(', text))
                            - len(re.findall(re.escape(name) + r'\s*\(', result)) for name in base_labels)
        require(removed_bases == len(row['baseDestructorSites']), 'shared base call inventory changed')
        require(result.count('RESOURCE_DestroyMovie(') - text.count('RESOURCE_DestroyMovie(') == total,
                'shared destructor source inventory changed')
        for pair in pairs:
            call = 'RESOURCE_DestroyMovie(' + pair['object'] + ');'
            require(result.count(call) - text.count(call) == len(pair['releaseSites']),
                    'shared destructor object inventory changed')
        return result
    if 'stackReceiverAliases' in row:
        from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP
        aliases = row['stackReceiverAliases']
        require(bool(aliases), 'empty receiver aliases')
        cast = '(CScriptGameResourceObjectMovieBase *)'
        edits = []
        for alias in aliases:
            old, new = alias['old'], alias['new']
            require(all(re.fullmatch(r'[A-Za-z_]\w*', name) for name in (old, new))
                    and old != new, 'invalid movie receiver alias')
            require(alias['entrySlot'] < 0 and alias['frameBytes'] > 0,
                    'invalid movie stack location')
            require(len(alias['receiverLeas']) >= 2, 'missing movie receiver witnesses')
            for address in alias['receiverLeas']:
                ins = by_site.get(int(address, 16))
                require(address in row['nativeInstructions'] and ins is not None
                        and ins.mnemonic == 'lea' and len(ins.operands) == 2,
                        'movie receiver LEA missing')
                dst, src = ins.operands
                require(dst.type == X86_OP_REG and dst.reg == X86_REG_ECX
                        and src.type == X86_OP_MEM and src.mem.base == X86_REG_ESP
                        and not src.mem.index
                        and src.mem.disp - alias['frameBytes'] == alias['entrySlot'],
                        'movie receiver stack identity changed')
            constructor = direct.get(alias['constructorSite'].lower())
            require(constructor is not None and constructor['target'].lower() == alias['constructorTarget']
                    and alias['constructorTarget'] in ('0x006e7b60', '0x0099a380'),
                    'movie constructor identity changed')
            require(alias['constructorSite'] in row['nativeInstructions'],
                    'movie constructor lacks witness')
            ctor_address = int(alias['constructorSite'], 16)
            ctor_index = next(i for i, ins in enumerate(instructions) if ins.address == ctor_address)
            receiver_found = False
            # An independent local flag store may sit between LEA and call.
            # Require every intervening instruction to be pinned and preserve
            # ECX/ESP without crossing a call or control-flow instruction.
            receiver_sites = {int(address, 16) for address in alias['receiverLeas']}
            for ins in reversed(instructions[max(0, ctor_index-5):ctor_index]):
                if ctor_address-ins.address > 16:
                    break
                if ins.address in receiver_sites:
                    receiver_found = True
                    break
                if (f'0x{ins.address:08X}' not in row['nativeInstructions']
                        or any(ins.group(g) for g in (CS_GRP_JUMP, CS_GRP_CALL, CS_GRP_RET))
                        or any(r in ins.regs_access()[1] for r in (X86_REG_ECX, X86_REG_ESP))):
                    break
            require(receiver_found, 'movie constructor receiver setup changed')
            if 'constructorReceiver' in alias:
                # A semantic handle keeps this native object separate from a
                # neighboring Thing/string spelling without renaming its uses.
                receiver = alias['constructorReceiver']
                require(re.fullmatch(r'[A-Za-z_]\w*', receiver) is not None
                        and receiver != new and new not in text,
                        'invalid distinct movie constructor handle')
                name = constructor.get('currentName', '')
                require(bool(name), 'movie constructor source name missing')
                pattern = re.compile(re.escape(name) + r'\s*\(\s*'
                    r'(?:\([\w: ]+\s*\*\)\s*)?&?(?P<receiver>'
                    + re.escape(receiver) + r')\s*\);')
                matches = list(pattern.finditer(text))
                require(len(matches) == 1, 'movie constructor source inventory changed')
                match = matches[0]
                edits.append((match.start('receiver'), match.end('receiver'), new))
                start_row = alias.get('startOperands')
                require(isinstance(start_row, dict), 'distinct movie start evidence missing')
                site = int(start_row['site'], 16)
                call = by_site.get(site)
                metadata = indirect.get(start_row['site'].lower())
                require(start_row['site'] in row['nativeInstructions'] and call is not None
                        and call.mnemonic == 'call' and call.operands[0].type == X86_OP_MEM
                        and call.operands[0].mem.disp == 0x5c8 and metadata is not None
                        and metadata.get('slot') == '0x5c8'
                        and metadata.get('depth') == alias['frameBytes'] + 8
                        and metadata.get('pushedStack') == [alias['entrySlot'], start_row['stringEntrySlot']],
                        'movie start native argument inventory changed')
                require(ctor_address < site and site - ctor_address <= 64,
                        'movie start not adjacent to construction')
                for role, delta, expected_slot in (
                        ('outputLea', -alias['frameBytes'], alias['entrySlot']),
                        ('inputLea', -alias['frameBytes'] - 4, start_row['stringEntrySlot'])):
                    at = int(start_row[role], 16)
                    lea = by_site.get(at)
                    require(start_row[role] in row['nativeInstructions'] and lea is not None
                            and lea.mnemonic == 'lea' and len(lea.operands) == 2,
                            'movie start argument LEA missing')
                    dst, src = lea.operands
                    require(dst.type == X86_OP_REG and src.type == X86_OP_MEM
                            and src.mem.base == X86_REG_ESP and not src.mem.index
                            and src.mem.disp + delta == expected_slot,
                            'movie start argument stack identity changed')
                    push = by_site.get(lea.address + lea.size)
                    require(push is not None and f'0x{push.address:08X}' in row['nativeInstructions']
                            and push.mnemonic == 'push' and push.operands[0].type == X86_OP_REG
                            and push.operands[0].reg == dst.reg and lea.address < site,
                            'movie start argument push changed')
                table = int(start_row['table'], 16)
                require(rdata.bytes_at(table + 0x5c8, 4) == bytes.fromhex(start_row['targetBytes']),
                        'movie start table target changed')
                from tools.script_recovery.convert_quest_unit import _text_order_sites
                entries = [entry for entry in (_text_order_sites(text, fn) or [])
                           if int(entry[3]['site'], 16) == site]
                require(len(entries) == 1 and entries[0][4] == '0x5c8'
                        and entries[0][2] == start_row['sourceArgs'],
                        'movie start printed operands changed')
                first, last = entries[0][:2]
                printed_receiver = start_row.get('printedInterfaceReceiver')
                if printed_receiver is not None:
                    require(printed_receiver == '*(int **)((int)this + 4)'
                            and len(start_row['sourceArgs']) == 3
                            and start_row['sourceArgs'][0] == printed_receiver,
                            'movie printed interface receiver changed')
                    owner_setup = [ins for ins in instructions[:12] if ins.mnemonic == 'mov'
                        and ins.operands[0].type == X86_OP_REG and ins.operands[1].type == X86_OP_REG
                        and ins.operands[1].reg == X86_REG_ECX
                        and ins.reg_name(ins.operands[0].reg) in ('esi','edi','ebx','ebp')]
                    require(len(owner_setup) == 1, 'movie interface owner changed')
                    load = by_site.get(int(start_row['receiverLoad'],16))
                    table_load = by_site.get(int(start_row['tableLoad'],16))
                    require(start_row['receiverLoad'] in row['nativeInstructions']
                            and start_row['tableLoad'] in row['nativeInstructions']
                            and load is not None and table_load is not None
                            and load.mnemonic == 'mov' and len(load.operands) == 2
                            and load.operands[0].type == X86_OP_REG and load.operands[0].reg == X86_REG_ECX
                            and load.operands[1].type == X86_OP_MEM and load.operands[1].size == 4
                            and load.operands[1].mem.base == owner_setup[0].operands[0].reg
                            and not load.operands[1].mem.index and load.operands[1].mem.disp == 4,
                            'movie interface receiver load changed')
                    require(table_load.mnemonic == 'mov' and len(table_load.operands) == 2
                            and table_load.operands[0].type == X86_OP_REG
                            and table_load.operands[0].reg == call.operands[0].mem.base
                            and table_load.operands[1].type == X86_OP_MEM and table_load.operands[1].size == 4
                            and table_load.operands[1].mem.base == X86_REG_ECX
                            and not table_load.operands[1].mem.index and table_load.operands[1].mem.disp == 0
                            and load.address + load.size == table_load.address,
                            'movie interface vtable load changed')
                    setup = [ins for ins in instructions if table_load.address < ins.address < site]
                    require(len(setup) == 4 and [ins.mnemonic for ins in setup] == ['lea','push','lea','push']
                            and all(X86_REG_ECX not in ins.regs_access()[1]
                                    and table_load.operands[0].reg not in ins.regs_access()[1] for ins in setup),
                            'movie interface receiver not preserved')
                    operands = start_row['sourceArgs'][:2] + [new]
                else:
                    require(len(start_row['sourceArgs']) in (1, 2), 'invalid movie start printed argument count')
                    operands = [start_row['sourceArgs'][0], new]
                edits.append((first, last, ','.join(operands)))
            inline = alias.get('inlineConstructor')
            if inline is not None:
                store = by_site.get(int(inline['store'],16))
                require(alias['constructorTarget'] == '0x0099a380' and store is not None
                        and inline['store'] in row['nativeInstructions'] and store.mnemonic == 'mov'
                        and store.operands[0].type == X86_OP_MEM and store.operands[0].size == 4
                        and store.operands[0].mem.base == X86_REG_ESP and not store.operands[0].mem.index
                        and store.operands[1].type == X86_OP_IMM and store.operands[1].imm == 0x01260ef4,
                        'inline movie vtable store changed')
                setup = [ins for ins in instructions if ctor_address < ins.address < store.address]
                zero_sites = inline.get('zeroSites', [])
                zero_registers = inline.get('zeroRegisters', [])
                require(isinstance(zero_sites,list) and len(zero_sites)==len(set(zero_sites))
                        and isinstance(zero_registers,list) and len(zero_registers)==len(zero_sites), 'invalid inline zero inventory')
                zeros = [ins for ins in setup if ins.mnemonic=='xor']
                require([f'0x{ins.address:08X}' for ins in zeros]==zero_sites
                        and [ins.reg_name(ins.operands[0].reg) for ins in zeros]==zero_registers
                        and all(f'0x{ins.address:08X}' in row['nativeInstructions']
                                and len(ins.operands)==2 and ins.operands[0].type==X86_OP_REG
                                and ins.operands[1].type==X86_OP_REG and ins.operands[0].reg==ins.operands[1].reg
                                and ins.operands[0].reg not in (X86_REG_ECX,X86_REG_ESP) for ins in zeros),
                        'inline constructor zero register changed')
                pushes = [ins for ins in setup if ins.mnemonic == 'push']
                require(len(pushes) == 2 and [f'0x{ins.address:08X}' for ins in pushes] == inline['pendingPushes']
                        and all(ins.operands[0].type == X86_OP_IMM for ins in pushes)
                        and [ins.operands[0].imm for ins in pushes] == [-1,0x0122d70e]
                        and all(ins.mnemonic in ('push','lea','xor') for ins in setup)
                        and store.operands[0].mem.disp - 8 - alias['frameBytes'] == alias['entrySlot'],
                        'inline movie pending string arguments changed')
                array_store = inline.get('sourceIndexZero',False)
                require(isinstance(array_store,bool),'invalid inline array store policy')
                old_store = inline['source']
                require(old_store == alias['constructorReceiver'] + ('[0]' if array_store else '') + ' = &PTR__scalar_deleting_destructor__01260ef4;'
                        and text.count(old_store) == 1, 'inline movie source vtable changed')
                at = text.index(old_store); edits.append((at,at+len(old_store),new+' = &PTR__scalar_deleting_destructor__01260ef4;'))
            begin, end = 0, len(text)
            if 'scopeStart' in alias:
                marker = alias['scopeStart']
                require(bool(marker) and text.count(marker) == 1, 'movie scope start changed')
                begin = text.index(marker) + len(marker)
            if 'scopeEnd' in alias:
                marker = alias['scopeEnd']
                require(bool(marker) and text.count(marker) == 1, 'movie scope end changed')
                end = text.index(marker)
            require(begin < end, 'movie scope order changed')
            address_of = alias.get('allowAddressOf', False)
            require(isinstance(address_of, bool), 'invalid address-of receiver policy')
            pattern = re.compile(re.escape(cast) + (r'&?' if address_of else '') + re.escape(old) + r'\b')
            matches = list(pattern.finditer(text, begin, end))
            require(alias['count'] > 0 and len(matches) == alias['count'],
                    'movie receiver source inventory changed')
            edits.extend((m.start(), m.end(), cast + new) for m in matches)
            inline = alias.get('inlineBaseCleanups')
            if inline is not None:
                require(alias.get('inlineConstructor') and inline['oldObject'] == old
                        and len(inline['sites']) == 2, 'inline movie cleanup inventory changed')
                for cleanup in inline['sites']:
                    member, release, obj, store, base = [by_site.get(int(cleanup[k],16)) for k in ('memberLea','release','objectLea','store','base')]
                    require(all(ins is not None and f'0x{ins.address:08X}' in row['nativeInstructions']
                                for ins in (member,release,obj,store,base)), 'inline cleanup witness missing')
                    for lea, delta in ((member,8),(obj,0)):
                        require(lea.mnemonic == 'lea' and lea.operands[0].type == X86_OP_REG
                                and lea.operands[0].reg == X86_REG_ECX and lea.operands[1].type == X86_OP_MEM
                                and lea.operands[1].mem.base == X86_REG_ESP and not lea.operands[1].mem.index
                                and lea.operands[1].mem.disp - alias['frameBytes'] == alias['entrySlot'] + delta,
                                'inline cleanup stack identity changed')
                    require(release.mnemonic == 'call' and release.operands[0].type == X86_OP_IMM
                            and release.operands[0].imm == 0x6e7ab0 and member.address + member.size == release.address
                            and base.mnemonic == 'call' and base.operands[0].type == X86_OP_IMM
                            and base.operands[0].imm == 0x99a430 and obj.address == release.address + release.size
                            and obj.address + obj.size == store.address and store.address + store.size == base.address,
                            'inline cleanup helper order changed')
                    require(store.mnemonic == 'mov' and store.operands[0].type == X86_OP_MEM
                            and store.operands[0].size == 4 and store.operands[0].mem.base == X86_REG_ESP
                            and not store.operands[0].mem.index
                            and store.operands[0].mem.disp - alias['frameBytes'] == alias['entrySlot']
                            and store.operands[1].type == X86_OP_IMM and store.operands[1].imm == 0x126008c,
                            'inline cleanup base vtable changed')
                array_store = inline.get('sourceIndexZero',False)
                addressed = inline.get('sourceAddressOf',True)
                require(isinstance(array_store,bool) and isinstance(addressed,bool),'invalid inline cleanup spelling policy')
                require(inline['sourceVtable'] == old + ('[0]' if array_store else '') + ' = &PTR__vector_deleting_destructor__0126008c;'
                        and inline['sourceCast'] == '(CPhysicsMeshInfo *)'+('&' if addressed else '')+old,
                        'inline cleanup source spelling changed')
                for spelling, replacement in ((inline['sourceVtable'],new+' = &PTR__vector_deleting_destructor__0126008c;'),
                                              (inline['sourceCast'],'(CPhysicsMeshInfo *)'+new)):
                    matches = list(re.finditer(re.escape(spelling),text[begin:end]))
                    require(len(matches) == len(inline['sites']), 'inline cleanup source inventory changed')
                    edits.extend((begin+m.start(),begin+m.end(),replacement) for m in matches)
            spellings = alias.get('additionalReceiverSpellings', [])
            require(isinstance(spellings, list), 'invalid additional movie receivers')
            seen = {old}
            for spelling in spellings:
                name, count = spelling['name'], spelling['count']
                require(re.fullmatch(r'[A-Za-z_]\w*', name) is not None
                        and name not in seen and name != new
                        and isinstance(count, int) and not isinstance(count, bool) and count > 0,
                        'invalid additional movie receiver spelling')
                seen.add(name)
                matches = list(re.compile(re.escape(cast + name) + r'\b').finditer(text, begin, end))
                require(len(matches) == count, 'additional movie receiver inventory changed')
                edits.extend((m.start(), m.end(), cast + new) for m in matches)
        edits.sort()
        require(all(a[1] <= b[0] for a, b in zip(edits, edits[1:])), 'overlapping movie aliases')
        for begin, end, replacement in reversed(edits):
            text = text[:begin] + replacement + text[end:]
        # Preserve the native ECX join's three stores. Leaving the C++ destructor
        # type in these assignments makes the generic noise filter discard them.
        for alias in aliases:
            if not alias.get('phiCount'):
                continue
            name = alias['phiVariable']
            require(re.fullmatch(r'[A-Za-z_]\w*', name) is not None,
                    'invalid movie phi variable')
            old = f'{name} = {cast}{alias["new"]};'
            require(text.count(old) == alias['phiCount'], 'movie phi source inventory changed')
            text = text.replace(old, f'{name} = {alias["new"]};')
        return text
    require(len(row['pairs']) == 2, 'pair coverage changed')
    for pair in row['pairs']:
        obj, member = pair['rawObject'], pair['rawMember']
        obj_off = re.search(r'_([0-9a-f]+)$', obj)
        member_off = re.search(r'_([0-9a-f]+)$', member)
        require(obj_off is not None and member_off is not None
                and int(obj_off.group(1), 16) - int(member_off.group(1), 16) == 8,
                'raw object/member layout changed')
        require(source.count(obj + '[0] = &PTR__scalar_deleting_destructor__01260ef4;') == 1
                and source.count(member + ' = 0;') == 1
                and source.count('(&' + member + ');') >= 1
                and source.count(obj + '[0] = &PTR__vector_deleting_destructor__0126008c;') >= 1,
                'raw movie construction or destruction changed')
        movie_start = indirect.get(pair['startSite'].lower())
        movie_end = direct.get(pair['cleanupSite'].lower())
        require(movie_start is not None and movie_start.get('slot') == '0x5c8'
                and movie_start['pushedStack'][0] == pair['startStack']
                and movie_end is not None and movie_end['target'].lower() == '0x006e7ab0'
                and movie_end['ecxStack'] == pair['cleanupStack'],
                'native movie call operands changed')
        marker = pair['startMarker']
        require(text.count(marker) == 1, 'restored movie constructor changed')
        begin = text.index(marker)
        stop = text.index(pair['endMarker'], begin) if 'endMarker' in pair else len(text)
        block = text[begin:stop]
        require([block.count(name) for name in pair['restoredMembers']] == pair['memberCounts'],
                'restored movie member uses changed')
        for name in pair['restoredMembers']:
            block = re.sub(r'\b' + re.escape(name) + r'\b', pair['restoredObject'], block)
        text = text[:begin] + block + text[stop:]
    return text
