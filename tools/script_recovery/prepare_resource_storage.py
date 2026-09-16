"""Prepare bounded live-entry storage without reusing externally visible IDs."""
import argparse
import difflib
import hashlib
import json
from pathlib import Path


def replace_once(source, old, new, count=1):
    if source.count(old) != count:
        raise ValueError('Resource storage source anchor changed: ' + old[:80])
    return source.replace(old, new)


def bounded_storage(source):
    source = source.replace('\r\n', '\n')
    source = replace_once(source, '#include <vector>', '#include <map>\n#include <limits>')
    source = replace_once(source, '        Kind kind;\n', '        Kind kind;\n        unsigned id = 0;\n')
    source = replace_once(source, 'return static_cast<unsigned>(m_entries.size());', 'return e.id;', 5)
    for method, kind in [('ReleaseResource', 'Resource'), ('DestroyThing', 'Thing'),
                         ('DestroyActorMap', 'ActorMap'), ('DestroyMovie', 'Movie')]:
        source = replace_once(source, f'void {method}(unsigned id) {{ Destroy(Get(id, Kind::{kind})); }}',
                              f'void {method}(unsigned id) {{ Release(id, Kind::{kind}); }}')
    source = replace_once(source, 'if (e->live && e->kind == Kind::Movie)',
                          'if (e.second->live && e.second->kind == Kind::Movie)')
    source = replace_once(source,
        'for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it) Destroy(**it);',
        'for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it) Destroy(*it->second);\n        m_entries.clear();')
    old = '''    Entry& Add(Kind kind) {
        CheckOpen();
        m_entries.emplace_back(std::make_unique<Entry>(kind));
        return *m_entries.back();
    }
    Entry& Get(unsigned id, Kind kind) {
        CheckOpen();
        if (!id || id > m_entries.size() || m_entries[id - 1]->kind != kind || !m_entries[id - 1]->live)
            throw std::runtime_error("Invalid or released retail resource");
        return *m_entries[id - 1];
    }
'''
    source = replace_once(source, old, Path(__file__).with_name('retail_resource_storage.inc').read_text())
    source = replace_once(source, '    std::vector<std::unique_ptr<Entry>> m_entries;',
        '    // IDs never repeat within a scope; erased handles remain invalid.\n'
        '    unsigned m_nextId = 0;\n    std::map<unsigned, std::unique_ptr<Entry>> m_entries;')
    return source


def prepare(runtime, action_proposal, output):
    original_path = Path(runtime) / 'FableScriptExtender/LuaRetailResources.h'
    original = original_path.read_bytes().decode('utf-8')
    action_proposal = Path(action_proposal)
    metadata = json.loads((action_proposal / 'proposal.json').read_text())
    if hashlib.sha256(original_path.read_bytes()).hexdigest() != metadata['sourceSha256']:
        raise ValueError('Runtime changed since action proposal; regenerate it first')
    candidate = bounded_storage((action_proposal / 'LuaRetailResources.h').read_text(encoding='utf-8'))
    if '\r\n' in original:
        candidate = candidate.replace('\n', '\r\n')
    patch = ''.join(difflib.unified_diff(original.splitlines(True), candidate.splitlines(True),
        fromfile='a/FableScriptExtender/LuaRetailResources.h', tofile='b/FableScriptExtender/LuaRetailResources.h'))
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    (output / 'LuaRetailResources.h').write_bytes(candidate.encode('utf-8'))
    (output / 'resource-integration.patch').write_bytes(patch.encode('utf-8'))
    result = {'status': 'proposal-only-not-applied', 'sourceSha256': metadata['sourceSha256'],
              'candidateSha256': hashlib.sha256(candidate.encode()).hexdigest(),
              'methods': metadata['methods'], 'globalBindings': metadata.get('globalBindings', []),
              'storage': 'ordered map of live entries; monotonically increasing non-reused IDs',
              'cleanup': 'explicit release erases storage; Close destroys remaining entries in reverse creation order',
              'exhaustion': 'reject at unsigned maximum rather than wrap and revive stale handles',
              'limitations': ['Full runtime build and in-game validation remain required']}
    (output / 'proposal.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, default=Path('D:/Code/ForgeFSE-retail-shadow'))
    parser.add_argument('--actions', type=Path, default=Path('work/man_resource_extension'))
    parser.add_argument('--out', type=Path, default=Path('work/man_resource_integration'))
    args = parser.parse_args()
    print(json.dumps(prepare(args.runtime, args.actions, args.out), indent=2))
