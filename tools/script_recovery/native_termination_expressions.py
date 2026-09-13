"""Recover a reviewed termination call followed by its decompiled AL test."""
import re


def fold_inline_termination(source, aliases, receiver, live):
    names = {'CScriptBase::IsActiveThreadTerminating'}
    names.update(name for name, resolved in aliases.items() if resolved == 'IsActiveThreadTerminating')
    protected = r'/\*[\s\S]*?\*/|//[^\n]*|"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\''
    call = (r'\(\s*(?:' + '|'.join(re.escape(n) for n in sorted(names)) +
            r')\s*\(\s*(?:\([\w:]+\s*\*\)\s*)?(?:this|param_1)\s*\)\s*,\s*'
            r'extraout_AL(?:_\d+)?\s*(?P<comparison>==|!=)\s*\'\\0\'\s*\)')
    pattern = re.compile(protected + '|(?P<call>' + call + ')')
    count = 0
    def replace(match):
        nonlocal count
        if match['call'] is None:
            return match[0]
        count += 1
        result = 'alive' if match['comparison'] == '==' else 'not alive'
        if live:
            # Retain both the query's short-circuit position and the saved AL fact
            # used by any later extraout comparison in the enclosing expression.
            return f'(function() alive = not {receiver}:IsActiveThreadTerminating(); return {result} end)()'
        return '(' + result + ')'
    return pattern.sub(replace, source), count
