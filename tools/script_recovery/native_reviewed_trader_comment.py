"""Recover reviewed Trader Escort comment handle and byte-slot lifetimes.

Retail body/source/intermediate hashes make the transformations fail closed.
The review covers retained speaker/fallback, actor identity, swap/clear, reply
flag and comment snapshot; the converter still lowers the original CFG.
"""
import hashlib
import re

def restore(text, fn, rdata, row):
    if row is None:
        return text
    def require(ok, reason):
        if not ok:
            raise ValueError('reviewed trader comment: ' + reason)
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
    if not hashlib.sha256(text.encode()).hexdigest() == expected:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    start = text.index('  xStack_30._4_4_ = ')
    end = text.index('  bVar2 = ', start)
    text = text[:start] + '  native_conversation_speaker = (CScriptThing_bv *)native_arg_speaker;\n  xStack_30 = (CScriptThing_bv *)native_arg_speaker;\n' + text[end:]
    start = text.index('    piVar13 = *(int **)&pCVar4->field_0x8;')
    condition = text.index('    if (xStack_28 != piVar13)', start)
    opening = text.index('{', condition)
    depth = 1
    end = opening + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    text = text[:start] + '    xStack_30 = (CScriptThing_bv *)pCVar4;\n' + text[end:]
    if not text.count('int *xStack_28;') == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    text, count = re.subn('^[ \\t]*int \\*xStack_28;[ \\t]*\\r?\\n(?:[ \\t]*\\r?\\n)*', '', text, count=1, flags=re.M)
    if not count == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    if not not re.search('\\bxStack_28\\b', text):
        raise ValueError('reviewed trader comment: recovered source shape changed')
    if not text.count('undefined1 xStack_30 [8];') == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    text = text.replace('undefined1 xStack_30 [8];', 'CScriptThing_bv xStack_30;', 1)
    predicates = ['\\(\\*\\(code \\*\\)PTR__IsAlive_CScriptThing__UBE_NXZ_01238db8\\)\\(xStack_30\\)', '\\(\\*\\*\\(code \\*\\*\\)\\(xStack_30\\._0_4_ \\+ (?:300|0x12c)\\)\\)\\(xStack_30\\)']
    for pattern in predicates:
        text, count = re.subn(pattern, 'CScriptThing::IsAlive((CScriptThing_bv *)xStack_30)', text)
        if not count == 1:
            raise ValueError('reviewed trader comment: recovered source shape changed')
    equality = '\\(\\*\\*\\(code \\*\\*\\)\\(\\*\\(int \\*\\)(?P<left>xStack_[0-9a-f]+)\\._4_4_ \\+ 0x138\\)\\)\\((?P<right>xStack_[0-9a-f]+)\\._4_4_\\)'
    text, count = re.subn(equality, lambda m: 'CScriptThing::IsEqualTo((CScriptThing_bv *)' + m['left'] + ', (CScriptThing_bv *)' + m['right'] + ')', text)
    if not count == 2:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    replacements = {'native_arg_speaker = (CScriptThing_bv *)((uint)native_arg_speaker & 0xffffff00);': 'native_reply_available = 0;', 'native_arg_speaker = (CScriptThing_bv *)CONCAT31(native_arg_speaker._1_3_,1);': 'native_reply_available = 1;', "if ((char)native_arg_speaker != '\\0')": 'if (native_reply_available != 0)', 'native_arg_comment_to_make = (CCharString_bv *)CONCAT31(native_arg_comment_to_make._1_3_,cVar3);': 'native_rock_troll_match = cVar3;'}
    for old, new in replacements.items():
        if not text.count(old) == 1:
            raise ValueError('reviewed trader comment: recovered source shape changed')
        text = text.replace(old, new, 1)
    declaration = '  char cVar3;'
    if not text.count(declaration) == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    text = text.replace(declaration, declaration + '\n  char native_reply_available;\n  char native_rock_troll_match;', 1)
    swap = 'CScriptThing::CScriptThing\\(\\(CScriptThing \\*\\)&xStack_c,\\(CScriptThing_bv \\*\\)xStack_24,unaff_EBX\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_24,\\(int\\)xStack_18\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_18,\\(int\\)&xStack_c\\);\\s*C3DClothPrimitive::~C3DClothPrimitive\\(\\(C3DClothPrimitive \\*\\)&xStack_c,unaff_EBX\\);'
    text, count = re.subn(swap, 'native_swap_handle = (CScriptThing_bv *)xStack_24;\n      xStack_24 = (CScriptThing_bv *)xStack_18;\n      xStack_18 = (CScriptThing_bv *)native_swap_handle;', text)
    if not count == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    empty = 'iVar5 = CCarriedReadableDef::CCarriedReadableDef\\(\\(CCarriedReadableDef \\*\\)&xStack_c\\);\\s*CScriptThing::operator=\\(\\(CScriptThing \\*\\)xStack_18,iVar5\\);\\s*C3DClothPrimitive::~C3DClothPrimitive\\(\\(C3DClothPrimitive \\*\\)&xStack_c,unaff_EBX\\);'
    text, count = re.subn(empty, 'xStack_18 = QUESTTHING_Empty();', text)
    if not count == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    text = text.replace('  char native_reply_available;', '  CScriptThing_bv native_swap_handle;\n  CScriptThing_bv native_conversation_speaker;\n  char native_reply_available;', 1)
    old = '(*(int **)(this + 0x40),native_arg_speaker,false,false);'
    if not text.count(old) == 1:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    text = text.replace(old, '(*(int **)(this + 0x40),native_conversation_speaker,false,false);', 1)
    for slot, name in [('xStack_24', 'local_nearest_handle'), ('xStack_18', 'local_furthest_handle')]:
        declaration = f'undefined1 {slot} [12];'
        if not text.count(declaration) == 1:
            raise ValueError('reviewed trader comment: recovered source shape changed')
        text = text.replace(declaration, f'CScriptThing_bv {slot};', 1)
        text = re.sub('\\b' + slot + '\\b', name, text)
    pattern = '(?P<call>\\(\\*\\*\\(code \\*\\*\\)\\(\\*\\*\\(int \\*\\*\\)\\(this \\+ 0x40\\) \\+ 0x13[48]\\)\\)\\s*\\([^;]*?\\(CScriptThing_bv \\*\\)(?P<slot>local_(?:nearest|furthest)_handle)[^;]*?\\);)'
    text, count = re.subn(pattern, lambda m: m['slot'] + ' = ' + m['call'], text)
    if not count == 3:
        raise ValueError('reviewed trader comment: recovered source shape changed')
    return text
