param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$workspaceRoot = Split-Path -Parent $rebuildRoot
$vcRoot = 'D:\Tools\vc71'
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
$executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage0.exe'
$winMainBehaviorExecutable = Join-Path $outDir 'FableTLC-WinMain-Behavior.exe'
$stage1Executable = Join-Path $outDir 'FableTLC-Reconstruction-Stage1.exe'
$progressSetupBehaviorExecutable = Join-Path $outDir 'FableTLC-ProgressDisplay-Behavior.exe'
$setCurrentPathBehaviorExecutable = Join-Path $outDir 'FableTLC-SetCurrentPath-Behavior.exe'
$getProjectPathBehaviorExecutable = Join-Path $outDir 'FableTLC-GetProjectPath-Behavior.exe'
$passPattern = 'FABLETLC_BOOTSTRAP_STAGE0 PASS'
$winMainPassPattern = 'FABLETLC_WINMAIN_BEHAVIOR PASS'
$progressSetupPassPattern = 'FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS'
$setCurrentPathPassPattern = 'FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS'
$getProjectPathPassPattern = 'FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS'

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
    $bootObjectChecker
)
$missing = @($required | Where-Object { -not (Test-Path -LiteralPath $_) })
if ($missing.Count -gt 0) {
    throw "Bootstrap prerequisites are missing: $($missing -join ', ')"
}

New-Item -ItemType Directory -Path $outDir -Force | Out-Null

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = @(
        (Join-Path $vcRoot 'include'),
        (Join-Path $rebuildRoot 'include')
    ) -join ';'
    $env:LIB = Join-Path $vcRoot 'lib'

    $compileOptions = @('/nologo', '/c', '/W3', '/MT', '/GS')
    if ($Configuration -eq 'Release') {
        $compileOptions += @('/O2', '/Oy')
    } else {
        $compileOptions += @('/Od', '/Zi')
    }

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

    Write-Output "BOOTSTRAP_BUILD PASS configuration=$Configuration executable=$executable"
    Write-Output "STAGE1_STARTUP PASS executable=$stage1Executable boundary=GFMain"
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}
