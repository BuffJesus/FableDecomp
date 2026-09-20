"""Rebuild one fall-through `switch` from the compiler's binary-search dispatch tree.

VC7.1 lowers a large sparse `switch (stage)` with fall-through case bodies (the campaign stage
chain in `CGameflowScript::Main`) to a binary search over the selector whose leaves are small
jump tables; the case bodies stay laid out in source order and each falls into the next. Ghidra
renders that as nested `if (sel < K)` / `if (sel != K)` trees with several `switch (sel)`
statements inside, the bodies scattered across the branches and linked by `goto`s into out-of-line
labels. Every such jump crosses a block Lua cannot enter.

This pass recognises the shape from evidence only:

* the selector is the scrutinee of at least two `switch` statements;
* a dispatch node is an `if` whose condition compares the selector with a constant, or a `switch`
  on it, not preceded in its block by a re-assignment of the selector (a body test on a re-used
  register is not dispatch);
* for every value named by a case label or a dispatch comparison (and a representative of every
  gap between them), the tree is evaluated symbolically from the first dispatch node down to the
  first effective statement: that is the value's entry;
* from each entry the top-level flow (falling out of dispatch branches, following unconditional
  gotos) is walked until it reaches another value's entry or a `return`: that is the value's body,
  and the entry it reached is its fall-through successor.

When the successors form one chain that covers every top-level statement, the function is rewritten
as the prefix, one `switch (sel)` with the bodies in chain order (each falling through), and
`default: return;`. `native_structured_switch` then lowers that to the guarded chain. Anything
unexpected (a dispatch node inside a body, a backward jump, an uncovered statement, a value outside
the cases that does not return) rejects the rewrite and leaves the statements untouched.

Two Ghidra idioms are normalised first, only when a selector was found:

* `X = EXPR; goto L;` where `L:` is the last line of a `do { ... } while (X)` body (a jump into the
  loop test) becomes `if (EXPR) goto HEAD; goto AFTER;` with `HEAD:` before the loop and `AFTER:`
  after it: the same control flow without entering the loop mid-way;
* a trailing `if (C) { REST }` (no else) whose block exit is a `return` becomes
  `if (!C) { return; } REST`, recursively: the early-return ladder the compiler emitted, which
  Ghidra nests because every early return shares the epilogue. The stage bodies inside that ladder
  come back up to the level the chain needs.
"""
import re

from tools.script_recovery.native_flat_control import Parser, Node

RE_INT = re.compile(r'^-?(?:0x[0-9a-fA-F]+|\d+)$')
RE_IDENT = re.compile(r'^[A-Za-z_]\w*$')


def _int(text):
    text = text.strip()
    return int(text, 0) if RE_INT.match(text) else None


def render(nodes, indent=''):
    out = []
    for node in nodes:
        kind = node.kind
        if kind == 'atom':
            out.append(indent + node.text)
        elif kind == 'label':
            out.append(indent + node.text + ':')
            out.extend(render(node.body, indent))
        elif kind == 'goto':
            out.append(indent + 'goto ' + node.text + ';')
        elif kind in ('break', 'continue'):
            out.append(indent + kind + ';')
        elif kind == 'case':
            out.append(indent + (f'case {node.text}:' if node.text else 'default:'))
        elif kind == 'if':
            if len(node.body) == 1 and node.body[0].kind == 'goto' and not node.other:
                out.append(f'{indent}if ({node.text}) goto {node.body[0].text};')
                continue
            out.append(f'{indent}if ({node.text}) {{')
            out.extend(render(node.body, indent + '  '))
            other = node.other
            while other:
                out.append(f'{indent}}}')
                if len(other) == 1 and other[0].kind == 'if':
                    chained = other[0]
                    out.append(f'{indent}else if ({chained.text}) {{')
                    out.extend(render(chained.body, indent + '  '))
                    other = chained.other
                    continue
                out.append(f'{indent}else {{')
                out.extend(render(other, indent + '  '))
                break
            out.append(f'{indent}}}')
        elif kind in ('while', 'for', 'switch'):
            out.append(f'{indent}{kind} ({node.text}) {{')
            out.extend(render(node.body, indent + '  '))
            out.append(f'{indent}}}')
        elif kind == 'do':
            out.append(f'{indent}do {{')
            out.extend(render(node.body, indent + '  '))
            out.append(f'{indent}}} while ({node.text});')
        elif kind == 'block':
            out.append(f'{indent}{{')
            out.extend(render(node.body, indent + '  '))
            out.append(f'{indent}}}')
        else:
            raise ValueError('unrenderable control node ' + kind)
    return out


