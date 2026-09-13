"""Prepare current, disabled source review bundles without deploying anything."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil

from prepare_meet_sister_release import prepare as prepare_sister
from validate_maze_research_package import validate as validate_maze

ROOT = Path(__file__).resolve().parents[2]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def record(path):
    return {'path': str(path.resolve()), 'sha256': sha(path)}


def prepare(forge, proof, output):
    if output.exists():
        raise ValueError('Output already exists')
    result = json.loads(proof.read_text())
    if not result['passed']:
        raise ValueError('Runtime proof did not pass')
    drift = [item['path'] for item in result['inputs'] if sha(Path(item['path'])) != item['sha256']]
    if drift:
        raise ValueError({'stale_runtime_proof': drift})
    validation = validate_maze(forge)
    if not validation['passed']:
        raise ValueError(validation['checks'])
    installed = Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters')
    installed_dlls = [installed / 'FableScriptExtender.dll', installed / 'FSE/FableScriptExtender.dll']
    before = [record(p) for p in installed_dlls]
    if any(item['sha256'] != 'c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31' for item in before):
        raise ValueError('Installed build differs from reviewed v23')
    baseline = ROOT / 'refs/fse_api_manifest.json'
    proposal_path = ROOT / 'refs/script_recovery/maze_research/runtime_evidence/api-proposal-20260912.json'
    proposal = json.loads(proposal_path.read_text())
    if proposal['baseline_manifest_sha256'] != sha(baseline):
        raise ValueError('Maze metadata proposal baseline has changed; review the delta')
    output.mkdir(parents=True)
    prepare_sister(forge, output / 'meet_sister')
    maze = output / 'maze_research'
    maze.mkdir()
    package = ROOT / 'refs/script_recovery/reconstructed/MazeResearch/FSE'
    manifest = {
        'status': 'disabled-source-package-not-deployable-binary-bundle',
        'native_name': 'V_MazeResearch', 'entity_ids': {'EmptyGrave': 64, 'HistoryBookcase': 65},
        'validation': validation,
        'files': [{'path': p.relative_to(package).as_posix(), 'sha256': sha(p)} for p in sorted(package.rglob('*')) if p.is_file()],
        'runtime_sources': [record(p) for p in sorted((forge / 'FableScriptExtender').iterdir()) if p.suffix in ('.h', '.cpp')],
        'activation': 'disabled; no legacy registration',
        'metadata_dependencies': ['../meet_sister/api-proposal.json', 'api-proposal.json'],
        'persistence_limit': 'RetailFlags and retained Things are transient Quest-owned C++ state; no Lua object crosses VMs; no save/load certification.',
        'live_gates': ['Bookcase cancellation at each journal boundary',
                       'EmptyGrave gates and SAINT/REDEEM macro normal/Escape paths',
                       'Concurrent UNLIMBO task and shared flag visibility',
                       'Streaming, resource lifetime, removal, rewards and cleanup']}
    (maze / 'package-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    shutil.copyfile(proposal_path, maze / 'api-proposal.json')
    shutil.copyfile(ROOT / 'refs/script_recovery/meet_sister/release_review_20260912/README.md', output / 'meet_sister/README.md')
    provenance = {
        'scope': 'Review-only source snapshots; no binary copied or activation performed.',
        'runtime_proof': record(proof), 'proof_inputs_verified': len(result['inputs']),
        'proof_scope': result['scope'], 'installed_v23': before,
        'offline_candidate_dll': record(forge / 'Release/FableScriptExtender.dll'),
        'candidate_limit': 'Identifies the existing offline build only; these manifests do not attest a clean binary rebuild or live acceptance.',
        'build_command': record(ROOT / 'work/maze-runtime-release-20260912/solution-x86-command.json'),
        'build_log': record(ROOT / 'work/maze-runtime-release-20260912/solution-x86-build.log'),
        'shared_manifest': record(baseline), 'preparer': record(Path(__file__))}
    (output / 'runtime-provenance.json').write_text(json.dumps(provenance, indent=2) + '\n')
    if before != [record(p) for p in installed_dlls]:
        raise ValueError('Installed DLLs changed during preparation')
    verification = {'passed': True, 'files': [record(p) for p in sorted(output.rglob('*')) if p.is_file()],
                    'checks': {'runtime_proof_inputs_current': True, 'maze_validation': True,
                               'meet_sister_validation': True, 'metadata_baseline_current': True,
                               'installed_v23_unchanged': True}}
    (output / 'verification.json').write_text(json.dumps(verification, indent=2) + '\n')
    print(json.dumps({'passed': True, 'proof_inputs_current': len(result['inputs']),
                      'maze_files': len(manifest['files']), 'output': str(output)}))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--forge-root', type=Path, required=True)
    parser.add_argument('--runtime-proof', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    prepare(args.forge_root.resolve(), args.runtime_proof.resolve(), args.output.resolve())
