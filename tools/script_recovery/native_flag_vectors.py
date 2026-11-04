"""Evidenced quest-owned, byte-per-flag vectors; never infer packed-bit layout."""
import re


def lower_flag_vectors(text, spec):
    from tools.script_recovery.native_evidence_lowering import (
        _PARENT, _THIS, off_re, _balanced_end, _split_top)
    vectors = getattr(spec, 'parent_flag_vectors', {})
    base = _PARENT if spec.entity else _THIS
    for off, evidence in vectors.items():
        if evidence.get('element') != 'Bool' or evidence.get('stride') != 1:
            continue
        name = evidence['name']
        begin = r'\*\(int \*\)\(' + base + r' \+ ' + off_re(off) + r'\)'
        index = r'(?:\(int\))?(?P<index>\w+)'
        addresses = [begin + r' \+ ' + index, index + r' \+ ' + begin]
        for address in addresses:
            cell = r'\*\((?:undefined1|char|byte|bool) \*\)\(' + address + r'\)'
            store = cell + r"\s*=\s*(?P<value>0|1|true|false|'\\0'|'\\x01');"
            def write(m):
                value = 'true' if m['value'] in ('1', 'true', "'\\x01'") else 'false'
                return f'GSI->SetStateFlagVectorAt("{name}", {m["index"]}, {value});'
            text = re.sub(store, write, text)
            text = re.sub(cell + r'(?!\s*(?:(?:<<|>>|[+*/%&|^-])?=(?!=)|\+\+|--))',
                          lambda m: f'ENGINE_FlagVectorByte("{name}", {m["index"]})', text)

        # Only the native zero-fill resize instantiation belongs to this API.
        for label, target in getattr(spec, 'call_labels', {}).items():
            if target != int(evidence['resize'], 16):
                continue
            starts = list(re.finditer(re.escape(label) + r'\s*\(', text))
            for m in reversed(starts):
                end = _balanced_end(text, m.end() - 1)
                if not end or text[end:end + 1] != ';':
                    continue
                args = _split_top(text[m.end():end - 1])
                if len(args) != 3:
                    continue
                member = r'(?:\([^;]*?\*\))?\(' + base + r' \+ ' + off_re(off) + r'\)'
                if not re.fullmatch(member, args[0].strip()):
                    continue
                fill = re.fullmatch(r'(?:\(int\))?&(\w+)', args[2].strip())
                if not fill:
                    continue
                definitions = list(re.finditer(r'(?m)^[ \t]*' + re.escape(fill[1])
                                              + r' = (0|1);', text[:m.start()]))
                if not definitions or definitions[-1][1] != '0':
                    continue
                gap = text[definitions[-1].end():m.start()]
                if re.search(r'[{}]|\b' + re.escape(fill[1]) + r'\b|\b(?:if|goto|while|for)\b', gap):
                    continue
                call = f'GSI->StateFlagVectorResize("{name}", {args[1].strip()});'
                text = text[:m.start()] + call + text[end + 1:]
    return text
