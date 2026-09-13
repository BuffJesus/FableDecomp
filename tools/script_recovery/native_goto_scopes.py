"""Identify C jumps that Lua can represent without entering a nested lexical scope."""
import re


def supported_jumps(statements):
    scopes, labels, uses = [], {}, []
    serial = 0
    for statement in statements:
        line = statement.strip()
        if line.startswith('}'):
            if not scopes:
                return set(), set()
            scopes.pop()
        label = re.match(r'^(LAB_[0-9a-f]+):\s*(.*)$', line)
        if label:
            if label[1] in labels:
                return set(), set()
            labels[label[1]] = tuple(scopes)
            line = label[2]
        jump = re.search(r'\bgoto (LAB_[0-9a-f]+);\s*$', line)
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
