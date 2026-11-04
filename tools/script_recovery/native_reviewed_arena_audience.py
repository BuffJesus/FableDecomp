"""Restore AnimateCrowd's copied CScriptThing from its local audience vector.

Retail F1ECD8/F1ECE0 copy Data and Info at cursor +4/+8; F1ECEC
retains Info. EBX starts at zero (F1ECC3) and advances by 12 (F1EE75).
The lowering already strips Info bookkeeping, leaving an empty handle and
one Data store. Replace those together with the corresponding list element.
Keep the random draw, switch, calls and loop control in their native order.
"""
import hashlib

ANIMATE_CROWD = 0x00F1EC70
NATIVE_SHA256 = '77b2fc129b20ba870f143b4e01d72edddda5700d7ab35290fe2c1dd5041df25e'


def recover_arena_audience(source, fn, rdata):
    if int(fn['address'], 16) != ANIMATE_CROWD:
        return source
    raw = rdata.bytes_at(ANIMATE_CROWD, int(fn['bodyEndExclusive'], 16) - ANIMATE_CROWD)
    if raw is None or hashlib.sha256(raw).hexdigest() != NATIVE_SHA256:
        return source
    newline = '\r\n' if '\r\n' in source else '\n'
    text = source.replace('\r\n', '\n')
    old = ('      xStack_18 = QUESTTHING_Empty();\n'
           '      xStack_18._4_4_ = *(undefined4 *)(iVar5 + 4 + (int)xStack_c);\n')
    # Verify the same cursor still traverses the table, not a scalar slot.
    anchors = ('xStack_c = GSI->GetAllThingsWithScriptName(&xStack_34);',
               '    iVar5 = 0;\n', '      iVar5 = iVar5 + 0xc;\n')
    if text.count(old) != 1 or any(text.count(a) != 1 for a in anchors[:1] + anchors[2:]):
        return source
    if text.count(anchors[1]) != 2:
        return source
    text = text.replace(old, '      xStack_18 = LOCALLIST_At(xStack_c, iVar5 / 0xc);\n')
    return text.replace('\n', newline)
