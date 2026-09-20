# Drive the retail Fable.exe window: capture / click / key / info.
param(
    [ValidateSet('capture','click','move','key','info','wait','lmb','hold')] [string]$Action = 'capture',
    [string]$Output = 'capture.png',
    [int]$X = 0, [int]$Y = 0,
    [string]$Keys = '',
    [int]$Seconds = 60
)
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName System.Windows.Forms
Add-Type @'
using System;
using System.Runtime.InteropServices;
public static class AtlasQaNative {
  [StructLayout(LayoutKind.Sequential)] public struct RECT { public int Left, Top, Right, Bottom; }
  [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr hWnd, out RECT rect);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
  [DllImport("user32.dll")] public static extern bool SetCursorPos(int x, int y);
  [DllImport("user32.dll")] public static extern void mouse_event(uint flags, uint dx, uint dy, uint data, UIntPtr extra);
  [DllImport("user32.dll")] public static extern void keybd_event(byte key, byte scan, uint flags, UIntPtr extra);
  [DllImport("user32.dll")] public static extern uint MapVirtualKey(uint code, uint mapType);
  [DllImport("user32.dll")] public static extern IntPtr GetForegroundWindow();
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr hWnd, int cmd);
}
'@
function Get-Fable {
    $p = Get-Process -Name 'Fable' -ErrorAction SilentlyContinue | Where-Object { $_.MainWindowHandle -ne [IntPtr]::Zero } | Select-Object -First 1
    return $p
}
if ($Action -eq 'wait') {
    $deadline = (Get-Date).AddSeconds($Seconds)
    while ((Get-Date) -lt $deadline) { $p = Get-Fable; if ($p) { "window $($p.Id) '$($p.MainWindowTitle)'"; exit 0 }; Start-Sleep -Milliseconds 500 }
    "no Fable window after $Seconds s"; exit 1
}
$process = Get-Fable
if (-not $process) { throw 'Fable.exe has no main window.' }
$handle = $process.MainWindowHandle
[AtlasQaNative]::ShowWindow($handle, 9) | Out-Null   # SW_RESTORE: a fullscreen game minimises when it loses focus
[AtlasQaNative]::SetForegroundWindow($handle) | Out-Null
Start-Sleep -Milliseconds 1500
$rect = New-Object AtlasQaNative+RECT
[AtlasQaNative]::GetWindowRect($handle, [ref]$rect) | Out-Null
switch ($Action) {
  'info' { [pscustomobject]@{ Id=$process.Id; Title=$process.MainWindowTitle; Left=$rect.Left; Top=$rect.Top; Width=$rect.Right-$rect.Left; Height=$rect.Bottom-$rect.Top; Responding=$process.Responding; Foreground=([AtlasQaNative]::GetForegroundWindow() -eq $handle) } }
  'capture' {
    $width = [Math]::Max(1, $rect.Right - $rect.Left); $height = [Math]::Max(1, $rect.Bottom - $rect.Top)
    $bitmap = New-Object Drawing.Bitmap $width, $height
    $graphics = [Drawing.Graphics]::FromImage($bitmap)
    $graphics.CopyFromScreen($rect.Left, $rect.Top, 0, 0, $bitmap.Size)
    $bitmap.Save($Output, [Drawing.Imaging.ImageFormat]::Png)
    $graphics.Dispose(); $bitmap.Dispose()
    Get-Item -LiteralPath $Output | Select-Object FullName,Length
  }
  'click' {
    # The game reads the mouse through DirectInput (relative motion): SetCursorPos does
    # not move the in-game cursor. Slam the cursor to the top-left corner with a huge
    # relative move, then walk to (X, Y) in steps so the game accumulates the deltas.
    [AtlasQaNative]::mouse_event(0x0001, [uint32]4294963296, [uint32]4294963296, 0, [UIntPtr]::Zero)
    Start-Sleep -Milliseconds 150
    $sx = 0; $sy = 0
    while ($sx -lt $X -or $sy -lt $Y) {
      $dx = [Math]::Min(40, $X - $sx); $dy = [Math]::Min(40, $Y - $sy)
      [AtlasQaNative]::mouse_event(0x0001, [uint32]$dx, [uint32]$dy, 0, [UIntPtr]::Zero)
      $sx += $dx; $sy += $dy
      Start-Sleep -Milliseconds 15
    }
    Start-Sleep -Milliseconds 400
    [AtlasQaNative]::mouse_event(0x0002,0,0,0,[UIntPtr]::Zero); Start-Sleep -Milliseconds 80
    [AtlasQaNative]::mouse_event(0x0004,0,0,0,[UIntPtr]::Zero)
  }
  'hold' {
    # hold one key (Keys) down for X milliseconds: movement (W/A/S/D) and charged actions; scan code for DirectInput
    $map = @{ ENTER=0x0D; ESC=0x1B; SPACE=0x20; UP=0x26; DOWN=0x28; LEFT=0x25; RIGHT=0x27; TAB=0x09; BACK=0x08; DEL=0x2E; END=0x23 }
    $k = $Keys.Trim()
    $vk = if ($map.ContainsKey($k)) { $map[$k] } else { [int][char]$k.ToUpper() }
    $sc = [AtlasQaNative]::MapVirtualKey([uint32]$vk, 0)
    [AtlasQaNative]::keybd_event([byte]$vk,[byte]$sc,0x0008,[UIntPtr]::Zero)
    Start-Sleep -Milliseconds ([Math]::Max(50, $X))
    [AtlasQaNative]::keybd_event([byte]$vk,[byte]$sc,0x000A,[UIntPtr]::Zero)
  }
  'lmb' {
    # left-button press/release where the in-game cursor already is (attack / confirm), X = repeat count
    $n = [Math]::Max(1, $X)
    for ($i = 0; $i -lt $n; $i++) {
      [AtlasQaNative]::mouse_event(0x0002,0,0,0,[UIntPtr]::Zero); Start-Sleep -Milliseconds 80
      [AtlasQaNative]::mouse_event(0x0004,0,0,0,[UIntPtr]::Zero); Start-Sleep -Milliseconds 350
    }
  }
  'move' {
    [AtlasQaNative]::mouse_event(0x0001, [uint32]4294963296, [uint32]4294963296, 0, [UIntPtr]::Zero)
    Start-Sleep -Milliseconds 150
    $sx = 0; $sy = 0
    while ($sx -lt $X -or $sy -lt $Y) {
      $dx = [Math]::Min(40, $X - $sx); $dy = [Math]::Min(40, $Y - $sy)
      [AtlasQaNative]::mouse_event(0x0001, [uint32]$dx, [uint32]$dy, 0, [UIntPtr]::Zero)
      $sx += $dx; $sy += $dy
      Start-Sleep -Milliseconds 15
    }
  }
  'key' {
    # Keys: names separated by spaces, e.g. "ENTER ESC DOWN"
    $map = @{ ENTER=0x0D; ESC=0x1B; SPACE=0x20; UP=0x26; DOWN=0x28; LEFT=0x25; RIGHT=0x27; TAB=0x09; BACK=0x08; DEL=0x2E; END=0x23 }
    foreach ($k in $Keys.Split(' ')) {
      if ($k -eq '') { continue }
      $vk = if ($map.ContainsKey($k)) { $map[$k] } else { [int][char]$k.ToUpper() }
      # In-game input is DirectInput: it only sees the scan code (KEYEVENTF_SCANCODE), a bare virtual key
      # is ignored once the hero is in the world. The frontend accepts either.
      $sc = [AtlasQaNative]::MapVirtualKey([uint32]$vk, 0)
      [AtlasQaNative]::keybd_event([byte]$vk,[byte]$sc,0x0008,[UIntPtr]::Zero); Start-Sleep -Milliseconds 120
      [AtlasQaNative]::keybd_event([byte]$vk,[byte]$sc,0x000A,[UIntPtr]::Zero); Start-Sleep -Milliseconds 250
    }
  }
}
