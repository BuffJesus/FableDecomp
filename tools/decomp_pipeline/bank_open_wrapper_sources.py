from pathlib import Path
R=Path(__file__).resolve().parents[2]
PARENT=R/'rebuild/build/native-bank-open-file-lifetime/00/98/0098e1e0/20261019T223121838870Z/acceptance.json'
FIXTURE=R/'rebuild/tests/integration/NativeBankOpenWrapperLifetime_test.cpp'
EXTRAS=[R/'rebuild/src/compiled'/n for n in ('00/9d/CBankFile_OpenFileReadOnly_009d1090.cpp','00/9d/CBankFileAsync_OpenFileReadOnly_009d5820.cpp','00/9d/CCountedPointer_DiskFile_Reset_009d1ee0.cpp','00/9c/CBankFile_FileAccessors_009cba40.cpp','00/9c/CBankFile_NotifyBankOpened_009cbc00.cpp','00/99/CDiskFileWin32_Open_0099a6a0.cpp','00/99/CDiskFileWin32_Close_0099A920.cpp','00/9a/CCountedPointer_CThreadedFile_Reset_009a9c80.cpp','00/9a/gapscan_sub009a9040_009a9040.cpp','00/99/CAFile_ConvertPathToFullPath_00999140.cpp','00/99/CCharString_ConversionOperator_0099e4c0.cpp')]
