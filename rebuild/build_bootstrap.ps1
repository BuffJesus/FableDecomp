param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$workspaceRoot = Split-Path -Parent $rebuildRoot
$vcRoot = 'D:\Tools\vc71'
$windowsSdkLibRoot = 'C:\Program Files (x86)\Windows Kits\10\Lib'
$outDir = Join-Path $rebuildRoot "build\bootstrap-$Configuration"
$bootstrapSource = Join-Path $rebuildRoot 'integration\bootstrap_main.cpp'
$retailSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_MemCmpUnsigned16_00403c60.cpp'
$winMainSource = Join-Path $rebuildRoot 'src\compiled\00\40\Global_WinMain_00403480.cpp'
$winMainBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\Global_WinMain_00403480_test.cpp'
$stage1BoundarySource = Join-Path $rebuildRoot 'integration\stage1_engine_boundary.cpp'
$progressSetupSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GFInitialiseSetupProgressDisplay_00413120.cpp'
$progressSetupBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GFInitialiseSetupProgressDisplay_00413120_test.cpp'
$setCurrentPathSource = Join-Path $rebuildRoot 'src\compiled\00\99\CAFile_SetCurrentPath_009974f0.cpp'
$setCurrentPathBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CAFile_SetCurrentPath_009974f0_test.cpp'
$getProjectPathSource = Join-Path $rebuildRoot 'src\compiled\00\99\CAFile_GetProjectPath_00997510.cpp'
$getProjectPathBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CAFile_GetProjectPath_00997510_test.cpp'
$wideStringConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_Constructor_0099aed0.cpp'
$wideStringConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_Constructor_0099aed0_test.cpp'
$wideStringDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_Destructor_0099b510.cpp'
$wideStringDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_Destructor_0099b510_test.cpp'
$charStringConstructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_Constructor_0099ebf0.cpp'
$charStringConstructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_Constructor_0099ebf0_test.cpp'
$charStringDestructorSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_Destructor_0099eae0.cpp'
$charStringDestructorBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_Destructor_0099eae0_test.cpp'
$profileStartSource = Join-Path $rebuildRoot 'src\compiled\00\9d\NProfileTimer_StartProfile_009d8240.cpp'
$profileStartBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\NProfileTimer_StartProfile_009d8240_test.cpp'
$profileEndSource = Join-Path $rebuildRoot 'src\compiled\00\9d\NProfileTimer_EndProfile_009d8250.cpp'
$profileEndBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\NProfileTimer_EndProfile_009d8250_test.cpp'
$asyncFailureHandlingSource = Join-Path $rebuildRoot 'src\compiled\00\9d\CBankFileAsync_SetEnableFailureHandling_009d5240.cpp'
$asyncFailureHandlingBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\CBankFileAsync_SetEnableFailureHandling_009d5240_test.cpp'
$startupLatchSource = Join-Path $rebuildRoot 'src\compiled\00\9d\Global_ClearStartupLatch_009d81e0.cpp'
$startupLatchBehaviorSource = Join-Path $rebuildRoot 'tests\00\9d\Global_ClearStartupLatch_009d81e0_test.cpp'
$fileInstallerGetSource = Join-Path $rebuildRoot 'src\compiled\00\40\CFileInstallerSingleton_Get_00404440.cpp'
$fileInstallerGetBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CFileInstallerSingleton_Get_00404440_test.cpp'
$defaultLanguageNameSource = Join-Path $rebuildRoot 'src\compiled\00\41\Global_GetDefaultLanguageName_00415530.cpp'
$defaultLanguageNameBehaviorSource = Join-Path $rebuildRoot 'tests\00\41\Global_GetDefaultLanguageName_00415530_test.cpp'
$wideStringFromCharSource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_CreateFromCharString_0099b6a0.cpp'
$wideStringFromCharBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_CreateFromCharString_0099b6a0_test.cpp'
$wideStringCopySource = Join-Path $rebuildRoot 'src\compiled\00\99\CWideString_CopyConstructor_0099b720.cpp'
$wideStringCopyBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CWideString_CopyConstructor_0099b720_test.cpp'
$charToWideStringSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_ToWideString_0099e480.cpp'
$charToWideStringBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_ToWideString_0099e480_test.cpp'
$primaryLeftAlignmentSource = Join-Path $rebuildRoot 'src\compiled\00\9b\TextLayout_SetPrimaryLeftAlignment_009bc890.cpp'
$primaryLeftAlignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\9b\TextLayout_SetPrimaryLeftAlignment_009bc890_test.cpp'
$secondaryLeftAlignmentSource = Join-Path $rebuildRoot 'src\compiled\00\9b\TextLayout_SetSecondaryLeftAlignment_009bc8a0.cpp'
$secondaryLeftAlignmentBehaviorSource = Join-Path $rebuildRoot 'tests\00\9b\TextLayout_SetSecondaryLeftAlignment_009bc8a0_test.cpp'
$cbaseRestoreASource = Join-Path $rebuildRoot 'src\compiled\00\99\CBase_RestoreVTable_0099a2e0.cpp'
$cbaseRestoreABehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CBase_RestoreVTable_0099a2e0_test.cpp'
$cbaseRestoreBSource = Join-Path $rebuildRoot 'src\compiled\00\99\CBase_RestoreVTable_0099a300.cpp'
$cbaseRestoreBBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CBase_RestoreVTable_0099a300_test.cpp'
$charStringDefaultSource = Join-Path $rebuildRoot 'src\compiled\00\99\CCharString_DefaultConstructor_0099e4b0.cpp'
$charStringDefaultBehaviorSource = Join-Path $rebuildRoot 'tests\00\99\CCharString_DefaultConstructor_0099e4b0_test.cpp'
$systemManagerInitSource = Join-Path $rebuildRoot 'src\compiled\00\40\CSystemManagerInit_Constructor_00403b10.cpp'
$systemManagerInitBehaviorSource = Join-Path $rebuildRoot 'tests\00\40\CSystemManagerInit_Constructor_00403b10_test.cpp'
$gfmainPhase1Source = Join-Path $rebuildRoot 'integration\gfmain_phase1.cpp'
$gfmainPhase2Source = Join-Path $rebuildRoot 'integration\gfmain_phase2.cpp'
$stage2BoundarySource = Join-Path $rebuildRoot 'integration\stage2_engine_boundary.cpp'
$visualBootSource = Join-Path $rebuildRoot 'integration\visual_boot_checkpoint.cpp'
$visualBootArtwork = Join-Path $rebuildRoot 'assets\boot\fabledecomp_boot_concept.png'
$visualBootBehaviorSource = Join-Path $rebuildRoot 'tests\integration\VisualBootCheckpoint_test.cpp'
$gfmainPhase1BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase1_test.cpp'
$gfmainPhase2BehaviorSource = Join-Path $rebuildRoot 'tests\integration\GFMain_Phase2_test.cpp'
$bootObjectChecker = Join-Path $workspaceRoot 'tools\check_boot_object.py'
$bootstrapObject = Join-Path $outDir 'bootstrap_main.obj'
$retailObject = Join-Path $outDir 'retail_00403c60.obj'
$winMainObject = Join-Path $outDir 'retail_winmain.obj'
$winMainBehaviorObject = Join-Path $outDir 'winmain_behavior.obj'
$stage1BoundaryObject = Join-Path $outDir 'stage1_engine_boundary.obj'
$progressSetupObject = Join-Path $outDir 'gfinitialise_setup_progress.obj'
$progressSetupBehaviorObject = Join-Path $outDir 'gfinitialise_setup_progress_behavior.obj'
$setCurrentPathObject = Join-Path $outDir 'set_current_path.obj'
$setCurrentPathBehaviorObject = Join-Path $outDir 'set_current_path_behavior.obj'
$getProjectPathObject = Join-Path $outDir 'get_project_path.obj'
$getProjectPathBehaviorObject = Join-Path $outDir 'get_project_path_behavior.obj'
$wideStringConstructorObject = Join-Path $outDir 'wide_string_constructor.obj'
$wideStringConstructorBehaviorObject = Join-Path $outDir 'wide_string_constructor_behavior.obj'
$wideStringDestructorObject = Join-Path $outDir 'wide_string_destructor.obj'
$wideStringDestructorBehaviorObject = Join-Path $outDir 'wide_string_destructor_behavior.obj'
$charStringConstructorObject = Join-Path $outDir 'char_string_constructor.obj'
$charStringConstructorBehaviorObject = Join-Path $outDir 'char_string_constructor_behavior.obj'
$charStringDestructorObject = Join-Path $outDir 'char_string_destructor.obj'
$charStringDestructorBehaviorObject = Join-Path $outDir 'char_string_destructor_behavior.obj'
$profileStartObject = Join-Path $outDir 'profile_start.obj'
$profileStartBehaviorObject = Join-Path $outDir 'profile_start_behavior.obj'
$charStringDefaultObject = Join-Path $outDir 'char-string-default-constructor.obj'
$systemManagerInitObject = Join-Path $outDir 'system-manager-init-constructor.obj'
$gfmainPhase1Object = Join-Path $outDir 'gfmain_phase1.obj'
$gfmainPhase2Object = Join-Path $outDir 'gfmain_phase2.obj'
$stage2BoundaryObject = Join-Path $outDir 'stage2_engine_boundary.obj'
$stage3BoundaryObject = Join-Path $outDir 'stage3_engine_boundary.obj'
$visualBoundaryObject = Join-Path $outDir 'visual_engine_boundary.obj'
$visualBootObject = Join-Path $outDir 'visual_boot_checkpoint.obj'
$visualBootBehaviorObject = Join-Path $outDir 'visual_boot_checkpoint_behavior.obj'
$visualBootBitmap = Join-Path $outDir 'fabledecomp_boot_concept.bmp'
$visualBootResourceSource = Join-Path $outDir 'visual_boot_checkpoint.rc'
$visualBootResource = Join-Path $outDir 'visual_boot_checkpoint.res'
$gfmainPhase1BehaviorObject = Join-Path $outDir 'gfmain_phase1_behavior.obj'
$gfmainPhase2BehaviorObject = Join-Path $outDir 'gfmain_phase2_behavior.obj'
$profileEndObject = Join-Path $outDir 'profile-end.obj'
$asyncFailureHandlingObject = Join-Path $outDir 'async-failure-handling.obj'
$startupLatchObject = Join-Path $outDir 'startup-latch-clear.obj'
$fileInstallerGetObject = Join-Path $outDir 'cfi-singleton-get.obj'
$executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage0.exe'
$winMainBehaviorExecutable = Join-Path $outDir 'FableTLC-WinMain-Behavior.exe'
$stage1Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage1.exe'
$progressSetupBehaviorExecutable = Join-Path $outDir 'FableTLC-ProgressDisplay-Behavior.exe'
$setCurrentPathBehaviorExecutable = Join-Path $outDir 'FableTLC-SetCurrentPath-Behavior.exe'
$getProjectPathBehaviorExecutable = Join-Path $outDir 'FableTLC-GetProjectPath-Behavior.exe'
$wideStringConstructorBehaviorExecutable = Join-Path $outDir 'FableTLC-WideStringConstructor-Behavior.exe'
$wideStringDestructorBehaviorExecutable = Join-Path $outDir 'FableTLC-WideStringDestructor-Behavior.exe'
$charStringConstructorBehaviorExecutable = Join-Path $outDir 'FableTLC-CharStringConstructor-Behavior.exe'
$charStringDestructorBehaviorExecutable = Join-Path $outDir 'FableTLC-CharStringDestructor-Behavior.exe'
$profileStartBehaviorExecutable = Join-Path $outDir 'FableTLC-ProfileStart-Behavior.exe'
$gfmainPhase1BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase1-Behavior.exe'
$gfmainPhase2BehaviorExecutable = Join-Path $outDir 'FableTLC-GFMainPhase2-Behavior.exe'
$stage2Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage2.exe'
$stage3Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage3.exe'
$visualCheckpointExecutable = Join-Path $outDir 'FableTLC-Reconstruction-VisualCheckpoint.exe'
$visualBootBehaviorExecutable = Join-Path $outDir 'FableTLC-VisualBoot-Behavior.exe'
$passPattern = 'FABLETLC_BOOTSTRAP_STAGE0 PASS'
$winMainPassPattern = 'FABLETLC_WINMAIN_BEHAVIOR PASS'
$progressSetupPassPattern = 'FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS'
$setCurrentPathPassPattern = 'FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS'
$getProjectPathPassPattern = 'FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS'
$wideStringConstructorPassPattern = 'FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS'
$wideStringDestructorPassPattern = 'FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS'
$charStringConstructorPassPattern = 'FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS'
$charStringDestructorPassPattern = 'FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS'
$profileStartPassPattern = 'FABLETLC_PROFILE_START_BEHAVIOR PASS'
$charStringDefaultPassPattern = 'FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS'
$systemManagerInitPassPattern = 'FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS'
$gfmainPhase1PassPattern = 'FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS'
$gfmainPhase2PassPattern = 'FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS'
$visualBootPassPattern = 'FABLETLC_VISUAL_BOOT_BEHAVIOR PASS'
$profileEndPassPattern = 'FABLETLC_PROFILE_END_BEHAVIOR PASS'
$asyncFailureHandlingPassPattern = 'FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS'
$startupLatchPassPattern = 'FABLETLC_STARTUP_LATCH_BEHAVIOR PASS'
$fileInstallerGetPassPattern = 'FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS'
$defaultLanguageNamePassPattern = 'FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS'
$wideStringFromCharPassPattern = 'FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS'
$wideStringCopyPassPattern = 'FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS'
$charToWideStringPassPattern = 'FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS'
$primaryLeftAlignmentPassPattern = 'FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS'
$secondaryLeftAlignmentPassPattern = 'FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS'
$cbaseRestoreAPassPattern = 'FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS'
$cbaseRestoreBPassPattern = 'FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS'

