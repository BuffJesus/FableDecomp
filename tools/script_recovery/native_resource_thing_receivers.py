"""Restore reviewed local-resource receivers without renaming neighboring stack objects."""
import hashlib
import re


GET_THING_RECEIVER = re.compile(
    r'(?P<head>\b\w*GetScriptThing\w*\s*\(\s*)'
    r'\(CScriptGameResourceObjectScriptedThingBase \*\)&?[A-Za-z]+Stack_[0-9a-f]+'
)


def restore(text, fn, row, rdata, source_sites):
    """Use pinned native calls to name only their C receiver operands.

    The export can count the register that supplies a hidden-result push twice.
    Broad stack alignment then renames unrelated movie and cleanup locals. A
    reviewed row instead proves the local resource's constructor and every
    GetScriptThing receiver instruction before changing those receiver tokens.
    """
    if row is None:
        return text
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_REG_ECX, X86_REG_ESP

    def require(ok, why):
        if not ok:
            raise ValueError(f'resource thing receiver {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    by_site = {ins.address: i for i, ins in enumerate(instructions)}

    anchor_site = int(row['anchorConstructor'], 16)
    anchor = next((call for call in fn['calls'] if int(call['site'], 16) == anchor_site), None)
    require(anchor and int(anchor['target'], 16) == int(row['anchorTarget'], 16)
            and anchor.get('ecxStack') == row['anchorSlot'], 'resource constructor changed')
    anchor_index = by_site.get(anchor_site)
    require(anchor_index is not None and anchor_index > 0, 'resource constructor missing')
    setup = instructions[anchor_index - 1]
    require(setup.mnemonic == 'lea' and setup.operands[0].type == X86_OP_REG
            and setup.operands[0].reg == X86_REG_ECX and setup.operands[1].type == X86_OP_MEM
            and setup.operands[1].mem.base == X86_REG_ESP and not setup.operands[1].mem.index
            and setup.operands[1].mem.disp + 4 == row['getterReceiverDisplacement'],
            'resource constructor receiver changed')

    target = int(row['getterTarget'], 16)
    calls = sorted((call for call in fn['calls'] if int(call['target'], 16) == target),
                   key=lambda call: int(call['site'], 16))
    wanted = [int(site, 16) for site in row['sites']]
    require(len(wanted) == len(set(wanted)) and
            [int(call['site'], 16) for call in calls] == wanted, 'getter site set changed')
    printed = {int(site['site'], 16): args[0] for _, _, args, site, _, _ in source_sites or []
               if int(site.get('target', '0'), 16) == target and args}
    require(len(printed) == len(wanted) and
            all(printed.get(site) == row['printedReceiver'] for site in wanted),
            'printed receiver changed')
    for site in wanted:
        index = by_site.get(site)
        require(index is not None and index >= 2, 'getter instruction missing')
        push, receiver = instructions[index - 2:index]
        require(push.mnemonic == 'push' and push.operands[0].type == X86_OP_REG
                and receiver.mnemonic == 'lea' and receiver.operands[0].type == X86_OP_REG
                and receiver.operands[0].reg == X86_REG_ECX
                and receiver.operands[1].type == X86_OP_MEM
                and receiver.operands[1].mem.base == X86_REG_ESP
                and not receiver.operands[1].mem.index
                and receiver.operands[1].mem.disp == row['getterReceiverDisplacement'],
                f'getter receiver changed at {site:#x}')

    replacement = f'xStack_{-row["anchorSlot"]:x}'
    require(re.search(r'\b' + re.escape(replacement) + r'\b', text) is not None,
            'resource anchor is absent from transformed source')
    result, count = GET_THING_RECEIVER.subn(
        lambda match: match['head'] +
        f'(CScriptGameResourceObjectScriptedThingBase *)&{replacement}', text)
    require(count == len(wanted), 'transformed getter count changed')
    return result
