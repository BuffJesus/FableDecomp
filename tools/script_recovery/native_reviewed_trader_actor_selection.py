"""Restore reviewed DarkwoodTrader retained cutscene actors and refusal byte."""
import hashlib
import re

def restore(text, fn, rdata, row):
    if row is None:
        return text
    def require(ok, reason):
        if not ok:
            raise ValueError('reviewed trader actor selection: ' + reason)
    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == int(row['address'], 16) and end == int(row['end'], 16), 'function extent changed')
    raw = rdata.bytes_at(start, end-start)
    require(raw is not None and hashlib.sha256(raw).hexdigest() == row['nativeSha256'], 'native body changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'export source changed')
    for address, encoded in row['anchors'].items():
        address, expected = int(address, 16), bytes.fromhex(encoded)
        require(start <= address and address+len(expected) <= end, 'anchor outside helper')
        require(rdata.bytes_at(address, len(expected)) == expected, 'native anchor changed')
    for address, entry in row['dependencies'].items():
        code = rdata.bytes_at(int(address, 16), entry['length'])
        require(code is not None and hashlib.sha256(code).hexdigest() == entry['sha256'], 'native dependency changed')
    recovered = _recover(text, row['intermediateSha256'])
    require(hashlib.sha256(recovered.encode()).hexdigest() == row['recoveredSha256'], 'recovered source changed')
    return recovered

def _recover(text, expected):
    if hashlib.sha256(text.encode()).hexdigest() != expected:
        raise ValueError("reviewed trader actor selection: source shape changed")
    counts = {}

    def sub(pattern, replacement, label):
        nonlocal text
        text, n = re.subn(pattern, replacement, text)
        if n != 1:
            raise ValueError('reviewed trader actor selection: ' + label + ' count changed')
        counts[label] = n
    sub('(\\(\\*\\*\\(code \\*\\*\\)\\(\\*\\(int \\*\\)xStack_144\\._4_4_ \\+ 0x138\\)\\)\\(xStack_12c\\._4_4_\\))', 'CScriptThing::IsEqualTo((CScriptThing_bv *)xStack_144,(CScriptThing_bv *)xStack_12c)', 'identity')
    sub('iVar8 = CCarriedReadableDef::CCarriedReadableDef__at6e7b40\\(\\(CCarriedReadableDef \\*\\)xStack_c0\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_12c,iVar8\\);\\s*C3DClothPrimitive::~C3DClothPrimitive\\(\\(C3DClothPrimitive \\*\\)xStack_c0,unaff_EDI\\);', 'xStack_12c = QUESTTHING_Empty();', 'clear')
    sub('CScriptThing::CScriptThing\\s*\\(\\(CScriptThing \\*\\)xStack_c0,\\(CScriptThing_bv \\*\\)xStack_144,unaff_EDI\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_144,\\(int\\)xStack_12c\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_12c,\\(int\\)xStack_c0\\);\\s*C3DClothPrimitive::~C3DClothPrimitive\\(\\(C3DClothPrimitive \\*\\)xStack_c0,unaff_EDI\\);', 'native_cutscene_swap = (CScriptThing_bv *)xStack_144;\n xStack_144 = (CScriptThing_bv *)xStack_12c;\n xStack_12c = (CScriptThing_bv *)native_cutscene_swap;', 'swap')
    sub('cStack_169 = bVar3;', 'cStack_169 = 0;', 'refusal_byte_init')
    text = text.replace('  char cVar4;', '  char cVar4;\n  CScriptThing_bv native_cutscene_swap;', 1)
    for slot, name in [('xStack_144', 'local_cutscene_nearest'), ('xStack_12c', 'local_cutscene_furthest')]:
        text = re.sub('\\b' + slot + '\\b', name, text)
        pattern = '(?P<call>\\(\\*\\*\\(code \\*\\*\\)\\(\\*\\*\\(int \\*\\*\\)\\(\\(int\\)this \\+ 4\\) \\+ 0x13[48]\\)\\)\\s*\\([^;]*?\\(CScriptThing_bv \\*\\)' + name + '[^;]*?\\);)'
        sub(pattern, lambda m: name + ' = ' + m['call'], name)
    return text