def _walk(nodes):
    for node in nodes:
        yield node
        yield from _walk(node.body)
        yield from _walk(node.other)


def find_selector(nodes):
    counts = {}
    for node in _walk(nodes):
        if node.kind == 'switch' and RE_IDENT.match(node.text.strip()):
            counts[node.text.strip()] = counts.get(node.text.strip(), 0) + 1
    candidates = [name for name, n in counts.items() if n >= 2]
    return candidates[0] if len(candidates) == 1 else None


def _assigns(node, name):
    return node.kind == 'atom' and re.match(r'^' + re.escape(name) + r'\s*=(?!=)', node.text) is not None


def _reads_before_write(nodes, name):
    """True when the first mention of `name` in `nodes` (in order, nested included) reads it."""
    word = re.compile(r'\b' + re.escape(name) + r'\b')
    for node in _walk(nodes):
        text = node.text if node.kind in ('atom', 'if', 'while', 'do', 'for', 'switch') else ''
        if not word.search(text):
            continue
        if _assigns(node, name):
            return word.search(text.split('=', 1)[1]) is not None
        return True
    return False


def rewrite_loop_condition_entries(nodes, serial):
    """`X = EXPR; goto L;` with `L:` the trailing label of `do { ... } while (X)` -> jumps around the loop."""
    trailing = {}   # label -> (parent list, do node)

    def index_loops(items):
        for node in items:
            if node.kind == 'do' and node.body and node.body[-1].kind == 'label' and not node.body[-1].body:
                trailing[node.body[-1].text] = (items, node)
            index_loops(node.body)
            index_loops(node.other)
    index_loops(nodes)
    if not trailing:
        return 0
    references = {}
    for node in _walk(nodes):
        if node.kind == 'goto':
            references[node.text] = references.get(node.text, 0) + 1
    rewritten = 0

    def rewrite(items):
        nonlocal rewritten
        i = 0
        while i < len(items):
            node = items[i]
            rewrite(node.body)
            rewrite(node.other)
            nxt = items[i + 1] if i + 1 < len(items) else None
            if (node.kind == 'atom' and nxt is not None and nxt.kind == 'goto' and nxt.text in trailing
                    and references.get(nxt.text) == 1):
                parent, loop = trailing[nxt.text]
                variable = loop.text.strip()
                assignment = re.match(r'^' + re.escape(variable) + r'\s*=\s*(.+);$', node.text) if RE_IDENT.match(variable) else None
                if assignment and loop in parent:
                    at = parent.index(loop)
                    serial[0] += 1
                    head, after = f'{nxt.text}_head{serial[0]}', f'{nxt.text}_after{serial[0]}'
                    # the assignment stays only when the loop body or the code after the loop reads it first
                    keep = [node] if (_reads_before_write(loop.body[:-1], variable)
                                      or _reads_before_write(parent[at + 1:], variable)) else []
                    replacement = keep + [Node('if', assignment[1].strip(), [Node('goto', head)]), Node('goto', after)]
                    items[i:i + 2] = replacement
                    loop.body.pop()          # the loop-test label is no longer referenced
                    parent[at:at + 1] = [Node('label', head), loop, Node('label', after)]
                    rewritten += 1
                    i += len(replacement)
                    continue
            i += 1
    rewrite(nodes)
    return rewritten


def _negate(condition):
    text = condition.strip()
    m = re.fullmatch(r'!\s*([A-Za-z_]\w*)', text)
    if m:
        return m[1]
    if RE_IDENT.match(text):
        return '!' + text
    m = re.fullmatch(r'([^()&|]+?)\s*(==|!=)\s*([^()&|]+)', text)
    if m:
        return f'{m[1].strip()} {"!=" if m[2] == "==" else "=="} {m[3].strip()}'
    return None


