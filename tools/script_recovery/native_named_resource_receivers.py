"""Preserve explicitly acquired named-actor resources in entity methods."""
import re

from tools.script_recovery.native_evidence_lowering import _split_top, _strip_addr


def named_resources(source: str) -> set[str]:
    """Find speech resources acquired from an explicit named-thing lookup.

    This does not substitute an actor name into a later call. The resource
    remains the receiver, including any subsequent acquisition of that object.
    A reused scalar slot is deliberately excluded. Require the recovered Speak
    operand before changing task waits; partially recovered speech paths remain
    outside this rule.
    """
    named = {}
    for match in re.finditer(r'GSI->GetThingWithScriptName\(([^;]+)\);', source):
        args = _split_top(match[1])
        slot = _strip_addr(args[0])
        if re.fullmatch(r'\w+', slot):
            named.setdefault(slot, match.start())
    speakers = set()
    for match in re.finditer(r'::_Speak_CScriptGameResourceObjectScriptedThingBase\w*\(([^;]+)\);', source):
        args = _split_top(match[1])
        if len(args) >= 7:
            speakers.add(_strip_addr(args[0]))
    resources = set()
    for match in re.finditer(r'RESOURCE_TryAcquire\(([^;]+)\)', source):
        args = _split_top(match[1])
        if len(args) != 3:
            continue
        obj, actor = map(_strip_addr, args[:2])
        if not re.fullmatch(r'\w+', obj):
            continue
        definitions = list(re.finditer(r'^\s*' + re.escape(obj) + r' = ([^;]+);', source, re.M))
        if (obj in speakers and actor in named and named[actor] < match.start()
                and len(definitions) == 1 and definitions[0][1] == 'RESOURCE_NewResource()'
                and definitions[0].start() < match.start()):
            resources.add(obj)
    return resources
