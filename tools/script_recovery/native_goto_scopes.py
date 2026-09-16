"""Identify C jumps that Lua can represent without entering a nested lexical scope."""
import re

LABEL_TOKEN = r'(?:LAB_[0-9a-f]+|FLOW_[a-z0-9_]+)'


def supported_jumps(statements):
    scopes, labels, uses = [], {}, []
    serial = 0
    for statement in statements:
        line = statement.strip()
        if line.startswith('}'):
            if not scopes:
                return set(), set()
            scopes.pop()
        label = re.match(r'^(' + LABEL_TOKEN + r'):\s*(.*)$', line)
        if label:
            if label[1] in labels:
                return set(), set()
            labels[label[1]] = tuple(scopes)
            line = label[2]
        jump = re.search(r'\bgoto (' + LABEL_TOKEN + r');\s*$', line)
        if jump:
            uses.append((line, jump[1], tuple(scopes)))
        if line.endswith('{'):
            serial += 1
            scopes.append(serial)
    if scopes:
        return set(), set()
    decisions = {}
    for line, target, scope in uses:
        destination = labels.get(target)
        valid = destination is not None and scope[:len(destination)] == destination
        decisions.setdefault(line, []).append((target, valid))
    # The emitter keys by normalized statement. If identical text occurs in more
    # than one scope, accept it only when every occurrence is safe.
    accepted = {line for line, outcomes in decisions.items() if all(valid for _, valid in outcomes)}
    used_labels = {target for line in accepted for target, _ in decisions[line]}
    return accepted, used_labels


def duplicate_sibling_tails(statements):
    """Rewrite `goto L` jumps whose label sits in a sibling block Lua cannot enter.

    When the label's region is straight-line code up to the closing brace of its block, the
    jump is equivalent to running that region and then leaving the block: the region is copied
    to the jump site and the jump retargeted to a synthetic label placed right after the block's
    closing brace (an enclosing scope of the jump, which Lua accepts). Other jumps are untouched.
    """
    scopes, labels, uses, closes = [], {}, [], {}
    serial = 0
    for index, statement in enumerate(statements):
        line = statement.strip()
        if line.startswith('}'):
            if not scopes:
                return statements
            closes[scopes.pop()] = index
        label = re.match(r'^(' + LABEL_TOKEN + r'):\s*(.*)$', line)
        if label:
            if label[1] in labels:
                return statements
            labels[label[1]] = (index, tuple(scopes))
            line = label[2]
        jump = re.search(r'\bgoto (' + LABEL_TOKEN + r');\s*$', line)
        if jump:
            uses.append((index, jump[1], tuple(scopes)))
        if line.endswith('{'):
            serial += 1
            scopes.append(serial)
    if scopes:
        return statements
    edits = {}          # jump index -> replacement lines
    synthetic = {}      # close index -> synthetic label name
    for index, target, scope in uses:
        if target not in labels:
            continue
        label_index, label_scope = labels[target]
        if scope[:len(label_scope)] == label_scope:
            continue    # already expressible
        if not label_scope:
            continue    # label at function level: nothing to leave
        # walk outward from the label's block until the jump site is inside the enclosing scope
        chosen = None
        for k in range(len(label_scope) - 1, -1, -1):
            outer = label_scope[:k]
            if scope[:len(outer)] != outer:
                continue
            close = closes.get(label_scope[k])
            if close is None or close <= label_index:
                break
            exit_index = close

            def chained(i):
                # `} else {` on one line, or `}` followed by an `else {` line
                if statements[i].strip().endswith('{'):
                    return i
                j = i + 1
                while j < len(statements) and not statements[j].strip():
                    j += 1
                return j if j < len(statements) and statements[j].strip().startswith('else') and statements[j].strip().endswith('{') else None
            while chained(exit_index) is not None:
                exit_index = chained(exit_index)
                # the line opens the next block of the chain: skip to its matching close by brace depth
                depth, j = 1, exit_index + 1
                while j < len(statements) and depth:
                    t = statements[j].strip()
                    if t.startswith('}'):
                        depth -= 1
                        if depth == 0:
                            break
                    if t.endswith('{'):
                        depth += 1
                    j += 1
                exit_index = j
            # the tail: everything from the label to that close, minus the braces that merely end the
            # blocks enclosing the label (plain `}` only; an else-chain there would change the meaning)
            region, depth, ok = [], 0, True
            head = re.match(r'^' + LABEL_TOKEN + r':\s*(.*)$', statements[label_index].strip())
            if head and head[1]:
                region.append(head[1])
            for raw in statements[label_index + 1:close]:
                t = raw.strip()
                if not t:
                    continue
                if re.search(r'\bgoto\b|^(?:' + LABEL_TOKEN + r'):', t):
                    ok = False
                    break
                if t.startswith('}'):
                    if depth == 0:
                        if t.endswith('{'):
                            ok = False
                            break
                        continue
                    depth -= 1
                if t.endswith('{'):
                    depth += 1
                region.append(t)
            if ok and depth == 0:
                chosen = (exit_index, region)
            break
        if chosen is None:
            continue
        exit_index, region = chosen
        indent = statements[index][:len(statements[index]) - len(statements[index].lstrip())]
        name = synthetic.setdefault(exit_index, f'FLOW_after_{target.lower()}')
        edits[index] = [indent + r for r in region if r] + [statements[index].replace(f'goto {target};', f'goto {name};')]
    if not edits:
        return statements
    out = []
    for index, statement in enumerate(statements):
        out.extend(edits.get(index, [statement]))
        if index in synthetic:
            out.append(statement[:len(statement) - len(statement.lstrip())] + synthetic[index] + ':')
    return out
