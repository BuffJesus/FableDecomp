"""Fresh exact native OS read-completion callback and independent behavior."""
import native_exact_roots
import hashlib
import tempfile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data
from threaded_read_completion_oracle import compare_completion, R, SOURCE, SYMBOL, ENTRY, SIZE, ADDRESS

ROOTS = [dict(name='read_completion',source=SOURCE,symbol=SYMBOL,address=ENTRY,size=SIZE,instructions=31,relocations=7)]
DATA_SOURCE = R/'rebuild/src/compiled/01/3b/CThreadedFileManager_ErrorState_013bc864.cpp'
DATA = [('?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA',0x13bc864,0,4),
        ('?m_bDirtyDiscError@CThreadedFileManager@@0_NA',0x13bc868,4,1)]


def audit_error_state():
    from pathlib import Path
    source_hash = hashlib.sha256(DATA_SOURCE.read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory(prefix='threaded-error-state-') as tmp:
        obj = Path(tmp)/'state.obj'
        result = V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(DATA_SOURCE)],V.env(),Path(tmp))
        assert result.returncode == 0,result.stdout+result.stderr
        c = Coff(obj)
        sections = set()
        for symbol,address,offset,size in DATA:
            section,actual_offset = c.symbols[symbol]
            assert actual_offset == offset
            sections.add(section)
            payload,relocations = c.sections[section]
            metadata = c.section_metadata[section]
            assert not payload and not relocations
            assert metadata['raw_pointer'] == 0 and metadata['raw_size'] == 5
            assert metadata['characteristics']&0x80000080 == 0x80000080
            assert read_virtual_data(EXE.read_bytes(),address,size) == bytes(size)
        assert len(sections) == 1
        object_hash = hashlib.sha256(obj.read_bytes()).hexdigest()
    assert hashlib.sha256(DATA_SOURCE.read_bytes()).hexdigest() == source_hash
    return dict(status='PASS',data_providers=2,source=str(DATA_SOURCE),source_sha256=source_hash,
                object_sha256=object_hash,layout=[dict(symbol=s,address=hex(a),offset=o,bytes=n) for s,a,o,n in DATA],
                qualification='One writable 5-byte uninitialized COFF contribution, no file payload or relocations; loader zeros match original. Data providers are not function recovery credit.')


def main():
    behavior = compare_completion()
    data = audit_error_state()
    native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-read-completion.json',[behavior,data],
                           'OS read completion; Win32/error callback/installer services controlled, no actual IO or concurrency.')
    print('NATIVE_THREADED_READ_COMPLETION_PASS cases=%d roots=1 data=2 omissions=1'%behavior['cases'])


if __name__ == '__main__':
    main()
