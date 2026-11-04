"""Keep loop backedges when code after a break jumps back into the loop's tail."""
import re

def _flatten_constant_loops(statements):
    """Expose a constant-true loop's header and exit as ordinary C labels.

    Copying just a loop tail to a later sibling branch loses the next iteration
    (Bandit Camp CheckAnyBanditsKilled, native D03556 -> D03417). A backward
    jump into its direct body can instead retain the entire loop via labels.
    Only a straight-line header ending in one conditional break qualifies.
    Conditional loops, forward entries, nested loops, switch bodies and ambiguous
    transfers remain with the existing control-flow passes. Fresh labels cannot
    merge existing identifiers.
    Input statements already have native local declarations hoisted/stripped.
    """
    from tools.script_recovery.native_goto_scopes import _parse,_unexpressible
    out=list(statements)
    while True:
        parsed=_parse(out)
        if parsed is None:return out
        blocks,scopes,labels,uses,opens=parsed
        chosen=None
        for index,target in _unexpressible(parsed):
            label_scope=scopes[labels[target]]
            if not label_scope:continue
            block=blocks[label_scope[-1]];parent=label_scope[:-1]
            if block['kind']!='loop' or scopes[index][:len(parent)]!=parent:continue
            start,end=block['open'],block['close']
            if index<=end:continue  # this repair covers backward re-entry after a loop break
            if not re.fullmatch(r'while\s*\(\s*true\s*\)\s*\{',out[start].strip()):continue
            if out[end].strip()!='}':continue
            header=[line.strip() for line in out[start+1:labels[target]] if line.strip()]
            if not header or not re.fullmatch(r'if\s*\(.*\)\s*break;',header[-1]):continue
            if any(line.endswith('{') or line.startswith('}') or
                   re.match(r'(?:if|else|while|for|do|switch|goto|return|break|continue)\b',line) or
                   re.match(r'(?:LAB_[0-9a-f]+(?:_c\d+)?|FLOW_[a-z0-9_]+):',line)
                   for line in header[:-1]):continue
            if any(re.match(r'\s*(?:switch|while|for|do)\b',line) for line in out[start+1:end]):continue
            rewrites={};ok=True
            for at in range(start+1,end):
                nearest=next((b for b in reversed(scopes[at]) if blocks[b]['kind']=='loop'),None)
                if nearest!=label_scope[-1]:continue
                if not re.search(r'\b(?:break|continue)\b',out[at]):continue
                m=re.fullmatch(r'\s*(?:if\s*\(.*\)\s*)?(break|continue);\s*',out[at])
                if not m:ok=False;break
                rewrites[at]=(m.start(1),m[1])
            if ok and any(at<labels[target] and word=='break' for at,(_,word) in rewrites.items()):
                chosen=(start,end,target,rewrites);break
        if chosen is None:return out
        start,end,target,rewrites=chosen
        names=set(re.findall(r'\b\w+\b','\n'.join(out)))
        def fresh(base):
            value=base;serial=0
            while value in names:serial+=1;value=f'{base}_{serial}'
            names.add(value);return value
        head=fresh('FLOW_loop_head_'+target.lower());exit=fresh('FLOW_loop_exit_'+target.lower())
        indent=out[start][:len(out[start])-len(out[start].lstrip())]
        replacement=[indent+head+':']
        for at in range(start+1,end):
            line=out[at]
            if at in rewrites:
                offset,word=rewrites[at];line=line[:offset]+f'goto {exit if word=="break" else head};'
            replacement.append(line)
        replacement.extend([indent+f'goto {head};',indent+exit+':'])
        out[start:end+1]=replacement


