"""Identify C jumps that Lua can represent without entering a nested lexical scope."""
import re

LABEL_TOKEN = r'(?:LAB_[0-9a-f]+(?:_c\d+)?|FLOW_[a-z0-9_]+)'   # `_cN`: a label inside a copied sibling tail


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


def merge_equivalent_regions(statements):
    """VC7.1 duplicates a cleanup tail (`PauseAll(false); DestroyMovie(x); goto END`) behind several
    labels at different nesting depths. A `goto L` whose straight-line region (label to its first
    `goto`/`return`, a following label counting as `goto that label`) is textually identical to a
    shallower label's region is retargeted to the shallower label, which Lua can reach; the deeper
    region stays in place for the paths that fall into it."""
    scopes, labels = [], {}
    depth_at = []
    for index, statement in enumerate(statements):
        line = statement.strip()
        if line.startswith('}'):
            if not scopes:
                return statements
            scopes.pop()
        depth_at.append(tuple(scopes))
        label = re.match(r'^(' + LABEL_TOKEN + r'):\s*(.*)$', line)
        if label:
            if label[1] in labels:
                return statements
            labels[label[1]] = index
            line = label[2]
        if line.endswith('{'):
            scopes.append(index)
    if scopes:
        return statements

    def region(name):
        index = labels[name]
        head = re.match(r'^' + LABEL_TOKEN + r':\s*(.*)$', statements[index].strip())
        body = [head[1]] if head and head[1] else []
        for raw in statements[index + 1:]:
            t = raw.strip()
            if not t:
                continue
            other = re.match(r'^(' + LABEL_TOKEN + r'):\s*(.*)$', t)
            if other:
                body.append(f'goto {other[1]};')
                return tuple(body)
            if t == '}':
                continue            # falling out of an enclosing block keeps the straight line
            if t.startswith('}') or t.endswith('{'):
                return None
            body.append(t)
            if re.match(r'^(?:goto ' + LABEL_TOKEN + r'|return(?: [^;]*)?);$', t):
                return tuple(body)
        return None

    regions = {name: region(name) for name in labels}
    retarget = {}
    for name, body in regions.items():
        if not body:
            continue
        candidates = [other for other, ob in regions.items() if other != name and ob == body
                      and len(depth_at[labels[other]]) < len(depth_at[labels[name]])]
        if candidates:
            retarget[name] = min(candidates, key=lambda o: len(depth_at[labels[o]]))
    if not retarget:
        return statements
    out = []
    for statement in statements:
        m = re.search(r'\bgoto (' + LABEL_TOKEN + r');\s*$', statement)
        if m and m[1] in retarget:
            statement = statement[:m.start(1)] + retarget[m[1]] + statement[m.end(1):]
        out.append(statement)
    return out


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
            region, depth, ok, terminated = [], 0, True, False
            head = re.match(r'^' + LABEL_TOKEN + r':\s*(.*)$', statements[label_index].strip())
            lines = ([head[1]] if head and head[1] else []) + [raw.strip() for raw in statements[label_index + 1:close]]
            # labels defined inside the tail are copied under a fresh name; jumps out of the tail are
            # legal when their label is visible from the jump site (an enclosing scope of it)
            internal = {m[1] for t in lines for m in [re.match(r'^(' + LABEL_TOKEN + r'):', t)] if m}
            suffix = f'_c{len(edits) + 1}'
            if any(re.search(r'\bgoto ' + re.escape(target) + r';', t) for t in lines):
                internal.add(target)
                region.append(f'{target}{suffix}:')     # the copy keeps its own head label for the backward jump

            def visible(name):
                lscope = labels.get(name, (None, None))[1]
                return lscope is not None and scope[:len(lscope)] == lscope

            def straight(name):
                # the target's own tail when it is straight-line code ending in a jump the copy site can take
                if name not in labels:
                    return None
                start = labels[name][0]
                head_ = re.match(r'^' + LABEL_TOKEN + r':\s*(.*)$', statements[start].strip())
                body = [head_[1]] if head_ and head_[1] else []
                for raw in statements[start + 1:]:
                    u = raw.strip()
                    if not u:
                        continue
                    if u == '}':
                        continue
                    if u.startswith('}') or u.endswith('{') or re.match(r'^' + LABEL_TOKEN + r':', u):
                        return None
                    body.append(u)
                    if u == 'return;':
                        return body
                    g = re.fullmatch(r'goto (' + LABEL_TOKEN + r');', u)
                    if g:
                        return body if visible(g[1]) else None
                return None

            skipping = 0     # inside a sibling `else {` branch at tail depth 0: not reachable by falling through
            for n, t in enumerate(lines):
                if not t:
                    continue
                if skipping:
                    if t.startswith('}'):
                        skipping -= 1
                        if skipping == 0 and t.endswith('{'):
                            skipping = 1           # `} else if (...) {` continues the chain
                    elif t.endswith('{'):
                        skipping += 1
                    continue
                if depth == 0 and t.startswith('else') and t.endswith('{'):
                    skipping = 1
                    continue
                inner_label = re.match(r'^(' + LABEL_TOKEN + r'):\s*(.*)$', t)
                if inner_label:
                    t = f'{inner_label[1]}{suffix}:' + (' ' + inner_label[2] if inner_label[2] else '')
                inner_goto = re.search(r'\bgoto (' + LABEL_TOKEN + r');', t)
                if inner_goto:
                    if inner_goto[1] in internal:
                        t = t[:inner_goto.start(1)] + inner_goto[1] + suffix + t[inner_goto.end(1):]
                    elif not visible(inner_goto[1]):
                        inlined = straight(inner_goto[1])
                        if inlined is None:
                            ok = False
                            break
                        cond = re.match(r'^(if \(.*\)) goto ' + re.escape(inner_goto[1]) + r';$', t.strip())
                        if cond:
                            region.extend([cond[1] + ' {'] + inlined + ['}'])
                        elif t.strip() == inner_goto.group(0):
                            region.extend(inlined)
                            if depth == 0:
                                later = {m[1] for u in lines[n + 1:] for m in [re.match(r'^(' + LABEL_TOKEN + r'):', u)] if m}
                                earlier_targets = {m[1] for u in lines[:n] for m in re.finditer(r'goto (' + LABEL_TOKEN + r');', u)}
                                if not (later & earlier_targets):
                                    terminated = True
                                    break
                        else:
                            ok = False
                            break
                        continue
                    region.append(t)
                    if depth == 0 and t.strip() == inner_goto.group(0).replace(inner_goto[1], inner_goto[1] + (suffix if inner_goto[1] in internal else '')):
                        # an unconditional jump: the rest of the tail only matters when an earlier jump lands in it
                        later = {m[1] for u in lines[n + 1:] for m in [re.match(r'^(' + LABEL_TOKEN + r'):', u)] if m}
                        earlier_targets = {m[1] for u in lines[:n] for m in re.finditer(r'\bgoto (' + LABEL_TOKEN + r');', u)}
                        if not (later & earlier_targets):
                            terminated = True
                            break
                    continue
                if t.startswith('}'):
                    if depth == 0:
                        if t.endswith('{'):
                            ok = False
                            break
                        continue
                    depth -= 1
                if depth == 0 and t in ('break;', 'continue;'):
                    ok = False      # would leave a loop the jump site may not share
                    break
                if t.endswith('{'):
                    depth += 1
                region.append(t)
                if depth == 0 and t == 'return;' and not internal:
                    terminated = True   # nothing after a top-level return in the tail runs
                    break
            if ok and internal and region and region[-1] == 'return;':
                ok = False
            if ok and depth == 0:
                chosen = (exit_index, region, terminated)
            break
        if chosen is None:
            continue
        exit_index, region, terminated = chosen
        indent = statements[index][:len(statements[index]) - len(statements[index].lstrip())]
        if not terminated and exit_index not in synthetic:
            base = f'FLOW_after_{target.lower()}'
            taken = set(synthetic.values())
            synthetic[exit_index] = base if base not in taken else f'{base}_{exit_index}'   # one label per exit point
        leave = [] if terminated else [f'goto {synthetic[exit_index]};']
        conditional = re.match(r'^(if \(.*\)) goto ' + re.escape(target) + r';\s*$', statements[index].strip())
        if conditional:
            # `if (c) goto L;` runs the tail only when c holds: keep it under the condition
            edits[index] = [indent + conditional[1] + ' {'] + [indent + '  ' + r for r in region + leave if r] + [indent + '}']
        else:
            edits[index] = [indent + r for r in region if r] + ([statements[index].replace(f'goto {target};', leave[0])] if leave else [])
    if not edits:
        return statements
    out = []
    for index, statement in enumerate(statements):
        out.extend(edits.get(index, [statement]))
        if index in synthetic:
            out.append(statement[:len(statement) - len(statement.lstrip())] + synthetic[index] + ':')
    return out
