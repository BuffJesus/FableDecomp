"""Match the native entity scalar-destructor return ABI."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose
from tools.script_recovery.prepare_oakvale_parent_teardown import once
from tools.script_recovery.native_oakvale_entity_ownership import prove

OUTPUT=ROOT/'work/oakvale_entity_scalar_abi_integration'


def edit(bodies):
    evidence=prove()
    header=once(bodies['LuaEntityHost.h'].decode(),'    void Destructor(bool bDelete);','    void* Destructor(unsigned flags);')
    source=once(bodies['LuaEntityHost.cpp'].decode(),'void LuaEntityHost::Destructor(bool bDelete) {',
        'void* LuaEntityHost::Destructor(unsigned flags) {\n    const bool bDelete=(flags&1u)!=0;')
    source=once(source,'    ReleaseNativeThing();\n}\r\n\r\nvoid LuaEntityHost::Main()',
        '    ReleaseNativeThing();\n    return this;\n}\r\n\r\nvoid LuaEntityHost::Main()')
    bodies['LuaEntityHost.h']=header.encode();bodies['LuaEntityHost.cpp']=source.encode()
    return dict(status='entity scalar destructor returns this and accepts native unsigned flags',native=evidence,
        limits='Host memory remains owned by its custom counted-pointer deleter; this change fixes notification return ABI, not allocation policy.')


def prepare():
    return compose(ROOT/'work/oakvale_entity_callbacks_integration',OUTPUT,edit,'oakvale-entity-scalar-abi.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
