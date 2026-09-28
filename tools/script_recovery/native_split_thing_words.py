"""A stack CScriptThing whose Data word Ghidra split into its own local.

V_BookCollecting BS_Teacher 0x00E56D10 fills `auStack_18` (a CScriptThing: vtable at -0x18, Data at -0x14, Info at
-0x10) with GetThingWithScriptName and then loops `while (uStack_14 != (int *)0x0) { cVar3 = (**(code **)(*uStack_14 +
300))(); ...}` -- IsAlive through the Data pointer, with the Data word printed as the separate `uStack_14`. Lifted raw
the call was unresolved and the loop condition a constant nil.

When `undefined1 X [4|8|12];` is used as a CScriptThing and the local named four bytes above it is only stored, compared
with null or dereferenced as a vtable base, that local is `X._4_4_`; the Data-pointer rules downstream then read the
validity tests and the calls on X.
"""
import re


def _offset(name):
    m = re.search(r'(?:local_|Stack_)([0-9a-f]+)$', name)
    return int(m.group(1), 16) if m else None


def merge_split_thing_words(text):
    for decl in re.finditer(r'^[ \t]*undefined1 (\w+) \[(?:4|8|12)\];[ \t]*\r?$', text, re.M):
        obj = decl.group(1)
        base = _offset(obj)
        if base is None or not re.search(r'\(CScriptThing(?:_bv)? \*\)&?' + re.escape(obj) + r'\b', text):
            continue
        # (the whole line with its newline: an empty line would end the lifter's declaration block early)
        for dm in re.finditer(r'^[ \t]*(?:undefined4|int \*|uint) (\w+);[ \t]*\r?\n', text, re.M):
            word = dm.group(1)
            if _offset(word) != base - 4:
                continue
            w = re.escape(word)
            body = text.replace(dm.group(0), '')
            allowed = [r'^[ \t]*' + w + r' = [^;]+;',                                   # a store
                       r'\(?(?:\(int \*\))?' + w + r' [!=]= \(int \*\)0x0\)?',            # a null test
                       r'\(\*\*\(code \*\*\)\(\*' + w + r' \+ (?:0x[0-9a-f]+|\d+)\)\)']    # a vtable base
            rest = body
            for a in allowed:
                rest = re.sub(a, '', rest, flags=re.M)
            if re.search(r'\b' + w + r'\b', rest):
                continue
            text = text.replace(dm.group(0), '')
            text = re.sub(r'\b' + w + r'\b', obj + '._4_4_', text)
            text = re.sub(r'\(\*\*\(code \*\*\)\(\*' + re.escape(obj + '._4_4_') + r' \+', f'(**(code **)(*(int *){obj}._4_4_ +', text)
            break
    return text
