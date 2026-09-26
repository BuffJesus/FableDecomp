"""Identify C jumps that Lua can represent without entering a nested lexical scope."""
import re

LABEL_TOKEN = r'(?:LAB_[0-9a-f]+(?:_c\d+)?|FLOW_[a-z0-9_]+)'   # `_cN`: a label inside a copied sibling tail

RE_LABEL_LINE = re.compile(r'^(' + LABEL_TOKEN + r'):\s*(.*)$')
RE_GOTO_TAIL = re.compile(r'\bgoto (' + LABEL_TOKEN + r');\s*$')


def _block_kind(opener):
    """'if' for an if/else block, 'loop' for while/for/do, 'other' for anything else Ghidra opens."""
    line = opener.lstrip('}').strip()
    if line.startswith(('if (', 'if(', 'else')):
        return 'if'
    if line.startswith(('while', 'for', 'do')):
        return 'loop'
    return 'other'


def _parse(statements):
    """Block structure of a statement list: (blocks, scope_at, labels, uses, open_at) or None when unbalanced.

    blocks: id -> {'open', 'close', 'kind'}; scope_at[i]: block ids enclosing line i; labels: name -> index;
    uses: [(index, target)]; open_at: open index -> id.
    """
    scopes, blocks, labels, uses, open_at, scope_at = [], {}, {}, [], {}, []
    serial = 0
    for index, statement in enumerate(statements):
        line = statement.strip()
        if line.startswith('}'):
            if not scopes:
                return None
            blocks[scopes.pop()]['close'] = index
        scope_at.append(tuple(scopes))
        label = RE_LABEL_LINE.match(line)
        if label:
            if label[1] in labels:
                return None
            labels[label[1]] = index
            line = label[2]
        jump = RE_GOTO_TAIL.search(line)
        if jump:
            uses.append((index, jump[1]))
        if line.endswith('{'):
            serial += 1
            blocks[serial] = {'open': index, 'close': None, 'kind': _block_kind(statement.strip())}
            open_at[index] = serial
            scopes.append(serial)
    if scopes:
        return None
    return blocks, scope_at, labels, uses, open_at


def _unexpressible(parsed):
    """[(index, target)] of the jumps whose label is not in an enclosing scope of the jump."""
    blocks, scope_at, labels, uses, _ = parsed
    out = []
    for index, target in uses:
        if target not in labels:
            continue
        label_scope = scope_at[labels[target]]
        if scope_at[index][:len(label_scope)] != label_scope:
            out.append((index, target))
    return out


def _chain_end(statements, parsed, close):
    """Index of the `}` that ends the if/else chain a block's closing brace at `close` belongs to."""
    blocks, _, _, _, open_at = parsed
    while True:
        if close in open_at:                      # `} else {` on one line
            close = blocks[open_at[close]]['close']
            continue
        nxt = close + 1
        while nxt < len(statements) and not statements[nxt].strip():
            nxt += 1
        if nxt < len(statements) and nxt in open_at and statements[nxt].strip().startswith('else'):
            close = blocks[open_at[nxt]]['close']
            continue
        return close


def _is_epilogue(statements, parsed, name, seen=()):
    """True when the label's region is a straight-line cleanup that ends in `return;` (or jumps to another
    such region): `native_cleanup_regions` hoists those on the Lua side."""
    blocks, scope_at, labels, _, open_at = parsed
    if name in seen or name not in labels:
        return False
    index = labels[name]
    head = RE_LABEL_LINE.match(statements[index].strip())
    pending = [head[2]] if head and head[2] else []
    j = index + 1
    while True:
        if pending:
            t = pending.pop(0)
        else:
            if j >= len(statements):
                return False                      # falls off the function: no terminal, not a cleanup region
            t = statements[j].strip()
            j += 1
        if not t:
            continue
        if t.startswith('}'):
            if t.endswith('{'):                   # `} else {`: skip the chain
                j = blocks[open_at[j - 1]]['close'] + 1
                continue
            closed = next(b for b in blocks.values() if b['close'] == j - 1)
            if closed['kind'] == 'loop':
                return False
            # a plain `}` followed by an else chain: skip it
            nxt = j
            while nxt < len(statements) and not statements[nxt].strip():
                nxt += 1
            if nxt < len(statements) and nxt in open_at and statements[nxt].strip().startswith('else'):
                j = blocks[open_at[nxt]]['close'] + 1
            continue
        if t == 'return;' or re.match(r'^return\b', t):
            return True
        g = re.fullmatch(r'goto (' + LABEL_TOKEN + r');', t)
        if g:
            return _is_epilogue(statements, parsed, g[1], seen + (name,))
        if RE_LABEL_LINE.match(t):
            other = RE_LABEL_LINE.match(t)
            return _is_epilogue(statements, parsed, other[1], seen + (name,))
        if t.endswith('{') or re.match(r'^(?:if|else|while|for|do|switch)\b', t) or t in ('break;', 'continue;'):
            return False


