"""Apply evidence-pinned retained-list construction and lifetime operations."""
from tools.script_recovery.native_reviewed_thing_outputs import restore as restore_operations


def restore(source, todo, calls, function, row, rdata, root=None):
    if row is None:
        return source, todo, calls
    source, todo, calls = restore_operations(source, todo, calls, function, row, rdata, root)
    todo = list(todo)
    for diagnostic, count in row.get('resolvedDiagnostics', {}).items():
        if todo.count(diagnostic) != count:
            raise ValueError('reviewed position list: diagnostic count changed')
        todo = [item for item in todo if item != diagnostic]
    return source, todo, calls
