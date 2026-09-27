"""Separate PlayWave's counter arrays from reused creature/vector stack slots.

Retail 0xF1EED0: GUI IDs occupy ESP+78/+7C/+80 and counts ESP+88/+8C/+90.
The indexed dword accesses and twelve-byte loop bounds establish three entries.
At F205FE the apparent `(int)auStack_f4` is a load of the first ID, not an address.
This recovery is invoked only for that reviewed retail function.
"""
import re


def recover_counter_arrays(text):
    start = re.search(r'^  pCVar15 = this \+ 0xd0;', text, re.M)
    end = re.search(r'^LAB_00f20ce5:', text, re.M)
    if not start or not end or start.start() >= end.start():
        return text
    head, scope, tail = text[:start.start()], text[start.start():end.start()], text[end.start():]
    # The converter has already unified stack-object aliases; standalone native
    # evidence retains Ghidra's original names. Only normalize within this lifetime.
    scope = re.sub(r'\bxStack_(e4|f4)\b', r'auStack_\1', scope)
    names = {'auStack_e4': 'initialCreatureCounts', 'auStack_f4': 'creatureCounterIds'}
    if any(re.search(r'\b' + name + r'\b', text) for name in (*names.values(), 'replacementCounterId')):
        return text
    # Adjacent slots are part of these arrays during this lifetime. Any separately
    # named use of their storage would need additional alias evidence.
    if re.search(r'\b(?:fStack|xStack)_(?:f0|ec|dc)\b', scope):
        return text
    # Require the reviewed initialization and bounds before changing any access.
    anchors = ('*(int *)(auStack_e4 + iVar4) = iVar6;',
               '*(undefined4 *)(auStack_f4 + iVar4) = 0xffffffff;',
               'iVar6 < 0xc', '0xb < iVar4')
    if not all(anchor in scope for anchor in anchors):
        return text
    # The first-ID replacement is a native assigned vcall. Stage its result first,
    # so call lowering still sees a top-level call with all operands intact.
    replacement = re.compile(r'^([ \t]*)auStack_f4 =\s*((?:\(\*\*\(code \*\*\)).*?);', re.M | re.S)
    matches = list(replacement.finditer(scope))
    if len(matches) != 1 or '+ 0x524))' not in matches[0].group(2):
        return text
    scope = replacement.sub(lambda m: f'{m[1]}replacementCounterId = {m[2]};\n'
                            f'{m[1]}ENGINE_SetListWord(creatureCounterIds, 0, replacementCounterId);', scope)
    # This is specifically the RemoveQuestInfoCounterList argument proven by
    # F205FE/F20604/F20605. An arbitrary cast of the array stays unsupported.
    scope = scope.replace('(**(code **)(**(int **)(this + 0x40) + 0x548))(*(int **)(this + 0x40),(int)auStack_f4);',
                          '(**(code **)(**(int **)(this + 0x40) + 0x548))(*(int **)(this + 0x40),ENGINE_ListWord(creatureCounterIds, 0));')
    for slot, name in names.items():
        index = r'(?P<offset>iVar4|iVar6)'
        deref = r'\*\((?:int|undefined4) \*\)\(' + slot + r' \+ ' + index + r'\)'
        scope = re.sub(r'^([ \t]*)' + deref + r' = ([^;]+);',
                       lambda m: f'{m[1]}ENGINE_SetListWord({name}, {m["offset"]} / 4, {"-1" if m[3] == "0xffffffff" else m[3]});', scope, flags=re.M)
        scope = re.sub(deref, lambda m: f'ENGINE_ListWord({name}, {m["offset"]} / 4)', scope)
        field = re.escape(slot) + r'\._0_4_'
        scope = re.sub(r'^([ \t]*)' + field + r' = ([^;]+);',
                       lambda m: f'{m[1]}ENGINE_SetListWord({name}, 0, {m[2]});', scope, flags=re.M)
        scope = re.sub(field, f'ENGINE_ListWord({name}, 0)', scope)
    if any(re.search(r'\b' + slot + r'\b', scope) for slot in names):
        return text
    return head + ('  initialCreatureCounts = ENGINE_EmptyList();\n'
                   '  creatureCounterIds = ENGINE_EmptyList();\n') + scope + tail
