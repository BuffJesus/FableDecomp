"""Freeze the validated converter/runtime pair for review, without activation."""
import hashlib
import json
import re
import shutil
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def merge_inputs(destination, additions):
    for path, expected in additions.items():
        path=Path(path)
        key=str(path.resolve() if path.is_absolute() else (ROOT/path).resolve())
        if key in destination and destination[key] != expected:
            raise ValueError('Evidence reports disagree about input: '+key)
        destination[key]=expected


def prepare(output=None):
    output = Path(output or ROOT/'work/oakvale_offline_candidate_20260914')
    if output.exists():
        raise FileExistsError('Refusing to overwrite bundle: '+str(output))
    build_dir = ROOT/'work/oakvale_runtime_build'
    package = ROOT/'refs/script_recovery/lifted/NewOakValeIntro/readable'
    stage = ROOT/'work/oakvale_entity_scalar_abi_integration'
    reports = {name: json.loads(path.read_text(encoding='utf-8')) for name, path in {
        'build': build_dir/'result.json',
        'host': build_dir/'host-lifecycle-result.json',
        'inventory': ROOT/'work/oakvale_api_inventory/inventory.json',
        'proposal': stage/'proposal.json',
        'readability': package/'READABILITY_REPORT.json',
        'nativeDeltas': ROOT/'work/oakvale_offline_native_deltas.result.json',
    }.items()}
    build, host, inventory, proposal = (reports[k] for k in ('build','host','inventory','proposal'))
    if build['status'] != 'passed' or host['status'] != 'passed' or inventory['unresolved']:
        raise ValueError('Passing build/host/registration evidence required')
    inputs = {}
    merge_inputs(inputs,build['sourceInputs'])
    merge_inputs(inputs,{str(build_dir/'runtime'/p): h for p,h in build['stagedInputs'].items()})
    merge_inputs(inputs,host['inputs']); merge_inputs(inputs,inventory['inputs'])
    delta=reports['nativeDeltas']
    if delta['exitCode'] != 0:
        raise ValueError('Native delta tests must pass')
    merge_inputs(inputs,delta['inputs'])
    merge_inputs(inputs,{str(ROOT/delta['log']):delta['logSha256'],build['dll']:build['dllSha256'],
        inventory['check']:inventory['checkSha256'],str(build_dir/'actual-host-lifecycle.exe'):host['executableSha256']})
    for path, expected in inputs.items():
        if digest(path) != expected:
            raise ValueError('Validated input changed: '+path)
    records = dict(proposal['additionalSources'])
    records['LuaRetailResources.h'] = proposal
    for name, row in records.items():
        if digest(stage/name) != row['candidateSha256'] or digest(build_dir/'runtime/FableScriptExtender'/name) != row['candidateSha256']:
            raise ValueError('Proposal does not match built runtime: '+name)
    for name, expected in proposal['helpers'].items():
        if digest(stage/name) != expected or digest(build_dir/'runtime/FableScriptExtender'/name) != expected:
            raise ValueError('Proposal helper does not match build: '+name)
    if not re.search(r'Quests\s*=\s*\{\s*\}', (package/'FSE/quests.lua').read_text()):
        raise ValueError('Standalone registration must remain empty')
    names = reports['readability']['mainBindings']['bindingNames']
    if len(names) != 16 or len(set(names)) != 16:
        raise ValueError('Expected sixteen unique native bindings')
    for name in names:
        if not (package/'FSE/NewOakValeIntro/Entities'/f'{name}.lua').is_file():
            raise ValueError('Missing entity: '+name)
    config = ['-- Disabled review bundle; generated from the validated readable package.',
        'RetailOverrides = {', '    enabled = false,',
        '    allowUnverifiedDisposable = false,', '    entries = {{',
        '        nativeName = "Q_NewOakValeIntro",',
        '        file = "NewOakValeIntro/NewOakValeIntro",',
        '        nativeLifetime = "NewOakValeIntro",',
        '        mode = "override",', '        evidenceLevel = "converter-offline-validated",',
        '        mutatingCallsAllowed = true,', '        saveWritesAllowed = true,',
        '        entity_scripts = {']
    for index, name in enumerate(names, 200):
        config.append(f'            {{ name = "{name}", file = "NewOakValeIntro/Entities/{name}", id = {index} }},')
    config += ['        },', '    }},', '}', '']
    # Only validated Lua inputs belong in the payload; exclude runtime logs.
    for source in (package/'FSE').rglob('*.lua'):
        key=str(source.resolve())
        if key not in inputs or digest(source)!=inputs[key]:
            raise ValueError('Unvalidated Lua payload: '+str(source))
        destination=output/'FSE'/source.relative_to(package/'FSE')
        destination.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(source,destination)
    shutil.copyfile(build['dll'], output/'FableScriptExtender.dll')
    (output/'FSE/retail_override.lua').write_text('\n'.join(config), encoding='utf-8')
    from tools.script_recovery.validate_new_oakvale_authority import validate
    reports['authority']=validate(output/'FSE',output/'FSE')
    if not reports['authority']['ok']:
        raise ValueError('Bundle authority validation failed: '+repr(reports['authority']['errors']))
    (output/'README.md').write_text('''# New Oakvale converter runtime candidate

Disabled review bundle. Nothing has been installed or activated.

The DLL includes native Thing ownership, Main/thread registration, frame query
placement, child VM cleanup and process termination before Lua destruction.
Native entity callbacks follow engine dispatch timing, and scalar destructor
notifications return the entity pointer with the native unsigned flag argument.
The Lua package has 51 ledger entries and 18/18 syntax checks. The override keeps
retail quest identity and selects nativeLifetime="NewOakValeIntro"; Quests is empty.

Both full regression suites passed (1,511 and 1,528 tests). Later native deltas and
the complete-host harness passed separately; see evidence/. The host harness checks
281 methods, emitted Init through the virtual callback, persistence, entity factories,
Windows fiber interleavings and shutdown of suspended callbacks.
All sixteen entity files also load in real VMs; their default callbacks and the
emitted Barrel predicate callback are checked through native forwarding instructions.

These are offline results. Live childhood playthrough, save/reload and unload parity
are deferred by the user's offline-only instruction. The mission body is a boundary in the complete-host harness;
its native control-flow and adapter tests are separate evidence.

manifest.json records file hashes and validation inputs. evidence/runtime.patch is
cumulative against the unchanged canonical runtime checkout. Native instruction
fixtures are excluded. Keep this candidate separate from earlier reconstructed ports.
''',encoding='utf-8')
    evidence = output/'evidence'; evidence.mkdir()
    for name, report in reports.items():
        (evidence/(name+'.json')).write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    shutil.copyfile(stage/'oakvale-entity-scalar-abi.patch', evidence/'runtime.patch')
    manifest = dict(schema='oakvale-converter-review-bundle/1', deploymentPerformed=False,
        overrideEnabled=False, standaloneQuestRegistered=False, nativeLifetime='NewOakValeIntro',
        dllSha256=build['dllSha256'], validatedInputs=inputs,
        limits=['Offline runtime/engine-double evidence only.',
                'Real engine scheduler quiescence, restore callback order and gameplay remain unverified.',
                'No retail instruction fixtures included in this bundle.'])
    manifest['files'] = {p.relative_to(output).as_posix():digest(p) for p in sorted(output.rglob('*')) if p.is_file()}
    (output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    return dict(output=str(output), files=len(manifest['files']), dllSha256=build['dllSha256'], overrideEnabled=False)


if __name__ == '__main__':
    print(json.dumps(prepare(),indent=2))