def _indent_of(statement):
    return statement[:len(statement) - len(statement.lstrip())]


def hoist_shared_tails(statements, max_rounds=200):
    """Move a shared continuation out of the if/else nest that hides it, so every jump to it is a Lua goto.

    VC7.1 places a continuation shared by two branches (the Xbox and PC halves of a tutorial, the
    then/else halves of a check) inside ONE of them and jumps into it from the other; Ghidra prints the
    label inside that branch's nested ifs, where Lua cannot jump. For a jump whose label sits under a chain
    of if/else blocks (never a loop) that the jump site is outside of, the label's tail -- from the label to
    the end of each enclosing if block on the way out, skipping the else chains that fall-through skips --
    is moved right after the outermost of those blocks' if/else chain:

        if (X) { pre; if (Y) { L: tail } } else { pc; goto L; } after
    becomes
        if (X) { pre; if (Y) { goto L; } } else { pc; goto L; } goto FLOW_past_l; L: tail FLOW_past_l: after

    A move, not a copy: the labels inside the tail keep their names and every jump keeps its meaning (the
    tail stays inside the same loops). A region that is a plain cleanup epilogue (straight line to `return`)
    is left alone for `native_cleanup_regions`. Each hoist must strictly reduce the number of unexpressible
    jumps, else it is undone; the pass repeats until no candidate is left.
    """
    failed = set()
    for _ in range(max_rounds):
        parsed = _parse(statements)
        if parsed is None:
            return statements
        pending = _unexpressible(parsed)
        if not pending:
            return statements
        blocks, scope_at, labels, uses, open_at = parsed
        progressed = False
        for index, target in pending:
            if target in failed:
                continue
            label_index = labels[target]
            label_scope, jump_scope = scope_at[label_index], scope_at[index]
            common = 0
            while common < min(len(label_scope), len(jump_scope)) and label_scope[common] == jump_scope[common]:
                common += 1
            path = label_scope[common:]
            if not path or any(blocks[b]['kind'] != 'if' for b in path):
                failed.add(target)
                continue
            if _is_epilogue(statements, parsed, target):
                failed.add(target)
                continue
            rewritten = _hoist(statements, parsed, target, path)
            if rewritten is None:
                failed.add(target)
                continue
            reparsed = _parse(rewritten)
            if reparsed is None or len(_unexpressible(reparsed)) >= len(pending):
                failed.add(target)
                continue
            statements = rewritten
            progressed = True
            break
        if not progressed:
            return statements
    return statements


def _hoist(statements, parsed, target, path):
    blocks, scope_at, labels, uses, open_at = parsed
    label_index = labels[target]
    outer = path[0]
    exit_index = _chain_end(statements, parsed, blocks[outer]['close'])
    # the tail: one segment per block on the path, innermost first; each runs from the label (or from the
    # end of the previous block's else chain) to that block's closing brace
    segments, start = [], label_index
    for block in reversed(path):
        close = blocks[block]['close']
        segments.append((start, close))
        start = _chain_end(statements, parsed, close) + 1
    stem = target.lower()
    head = RE_LABEL_LINE.match(statements[label_index].strip())
    indent = _indent_of(statements[exit_index])
    base_depth = len(scope_at[label_index])
    region, in_place = [indent + f'{target}:'], {}        # in_place: segment start index -> replacement lines
    for k, (seg_start, seg_end) in enumerate(segments):
        body = []
        for i in range(seg_start, seg_end):
            t = statements[i].strip()
            if i == label_index:
                t = head[2] if head and head[2] else ''
            if not t:
                continue
            depth = max(0, len(scope_at[i]) - (base_depth - k))
            body.append(indent + '  ' * depth + t)
        if not body:
            continue            # an empty segment: falling out of the block reaches the next one anyway
        name = target if k == 0 else f'FLOW_hoist_{stem}_{k}'
        if k:
            region.append(indent + f'{name}:')
        region.extend(body)
        in_place[seg_start] = [_indent_of(statements[seg_start]) + f'goto {name};']
    if len(region) == 1:
        block = region        # nothing to run: the label just marks the way out of the chain
    else:
        past = f'FLOW_past_{stem}'
        block = [indent + f'goto {past};'] + region + [indent + f'{past}:']
    removed = {i for seg_start, seg_end in segments for i in range(seg_start, seg_end)}
    out = []
    for i, statement in enumerate(statements):
        if i in in_place:
            out.extend(in_place[i])
        if i in removed:
            continue
        out.append(statement)
        if i == exit_index:
            out.extend(block)
    return out


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