def _flatten_conditional_if_reentry(statements):
    """Expose a conditional loop reached backward through nested if tails.

    Accept only a flag condition with optional simple register-copy operands.
    Nested loops remain structured, and their own break/continue are unchanged.
    Every transfer belonging to the removed outer loop is rewritten explicitly.
    """
    from tools.script_recovery.native_goto_scopes import _parse, _unexpressible, _is_epilogue
    out = list(statements)
    while True:
        parsed = _parse(out)
        if parsed is None:
            return out
        blocks, scopes, labels, uses, opens = parsed
        chosen = None
        for index, target in _unexpressible(parsed):
            if _is_epilogue(out, parsed, target):
                continue
            label_scope, jump_scope = scopes[labels[target]], scopes[index]
            common = 0
            while common < min(len(label_scope), len(jump_scope)) and label_scope[common] == jump_scope[common]:
                common += 1
            path = label_scope[common:]
            if len(path) < 2 or blocks[path[0]]['kind'] != 'loop':
                continue
            if any(blocks[b]['kind'] != 'if' for b in path[1:]):
                continue
            loop = path[0]; block = blocks[loop]
            start, end = block['open'], block['close']
            if index <= end or out[end].strip() != '}':
                continue
            match = re.fullmatch(r'while\s*\((.*?)\)\s*\{', out[start].strip())
            if not match:
                continue
            operands = [s.strip() for s in match[1].split(',')]
            if not re.fullmatch(r'!?[A-Za-z_]\w*', operands[-1]) or operands[-1] in ('true', 'false'):
                continue
            if any(not re.fullmatch(r'[A-Za-z_]\w* = [A-Za-z_]\w*', operand) for operand in operands[:-1]):
                continue
            if any(re.match(r'\s*switch\b', line) for line in out[start+1:end]):
                continue
            rewrites = {}; valid = True
            for at in range(start+1, end):
                nearest = next((b for b in reversed(scopes[at]) if blocks[b]['kind'] == 'loop'), None)
                if nearest != loop:
                    # The lifter does not support continue in every conditional
                    # inner-loop shape. Keep that whole recovery unresolved.
                    if re.search(r'\bcontinue\b', out[at]):
                        valid = False; break
                    continue
                if not re.search(r'\b(?:break|continue)\b', out[at]):
                    continue
                transfer = re.fullmatch(r'\s*(?:if\s*\(.*\)\s*)?(break|continue);\s*', out[at])
                if not transfer:
                    valid = False; break
                rewrites[at] = transfer.start(1), transfer[1]
            if valid:
                chosen = start, end, target, operands, rewrites
                break
        if chosen is None:
            return out
        start, end, target, operands, rewrites = chosen
        names = set(re.findall(r'\b\w+\b', '\n'.join(out)))
        def fresh(base):
            value = base; serial = 0
            while value in names:
                serial += 1; value = f'{base}_{serial}'
            names.add(value); return value
        head = fresh('FLOW_loop_head_'+target.lower()); exit = fresh('FLOW_loop_exit_'+target.lower())
        indent = out[start][:len(out[start])-len(out[start].lstrip())]
        replacement = [indent+head+':']
        replacement.extend(indent+operand+';' for operand in operands[:-1])
        replacement.append(indent+f'if (!({operands[-1]})) goto {exit};')
        for at in range(start+1, end):
            line = out[at]
            if at in rewrites:
                offset, word = rewrites[at]
                line = line[:offset]+f'goto {exit if word == "break" else head};'
            replacement.append(line)
        replacement.extend([indent+f'goto {head};', indent+exit+':'])
        out[start:end+1] = replacement


def _rotate_forward_counted_entry(statements):
    """Rotate an unreachable counted while entered at its first body label.

    The entry jump skips the header once. Subsequent iterations must still run
    the increment and comparison. Keep the loop itself so body breaks retain
    their owner; decline continue, which would target the old header position.
    """
    from tools.script_recovery.native_goto_scopes import _parse, _unexpressible, LABEL_TOKEN
    out=list(statements)
    while True:
        parsed=_parse(out)
        if parsed is None:return out
        blocks,scopes,labels,uses,opens=parsed;chosen=None
        for index,target in _unexpressible(parsed):
            scope=scopes[labels[target]]
            if not scope:continue
            block=blocks[scope[-1]];start,end=block['open'],block['close']
            if block['kind']!='loop' or index>=start or out[end].strip()!='}':continue
            # No normal header entry: the original function reaches this loop
            # only through a jump into its direct body.
            previous=next((line.strip() for line in reversed(out[:start]) if line.strip()),'')
            if not re.fullmatch(r'(?:return|goto '+LABEL_TOKEN+r');',previous):continue
            match=re.fullmatch(r'while\s*\(\s*(\w+) = \1 \+ (1),\s*(\1 (?:!=|<|<=) (?:\w+|LOCALLIST_Count\(\w+\)))\s*\)\s*\{',out[start].strip())
            if not match:continue
            first=next((i for i in range(start+1,end) if out[i].strip()),None)
            if first!=labels[target] or out[first].strip()!=target+':':continue
            if any(re.search(r'\bcontinue\b',line) for line in out[start+1:end]):continue
            # Other external entries may bypass a different prefix; leave them
            # unresolved until their own control flow has been established.
            if any(name!=target and start<labels.get(name,-1)<end and not start<at<end for at,name in uses):continue
            chosen=start,end,first,target,match;break
        if chosen is None:return out
        start,end,first,target,match=chosen;indent=out[start][:len(out[start])-len(out[start].lstrip())]
        out[start:end+1]=[indent+target+':',indent+'while (true) {',*out[first+1:end],indent+'  '+match[1]+' = '+match[1]+' + 1;',indent+'  if (!('+match[3]+')) break;',out[end]]


def flatten(statements):
    return _flatten_conditional_if_reentry(_flatten_constant_loops(_rotate_forward_counted_entry(statements)))