$required = @(
    (Join-Path $vcRoot 'bin\cl.exe'),
    (Join-Path $vcRoot 'bin\link.exe'),
    $bootstrapSource,
    $retailSource,
    $winMainSource,
    $winMainBehaviorSource,
    $stage1BoundarySource,
    $progressSetupSource,
    $progressSetupBehaviorSource,
    $setCurrentPathSource,
    $setCurrentPathBehaviorSource,
    $getProjectPathSource,
    $getProjectPathBehaviorSource,
    $wideStringConstructorSource,
    $wideStringConstructorBehaviorSource,
    $wideStringDestructorSource,
    $wideStringDestructorBehaviorSource,
    $charStringConstructorSource,
    $charStringConstructorBehaviorSource,
    $charStringDestructorSource,
    $charStringDestructorBehaviorSource,
    $profileStartSource,
    $profileStartBehaviorSource,
    $profileEndSource,
    $profileEndBehaviorSource,
    $asyncFailureHandlingSource,
    $asyncFailureHandlingBehaviorSource,
    $startupLatchSource,
    $startupLatchBehaviorSource,
    $fileInstallerGetSource,
    $fileInstallerGetBehaviorSource,
    $defaultLanguageNameSource,
    $defaultLanguageNameBehaviorSource,
    $wideStringFromCharSource,
    $wideStringFromCharBehaviorSource,
    $wideStringCopySource,
    $wideStringCopyBehaviorSource,
    $charToWideStringSource,
    $charToWideStringBehaviorSource,
    $primaryLeftAlignmentSource,
    $primaryLeftAlignmentBehaviorSource,
    $secondaryLeftAlignmentSource,
    $secondaryLeftAlignmentBehaviorSource,
    $cbaseRestoreASource,
    $cbaseRestoreABehaviorSource,
    $cbaseRestoreBSource,
    $cbaseRestoreBBehaviorSource,
    $charStringDefaultSource,
    $charStringDefaultBehaviorSource,
    $systemManagerInitSource,
    $systemManagerInitBehaviorSource,
    $gfmainPhase1Source,
    $gfmainPhase2Source,
    $stage2BoundarySource,
    $visualBootSource,
    $visualBootArtwork,
    $visualBootBehaviorSource,
    $gfmainPhase1BehaviorSource,
    $gfmainPhase2BehaviorSource,
    $bootObjectChecker
)
$missing = @($required | Where-Object { -not (Test-Path -LiteralPath $_) })
if ($missing.Count -gt 0) {
    throw "Bootstrap prerequisites are missing: $($missing -join ', ')"
}

