import sys,json,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from gate_game_component_abi import Coff
from variable_allocator_parity import check
w=Path(tempfile.mkdtemp(prefix='installer-production-'));env=check_one.env();obj=w/'destructor.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/CFileInstaller_Destructor_009c5170.cpp')],env,w);assert r.returncode==0,r.stdout+r.stderr
c=Coff(obj);symbol='??1CFileInstaller@@UAE@XZ';section,offset=c.symbols[symbol];body,relocs=c.sections[section]
# Relocation identities are verified by the full-body check below.
targets={'??_7CFileInstaller@@6B@':0x129b43c,'??_7?$CTBaseSingleton@VCFileInstaller@@@@6B@':0x129b414,'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,'??1CCriticalSectionLock@@QAE@XZ':0xa629a0,'?Release@CCriticalSectionLock@@QAEXXZ':0xa62960,'?Lock@CCriticalSectionLock@@QAEXXZ':0xa62980,'?ClearBuffers@CFileInstaller@@QAEXXZ':0x9c5110,'__imp__SetEvent@4':0x143fde0,'__imp__Sleep@4':0x143fe1c,'__imp__CloseHandle@4':0x143fe30,'__imp__DeleteCriticalSection@4':0x143fde4,'?Erase@CBasicRetailBankAliasMap@@QAEXPAUCNode@1@@Z':0x9aa830,'?Erase@InstallerCompletedFileSet@@QAEXPAUNode@1@@Z':0x4aba70,'?Clear@InstallerActiveFileList@@QAEXXZ':0x9c71a0,'?Clear@InstallerCountedFileList@@QAEXXZ':0x9c6cc0,'?FableInstallerDestroyBufferRange@@YIXPAUInstallerBufferChunk@@0PAX@Z':0x9c6d60,'_free':0xbfea14}
report=check(obj,symbol,0x9c5170,410,len(relocs),targets)
(ROOT/'rebuild/compile-gate/installer-destructor-parity.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
deleting=check(obj,'??_GCFileInstaller@@UAEPAXI@Z',0x9c7900,30,2,{'??1CFileInstaller@@UAE@XZ':0x9c5170,'??3@YAXPAX@Z':0xbfe9bc})
(ROOT/'rebuild/compile-gate/installer-deleting-destructor-parity.json').write_text(json.dumps(deleting,indent=2)+'\n');print(json.dumps(deleting,indent=2))

base = check(obj, '??_G?$CTBaseSingleton@VCFileInstaller@@@@UAEPAXI@Z',
             0x9c5b60, 31, 2,
             {'??_7?$CTBaseSingleton@VCFileInstaller@@@@6B@': 0x129b414,
              '??3@YAXPAX@Z': 0xbfe9bc})
(ROOT/'rebuild/compile-gate/installer-singleton-base-parity.json').write_text(json.dumps(base,indent=2)+'\n')
