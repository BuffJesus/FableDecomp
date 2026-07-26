# Multiplayer reconstruction

This module is the first C++23 proof of concept for turning recovered retail
behavior into ordinary, human-facing subsystem code. It models one serialized
`CGameEvent` record; it does not replace the VC7.1 retail-parity sources.

## Evidence boundary

The codec is derived from these byte-backed functions and layout facts:

- `CGameEvent::CompressIntoBuffer @ 0x009F1810`;
- `CGameEvent::InitFromCompressedBuffer @ 0x009F1870`;
- retail object size `0x28`;
- payload storage at `+0x05..+0x24` (32 bytes);
- payload length, valid, and replacement fields at `+0x25..+0x27`.

The encoded record remains exactly:

```text
[u16 little-endian id|replacement][u8 source][u8 payload size][payload]
```

The source byte is deliberately not given a stronger gameplay name yet. The
evidence calls it both a player and a subfield, so the modern API preserves its
value without inventing semantics.

## Intentional modernization

- `GameEvent` owns its payload and enforces the 15-bit ID and 32-byte retail
  capacity when constructed.
- `GameEventCodec` uses `std::span`, `std::expected`, fixed-width wire fields,
  and explicit endian conversion.
- Runtime sizes use `std::size_t`, so the API is pointer-width neutral and
  already builds as native x64.
- Decode errors distinguish truncated headers, unsafe lengths, and truncated
  payloads.
- The decoder reports bytes consumed so package code can walk concatenated
  retail records without pointer arithmetic leaking into gameplay classes.

Package and package-set framing are intentionally unsupported until their
layouts and boundary behavior receive the same focused review. Networking,
transport, lobby behavior, and `CheckSync` are also outside this module.

EgoMP independently demonstrates useful TLC player/creature, motion, and
region-load integration seams, but its protocol is not inherited here. See
[`docs/EGOMP_MULTIPLAYER_AUDIT.md`](../../../docs/EGOMP_MULTIPLAYER_AUDIT.md)
for the evidence boundary and the recommended host-authoritative path.

## Build and test

From the repository root:

```powershell
cmake -S rebuild/modern -B rebuild/build/modern -G "MinGW Makefiles"
cmake --build rebuild/build/modern
ctest --test-dir rebuild/build/modern --output-on-failure
```

Build products remain under the ignored `rebuild/build/` tree.
