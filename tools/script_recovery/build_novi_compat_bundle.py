"""Reproduce the stock-FSE + NoviCompatibility sidecar bundle that runs the converter's readable Lua.

Pipeline (each step is what produced work/new-oakvale-original-fse-20260912/local-candidate-v3 on 2026-09-14):
  1. Copy the canonical ForgeFSE-retail-shadow source tree (working tree, not Release/.git).
  2. Apply work/oakvale_entity_scalar_abi_integration/oakvale-entity-scalar-abi.patch (the complete
     retail-resource / entity-scalar ABI implementation). It applies cleanly ONLY to that tree.
  3. Re-apply the three sidecar deltas that turn ForgeFSE into a coexisting add-on DLL:
       - FableAPI.cpp: script/log base path "<dll dir>/NoviCompatibility" instead of "/FSE"
       - dllmain.cpp: disable InstallMapResourceAliasHook / LoadStartupMapResourceAlias /
         RetailScriptShadowRunner::RunConfigured, and replace MyHook/InstallHook/DllMain with
         NoviCompatibilityChain.inl (chains behind stock FSE's 0xCDB355 hook, exports NoviCompatibilityStart)
  4. MSBuild Release|x86 (dash-style switches: Git Bash mangles "/p:" into paths).
  5. Assemble the bundle: stock FableScriptExtender.dll + empty FSE/quests.lua + NoviLocalLauncher.exe +
     local_test.py + NoviCompatibility.dll + NoviCompatibility/ = the converter readable package verbatim
     (quests.lua registers only PartyMode; NewOakValeIntro is loaded through retail_override.lua, which
     replaces the native Q_NewOakValeIntro allocator instead of adding a duplicate custom quest).

Never edit the readable Lua here. If the Lua needs a binding the DLL lacks, fix the DLL.
"""
import argparse, hashlib, json, pathlib, re, shutil, subprocess, sys

REPO = pathlib.Path(__file__).resolve().parents[2]
WORK = REPO / 'work' / 'new-oakvale-original-fse-20260912'
MSBUILD = pathlib.Path(r'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe')


def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()


def stage_source(shadow, patch, out):
    if out.exists(): shutil.rmtree(out)
    (out / 'FableScriptExtender').mkdir(parents=True)
    shutil.copy2(shadow / 'FableScriptExtender.sln', out)
    for p in (shadow / 'FableScriptExtender').iterdir():
        if p.is_file() and p.suffix in {'.cpp', '.h', '.rc', '.vcxproj', '.filters', '.inl'}:
            shutil.copy2(p, out / 'FableScriptExtender' / p.name)
    shutil.copytree(shadow / 'Vendor', out / 'Vendor')
    subprocess.run(['git', 'init', '-q', '.'], cwd=out, check=True)
    subprocess.run(['git', 'apply', str(patch)], cwd=out, check=True)


def apply_sidecar_deltas(out, sidecar_src):
    root = out / 'FableScriptExtender'
    p = root / 'FableAPI.cpp'; s = p.read_text(encoding='utf-8', errors='replace')
    anchor = 'std::string(dllPath) + "/FSE"'
    assert s.count(anchor) == 1, 'FSE base-path anchor'
    p.write_text(s.replace(anchor, 'std::string(dllPath) + "/NoviCompatibility"'), encoding='utf-8')

    p = root / 'dllmain.cpp'; s = p.read_text(encoding='utf-8', errors='replace')
    for call in ['InstallMapResourceAliasHook();', 'LoadStartupMapResourceAlias();',
                 'RetailScriptShadowRunner::RunConfigured(g_fseBasePath);']:
        pat = re.compile(r'^(\s*)' + re.escape(call) + r'\s*$', re.M)
        assert len(pat.findall(s)) == 1, ('anchor', call)
        s = pat.sub(lambda m: m.group(1) + '/* Local compatibility build: disabled ' + call + ' */', s, count=1)
    s = s[:s.index('void __declspec(naked) MyHook()')] + '#include "NoviCompatibilityChain.inl"\n'
    p.write_text(s, encoding='utf-8')
    shutil.copy2(sidecar_src / 'FableScriptExtender' / 'NoviCompatibilityChain.inl', root / 'NoviCompatibilityChain.inl')


def build(out):
    log = out.parent / (out.name + '-build.log')
    r = subprocess.run([str(MSBUILD), str(out / 'FableScriptExtender.sln'), '-t:Rebuild',
                        '-p:Configuration=Release', '-p:Platform=x86', '-m', '-nologo', '-v:m'],
                       capture_output=True, text=True)
    log.write_text(r.stdout + r.stderr)
    if r.returncode: sys.exit('msbuild failed, see ' + str(log))
    dll = out / 'Release' / 'FableScriptExtender.dll'
    data = dll.read_bytes()
    for needle in [b'NoviCompatibilityStart', b'TurnOakvaleHeroIntoChild', b'/NoviCompatibility']:
        assert needle in data, needle
    return dll


def assemble(bundle, template, readable, dll):
    if bundle.exists(): shutil.rmtree(bundle)
    bundle.mkdir()
    for n in ['FableScriptExtender.dll', 'NoviLocalLauncher.exe', 'local_test.py', 'README.md']:
        shutil.copy2(template / n, bundle / n)
    (bundle / 'FSE').mkdir(); (bundle / 'FSE' / 'quests.lua').write_text('Quests = {}\n')
    shutil.copy2(dll, bundle / 'NoviCompatibility.dll')
    shutil.copytree(readable, bundle / 'NoviCompatibility')
    m = json.loads((template / 'manifest.json').read_text())
    m['files'] = {p.relative_to(bundle).as_posix(): sha(p) for p in sorted(bundle.rglob('*')) if p.is_file() and p.suffix != '.log'}
    m['lua_source'] = str(readable); m['sidecar_source'] = str(dll.parents[1])
    (bundle / 'manifest.json').write_text(json.dumps(m, indent=2))
    for p in readable.rglob('*.lua'):
        assert sha(p) == sha(bundle / 'NoviCompatibility' / p.relative_to(readable)), p


def main():
    a = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    a.add_argument('--shadow', type=pathlib.Path, default=pathlib.Path(r'D:\Code\ForgeFSE-retail-shadow'))
    a.add_argument('--patch', type=pathlib.Path, default=REPO / 'work/oakvale_entity_scalar_abi_integration/oakvale-entity-scalar-abi.patch')
    a.add_argument('--sidecar-src', type=pathlib.Path, default=WORK / 'sidecar-source')
    a.add_argument('--readable', type=pathlib.Path, default=REPO / 'work/oakvale_readable_playtest_fix2_20260915/FSE')
    a.add_argument('--template', type=pathlib.Path, default=WORK / 'local-candidate-v2')
    a.add_argument('--out-source', type=pathlib.Path, default=WORK / 'sidecar-abi-v2')
    a.add_argument('--bundle', type=pathlib.Path, default=WORK / 'local-candidate-v3')
    a.add_argument('--skip-build', action='store_true')
    args = a.parse_args()
    if not args.skip_build:
        stage_source(args.shadow, args.patch, args.out_source)
        apply_sidecar_deltas(args.out_source, args.sidecar_src)
        dll = build(args.out_source)
    else:
        dll = args.out_source / 'Release' / 'FableScriptExtender.dll'
    assemble(args.bundle, args.template, args.readable, dll)
    print('bundle ready:', args.bundle)
    print('launch: python', args.bundle / 'local_test.py', '--game-dir "<Fable dir>" --launch --save-dir "<saves>"')


if __name__ == '__main__':
    main()
