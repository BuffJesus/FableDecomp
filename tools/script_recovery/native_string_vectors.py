"""Lower explicitly evidenced, quest-owned value-string vectors."""
import re


def lower_string_vectors(text, spec):
    from tools.script_recovery.native_evidence_lowering import (
        _THIS, off_re, _balanced_end, _split_top)
    if spec.entity:
        return text
    for off, evidence in (getattr(spec, 'parent_string_snapshots', {}) or {}).items():
        if evidence.get('storage') != 'questStringVector' or evidence.get('element') != 'CCharString':
            continue
        init = evidence.get('initialization', {})
        if getattr(spec, 'native_address', None) not in {int(a, 16) for a in init.get('functions', [])}:
            continue
        if init.get('stride') != 4:
            continue
        labels = getattr(spec, 'call_labels', {})
        def calls(target):
            for label, address in labels.items():
                if address != int(target, 16):
                    continue
                for match in re.finditer(re.escape(label) + r'\s*\(', text):
                    end = _balanced_end(text, match.end() - 1)
                    if end and text[end:end + 1] == ';':
                        yield match, end + 1, _split_top(text[match.end():end - 1])
        name = evidence['name']
        member = r'(?:\([^;]*?\*\))?\(' + _THIS + r' \+ ' + off_re(off) + r'\)'
        edits = []
        # Resize(count, empty string) is bracketed by an empty constructor and
        # its destructor. Require the same temporary and uninterrupted sequence.
        for resize, resize_end, args in calls(init['resize']):
            if len(args) != 3 or not re.fullmatch(member, args[0].strip()):
                continue
            if not re.fullmatch(r'\w+', args[2].strip()):
                continue
            for ctor, ctor_end, ctor_args in calls(init['emptyConstructor']):
                if len(ctor_args) != 1 or ctor_end > resize.start() or text[ctor_end:resize.start()].strip():
                    continue
                line_start = text.rfind('\n', 0, ctor.start()) + 1
                assign = re.fullmatch(r'[ \t]*(\w+) = ', text[line_start:ctor.start()])
                if not assign or assign[1] != args[2].strip():
                    continue
                temporary = re.fullmatch(r'(?:\(CCharString \*\))?&?(\w+)', ctor_args[0].strip())
                if not temporary:
                    continue
                for dtor, dtor_end, dtor_args in calls(init['destructor']):
                    if dtor.start() < resize_end or text[resize_end:dtor.start()].strip():
                        continue
                    if len(dtor_args) != 1 or not re.fullmatch(r'(?:\(CCharString \*\))?&?' + re.escape(temporary[1]), dtor_args[0].strip()):
                        continue
                    edits.append((line_start, dtor_end, f'  GSI->StateStringVectorResize("{name}", {args[1].strip()});'))
        begin = r'\*\(int \*\)\(' + _THIS + r' \+ ' + off_re(off) + r'\)'
        destination = r'\(CCharString \*\)\(' + begin + r' \+ (?P<index>\w+) \* 4\)'
        global_offset = off_re(int(evidence['global'], 16))
        for call, end, args in calls(init['assign']):
            if len(args) != 2:
                continue
            dst = re.fullmatch(destination, args[0].strip())
            if not dst:
                continue
            source = r'\(CCharString(?:_bv)? \*\)\(\*\(int \*\)\(DAT_0143e90c \+ ' + global_offset + r'\) \+ ' + re.escape(dst['index']) + r' \* 4\)'
            if not re.fullmatch(source, args[1].strip()):
                continue
            value = f'ENGINE_GlobalGameDataStringAt({evidence["global"]}, {dst["index"]})'
            edits.append((call.start(), end, f'GSI->SetStateStringVectorAt("{name}", {dst["index"]}, {value});'))
        edits.sort()
        if any(a[1] > b[0] for a, b in zip(edits, edits[1:])):
            raise ValueError('overlapping evidenced string vector operations')
        for start, end, replacement in reversed(edits):
            text = text[:start] + replacement + text[end:]
    return text
