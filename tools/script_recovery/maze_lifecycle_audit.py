"""Pin the lifecycle audit without treating host ownership differences as parity."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData, ROOT, CLUSTERS


def audit(data=None, runtime=Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender')):
    data = data or RData()
    witness = json.loads(Path(__file__).with_name('maze_lifecycle_witness.json').read_text())
    cluster = json.loads((CLUSTERS / 'V_MazeResearch.json').read_text(encoding='utf-8-sig'))
    for role, digest in witness['sources'].items():
        fn = next(fn for fn in cluster['lifecycle'] if fn['role'] == role)
        if hashlib.sha256(fn['decompile'].encode()).hexdigest() != digest:
            raise ValueError('Maze lifecycle source changed: ' + role)
    for address, value in witness['strings'].items():
        if data.string_at(int(address, 16)) != value:
            raise ValueError('Maze lifecycle literal changed: ' + address)
    for region in witness['native']:
        raw = data.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            raise ValueError('Maze lifecycle native evidence changed: ' + region['name'])
    for item in witness['host']:
        text = (runtime / item['file']).read_text()
        start = text.find(item['start']); end = text.find(item['end'], start + len(item['start']))
        if start < 0 or end < 0 or hashlib.sha256(text[start:end].encode()).hexdigest() != item['sha256']:
            raise ValueError('Maze lifecycle host correspondence changed: ' + item['file'])
    return dict(witness, status='checked evidence; semantic differences remain',
        recovered=[
            'EA7861/EA78C9 register EmptyGrave then HistoryBookcase, flags zero; EA78DE base post-bind precedes interface+100 EA78E8 and objective EA7945. Checked FinalizeEntityBindings preserves that ordering.',
            'EA7750 clears exactly parent SwordTaken+48 and BookRead+49. OnPersist transfers only these fields; transient Sword/map are excluded.',
            'EA81AA..EA81F4 compares old/new Info identity, releases old before storing/incrementing new, skips replacement on identical Info. EA81F4..EA8239 destroys lookup output and key before parent-Sword limbo EA824E.',
            'EA86C6 destroys parent flags; EA86CE destroys Sword; EA86D5 calls base destructor.'],
        integrationGaps=[
            {'id': 'retained-sword-lua-owner', 'requiresHostChange': True,
             'detail': 'Candidate local sword and GetRetainedRetailThing userdata can keep old wrappers alive after parent replacement/destruction. Lua nil/lexical scope does not establish native destruction time. Need quest-owned native slot plus atomic named lookup/assignment and consumers that do not export shared ownership into Lua.'},
            {'id': 'thread-pre-entry-cancellation', 'requiresHostChange': True,
             'detail': 'Host Main/ThreadRunner perform termination checks before entering Lua. Native UnLimbo reads flag first and, if false, yields once before its first cancellation query. Need an opt-in native entry policy preserving existing callers; Lua cannot undo a host-suppressed entry.'},
            {'id': 'flags-and-parent-teardown', 'requiresHostChange': True,
             'detail': 'RetailFlags shared_ptr survives parent destruction when retained in helper Lua or RunMacroWithFlags. Native derived destructor destroys map before Sword/base. Establish scheduler quiescence and borrowed lifetime or an explicit teardown protocol; adding flags to OnPersist is not justified.'},
            {'id': 'engine-restoration-order', 'requiresHostChange': False,
             'detail': 'Field transfer tests do not prove engine ordering of Init/OnPersist, entity activation, thread reconstruction, macro restart or streaming. Requires engine integration evidence.'}])


if __name__ == '__main__':
    result = audit()
    output = ROOT / 'work/maze_converter/LIFECYCLE_AUDIT.json'
    output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'])
