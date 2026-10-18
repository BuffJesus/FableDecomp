# Platform services (fork step 3)

The fork's replacements for retail's Win32/DirectX platform code, behind small interfaces. First
piece: **mouse input**.

- `MouseAccumulator` (portable, header-only): each OS report is folded into running sums as it
  arrives, and the world tick takes one `MouseFrame` (summed dx/dy, wheel, held and pressed
  buttons, report count). There is no fixed buffer, so polling rate cannot drop motion. Retail's
  `CMouseDX` reads a 256-record DirectInput buffer once per 15 Hz world frame, and 4-8 kHz mice
  overflow it (docs/modern/FRAME_TIMING.md "Live results").
- `RawMouseWin32`: Win32 Raw Input (`WM_INPUT`) feeding the accumulator.

## Verified (2026-10-17)

- `fable_platform_tests`: 2,000 reports in one tick are all summed; clicks inside one tick are
  kept; held buttons carry over.
- `fable_rawmouse_probe --seconds 12` while injecting 5 × 1,500 `SendInput` moves (the bursts
  that made retail return `DI_BUFFEROVERFLOW`): 7,533 reports counted (7,500 injected, the rest
  from the real mouse), busiest 15 Hz tick 459 reports, nothing dropped.

## Not covered yet

Keyboard, pads (XInput/GameInput), the clock, the window, files and config. See
MODERNIZATION step 3.
