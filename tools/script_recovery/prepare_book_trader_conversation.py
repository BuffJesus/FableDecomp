"""Stage and compile a conversation binding without changing the runtime checkout."""
import difflib
import hashlib
import json
import shutil
import subprocess
from pathlib import Path

from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_book_trader_conversation import verify
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment, find_vcvars


def prepare(runtime=Path('D:/Code/ForgeFSE-retail-shadow')):
    source = Path(__file__).parent
    output = ROOT / 'work/book_trader_conversation'
    output.mkdir(parents=True, exist_ok=True)
    adapter = source / 'book_trader_conversation_adapter.inc'
    report = {'status': 'pending integration', 'runtimeChanged': False, 'commands': [],
              'nativeEvidence': verify(RData())}
    replacements = {
        'LuaQuestState.h': ('    void AddLineToConversation(',
            '    int StartConversationWithHero(CScriptThing*, bool, bool);\n'
            '    void AddConversationLineToHero(int, const std::string&, CScriptThing*, bool);\n'),
        'LuaManager.cpp': ('    questState_type["AddLineToConversation"] =',
            '    questState_type["StartConversationWithHero"] = &LuaQuestState::StartConversationWithHero;\n'
            '    questState_type["AddConversationLineToHero"] = &LuaQuestState::AddConversationLineToHero;\n')}
    patch = []
    inputs = [adapter, source / 'book_trader_conversation_harness.cpp',
              source / 'runtime_checks/retail_resources_smoke.cpp']
    for name in ('LuaQuestState.h', 'LuaQuestState.cpp', 'LuaManager.cpp'):
        path = runtime / 'FableScriptExtender' / name
        inputs.append(path)
        original = path.read_bytes().decode('utf-8')
        if 'AddConversationLineToHero' in original or 'StartConversationWithHero' in original:
            raise ValueError('Conversation extension already exists: ' + name)
        newline = '\r\n' if '\r\n' in original else '\n'
        if name in replacements:
            marker, declaration = replacements[name]
            if original.count(marker) != 1:
                raise ValueError('Runtime insertion point changed: ' + name)
            updated = original.replace(marker, declaration.replace('\n', newline) + marker, 1)
        else:
            updated = original + newline + adapter.read_text().replace('\n', newline)
        (output / name).write_bytes(updated.encode('utf-8'))
        patch.extend(difflib.unified_diff(original.splitlines(True), updated.splitlines(True),
            fromfile='a/FableScriptExtender/' + name, tofile='b/FableScriptExtender/' + name))
    patch_path = output / 'conversation-integration.patch'
    patch_path.write_bytes(''.join(patch).encode('utf-8'))
    env = compiler_environment(find_vcvars())
    compiler = shutil.which('cl.exe', path=env['PATH'])
    vendor = runtime / 'Vendor'
    lua = vendor / 'lua'
    lua_sources = sorted(p for p in lua.glob('*.c') if p.name not in ('lua.c', 'luac.c', 'onelua.c'))
    inputs += lua_sources + list((runtime / 'FableScriptExtender').glob('*.h'))
    digest = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
    report['inputs'] = {str(p): digest(p) for p in inputs}
    def run(name, command, cwd=output):
        completed = subprocess.run(command, cwd=cwd, env=env, text=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (output / (name + '.log')).write_text(completed.stdout)
        report['commands'].append({'name': name, 'exitCode': completed.returncode})
        if completed.returncode:
            raise RuntimeError(completed.stdout)
        return completed.stdout.strip()
    run('patch-check', ['git', 'apply', '--check', str(patch_path)], runtime)
    run('lua-build', [compiler, '/nologo', '/c', '/TC', '/MD', '/O2', '/I' + str(lua), *map(str, lua_sources)])
    run('adapter-build', [compiler, '/nologo', '/EHsc', '/std:c++17', '/MD', '/O2',
        '/I' + str(source), '/I' + str(runtime / 'FableScriptExtender'),
        '/I' + str(vendor), '/I' + str(lua), str(source / 'book_trader_conversation_harness.cpp'),
        *[str(output / (p.stem + '.obj')) for p in lua_sources], '/Fe:conversation.exe'])
    report['compiledTest'] = run('adapter-test', [str(output / 'conversation.exe')])
    if any(digest(Path(p)) != expected for p, expected in report['inputs'].items()):
        raise ValueError('Compilation inputs changed during validation')
    report['patchSha256'] = digest(patch_path)
    (output / 'proposal.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


if __name__ == '__main__':
    print(json.dumps(prepare(), indent=2))
