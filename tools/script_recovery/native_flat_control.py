"""Lower structured decompiler control flow to a single label scope.

This pass changes control structure only. Native expressions and effectful
statements remain inputs to the ordinary lifter, including its diagnostics.
Unsupported or unbalanced structures raise before returning partial output.
"""
import re
from dataclasses import dataclass, field
from tools.script_recovery.native_sequence_conditions import _tree
from tools.script_recovery.native_conditions import _unwrap


@dataclass
class Node:
    kind: str
    text: str = ''
    body: list = field(default_factory=list)
    other: list = field(default_factory=list)


def parenthesized(text, prefix):
    """Return the balanced condition and suffix, protecting quoted literals."""
    match = re.match(re.escape(prefix) + r'\s*\(', text)
    if not match:
        raise ValueError('expected ' + prefix + ' condition: ' + text)
    start, depth, quote, escaped = match.end(), 1, None, False
    for i in range(start, len(text)):
        char = text[i]
        if quote:
            if escaped:
                escaped = False
            elif char == '\\':
                escaped = True
            elif char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif char == '(':
            depth += 1
        elif char == ')':
            depth -= 1
            if depth == 0:
                return text[start:i], text[i + 1:].strip()
    raise ValueError('unbalanced ' + prefix + ' condition')


def for_parts(text):
    parts, start, depth, quote, escaped = [], 0, 0, None, False
    for i, char in enumerate(text):
        if quote:
            if escaped:
                escaped = False
            elif char == '\\':
                escaped = True
            elif char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif char in '([':
            depth += 1
        elif char in ')]':
            depth -= 1
        elif char == ';' and depth == 0:
            parts.append(text[start:i].strip())
            start = i + 1
    parts.append(text[start:].strip())
    if len(parts) != 3 or depth or quote:
        raise ValueError('unsupported for header: ' + text)
    return parts


class Parser:
    def __init__(self, statements):
        self.lines = []
        for statement in statements:
            text = statement.strip()
            if not text:
                continue
            if text.startswith('}') and text != '}':
                self.lines.extend(['}', text[1:].strip()])
            else:
                self.lines.append(text)
        self.index = 0

    def peek(self):
        return self.lines[self.index] if self.index < len(self.lines) else None

    def take(self):
        text = self.peek()
        if text is None:
            raise ValueError('unexpected end of control flow')
        self.index += 1
        return text

    def block(self, closing=False):
        nodes = []
        while self.peek() is not None and self.peek() != '}':
            nodes.append(self.node(self.take()))
        if closing:
            if self.take() != '}':
                raise ValueError('expected closing brace')
        elif self.peek() == '}':
            raise ValueError('unmatched closing brace')
        return nodes

    def node(self, text):
        if text == '{':
            return Node('block', body=self.block(True))
        if text == 'do {':
            body = self.block(True)
            condition, tail = parenthesized(self.take(), 'while')
            if tail != ';':
                raise ValueError('unsupported do/while tail')
            return Node('do', condition, body)
        for kind in ('if', 'while', 'for', 'switch'):
            if re.match(kind + r'\s*\(', text):
                condition, tail = parenthesized(text, kind)
                if tail == '{':
                    body = self.block(True)
                elif kind == 'if' and tail and tail.endswith(';'):
                    body = [self.node(tail)]
                else:
                    raise ValueError('unsupported ' + kind + ' body: ' + text)
                other = []
                if kind == 'if' and self.peek() and self.peek().startswith('else'):
                    alternate = self.take()
                    if alternate == 'else {':
                        other = self.block(True)
                    elif alternate.startswith('else if'):
                        other = [self.node(alternate[5:])]
                    else:
                        raise ValueError('unsupported else body')
                return Node(kind, condition, body, other)
        if text.startswith(('else', 'case ')) or text == 'default:':
            if text == 'default:':
                return Node('case', '')
            if text.startswith('case ') and text.endswith(':'):
                return Node('case', text[5:-1].strip())
            raise ValueError('misplaced or unsupported branch: ' + text)
        label = re.match(r'^([A-Za-z_]\w*):(?!:)\s*(.*)$', text)
        if label:
            body = [self.node(label[2])] if label[2] else []
            return Node('label', label[1], body)
        if text in ('break;', 'continue;'):
            return Node(text[:-1])
        jump = re.fullmatch(r'goto ([A-Za-z_]\w*);', text)
        if jump:
            return Node('goto', jump[1])
        if text.endswith(';') or text.startswith('@@'):
            return Node('atom', text)
        raise ValueError('unrecognized control statement: ' + text)


