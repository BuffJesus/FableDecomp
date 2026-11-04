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
  The fork currently uses virtual keys, with distinct left/right Shift/Ctrl/Alt. Retail bindings
  use DIK scan codes (see `docs/engine/INPUT.md`), so binding translation is still required.
  The Win32 adapter resolves modifier sides using `MakeCode`/E0 and `MapVirtualKeyW`;
  releasing one side keeps the other side held. It ignores overrun and fake/unmapped reports.
  Sources: [RAWKEYBOARD](https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawkeyboard),
  [MapVirtualKeyW](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mapvirtualkeyw).

- `WorldClock` (portable, header-only): retail's world-tick scheduler. A world frame is due when
  `predicted_present * 15 - frame > 1` (`IsTimeForServerUpdate` 0x41674A); `predicted_present` is the
  next vblank after the queued frames, never earlier than now (0x4166E2). Each Update runs at most
  one world frame, and the renderer draws at `predicted_present * 15 - frame`.

- `Pad` + `XInputPad`: pads through XInput, with retail's stick semantics. Retail's `CJoystickDX`
  (DirectInput) sets range 0..0x10000 and a **20 % dead zone on the left stick only** (EnumDevices
  callback 0xAB6B10), and stores axes as `2*raw/65536 - 1` with +y down (UpdateEvents 0xAB6E40).
  The fork keeps those values and reports the triggers separately; through DirectInput an Xbox
  pad's triggers share one axis. `PadAccumulator` keeps presses between ticks.

- `InputRecordingWriter` / `InputRecordingReader`: streaming, portable recording of each world
  tick's mouse frame, keyboard snapshot and ordered transitions, and four pad frames. Playback
  returns one `InputTick` per world update, without sampling the OS again. This is input transport
  for the replay tool; engine RNG capture, retail input hooks and world-state comparison remain
  unimplemented. The caller supplies the seed metadata; the standalone probe uses zero.

## Input recordings (2026-10-18)

`fable_rawmouse_probe --seconds 10 --record sample.finput` captures the standalone probe's
15 Hz input ticks, including all four XInput slots. The probe uses background Raw Input and a
hidden window; it does not launch, attach to, or inject input into the game. `fable_input_check
sample.finput` validates and prints counts. Both tools return a nonzero exit status on errors.
Capture ends at the last sampled world tick; reports after that tick are not part of the file.

Consumers link `fable_input_recording`, construct a reader on a binary stream, and call `next()`
once per world update. A null result means a validated completion footer and EOF. Writers must
call `finish()` explicitly; interrupted recordings are rejected. Do not reuse a reader or writer
after an I/O error. The caller owns the stream and serializes access; these classes are not
thread-safe. Frame payloads are bounded to 131,316 bytes (65,536 key transitions per tick).

Version 1 is a fork format with little-endian fields and no native structs on disk:

| Part | Fields, in order |
|---|---|
| Header (36 bytes) | ASCII `FBLINPUT`, u32 version=1, u32 tick rate=15, u64 caller seed, u64 first tick, u32 CRC |
| Tick record | u32 payload length, payload, u32 CRC over length + payload |
| Tick payload | u64 tick; mouse i64 dx/dy, i32 wheel, u32 held/pressed, u64 reports; keyboard 32-byte held and pressed bitmaps, u32 event count, `(u8 key, u8 down)` events; four pads |
| Pad (33 bytes) | u8 connected; six IEEE-754 binary32 axes (LX, LY, RX, RY, LT, RT); u32 held, u32 pressed |
| Footer (16 bytes) | u32 zero, u64 tick count, u32 CRC over those 12 bytes, then EOF |

CRC is standard reflected CRC-32/ISO-HDLC (polynomial 0xEDB88320, initial/final XOR
0xFFFFFFFF). Bitmap bit `key % 8` in byte `key / 8` represents that virtual key. Payloads are
244 + 2 * event count bytes. Booleans must be 0 or 1; axes must be finite and normalized
(sticks -1..1, triggers 0..1). Tick numbers must be contiguous from the header's first tick.
Lengths, checksums, event counts, version, rate, tick order, footer count and trailing data are
checked before a recording is accepted as complete. CRC detects accidental damage; it is not
an authenticity check.

Tests cover taps between ticks, neutral ticks with held keys, all four pad slots, 8,000 mouse
reports whose summed motion exceeds i32, exact float bits, wire positions, sequence bounds,
every truncation and single-byte corruption of the fixture, malformed lengths/counts/booleans/
axes, and output errors. Cross-compiler fixtures are generated locally, outside the repository.

## Verified (2026-10-17)

- `fable_platform_tests`: 2,000 reports in one tick are all summed; clicks inside one tick are
  kept; held buttons carry over.
- `fable_worldclock_tests`: replays the retail live sample (60 Hz presents → 15 Hz world, 4 renders
  per world frame) and keeps the world at 15 Hz at 144/165/240/1000 Hz refresh; a late frame
  catches up one per Update.
- `fable_keyboard_tests`; live: 60 injected F24 taps (down and up within about 1 ms) gave 120
  events and 60 presses.
- `fable_pad_tests`: dead zone (centre inside 20 %, rescaled outside, continuous at the edge),
  XInput range, a press between ticks kept. Live: an Xbox One Wireless Controller (Xbox Wireless
  Adapter) shows in XInput slot 0, sticks at exactly 0 at rest after the dead zone.
- `fable_rawmouse_probe --seconds 12` while injecting 5 × 1,500 `SendInput` moves (the bursts
  that made retail return `DI_BUFFEROVERFLOW`): 7,533 reports counted (7,500 injected, the rest
  from the real mouse), busiest 15 Hz tick 459 reports, nothing dropped.

## Not covered yet

GameInput (newer pads), rumble, the real vblank source for `predicted_present`, the engine window,
general file/config services, retail scan-code binding translation, engine replay hooks and world-state comparison. See
MODERNIZATION step 3.

## Dependency boundary (2026-10-18)

Portable consumers link `fable_platform` or `fable_input_recording`. Backend
headers `RawMouseWin32.hpp` and `XInputPad.hpp` live in `win32/include`, exported
only by `fable_platform_win32`. Their include spellings are unchanged.
`FABLE_BUILD_DESKTOP_TOOLS=OFF` omits input probes and the texture title viewer;
the backend and deterministic keyboard tests still build without opening a window.
`WorldClock` is application scheduling policy currently housed here; see the
[runtime ownership contract](../../../docs/modern/ENGINE_ARCHITECTURE.md).
