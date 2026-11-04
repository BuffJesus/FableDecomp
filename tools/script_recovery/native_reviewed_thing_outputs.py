"""Apply evidence-pinned ownership operations to an existing scoped function."""
import hashlib
from pathlib import Path


def restore(source, todo, calls, fn, row, rdata, root=None):
    if row is None:
        return source, todo, calls
    root = Path(root) if root is not None else Path(__file__).resolve().parents[2]

    def require(ok, reason):
        if not ok:
            raise ValueError('reviewed Thing outputs: ' + reason)

    start, end = int(fn['address'], 16), int(fn['bodyEndExclusive'], 16)
    require(start == int(row['address'], 16) and end == int(row['end'], 16), 'extent changed')
    require(hashlib.sha256(fn['decompile'].encode()).hexdigest() == row['sourceSha256'], 'export changed')
    native = rdata.bytes_at(start, end-start)
    require(native is not None and hashlib.sha256(native).hexdigest() == row['nativeSha256'], 'native body changed')
    for address, encoded in row['anchors'].items():
        address, expected = int(address, 16), bytes.fromhex(encoded)
        require(start <= address and address+len(expected) <= end, 'anchor outside function')
        require(rdata.bytes_at(address, len(expected)) == expected, 'cleanup anchor changed')
    for address, entry in row['dependencies'].items():
        native = rdata.bytes_at(int(address, 16), entry['length'])
        require(native is not None and hashlib.sha256(native).hexdigest() == entry['sha256'], 'native dependency changed')
    for entry in row['sidecar'].values():
        require(hashlib.sha256((root/entry['path']).read_bytes()).hexdigest() == entry['sha256'], 'sidecar changed')
    require(hashlib.sha256(source.encode()).hexdigest() == row['intermediateSha256'], 'intermediate changed')
    resolved = row.get('resolvedTodoDiagnostics', [])
    require(isinstance(resolved, list) and all(isinstance(d, str) for d in resolved)
            and len(set(resolved)) == len(resolved), 'invalid resolved diagnostics')
    for diagnostic in resolved:
        require(diagnostic.startswith('unlifted: ') and todo.count(diagnostic) == 1,
                'resolved diagnostic missing or ambiguous')
        require(source.count('-- TODO(native): ' + diagnostic[len('unlifted: '):]) == 1,
                'resolved diagnostic source missing or ambiguous')
    for operation in row['operations']:
        require(source.count(operation['before']) == operation.get('count', 1), 'ownership operation changed')
        source = source.replace(operation['before'], operation['after'])
    require(hashlib.sha256(source.encode()).hexdigest() == row['recoveredSha256'], 'recovered shape changed')
    for diagnostic in resolved:
        require('-- TODO(native): ' + diagnostic[len('unlifted: '):] not in source,
                'resolved diagnostic remains in recovered source')
    remaining = [d for d in todo if d not in resolved] if resolved else todo
    return source, remaining, [*calls, *row['calls']]