class Lowerer:
    def __init__(self, nodes, reserved):
        self.nodes, self.reserved = nodes, set(reserved)
        self.serial, self.lines, self.labels = 0, [], {}
        self.native_labels = {}
        self.switch_values = []
        self.unresolved_conditions = []
        def collect(items):
            for node in items:
                if node.kind == 'label':
                    if node.text in self.native_labels:
                        raise ValueError('duplicate native label ' + node.text)
                    self.native_labels[node.text] = (node.text if re.fullmatch(r'LAB_[0-9a-f]+', node.text)
                                                     else self.new_label('native_label'))
                collect(node.body)
                collect(node.other)
        collect(nodes)

    def new_name(self, prefix):
        while True:
            self.serial += 1
            name = prefix + str(self.serial)
            if name not in self.reserved:
                self.reserved.add(name)
                return name

    def new_label(self, reason):
        name = self.new_name('FLOW_' + reason + '_')
        self.labels[name] = reason
        return name

    def emit(self, text):
        self.lines.append(text)

    def decision(self, condition, yes, no):
        condition = _unwrap(condition)
        if condition.startswith('!') and not condition.startswith('!='):
            return self.decision(condition[1:].strip(), no, yes)
        try:
            tree = _tree(condition)
        except ValueError as error:
            self.unresolved_conditions.append({'expression': condition, 'reason': str(error)})
            tree = ('leaf', condition)
        self.decision_tree(tree, yes, no)

    def decision_tree(self, tree, yes, no):
        kind = tree[0]
        if kind == 'sequence':
            for effect in tree[1]:
                self.emit(effect + ';')
            self.decision_tree(tree[2], yes, no)
        elif kind in ('&&', '||'):
            middle = self.new_label('condition')
            self.decision_tree(tree[1], middle if kind == '&&' else yes,
                               no if kind == '&&' else middle)
            self.emit(middle + ':')
            self.decision_tree(tree[2], yes, no)
        elif kind == 'leaf' and _unwrap(tree[1]).startswith('!'):
            self.decision(tree[1], yes, no)
        else:
            self.emit(f'if ({tree[1]}) goto {yes};')
            self.emit(f'goto {no};')

    def block(self, nodes, break_to=None, continue_to=None):
        for node in nodes:
            kind = node.kind
            if kind == 'atom':
                self.emit(node.text)
            elif kind == 'label':
                self.emit(self.native_labels[node.text] + ':')
                self.block(node.body, break_to, continue_to)
            elif kind == 'goto':
                if node.text not in self.native_labels:
                    raise ValueError('missing native jump target ' + node.text)
                self.emit('goto ' + self.native_labels[node.text] + ';')
            elif kind in ('break', 'continue'):
                target = break_to if kind == 'break' else continue_to
                if target is None:
                    raise ValueError(kind + ' outside its control structure')
                self.emit('goto ' + target + ';')
            elif kind == 'block':
                self.block(node.body, break_to, continue_to)
            elif kind == 'if':
                entry = self.new_label('then')
                alternate = self.new_label('else')
                finish = self.new_label('endif') if node.other else alternate
                self.decision(node.text, entry, alternate)
                self.emit(entry + ':')
                self.block(node.body, break_to, continue_to)
                if node.other:
                    self.emit('goto ' + finish + ';')
                    self.emit(alternate + ':')
                    self.block(node.other, break_to, continue_to)
                self.emit(finish + ':')
            elif kind in ('while', 'do', 'for'):
                head, step, finish = (self.new_label(name) for name in ('loop', 'loop_step', 'loop_end'))
                condition = node.text
                initializer, increment = '', ''
                if kind == 'for':
                    initializer, condition, increment = for_parts(node.text)
                    if initializer:
                        self.emit(initializer + ';')
                self.emit(head + ':')
                if kind != 'do' and condition:
                    entry = self.new_label('loop_body')
                    self.decision(condition, entry, finish)
                    self.emit(entry + ':')
                self.block(node.body, finish, step)
                self.emit(step + ':')
                if increment:
                    self.emit(increment + ';')
                if kind == 'do':
                    self.decision(condition, head, finish)
                else:
                    self.emit('goto ' + head + ';')
                self.emit(finish + ':')
            elif kind == 'switch':
                finish = self.new_label('switch_end')
                value = self.new_name('native_arg_switch_')
                self.switch_values.append(value)
                cases = [(i, child.text, self.new_label('case')) for i, child in enumerate(node.body) if child.kind == 'case']
                if len({case for _, case, _ in cases}) != len(cases):
                    raise ValueError('duplicate switch case')
                self.emit(f'{value} = {node.text};')
                default = finish
                for _, case, label in cases:
                    if case:
                        self.emit(f'if ({value} == {case}) goto {label};')
                    else:
                        default = label
                self.emit('goto ' + default + ';')
                case_labels = {i: label for i, _, label in cases}
                for i, child in enumerate(node.body):
                    if i in case_labels:
                        self.emit(case_labels[i] + ':')
                    else:
                        self.block([child], finish, continue_to)
                self.emit(finish + ':')
            else:
                raise ValueError('case label outside switch')


def flatten_control(statements):
    nodes = Parser(statements).block()
    reserved = set(re.findall(r'\b\w+\b', '\n'.join(statements)))
    lowerer = Lowerer(nodes, reserved)
    lowerer.block(nodes)
    used = set(re.findall(r'\bgoto (\w+);', '\n'.join(lowerer.lines)))
    unused = set(lowerer.labels) - used
    lines = [line for line in lowerer.lines if not (line.endswith(':') and line[:-1] in unused)]
    return lines, {'syntheticLabels': lowerer.labels, 'unusedSyntheticLabels': sorted(unused),
        'nativeLabels': lowerer.native_labels, 'switchLocals': lowerer.switch_values,
        'unresolvedConditions': lowerer.unresolved_conditions,
        'limitations': ['experimental control structure only; native operands and ownership remain separately unresolved',
                       'operand alias/staging data flow across the new joins must be validated before promotion'],
        'inputStatements': len(statements), 'outputStatements': len(lines)}