New-Item -ItemType Directory -Path $outDir -Force | Out-Null

$windowsSdkUmLib = Get-ChildItem -LiteralPath $windowsSdkLibRoot -Directory |
    Where-Object {
        Test-Path -LiteralPath (Join-Path $_.FullName 'um\x86\User32.Lib')
    } |
    Sort-Object { [version]$_.Name } -Descending |
    Select-Object -First 1 -ExpandProperty FullName
if (-not $windowsSdkUmLib) {
    throw 'A Windows SDK x86 import-library directory is required for the visual checkpoint.'
}
$windowsSdkUmLib = Join-Path $windowsSdkUmLib 'um\x86'

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = @(
        (Join-Path $vcRoot 'include'),
        (Join-Path $rebuildRoot 'include')
    ) -join ';'
    $env:LIB = @(
        (Join-Path $vcRoot 'lib'),
        $windowsSdkUmLib
    ) -join ';'

    $compileOptions = @('/nologo', '/c', '/W3', '/MT', '/GS')
    if ($Configuration -eq 'Release') {
        $compileOptions += @('/O2', '/Oy')
    } else {
        $compileOptions += @('/Od', '/Zi')
    }

    function Invoke-VerifiedLeaf {
        param(
            [string]$Address,
            [string]$Description,
            [string]$Source,
            [string]$BehaviorSource,
            [string]$OutputStem,
            [string]$PassPattern,
            [string]$AllowMove
        )

        $object = Join-Path $outDir "$OutputStem.obj"
        $behaviorObject = Join-Path $outDir "$OutputStem-behavior.obj"
        $behaviorExecutable = Join-Path $outDir "$OutputStem-behavior.exe"

        & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
            "/Fo$object" $Source
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $object)) {
            throw "Failed to compile $Description."
        }

        $checkArguments = @(
            $bootObjectChecker,
            '--root',
            $workspaceRoot,
            '--object',
            $object,
            '--address',
            $Address
        )
        if ($AllowMove) {
            $checkArguments += @('--allow-move', $AllowMove)
        }
        & python @checkArguments
        if ($LASTEXITCODE -ne 0) {
            throw "$Description failed its retail parity/residue gate."
        }

        & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
            "/Fo$behaviorObject" $BehaviorSource
        if (
            $LASTEXITCODE -ne 0 -or
            -not (Test-Path -LiteralPath $behaviorObject)
        ) {
            throw "Failed to compile the $Description behavior fixture."
        }

        & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
            "/out:$behaviorExecutable" $object $behaviorObject
        if (
            $LASTEXITCODE -ne 0 -or
            -not (Test-Path -LiteralPath $behaviorExecutable)
        ) {
            throw "Failed to link the $Description behavior fixture."
        }

        $behaviorOutput = & $behaviorExecutable 2>&1
        $behaviorExitCode = $LASTEXITCODE
        $behaviorOutput | Write-Output
        if (
            $behaviorExitCode -ne 0 -or
            (($behaviorOutput -join "`n") -notmatch [regex]::Escape($PassPattern))
        ) {
            throw "$Description behavior fixture failed with exit code $behaviorExitCode."
        }
    }

    Invoke-VerifiedLeaf `
        -Address '0099e4b0' `
        -Description 'CCharString default constructor' `
        -Source $charStringDefaultSource `
        -BehaviorSource $charStringDefaultBehaviorSource `
        -OutputStem 'char-string-default-constructor' `
        -PassPattern $charStringDefaultPassPattern

    Invoke-VerifiedLeaf `
        -Address '00403b10' `
        -Description 'CSystemManagerInit constructor' `
        -Source $systemManagerInitSource `
        -BehaviorSource $systemManagerInitBehaviorSource `
        -OutputStem 'system-manager-init-constructor' `
        -PassPattern $systemManagerInitPassPattern `
        -AllowMove '0x20:3:0x50'

    Invoke-VerifiedLeaf `
        -Address '009d8250' `
        -Description 'NProfileTimer end-profile no-op' `
        -Source $profileEndSource `
        -BehaviorSource $profileEndBehaviorSource `
        -OutputStem 'profile-end' `
        -PassPattern $profileEndPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d5240' `
        -Description 'CBankFileAsync failure-handling policy setter' `
        -Source $asyncFailureHandlingSource `
        -BehaviorSource $asyncFailureHandlingBehaviorSource `
        -OutputStem 'async-failure-handling' `
        -PassPattern $asyncFailureHandlingPassPattern

    Invoke-VerifiedLeaf `
        -Address '009d81e0' `
        -Description 'TLC startup-latch clear leaf' `
        -Source $startupLatchSource `
        -BehaviorSource $startupLatchBehaviorSource `
        -OutputStem 'startup-latch-clear' `
        -PassPattern $startupLatchPassPattern

    # Avoid "installer" in the executable name: Windows' legacy installer
    # detection otherwise requests elevation before this manifest-free VC7.1
    # console fixture can run.
    Invoke-VerifiedLeaf `
        -Address '00404440' `
        -Description 'CFileInstaller counted singleton retrieval' `
        -Source $fileInstallerGetSource `
        -BehaviorSource $fileInstallerGetBehaviorSource `
        -OutputStem 'cfi-singleton-get' `
        -PassPattern $fileInstallerGetPassPattern

    Invoke-VerifiedLeaf `
        -Address '00415530' `
        -Description 'default-language narrow-string factory' `
        -Source $defaultLanguageNameSource `
        -BehaviorSource $defaultLanguageNameBehaviorSource `
        -OutputStem 'default-language-name' `
        -PassPattern $defaultLanguageNamePassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b6a0' `
        -Description 'CWideString narrow-string conversion factory' `
        -Source $wideStringFromCharSource `
        -BehaviorSource $wideStringFromCharBehaviorSource `
        -OutputStem 'wide-string-from-char' `
        -PassPattern $wideStringFromCharPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099b720' `
        -Description 'CWideString counted-storage copy constructor' `
        -Source $wideStringCopySource `
        -BehaviorSource $wideStringCopyBehaviorSource `
        -OutputStem 'wide-string-copy' `
        -PassPattern $wideStringCopyPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099e480' `
        -Description 'CCharString to CWideString conversion' `
        -Source $charToWideStringSource `
        -BehaviorSource $charToWideStringBehaviorSource `
        -OutputStem 'char-to-wide-string' `
        -PassPattern $charToWideStringPassPattern

    Invoke-VerifiedLeaf `
        -Address '009bc890' `
        -Description 'primary left-alignment flag setter' `
        -Source $primaryLeftAlignmentSource `
        -BehaviorSource $primaryLeftAlignmentBehaviorSource `
        -OutputStem 'primary-left-alignment' `
        -PassPattern $primaryLeftAlignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '009bc8a0' `
        -Description 'secondary left-alignment flag setter' `
        -Source $secondaryLeftAlignmentSource `
        -BehaviorSource $secondaryLeftAlignmentBehaviorSource `
        -OutputStem 'secondary-left-alignment' `
        -PassPattern $secondaryLeftAlignmentPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099a2e0' `
        -Description 'first folded CBase vtable restore' `
        -Source $cbaseRestoreASource `
        -BehaviorSource $cbaseRestoreABehaviorSource `
        -OutputStem 'cbase-restore-a' `
        -PassPattern $cbaseRestoreAPassPattern

    Invoke-VerifiedLeaf `
        -Address '0099a300' `
        -Description 'second folded CBase vtable restore' `
        -Source $cbaseRestoreBSource `
        -BehaviorSource $cbaseRestoreBBehaviorSource `
        -OutputStem 'cbase-restore-b' `
        -PassPattern $cbaseRestoreBPassPattern

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$retailObject" $retailSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $retailObject)) {
        throw 'Failed to compile the verified retail function.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$bootstrapObject" $bootstrapSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $bootstrapObject)) {
        throw 'Failed to compile the reconstruction bootstrap.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$winMainObject" $winMainSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainObject)) {
        throw 'Failed to compile the recovered WinMain wrapper.'
    }

    & python $bootObjectChecker --root $workspaceRoot --object $winMainObject --address 00403480
    if ($LASTEXITCODE -ne 0) {
        throw 'The recovered WinMain wrapper differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$winMainBehaviorObject" $winMainBehaviorSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainBehaviorObject)) {
        throw 'Failed to compile the WinMain behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$stage1BoundaryObject" $stage1BoundarySource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $stage1BoundaryObject)) {
        throw 'Failed to compile the Stage 1 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions "/Fo$progressSetupObject" $progressSetupSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $progressSetupObject)) {
        throw 'Failed to compile the GFInitialise progress-display setup leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot --object $progressSetupObject --address 00413120
    if ($LASTEXITCODE -ne 0) {
        throw 'The progress-display setup leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$progressSetupBehaviorObject" $progressSetupBehaviorSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $progressSetupBehaviorObject)) {
        throw 'Failed to compile the progress-display setup behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$setCurrentPathObject" $setCurrentPathSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $setCurrentPathObject)) {
        throw 'Failed to compile the CAFile current-path leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $setCurrentPathObject --address 009974f0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CAFile current-path leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$setCurrentPathBehaviorObject" $setCurrentPathBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $setCurrentPathBehaviorObject)
    ) {
        throw 'Failed to compile the CAFile current-path behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$getProjectPathObject" $getProjectPathSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $getProjectPathObject)) {
        throw 'Failed to compile the CAFile project-path leaf.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $getProjectPathObject --address 00997510
    if ($LASTEXITCODE -ne 0) {
        throw 'The CAFile project-path leaf differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$getProjectPathBehaviorObject" $getProjectPathBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $getProjectPathBehaviorObject)
    ) {
        throw 'Failed to compile the CAFile project-path behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringConstructorObject" $wideStringConstructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorObject)
    ) {
        throw 'Failed to compile the CWideString default constructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $wideStringConstructorObject --address 0099aed0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CWideString default constructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringConstructorBehaviorObject" $wideStringConstructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorBehaviorObject)
    ) {
        throw 'Failed to compile the CWideString constructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringDestructorObject" $wideStringDestructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorObject)
    ) {
        throw 'Failed to compile the CWideString destructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $wideStringDestructorObject --address 0099b510
    if ($LASTEXITCODE -ne 0) {
        throw 'The CWideString destructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$wideStringDestructorBehaviorObject" $wideStringDestructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorBehaviorObject)
    ) {
        throw 'Failed to compile the CWideString destructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringConstructorObject" $charStringConstructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorObject)
    ) {
        throw 'Failed to compile the CCharString constructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $charStringConstructorObject --address 0099ebf0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CCharString constructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringConstructorBehaviorObject" $charStringConstructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorBehaviorObject)
    ) {
        throw 'Failed to compile the CCharString constructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringDestructorObject" $charStringDestructorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorObject)
    ) {
        throw 'Failed to compile the CCharString destructor.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $charStringDestructorObject --address 0099eae0
    if ($LASTEXITCODE -ne 0) {
        throw 'The CCharString destructor differs from retail outside relocations.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$charStringDestructorBehaviorObject" $charStringDestructorBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorBehaviorObject)
    ) {
        throw 'Failed to compile the CCharString destructor behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$profileStartObject" $profileStartSource
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $profileStartObject)) {
        throw 'Failed to compile the retail profile-start no-op.'
    }

    & python $bootObjectChecker --root $workspaceRoot `
        --object $profileStartObject --address 009d8240
    if ($LASTEXITCODE -ne 0) {
        throw 'The profile-start no-op differs from retail.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$profileStartBehaviorObject" $profileStartBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $profileStartBehaviorObject)
    ) {
        throw 'Failed to compile the profile-start behavior fixture.'
    }

    $linkOptions = @(
        '/nologo',
        '/subsystem:console',
        "/out:$executable"
    )
    if ($Configuration -eq 'Debug') {
        $linkOptions += '/debug'
    }

    & (Join-Path $vcRoot 'bin\link.exe') @linkOptions $bootstrapObject $retailObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $executable)) {
        throw 'Failed to link the reconstruction bootstrap executable.'
    }

    $output = & $executable 2>&1
    $exitCode = $LASTEXITCODE
    $output | Write-Output
    if ($exitCode -ne 0 -or (($output -join "`n") -notmatch [regex]::Escape($passPattern))) {
        throw "Bootstrap execution failed with exit code $exitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$winMainBehaviorExecutable" $winMainObject $winMainBehaviorObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $winMainBehaviorExecutable)) {
        throw 'Failed to link the WinMain behavior fixture.'
    }

    $winMainOutput = & $winMainBehaviorExecutable 2>&1
    $winMainExitCode = $LASTEXITCODE
    $winMainOutput | Write-Output
    if (
        $winMainExitCode -ne 0 -or
        (($winMainOutput -join "`n") -notmatch [regex]::Escape($winMainPassPattern))
    ) {
        throw "WinMain behavior fixture failed with exit code $winMainExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage1Executable" $winMainObject $stage1BoundaryObject
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $stage1Executable)) {
        throw 'Failed to link the Stage 1 startup executable.'
    }

    $stage1Process = Start-Process -FilePath $stage1Executable -Wait -PassThru
    if ($stage1Process.ExitCode -ne 0) {
        throw "Stage 1 startup executable failed with exit code $($stage1Process.ExitCode)."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$progressSetupBehaviorExecutable" `
        $progressSetupObject $progressSetupBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $progressSetupBehaviorExecutable)
    ) {
        throw 'Failed to link the progress-display setup behavior fixture.'
    }

    $progressSetupOutput = & $progressSetupBehaviorExecutable 2>&1
    $progressSetupExitCode = $LASTEXITCODE
    $progressSetupOutput | Write-Output
    if (
        $progressSetupExitCode -ne 0 -or
        (($progressSetupOutput -join "`n") -notmatch [regex]::Escape($progressSetupPassPattern))
    ) {
        throw "Progress-display setup fixture failed with exit code $progressSetupExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$setCurrentPathBehaviorExecutable" `
        $setCurrentPathObject $setCurrentPathBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $setCurrentPathBehaviorExecutable)
    ) {
        throw 'Failed to link the CAFile current-path behavior fixture.'
    }

    $setCurrentPathOutput = & $setCurrentPathBehaviorExecutable 2>&1
    $setCurrentPathExitCode = $LASTEXITCODE
    $setCurrentPathOutput | Write-Output
    if (
        $setCurrentPathExitCode -ne 0 -or
        (($setCurrentPathOutput -join "`n") -notmatch [regex]::Escape($setCurrentPathPassPattern))
    ) {
        throw "CAFile current-path fixture failed with exit code $setCurrentPathExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$getProjectPathBehaviorExecutable" `
        $getProjectPathObject $getProjectPathBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $getProjectPathBehaviorExecutable)
    ) {
        throw 'Failed to link the CAFile project-path behavior fixture.'
    }

    $getProjectPathOutput = & $getProjectPathBehaviorExecutable 2>&1
    $getProjectPathExitCode = $LASTEXITCODE
    $getProjectPathOutput | Write-Output
    if (
        $getProjectPathExitCode -ne 0 -or
        (($getProjectPathOutput -join "`n") -notmatch [regex]::Escape($getProjectPathPassPattern))
    ) {
        throw "CAFile project-path fixture failed with exit code $getProjectPathExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$wideStringConstructorBehaviorExecutable" `
        $wideStringConstructorObject $wideStringConstructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringConstructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CWideString constructor behavior fixture.'
    }

    $wideStringConstructorOutput = & $wideStringConstructorBehaviorExecutable 2>&1
    $wideStringConstructorExitCode = $LASTEXITCODE
    $wideStringConstructorOutput | Write-Output
    if (
        $wideStringConstructorExitCode -ne 0 -or
        (
            ($wideStringConstructorOutput -join "`n") -notmatch
            [regex]::Escape($wideStringConstructorPassPattern)
        )
    ) {
        throw "CWideString constructor fixture failed with exit code $wideStringConstructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$wideStringDestructorBehaviorExecutable" `
        $wideStringConstructorObject `
        $wideStringDestructorObject `
        $wideStringDestructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $wideStringDestructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CWideString destructor behavior fixture.'
    }

    $wideStringDestructorOutput = & $wideStringDestructorBehaviorExecutable 2>&1
    $wideStringDestructorExitCode = $LASTEXITCODE
    $wideStringDestructorOutput | Write-Output
    if (
        $wideStringDestructorExitCode -ne 0 -or
        (
            ($wideStringDestructorOutput -join "`n") -notmatch
            [regex]::Escape($wideStringDestructorPassPattern)
        )
    ) {
        throw "CWideString destructor fixture failed with exit code $wideStringDestructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$charStringConstructorBehaviorExecutable" `
        $charStringConstructorObject `
        $charStringConstructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringConstructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CCharString constructor behavior fixture.'
    }

    $charStringConstructorOutput = & $charStringConstructorBehaviorExecutable 2>&1
    $charStringConstructorExitCode = $LASTEXITCODE
    $charStringConstructorOutput | Write-Output
    if (
        $charStringConstructorExitCode -ne 0 -or
        (
            ($charStringConstructorOutput -join "`n") -notmatch
            [regex]::Escape($charStringConstructorPassPattern)
        )
    ) {
        throw "CCharString constructor fixture failed with exit code $charStringConstructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$charStringDestructorBehaviorExecutable" `
        $charStringDestructorObject `
        $charStringDestructorBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $charStringDestructorBehaviorExecutable)
    ) {
        throw 'Failed to link the CCharString destructor behavior fixture.'
    }

    $charStringDestructorOutput = & $charStringDestructorBehaviorExecutable 2>&1
    $charStringDestructorExitCode = $LASTEXITCODE
    $charStringDestructorOutput | Write-Output
    if (
        $charStringDestructorExitCode -ne 0 -or
        (
            ($charStringDestructorOutput -join "`n") -notmatch
            [regex]::Escape($charStringDestructorPassPattern)
        )
    ) {
        throw "CCharString destructor fixture failed with exit code $charStringDestructorExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$profileStartBehaviorExecutable" `
        $profileStartObject `
        $profileStartBehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $profileStartBehaviorExecutable)
    ) {
        throw 'Failed to link the profile-start behavior fixture.'
    }

    $profileStartOutput = & $profileStartBehaviorExecutable 2>&1
    $profileStartExitCode = $LASTEXITCODE
    $profileStartOutput | Write-Output
    if (
        $profileStartExitCode -ne 0 -or
        (($profileStartOutput -join "`n") -notmatch [regex]::Escape($profileStartPassPattern))
    ) {
        throw "Profile-start fixture failed with exit code $profileStartExitCode."
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase1Object" $gfmainPhase1Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1Object)
    ) {
        throw 'Failed to compile the GFMain Phase 1 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase2Object" $gfmainPhase2Source
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2Object)
    ) {
        throw 'Failed to compile the GFMain Phase 2 integration unit.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$stage2BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage2BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 2 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        "/Fo$stage3BoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3BoundaryObject)
    ) {
        throw 'Failed to compile the Stage 3 engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        /DFABLETLC_ENABLE_GFMAIN_PHASE2 `
        /DFABLETLC_ENABLE_VISUAL_BOOT `
        "/Fo$visualBoundaryObject" $stage2BoundarySource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBoundaryObject)
    ) {
        throw 'Failed to compile the authored visual engine boundary.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$visualBootObject" $visualBootSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootObject)
    ) {
        throw 'Failed to compile the visual boot checkpoint.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$visualBootBehaviorObject" $visualBootBehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootBehaviorObject)
    ) {
        throw 'Failed to compile the visual boot behavior fixture.'
    }

    Add-Type -AssemblyName PresentationCore
    $pngStream = [System.IO.File]::OpenRead($visualBootArtwork)
    try {
        $decoder = New-Object System.Windows.Media.Imaging.PngBitmapDecoder(
            $pngStream,
            [System.Windows.Media.Imaging.BitmapCreateOptions]::PreservePixelFormat,
            [System.Windows.Media.Imaging.BitmapCacheOption]::OnLoad)
        $encoder = New-Object System.Windows.Media.Imaging.BmpBitmapEncoder
        $encoder.Frames.Add($decoder.Frames[0])
        $bitmapStream = [System.IO.File]::Create($visualBootBitmap)
        try {
            $encoder.Save($bitmapStream)
        } finally {
            $bitmapStream.Dispose()
        }
    } finally {
        $pngStream.Dispose()
    }

    $resourceBitmapPath = $visualBootBitmap.Replace('\', '/')
    Set-Content -LiteralPath $visualBootResourceSource `
        -Value "101 BITMAP `"$resourceBitmapPath`"" `
        -Encoding Ascii
    & (Join-Path $vcRoot 'bin\rc.exe') `
        "/fo$visualBootResource" $visualBootResourceSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootResource)
    ) {
        throw 'Failed to compile the visual boot artwork resource.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase1BehaviorObject" $gfmainPhase1BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 1 behavior fixture.'
    }

    & (Join-Path $vcRoot 'bin\cl.exe') @compileOptions `
        "/Fo$gfmainPhase2BehaviorObject" $gfmainPhase2BehaviorSource
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2BehaviorObject)
    ) {
        throw 'Failed to compile the GFMain Phase 2 behavior fixture.'
    }

    $phase1RuntimeObjects = @(
        $gfmainPhase1Object,
        $stage2BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $profileStartObject,
        $systemManagerInitObject
    )

    $phase2RuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $stage3BoundaryObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $profileStartObject,
        $profileEndObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    $visualRuntimeObjects = @(
        $gfmainPhase1Object,
        $gfmainPhase2Object,
        $visualBoundaryObject,
        $visualBootObject,
        $setCurrentPathObject,
        $getProjectPathObject,
        $wideStringConstructorObject,
        $wideStringDestructorObject,
        $charStringConstructorObject,
        $charStringDefaultObject,
        $charStringDestructorObject,
        $profileStartObject,
        $profileEndObject,
        $asyncFailureHandlingObject,
        $startupLatchObject,
        $fileInstallerGetObject,
        $systemManagerInitObject
    )

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase1BehaviorExecutable" `
        @phase1RuntimeObjects $gfmainPhase1BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase1BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 1 behavior fixture.'
    }

    $gfmainPhase1Output = & $gfmainPhase1BehaviorExecutable 2>&1
    $gfmainPhase1ExitCode = $LASTEXITCODE
    $gfmainPhase1Output | Write-Output
    if (
        $gfmainPhase1ExitCode -ne 0 -or
        (($gfmainPhase1Output -join "`n") -notmatch [regex]::Escape($gfmainPhase1PassPattern))
    ) {
        throw "GFMain Phase 1 fixture failed with exit code $gfmainPhase1ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$gfmainPhase2BehaviorExecutable" `
        @phase2RuntimeObjects $gfmainPhase2BehaviorObject
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $gfmainPhase2BehaviorExecutable)
    ) {
        throw 'Failed to link the GFMain Phase 2 behavior fixture.'
    }

    $gfmainPhase2Output = & $gfmainPhase2BehaviorExecutable 2>&1
    $gfmainPhase2ExitCode = $LASTEXITCODE
    $gfmainPhase2Output | Write-Output
    if (
        $gfmainPhase2ExitCode -ne 0 -or
        (($gfmainPhase2Output -join "`n") -notmatch [regex]::Escape($gfmainPhase2PassPattern))
    ) {
        throw "GFMain Phase 2 fixture failed with exit code $gfmainPhase2ExitCode."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage2Executable" $winMainObject @phase1RuntimeObjects
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage2Executable)
    ) {
        throw 'Failed to link the Stage 2 startup executable.'
    }

    & $stage2Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Stage 2 startup failed with exit code $LASTEXITCODE."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$stage3Executable" $winMainObject @phase2RuntimeObjects
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $stage3Executable)
    ) {
        throw 'Failed to link the Stage 3 startup executable.'
    }

    & $stage3Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Stage 3 startup failed with exit code $LASTEXITCODE."
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
        "/out:$visualCheckpointExecutable" `
        $winMainObject @visualRuntimeObjects $visualBootResource `
        user32.lib gdi32.lib
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualCheckpointExecutable)
    ) {
        throw 'Failed to link the authored visual startup executable.'
    }

    & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:console `
        "/out:$visualBootBehaviorExecutable" `
        $visualBootObject $visualBootBehaviorObject $visualBootResource `
        user32.lib gdi32.lib
    if (
        $LASTEXITCODE -ne 0 -or
        -not (Test-Path -LiteralPath $visualBootBehaviorExecutable)
    ) {
        throw 'Failed to link the visual boot behavior fixture.'
    }

    $visualBootOutput = & $visualBootBehaviorExecutable 2>&1
    $visualBootExitCode = $LASTEXITCODE
    $visualBootOutput | Write-Output
    if (
        $visualBootExitCode -ne 0 -or
        (($visualBootOutput -join "`n") -notmatch [regex]::Escape($visualBootPassPattern))
    ) {
        throw "Visual boot fixture failed with exit code $visualBootExitCode."
    }

    Write-Output "BOOTSTRAP_BUILD PASS configuration=$Configuration executable=$executable"
    Write-Output "STAGE1_STARTUP PASS executable=$stage1Executable boundary=GFMain"
    Write-Output "STAGE2_STARTUP PASS executable=$stage2Executable boundary=GFMainPhase2"
    Write-Output "STAGE3_STARTUP PASS executable=$stage3Executable boundary=GFMainPhase3"
    Write-Output "VISUAL_BOOT_CHECKPOINT PASS executable=$visualCheckpointExecutable boundary=AuthoredVisualCheckpoint"
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}
