"""Recover Madame's retail beer presentation comparison."""
import hashlib
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_REG_EDX


ROOT = Path(__file__).resolve().parents[2]
BEFORE = '''        bVar3 = me:MsgIsPresentedWithItem()
        if bVar3 then xStack_178 = _G.g_PresentedItemName end
        if bVar3 then
            if xStack_178 == nil then
                bVar3 = false
                if bVar3 then
                    goto LAB_00e3bf28
                end
            else
                iVar5 = ((xStack_120 == "OBJECT_BEER_TANKARD") and 0 or 1)
                -- TODO(native): xStack_124_b3 = !(iVar5 != 0);
                if xStack_124_b3 ~= 0 then goto LAB_00e3bf28 end
            end
            goto FLOW_past_lab_00e3bf28
            ::LAB_00e3bf28::'''
AFTER = '''        g_PresentedItemName = nil
        bVar3 = me:MsgIsPresentedWithItem()
        if bVar3 then xStack_178 = g_PresentedItemName end
        if bVar3 then
            if xStack_178 == nil then
                bVar3 = false
                if bVar3 then
                    goto LAB_00e3bf28
                end
            else
                xStack_120 = xStack_178
                iVar5 = ((xStack_120 == "OBJECT_BEER_TANKARD") and 0 or 1)
                xStack_124_b3 = (iVar5 == 0)
                if xStack_124_b3 then goto LAB_00e3bf28 end
            end
            goto FLOW_past_lab_00e3bf28
            ::LAB_00e3bf28::'''
DIAGNOSTIC = 'unlifted: xStack_124_b3 = !(iVar5 != 0);'


def restore(source, todo, fn, row, rdata):
    if row is None:
        return source, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed Madame presented item {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    sidecar = (ROOT / row['sidecarSource']).read_bytes()
    require(hashlib.sha256(sidecar).hexdigest() == row['sidecarSha256']
            and b'bool LuaEntityAPI::MsgIsPresentedWithItem' in sidecar
            and b'lua.set("g_PresentedItemName", std::string(text))' in sidecar
            and b'if (text)' in sidecar,
            'sidecar item-name ABI changed')
    require(rdata.string_at(int(row['itemNameAddress'], 16)) == row['itemName']
            and row['itemName'] == 'OBJECT_BEER_TANKARD', 'item name changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    poll = next((call for call in fn['indirectCalls'] if call['site'].lower() == '0x00e3bedc'), None)
    compare = next((call for call in fn['calls'] if call['site'].lower() == '0x00e3bf13'), None)
    require(poll is not None and poll['slot'] == '0x8c'
            and compare is not None and compare['target'].lower() == '0x00411570',
            'native poll or comparison changed')
    cmp_ins = indexed[0xE3BF13]
    branch = indexed[0xE3BF22]
    require(cmp_ins.mnemonic == 'call' and cmp_ins.operands[0].type == X86_OP_IMM
            and cmp_ins.operands[0].imm == 0x411570
            and branch.mnemonic == 'je' and branch.operands[0].type == X86_OP_IMM
            and branch.operands[0].imm == 0xE3C0B9,
            'comparison branch changed')
    item_load = indexed[0xE3BF0E]
    require(item_load.mnemonic == 'mov' and item_load.operands[1].type == X86_OP_IMM
            and item_load.operands[1].imm == int(row['itemNameAddress'], 16)
            and item_load.operands[0].reg == X86_REG_EDX,
            'comparison item operand changed')
    # Once the receiver reaching walk skips alignment filler (2026-10-02) the presented-item
    # out-parameter resolves to its true slot, the empty CCharString at -0x17c that
    # MsgIsPresentedWithItem fills; earlier exports printed the neighbouring -0x178.
    before, after = BEFORE, AFTER
    paired = '        xStack_17c = ""\n' + BEFORE.replace('xStack_178', 'xStack_17c')
    if source.count(paired) == 1 and 'xStack_178' not in source:
        before, after = paired, '        xStack_17c = ""\n' + AFTER.replace('xStack_178', 'xStack_17c')
    require(source.count(before) == 1 and todo.count(DIAGNOSTIC) == 1,
            'lifted comparison changed')
    source = source.replace(before, after, 1)
    todo = [entry for entry in todo if entry != DIAGNOSTIC]
    return source, todo