def invert_trailing_ifs(items):
    """`... if (C) { REST }  [labels]  return;` -> `... if (!C) { return; } REST [labels] return;` (recursively)."""
    inverted = 0
    i = 0
    while i < len(items):
        node = items[i]
        if node.kind == 'if' and node.body and not node.other:
            j = i + 1
            while j < len(items) and items[j].kind == 'label' and not items[j].body:
                j += 1
            exits = j == len(items) or (items[j].kind == 'atom' and items[j].text == 'return;')
            negated = _negate(node.text) if exits else None
            if negated is not None and not any(n.kind in ('break', 'continue') for n in node.body):
                items[i:i + 1] = [Node('if', negated, [Node('atom', 'return;')])] + node.body
                inverted += 1
                continue        # re-examine the spliced body at the same index
        i += 1
    return inverted


class Reject(Exception):
    pass


def _compare(value, op, constant):
    return {'<': value < constant, '<=': value <= constant, '>': value > constant, '>=': value >= constant,
            '==': value == constant, '!=': value != constant}[op]


FLIP = {'<': '>', '<=': '>=', '>': '<', '>=': '<=', '==': '==', '!=': '!='}
NOT = {'<': '>=', '<=': '>', '>': '<=', '>=': '<', '==': '!=', '!=': '=='}


def dispatch_condition(text, selector):
    """(op, constant, constant spelling) for a condition comparing the selector with a constant, else None."""
    t = text.strip()
    negate = False
    m = re.fullmatch(r'!\s*\((.*)\)', t)
    if m:
        negate, t = True, m[1].strip()
    m = re.fullmatch(re.escape(selector) + r'\s*(<=|>=|==|!=|<|>)\s*(-?(?:0x[0-9a-fA-F]+|\d+))', t)
    if m:
        op, constant, text = m[1], int(m[2], 0), m[2]
    else:
        m = re.fullmatch(r'(-?(?:0x[0-9a-fA-F]+|\d+))\s*(<=|>=|==|!=|<|>)\s*' + re.escape(selector), t)
        if not m:
            return None
        op, constant, text = FLIP[m[2]], int(m[1], 0), m[1]
    if negate:
        op = NOT[op]
    return op, constant, text


def flatten_switch_tree(statements):
    """Rewrite the statements when a dispatch tree over one selector is found; else return them unchanged."""
    try:
        nodes = Parser(statements).block()
    except ValueError as error:
        return statements, {'status': 'skipped', 'reason': str(error)}
    selector = find_selector(nodes)
    if selector is None:
        return statements, {'status': 'skipped', 'reason': 'no selector with two switches'}
    serial = [0]
    loop_entries = rewrite_loop_condition_entries(nodes, serial)
    inverted = invert_trailing_ifs(nodes)
    try:
        output_lines, evidence = _flatten(nodes, selector)
    except Reject as error:
        return render(nodes), {'status': 'rejected', 'selector': selector, 'reason': str(error),
                               'loopConditionEntries': loop_entries, 'trailingIfsInverted': inverted}
    evidence.update({'status': 'flattened', 'selector': selector, 'loopConditionEntries': loop_entries,
                     'trailingIfsInverted': inverted})
    return output_lines, evidence


