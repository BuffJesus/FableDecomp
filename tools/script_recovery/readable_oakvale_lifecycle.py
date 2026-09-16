"""Represent native destruction in the host ledger, not as a bogus Lua body."""
import hashlib
from tools.script_recovery.native_oakvale_lifecycle import prove

RAW_SHA='f1e11a20191bc717378e8b8b0f0be389c9be2a0a88d253fd65402454fb95ce83'


def lower(source):
    start=source.index('\nfunction destructor(')+1
    if hashlib.sha256(source[start:].encode()).hexdigest()!=RAW_SHA:
        raise ValueError('Oakvale destructor draft changed')
    evidence=prove()
    return source[:start]+'''-- Destruction belongs to LuaQuestHost under nativeLifetime="NewOakValeIntro".
-- The host closes Lua, owned timers and speech vectors, then the native script
-- base. Scalar flags control allocation release. See the C++ lifecycle ledger.
''',dict(status='host-owned native lifecycle',requiredNativeLifetime='NewOakValeIntro',
        implementation='LuaQuestHost::Destructor',evidence=evidence,
        runtimeEvidence='work/oakvale_runtime_build/host-lifecycle-result.json',
        limits='Actual-host engine-double tests passed; live scheduler/unload remains unverified.')
