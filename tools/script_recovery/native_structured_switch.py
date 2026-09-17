"""Lower switches without fall-through to a readable single-iteration loop.

The loop gives case-level break its native destination. A continue targeting an
outer loop would change meaning, so such switches are rejected here and belong
to the general control-flow backend instead.
"""
import re
from tools.script_recovery.native_flat_control import Parser, Lowerer, Node


def free_continue(nodes):
    for node in nodes:
        if node.kind == 'continue':
            return True
        if node.kind not in ('while', 'do', 'for') and (free_continue(node.body) or free_continue(node.other)):
            return True
    return False


def lower_nonfallthrough_switches(statements, allow_fallthrough=False):
    if not any(re.match(r'\s*switch\s*\(', s) for s in statements):
        return statements, []
    try:
        nodes = Parser(statements).block()
        names = Lowerer(nodes, set(re.findall(r'\b\w+\b', '\n'.join(statements))))
        evidence = []
        def transform(items):
            result = []
            for node in items:
                if node.kind != 'switch':
                    result.append(Node(node.kind, node.text, transform(node.body), transform(node.other)))
                    continue
                if free_continue(node.body):
                    raise ValueError('switch contains continue targeting an outer loop')
                groups, labels, body = [], [], []
                for child in node.body:
                    if child.kind == 'case':
                        if body:
                            groups.append((labels, body))
                            labels, body = [], []
                        labels.append(child.text)
                    else:
                        if not labels:
                            raise ValueError('switch has statements before its first case')
                        body.append(child)
                if labels:
                    groups.append((labels, body))
                all_labels = [label for labels, _ in groups for label in labels]
                if len(set(all_labels)) != len(all_labels):
                    raise ValueError('duplicate switch case')
                for index, (labels, body) in enumerate(groups):
                    if any(label and not re.fullmatch(r'-?(?:0x[0-9a-fA-F]+|\d+|[A-Z_]\w*)', label) for label in labels):
                        raise ValueError('unreviewed switch case expression')
                    if not body or not (body[-1].kind in ('break', 'goto') or
                            body[-1].kind == 'atom' and re.match(r'return(?:\s|;)', body[-1].text)):
                        # fall-through into the textually next group: an explicit jump to a label at the start of
                        # that group's body (a sibling block; the goto-scope pass copies the tail at the jump)
                        if not allow_fallthrough:
                            raise ValueError('switch has fall-through or a conditional exit')
                        if index + 1 >= len(groups):
                            continue                # the last group simply leaves the switch
                        target = f'FLOW_case_{len(names.native_labels)}_{index + 1}'
                        names.native_labels[target] = target
                        body.append(Node('goto', target, [], []))
                        groups[index + 1] = (groups[index + 1][0], [Node('label', target, [], [])] + groups[index + 1][1])
                value = names.new_name('native_arg_switch_')
                alternate = []
                defaults = [(labels, body) for labels, body in groups if '' in labels]
                if defaults:
                    if len(defaults[0][0]) != 1:
                        raise ValueError('default grouped with a named case')
                    alternate = transform(defaults[0][1])
                for labels, body in reversed(groups):
                    if '' in labels:
                        continue
                    condition = ' || '.join(f'{value} == {label}' for label in labels)
                    alternate = [Node('if', condition, transform(body), alternate)]
                result.extend([Node('atom', f'{value} = {node.text};'), Node('do', 'false', alternate)])
                evidence.append({'status': 'lowered', 'selector': node.text, 'local': value,
                    'cases': all_labels, 'policy': 'single selector evaluation; no fall-through; no outer-loop continue'})
            return result

        transformed = transform(nodes)
        output = []
        def render(items):
            for node in items:
                kind = node.kind
                if kind == 'atom':
                    output.append(node.text)
                elif kind == 'label':
                    output.append(names.native_labels[node.text] + ':')
                    render(node.body)
                elif kind == 'goto':
                    if node.text not in names.native_labels:
                        raise ValueError('missing native label ' + node.text)
                    output.append('goto ' + names.native_labels[node.text] + ';')
                elif kind in ('break', 'continue'):
                    output.append(kind + ';')
                elif kind == 'if':
                    child = node.body[0] if len(node.body) == 1 else None
                    if child and child.kind in ('goto', 'break') and not node.other:
                        instruction = ('goto ' + names.native_labels[child.text] + ';' if child.kind == 'goto' else 'break;')
                        output.append(f'if ({node.text}) {instruction}')
                    else:
                        output.append(f'if ({node.text}) {{')
                        render(node.body)
                        output.append('}')
                        if node.other:
                            output.append('else {')
                            render(node.other)
                            output.append('}')
                elif kind in ('while', 'for'):
                    output.append(f'{kind} ({node.text}) {{')
                    render(node.body)
                    output.append('}')
                elif kind == 'do':
                    output.append('do {')
                    render(node.body)
                    output.append(f'}} while ({node.text});')
                elif kind == 'block':
                    output.append('{')
                    render(node.body)
                    output.append('}')
                else:
                    raise ValueError('unlowered control node ' + kind)
        render(transformed)
        for item in evidence:
            item['labelMap'] = names.native_labels
        return output, evidence
    except (ValueError, KeyError) as error:
        return statements, [{'status': 'rejected', 'reason': str(error)}]