def _flatten(nodes, selector):
    def dispatch_shaped(node):
        return ((node.kind == 'if' and dispatch_condition(node.text, selector) is not None)
                or (node.kind == 'switch' and node.text.strip() == selector))
    start = next((i for i, n in enumerate(nodes) if dispatch_shaped(n)), None)
    if start is None:
        raise Reject('no dispatch node at function level')
    prefix, tree = nodes[:start], nodes[start:]

    # flat items: the dispatch structure made explicit, bodies as opaque nodes
    flat = []

    def expand(items, tainted):
        for node in items:
            if node.kind == 'if' and not tainted and dispatch_condition(node.text, selector) is not None:
                flat.append(('if', node))
                expand(node.body, tainted)
                if node.other:
                    flat.append(('else', node))
                    expand(node.other, tainted)
                flat.append(('endif', node))
            elif node.kind == 'switch' and not tainted and node.text.strip() == selector:
                flat.append(('switch', node))
                for child in node.body:
                    if child.kind == 'case':
                        flat.append(('case', child))
                    else:
                        expand([child], tainted)
                flat.append(('endswitch', node))
            elif node.kind == 'label':
                flat.append(('label', node))
                expand(node.body, tainted)
            elif node.kind == 'goto':
                flat.append(('goto', node))
            elif node.kind == 'atom' and node.text == 'return;':
                flat.append(('return', node))
            elif node.kind in ('break', 'continue'):
                flat.append((node.kind, node))
            elif node.kind == 'case':
                raise Reject('case outside a dispatch switch')
            else:
                flat.append(('node', node))
            if _assigns(node, selector):
                tainted = True
    expand(tree, False)
    if any(kind == 'continue' for kind, _ in flat):
        raise Reject('continue at dispatch level')

    label_at = {}
    for i, (kind, node) in enumerate(flat):
        if kind == 'label':
            if node.text in label_at:
                raise Reject('duplicate label ' + node.text)
            label_at[node.text] = i
    for kind, node in flat:
        if kind == 'goto' and node.text not in label_at:
            raise Reject('goto to a nested label ' + node.text)
    closers, opener_of, stack = {}, {}, []
    for i, (kind, node) in enumerate(flat):
        if kind in ('if', 'switch'):
            stack.append(i)
            opener_of[id(node)] = i
        elif kind in ('endif', 'endswitch'):
            closers[stack.pop()] = i

    def else_index(i):
        node = flat[i][1]
        return next((j for j in range(i + 1, closers[i]) if flat[j] == ('else', node)), None)

    def enclosing_switch(i):
        depth = 0
        for j in range(i - 1, -1, -1):
            kind = flat[j][0]
            if kind == 'endswitch':
                depth += 1
            elif kind == 'switch':
                if depth == 0:
                    return j
                depth -= 1
        raise Reject('break outside a switch')

    # the values: case labels and comparison constants, with the spelling each was first seen in
    spelling, case_values = {}, set()
    for kind, node in flat:
        if kind == 'case' and node.text:
            value = _int(node.text)
            if value is None:
                raise Reject('non-numeric case ' + node.text)
            spelling.setdefault(value, node.text)
            case_values.add(value)
        elif kind == 'if':
            _, constant, text = dispatch_condition(node.text, selector)
            spelling.setdefault(constant, text)
    if not spelling:
        raise Reject('no values')
    values = sorted(spelling)
    gaps = {values[0] - 1, values[-1] + 1}
    for a, b in zip(values, values[1:]):
        if b - a > 1:
            gaps.add(a + 1)

    def resolve(value):
        """(entry index, is terminal) for `value`: the dispatch tree evaluated from its first node."""
        i, steps = 0, 0
        while True:
            steps += 1
            if steps > 100000:
                raise Reject('dispatch walk does not terminate')
            if i >= len(flat):
                return None, True
            kind, node = flat[i]
            if kind == 'if':
                op, constant, _ = dispatch_condition(node.text, selector)
                if _compare(value, op, constant):
                    i += 1
                else:
                    e = else_index(i)
                    i = (e + 1) if e is not None else closers[i] + 1
            elif kind == 'else':
                i = closers[opener_of[id(node)]] + 1
            elif kind in ('endif', 'endswitch', 'case', 'label'):
                i += 1
            elif kind == 'switch':
                chosen, default = None, None
                for j in range(i + 1, closers[i]):
                    if flat[j][0] == 'case':
                        if flat[j][1].text == '':
                            default = j
                        elif _int(flat[j][1].text) == value:
                            chosen = j
                            break
                    elif flat[j][0] == 'switch':
                        raise Reject('nested dispatch switch')
                i = (chosen if chosen is not None else default if default is not None else closers[i]) + 1
            elif kind == 'goto':
                i = label_at[node.text]
            elif kind == 'return':
                return i, True
            elif kind == 'break':
                i = closers[enclosing_switch(i)] + 1
            else:
                e = i        # the entry keeps the labels that name it
                while e > 0 and flat[e - 1][0] == 'label':
                    e -= 1
                return e, False

    entries = {value: resolve(value) for value in values}
    if not all(resolve(gap)[1] for gap in gaps):
        raise Reject('a value outside the cases runs a body')

    groups = {}          # (entry, terminal) -> values, in ascending value order
    for value in values:
        groups.setdefault(entries[value], []).append(value)
    entry_group = {entry: (entry, terminal) for (entry, terminal) in groups if not terminal}

    def walk(entry):
        """(body nodes, successor group key or None) from an entry index."""
        body, i, seen = [], entry, set()
        while True:
            if i in seen:
                raise Reject('body walk loops')
            seen.add(i)
            if i >= len(flat):
                return body, None
            if i != entry and i in entry_group:
                return body, entry_group[i]
            kind, node = flat[i]
            if kind in ('node', 'label'):
                body.append(node)
                i += 1
            elif kind == 'goto':
                target = label_at[node.text]
                if target in entry_group:
                    return body, entry_group[target]
                if target < i:
                    raise Reject('backward jump at body level to ' + node.text)
                i = target
            elif kind == 'return':
                body.append(node)
                return body, None
            elif kind in ('case', 'endif', 'endswitch'):
                i += 1
            elif kind == 'else':
                i = closers[opener_of[id(node)]] + 1
            elif kind == 'break':
                i = closers[enclosing_switch(i)] + 1
            else:
                raise Reject(f'dispatch node inside a body ({node.text[:60]})')

    bodies, successor = {}, {}
    for key in groups:
        if not key[1]:
            bodies[key], successor[key] = walk(key[0])
    incoming = {}
    for key, nxt in successor.items():
        if nxt is not None:
            incoming[nxt] = incoming.get(nxt, 0) + 1
    heads = [key for key in bodies if key not in incoming]
    if len(heads) != 1 or any(n != 1 for n in incoming.values()):
        raise Reject(f'bodies do not form one chain ({len(heads)} heads)')
    order, key = [], heads[0]
    while key is not None:
        order.append(key)
        key = successor[key]
    if len(order) != len(bodies):
        raise Reject('chain does not visit every body')

    covered = set()
    for key in order:
        for node in bodies[key]:
            if id(node) in covered:
                raise Reject('statement in two bodies')
            covered.add(id(node))
    # labels that only name a dispatch-level return stay with the default
    default_labels = []
    for i, (kind, node) in enumerate(flat):
        if kind == 'return' and id(node) not in covered:
            e = i
            while e > 0 and flat[e - 1][0] == 'label' and id(flat[e - 1][1]) not in covered:
                e -= 1
            for k in range(e, i):
                default_labels.append(flat[k][1])
                covered.add(id(flat[k][1]))
    for kind, node in flat:
        if kind in ('node', 'label') and id(node) not in covered:
            raise Reject(f'uncovered statement ({node.text[:60]})')

    out = render(prefix)
    out.append(f'switch({selector}) {{')
    for key in order:
        for value in groups[key]:
            out.append(f'case {spelling[value]}:')
        out.extend(render(bodies[key], '  '))
    for key, members in groups.items():
        # a case label whose body is a plain return; a comparison constant that returns is the default
        if key[1] and any(value in case_values for value in members):
            for value in members:
                if value in case_values:
                    out.append(f'case {spelling[value]}:')
            out.append('  return;')
    out.append('default:')
    out.extend(render(default_labels, '  '))
    out.append('  return;')
    out.append('}')
    # labels nothing jumps to any more (the dispatch stubs are gone)
    referenced = set(re.findall(r'\bgoto (\w+);', '\n'.join(out)))
    out = [line for line in out if not (re.fullmatch(r'\s*(\w+):', line) and line.strip()[:-1] != 'default'
                                        and line.strip()[:-1] not in referenced)]
    return out, {'values': [spelling[v] for v in values],
                 'chain': [[spelling[v] for v in groups[key]] for key in order], 'prefixStatements': len(prefix)}
