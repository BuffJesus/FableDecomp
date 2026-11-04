# Integer viewport finite native behavior passes; near grade remains unmet

The first standalone comparison passed 147,456 cases and 294,912 side snapshots
across twelve masked x87 modes. It did not time out. Independent saved-result
review confirms the retained source, linked bytes, original bytes and receipt
bindings, with zero reviewer engine/build/DLL invocations and no retry.

Evidence under `work/native-recreation/rendering/009bef80/20261014/`:

- Manifest: `b8ed26e0860bea4ef5d7af03654520e889c0e9abfe038ec10c4c278fdc7d0ace`.
- Independent whole-source review: `e10f9e0122adf0d781c2337475e8b700e19aa1cefabb5edb371b01a18f097190`.
- Native result: `dda66f6f24c0021aa8a831edccbab5c682065b14089f18a585cedf8dcf374179`.
- Actual outer invocation: `0fa090cdaaa74c3aea2297e887133e794ba413cb365965da0f7842e19149aca2`.
- Independent saved review: `fbad30ac89fc4945d5641f9d8d904cd30bd7c3f67ddf0b76319ef02471d45450`.
- Saved-review invocation: `1e40be9a0b0e18b16e21e5f68a74918f2fb82aab6b1fb71f25c8e969724d5c22`.

The qualified work-only closure is in `qualified-functional-closure/`.
Its SHA256 is `99ca78f8de878f8bee51c8f83ac90b8c04449b1c23b687a7d8260504be118919`.
The 441-byte endpoint's corrected opcode similarity is 75.9857%, below the
90% threshold despite instruction-count delta being within ten percent. It
remains a finite functional candidate with no near/reconstruction/coverage credit
or canonical promotion. The passing fixture will not be rerun.

Complete final arena/globals/device-observation and abstract CRT FP endpoints
agree. The result field `full_event_trace_matches` means the single bounded
DeviceObservation structure, including the complete display/shader state at the
device callback; it does not include an empty-notification trace. Source-derived
counts/bounds and terminal success are not saved live counters or per-case memory
dumps. No independent expectedCalls assertion, unmasked exceptions, intermediate
stores, raw x87 stack registers, constructed object lifetimes or physical device
behavior is established. All CRT/profile/raw-object/alias and no-x64 qualifications
remain in the retained records. Earlier compilation and inventory failures remain
unchanged and unaccepted.

Next is a new clear-path composition with the genuine endpoint/shader/empty
notification providers. The readonly union has eighteen original roots, 86 own
operand sites and nineteen internal direct edges. Its receipt is under the clear
packet's `integer-viewport-union-readonly-first/`, SHA256
`118438c4337c87b4b9a7f0ef7cac24486a981af1597f760a866296646e888a8d`.
No new composition execution or canonical/x64 promotion follows from that union.
