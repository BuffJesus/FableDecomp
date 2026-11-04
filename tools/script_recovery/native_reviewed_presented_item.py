"""Recover two reviewed Magicman item comparisons through the sidecar ABI."""
import hashlib
from pathlib import Path
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = Path(__file__).resolve().parents[2]

POLL = re.compile(
    r'^(?P<ind> *)cVar6 = me:MsgIsPresentedWithItem\(\)\n'
    r'(?P=ind)if cVar6 then\n'
    r'(?P=ind)    if unaff_EBP == nil then\n'
    r'(?P=ind)        bVar5 = false\n'
    r'(?P=ind)        if bVar5 then\n'
    r'(?P=ind)            goto (?P<label>LAB_00e[0-9a-f]+)\n'
    r'(?P=ind)        end\n'
    r'(?P=ind)    else\n'
    r'(?P=ind)        -- TODO\(native\): iVar21 = CBasicString<char>::Compare\(\(void \*\)\*unaff_EBP,"OBJECT_BEER_TANKARD"\);\n'
    r'(?P=ind)        if iVar21 == 0 then goto (?P=label) end\n'
    r'(?P=ind)    end\n'
    r'(?P=ind)    goto (?P<flow>FLOW_past_lab_00e[0-9a-f]+)\n'
    r'(?P=ind)    ::(?P=label)::', re.M)


def restore(body, todo, fn, row, rdata):
    if row is None:
        return body, todo

    def require(ok, why):
        if not ok:
            raise ValueError(f'reviewed presented item {fn["address"]}: {why}')

    start = int(fn['address'], 16)
    size = int(fn['bodyEndExclusive'], 16) - start
    raw = rdata.bytes_at(start, size)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'],
            'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'],
            'exported source changed')
    source_path = ROOT / row['sidecarSource']
    sidecar = source_path.read_bytes()
    require(hashlib.sha256(sidecar).hexdigest() == row['sidecarSha256']
            and b'bool LuaEntityAPI::MsgIsPresentedWithItem' in sidecar
            and b'lua.set("g_PresentedItemName", std::string(text))' in sidecar
            and b'if (text)' in sidecar,
            'sidecar presented-item ABI changed')
    require(rdata.string_at(int(row['itemNameAddress'], 16)) == row['itemName'],
            'native item name changed')
    instructions = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(raw, start))
    require(instructions and instructions[-1].address + instructions[-1].size == start + size,
            'native disassembly incomplete')
    indexed = {ins.address: ins for ins in instructions}
    for site, encoded in row['nativeInstructions'].items():
        ins = indexed.get(int(site, 16))
        require(ins is not None and ins.bytes.hex() == encoded,
                f'native instruction changed at {site}')
    indirect = {c['site'].lower(): c for c in fn['indirectCalls']}
    direct = {c['site'].lower(): c for c in fn['calls']}
    require(len(row['sites']) == 2 and fn['decompile'].count('CBasicString<char>::Compare') >= 2,
            'native comparison coverage changed')
    for site in row['sites']:
        poll = indirect.get(site['poll'].lower())
        compare = direct.get(site['compare'].lower())
        require(poll is not None and poll.get('slot') == '0x8c'
                and compare is not None and compare['target'].lower() == '0x00411570',
                'native presented-item call changed')

    text = '\n'.join(body)
    matches = list(POLL.finditer(text))
    require(len(matches) == 2 and [m.group('label') for m in matches]
            == [site['label'] for site in row['sites']]
            and all(m.group('flow') == 'FLOW_past_' + m.group('label').lower()
                    for m in matches),
            'lifted item comparison changed')

    def replace(match):
        indent = match.group('ind')
        label = match.group('label')
        return ('\n'.join([
            indent + 'g_PresentedItemName = nil',
            indent + 'cVar6 = me:MsgIsPresentedWithItem()',
            indent + 'if cVar6 then',
            indent + f'    if g_PresentedItemName == "{row["itemName"]}" then goto {label} end',
            indent + f'    goto FLOW_past_{label.lower()}',
            indent + f'    ::{label}::',
        ]))

    rewritten = POLL.sub(replace, text)
    diagnostic = 'unlifted: iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");'
    require(todo.count(diagnostic) == 2, 'comparison diagnostics changed')
    todo = [entry for entry in todo if entry != diagnostic]
    return rewritten.split('\n'), todo
