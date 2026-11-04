"""Fresh VC71 parity gate: CConsole private mode helpers (lib_console2 out-of-line copies), ConfirmAlternative, ResetHistory, GetHelpBarString; CConsoleCommandParameters::GetCorrectlyTypedParameter."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??_7CGenericVar@@6B@':0x129c3e4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_ModeHelpers_009ea340.cpp'
ROOTS=[
 dict(name='is_history_mode',source=F,symbol='?IsHistoryMode@CConsole@@ABE_NXZ',address=0x9ea340,size=4,instructions=2,relocations=0),
 dict(name='is_guess_mode',source=F,symbol='?IsGuessMode@CConsole@@ABE_NXZ',address=0x9ea350,size=4,instructions=2,relocations=0),
 dict(name='get_help_bar_string',source=F,symbol='?GetHelpBarString@CConsole@@ABE?AVCCharString@@XZ',address=0x9ea560,size=22,instructions=9,relocations=1),
 dict(name='unset_guess_mode',source=F,symbol='?UnsetGuessMode@CConsole@@AAEXXZ',address=0x9ea580,size=5,instructions=2,relocations=0),
 dict(name='unset_history_mode',source=F,symbol='?UnsetHistoryMode@CConsole@@AAEXXZ',address=0x9ea590,size=5,instructions=2,relocations=0),
 dict(name='confirm_alternative',source=F,symbol='?ConfirmAlternative@CConsole@@AAEXXZ',address=0x9ea600,size=22,instructions=10,relocations=0),
 dict(name='typed_parameter',source=F,symbol='?GetCorrectlyTypedParameter@CConsoleCommandParameters@@QBE?AVCGenericVar@@J@Z',address=0x9ea6c0,size=124,instructions=33,relocations=6),
 dict(name='reset_history',source=F,symbol='?ResetHistory@CConsole@@AAEXXZ',address=0x9eaa40,size=9,instructions=4,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-mode-helpers.json',[],
  'CConsole private mode helpers (lib_console2 out-of-line copies), ConfirmAlternative, ResetHistory, GetHelpBarString; CConsoleCommandParameters::GetCorrectlyTypedParameter')
 print('NATIVE_CONSOLE_MODE_HELPERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