def rotate_mid_entered_loops(statements):
    """`while( true ) { A; LAB: B; }` reachable only through `goto LAB` (the line before the loop is an
    unconditional jump or return) is the same loop rotated: `LAB: while( true ) { B; A; }`. Lua cannot jump into a
    loop body, and copying B to the jump site and then leaving the loop skipped the retry (GTDI_Maze 0x00E27E90:
    the hero TryAcquire retry, entered at its NewScriptFrame, 2026-09-26). Declined when A holds a label, the body
    a `continue` (it would re-enter at B instead of A), or a jump to LAB comes from inside the loop."""
    out = list(statements)
    i = 0
    while i < len(out):
        if out[i].strip() not in ('while( true ) {', 'while (true) {'):
            i += 1
            continue
        depth, j = 1, i + 1
        while j < len(out) and depth:
            t = out[j].strip()
            if t.startswith('}'):
                depth -= 1
                if depth == 0:
                    break
            if t.endswith('{'):
                depth += 1
            j += 1
        if j >= len(out) or out[j].strip() != '}':
            i += 1
            continue
        prev = next((out[k].strip() for k in range(i - 1, -1, -1) if out[k].strip()), '')
        body = out[i + 1:j]
        depth, split = 0, None
        for m, raw in enumerate(body):
            t = raw.strip()
            if t.startswith('}'):
                depth -= 1
            if depth == 0 and re.fullmatch(LABEL_TOKEN + r':', t) and split is None:
                split = m
            if t.endswith('{'):
                depth += 1
        if (split is None or split == 0 or not re.fullmatch(r'(?:goto ' + LABEL_TOKEN + r'|return);', prev)
                or any(re.match(r'^' + LABEL_TOKEN + r':', r.strip()) for r in body[:split])
                or any(r.strip() == 'continue;' for r in body)):
            i += 1
            continue
        label = body[split].strip()[:-1]
        if any(re.search(r'\bgoto ' + re.escape(label) + r';', r) for r in out[i:j + 1]):
            i += 1
            continue
        indent = out[i][:len(out[i]) - len(out[i].lstrip())]
        out[i:j + 1] = [indent + label + ':', out[i]] + body[split + 1:] + body[:split] + [out[j]]
        i = j + 1
    return out


def duplicate_sibling_tails(statements):
    """Rewrite `goto L` jumps whose label sits in a sibling block Lua cannot enter.

    When the label's region is straight-line code up to the closing brace of its block, the
    jump is equivalent to running that region and then leaving the block: the region is copied
    to the jump site and the jump retargeted to a synthetic label placed right after the block's
    closing brace (an enclosing scope of the jump, which Lua accepts). Other jumps are untouched.
    """
    statements = rotate_mid_entered_loops(statements)
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
                                earlier_targets = {m[1] for u in lines[:n] for m in re.finditer(r'\bgoto (' + LABEL_TOKEN + r');', u)}
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
                if depth == 0 and t == 'return;':
                    # nothing after a top-level return runs unless an earlier jump lands beyond it
                    later = {m[1] for u in lines[n + 1:] for m in [re.match(r'^(' + LABEL_TOKEN + r'):', u)] if m}
                    earlier_targets = {m[1] for u in lines[:n] for m in re.finditer(r'\bgoto (' + LABEL_TOKEN + r');', u)}
                    if not (later & earlier_targets):
                        terminated = True
                        break
            if ok and internal and region and region[-1] == 'return;':
                ok = False
            if ok:
                # a renamed jump whose renamed label the copy never reached (the copy terminated before
                # it): the original label must be visible from the copy site, else the copy is broken
                defined = {m[1] for r in region for m in [re.match(r'^(' + LABEL_TOKEN + r'):', r.strip())] if m}
                for n, r in enumerate(region):
                    g = re.search(r'\bgoto (' + LABEL_TOKEN + r');', r)
                    if g and g[1].endswith(suffix) and g[1] not in defined:
                        original = g[1][:-len(suffix)]
                        if visible(original):
                            region[n] = r[:g.start(1)] + original + r[g.end(1):]
                        else:
                            ok = False
                            break
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
