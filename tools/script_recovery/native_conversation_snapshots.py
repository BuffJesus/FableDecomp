"""Explicitly owned, evidenced script-visible conversation snapshots."""
import re


def lower_owned_conversation_snapshots(text, spec):
    from tools.script_recovery.native_evidence_lowering import (
        _THIS, _PARENT, off_re, _balanced_end, _split_top, CONVERSATION_STRINGS)
    address = getattr(spec, 'native_address', None)
    for off, evidence in (getattr(spec, 'parent_conversation_snapshots', {}) or {}).items():
        if evidence.get('storage') != 'questConversationVector':
            continue
        if evidence.get('element') != 'CConversation' or evidence.get('stride') != 0x5c:
            continue
        name, glob = evidence['name'], int(evidence['global'], 16)
        base = _PARENT if spec.entity else _THIS
        initialization = evidence.get('initialization', {})
        if not spec.entity and address in {int(a, 16) for a in initialization.get('functions', [])}:
            member = r'(?:\(int\))?\(?' + base + r'\)? \+ ' + off_re(off)
            for label, target in (getattr(spec, 'call_labels', {}) or {}).items():
                if target != int(initialization['assign'], 16):
                    continue
                for match in reversed(list(re.finditer(re.escape(label) + r'\s*\(', text))):
                    end = _balanced_end(text, match.end() - 1)
                    if not end or text[end:end+1] != ';':
                        continue
                    args = _split_top(text[match.end():end-1])
                    if len(args) != 2 or not re.fullmatch(member, args[0].strip()):
                        continue
                    if not re.fullmatch(r'DAT_0143e90c \+ ' + off_re(glob), args[1].strip()):
                        continue
                    text = text[:match.start()] + f'GSI->SnapshotGlobalConversations("{name}", {hex(glob)});' + text[end+1:]
        if address not in {int(a, 16) for a in evidence.get('readFunctions', [])}:
            continue
        begin = r'\*\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\)'
        text = re.sub(r'\*\((?:int|undefined4) \*\)\((\w+) \* 0x5c \+ 0x28 \+ ' + begin + r'\)',
                      lambda m: f'ENGINE_StateConversationLines("{name}", {m[1]})', text)
        def index(var, scale, pos):
            prefix = text[:pos]
            assignments = list(re.finditer(r'^[ \t]*' + re.escape(var) + r' = ([^;]+);', prefix, re.M))
            if assignments:
                last = assignments[-1]
                value = re.fullmatch(r'(\w+) \* ' + scale, last[1])
                variable = re.escape(var)
                barrier = re.search(r'[{}]|\b(?:goto|return|if|else|for|while|do)\b|(?m:^[ \t]*\w+:(?!:))'
                    + r'|\b' + variable + r'\s*(?:(?:<<|>>|[+*/%&|^-])?=(?!=)|\+\+|--)'
                    + r'|(?:\+\+|--)\s*' + variable + r'\b|&\s*\(?\s*' + variable + r'\b', prefix[last.end():])
                if value and not barrier:
                    return value[1]
            return f'({var}) / {scale}'
        def string(match):
            field = CONVERSATION_STRINGS.get(int(match['offset'], 16))
            if field is None:
                return match[0]
            i = index(match['record'], '0x5c', match.start())
            j = index(match['line'], '4', match.start())
            return f'ENGINE_StateConversationString("{name}", {i}, "{field}", {j})'
        text = re.sub(r'\(CCharString(?:_bv)? \*\)\(\*\(int \*\)\((?P<record>\w+) \+ (?P<offset>0x[0-9a-f]+) \+ ' + begin + r'\) \+ (?P<line>\w+)\)', string, text)
        text = re.sub(r'^([ \t]*)CCharString::CCharString\(\s*(?:\(CCharString \*\))?&?(\w+),\s*(ENGINE_StateConversationString\([^;]*\))\);', r'\1\2 = \3;', text, flags=re.M)
    return text
