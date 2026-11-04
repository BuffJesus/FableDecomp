"""Exact installer Pause, dirty-disc storage and genuine scoped-lock behavior."""
import hashlib
import tempfile
from pathlib import Path
import check_one as V
import native_exact_roots
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from installer_pause_oracle import compare_pause,R,SOURCE,SYMBOL,ENTRY,SIZE,ADDRESS

ROOTS = [dict(name='pause',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=SIZE,instructions=34,relocations=6)]
DATA_SOURCE = R/'rebuild/src/compiled/01/3c/CFileInstaller_DirtyDisc_013ca814.cpp'
DATA_SYMBOL = '?DirtyDisc@CFileInstaller@@1_NA'


def audit_dirty_state():
    source_hash = hashlib.sha256(DATA_SOURCE.read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory(prefix='installer-dirty-state-') as tmp:
        obj = Path(tmp)/'state.obj'
        result = V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(DATA_SOURCE)],V.env(),Path(tmp))
        assert result.returncode == 0,result.stdout+result.stderr
        c = Coff(obj);section,offset = c.symbols[DATA_SYMBOL]
        assert offset == 0
        payload,relocations = c.sections[section];metadata = c.section_metadata[section]
        assert not payload and not relocations
        assert metadata['raw_pointer'] == 0 and metadata['raw_size'] == 1
        assert metadata['characteristics']&0x80000080 == 0x80000080
        assert read_virtual_data(EXE.read_bytes(),0x13ca814,1) == b'\0'
        object_hash = hashlib.sha256(obj.read_bytes()).hexdigest()
    assert hashlib.sha256(DATA_SOURCE.read_bytes()).hexdigest() == source_hash
    return dict(status='PASS',data_providers=1,source=str(DATA_SOURCE),source_sha256=source_hash,
                symbol=DATA_SYMBOL,address='013ca814',bytes=1,object_sha256=object_hash,
                qualification='One writable uninitialized COFF byte, no relocations/file payload; loader zero matches original. No function credit for data.')


def main():
    behavior = compare_pause()
    assert behavior['cases'] == 4860 and behavior['providers'] == behavior['missing_provider_checks'] == 4
    data = audit_dirty_state()
    native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-installer-pause.json',[behavior,data],
                           'Actual scoped lock/Release/destructor; Win32 lock/event/wait boundaries controlled, no actual APC or worker.')
    print('NATIVE_INSTALLER_PAUSE_PASS cases=4860 providers=4 omissions=4 grade=MATCH data=1')


if __name__ == '__main__':main()
