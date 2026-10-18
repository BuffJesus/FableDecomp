# Platform services (fork step 3)

The fork's replacements for retail's Win32/DirectX platform code, behind small interfaces. First
piece: **mouse input**.

- `MouseAccumulator` (portable, header-only): each OS report is folded into running sums as it
  arrives, and the world tick takes one `MouseFrame` (summed dx/dy, wheel, held and pressed
  buttons, report count). There is no fixed buffer, so polling rate cannot drop motion. Retail's
  `CMouseDX` reads a 256-record DirectInput buffer once per 15 Hz world frame, and 4-8 kHz mice
  overflow it (docs/modern/FRAME_TIMING.md "Live results").
- `RawMouseWin32`: Win32 Raw Input (`WM_INPUT`) feeding the accumulator.
- `KeyboardAccumulator` + `RawKeyboardWin32`: what retail's `CKeyboardDX::Update` (0xAB69F0) gives
  the game per tick, i.e. the held-key snapshot (`GetDeviceState`) plus the ordered transitions since
  the last tick (buffered `GetDeviceData`), so a tap between two ticks still counts. Auto-repeat is
  not an event; focus loss releases held keys.

- `WorldClock` (portable, header-only): retail's world-tick scheduler. A world frame is due when
  `predicted_present * 15 - frame > 1` (`IsTimeForServerUpdate` 0x41674A); `predicted_present` is the
  next vblank after the queued frames, never earlier than now (0x4166E2). Each Update runs at most
  one world frame, and the renderer draws at `predicted_present * 15 - frame`.

## Verified (2026-10-17)

- `fable_platform_tests`: 2,000 reports in one tick are all summed; clicks inside one tick are
  kept; held buttons carry over.
- `fable_worldclock_tests`: replays the retail live sample (60 Hz presents → 15 Hz world, 4 renders
  per world frame) and keeps the world at 15 Hz at 144/165/240/1000 Hz refresh; a late frame
  catches up one per Update.
- `fable_keyboard_tests`; live: 60 injected F24 taps (down and up within about 1 ms) gave 120
  events and 60 presses.
- `fable_rawmouse_probe --seconds 12` while injecting 5 × 1,500 `SendInput` moves (the bursts
  that made retail return `DI_BUFFEROVERFLOW`): 7,533 reports counted (7,500 injected, the rest
  from the real mouse), busiest 15 Hz tick 459 reports, nothing dropped.

## Not covered yet

Pads (XInput/GameInput), the real vblank source for `predicted_present`, the window,
files and config. See
MODERNIZATION step 3.
