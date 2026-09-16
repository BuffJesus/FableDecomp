"""Record proven cancellation behavior and the remaining engine integration gate."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.maze_teardown_native import validate, execute


def report():
    _,witness=validate()
    output=ROOT/'work/maze_converter/teardown_evidence'
    result={'status':'native cancellation/removal identified; global engine teardown remains unverified',
        'runtimeChanged':False,'nativeEvidence':witness,
        'decompileHashes':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in output.glob('*.c')},
        'execution':{
            'inactive':execute(active=False),
            'oneResume':execute(resumes=1),'fiveResumes':execute(resumes=5),
            'noncooperatingBoundedObservation':execute(resumes=None),
            'namedKill':execute(kind='named')},
        'apiDesign':{
            'candidateNativeDrain':'CScriptBase::KillAllThreads at CB7F60; ECX=script base, no stack arguments.',
            'context':'Known native caller4B3A77 installs current active quest at manager+88 around CB7F60. Reuse the manager-controlled deactivation path; do not add a raw Lua-callable KillAllThreads or call it from the currently executing parent helper.',
            'notADrain':'KillSpawnedFunction(name,id) CB7AA0 sets cancellation only; host CleanupThreads releases Lua registry references only.',
            'composition':'Entry proposal blocks new native entries; native manager drain pumps suspended helpers so native Lua cancellation exits and Sword resources remain alive throughout. Only after process removal and all callbacks return may flags, Sword and Lua be released.',
            'nativeOrder':'Manager drain/removal must precede derived destructor. Maze derived destructor itself destroys flags then Sword then invokes base-storage cleanup. Moving a drain into that late base cleanup cannot protect already destroyed derived members.'},
        'exactIntegrationGate':[
            'Observe all activated Maze teardown paths (normal delayed deactivation, region unload, load/restore, forced shutdown) reaching manager-controlled drain before the Lua host releases its VM or native derived members.',
            'At successful drain return, verify spawned list parent+4 has only its sentinel and entity vector end+0C equals begin+08; process active bytes+4 are zero and cancellation cleanup has returned.',
            'Prove no manager/engine queue can subsequently redispatch removed functions or entities against the freed parent. Known direct-call xrefs do not prove all virtual deletion paths.',
            'Account for macro callbacks and shared flag owners as well as tracked opted-in helper callbacks. NativeEntryCallbacksDrained alone is insufficient.',
            'If a suspended process does not cooperate, native TerminateProcess can keep pumping without bound. Do not free state based on an invented timeout or skip native unwind.',
            'Preserve manager current-quest context and avoid draining the currently executing fiber recursively. Actual engine context is needed to validate this precondition.'],
        'limit':'Native CPU tests execute original termination and named-kill instructions with callback doubles. KillAllThreads container removal and manager/destructor ordering are static native evidence; real fiber scheduling and all deletion routes have not been executed.'}
    (output/'TEARDOWN_REPORT.json').write_text(json.dumps(result,indent=2)+'\n')
    return result


if __name__=='__main__':print(report()['status'])
