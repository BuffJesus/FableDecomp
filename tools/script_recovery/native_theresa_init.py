"""Exact Theresa Init boundary and actor ownership contract."""
import hashlib


def verify(data):
    address, size = 0xDAC4F0, 142
    digest = '79a6875510fa0b477c26d90f7c0c4be411ec4939ff4df5382a789e86ba428244'
    raw = data.bytes_at(address, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != digest:
        raise ValueError('Theresa Init bytes changed')
    return dict(address=address, size=size, sha256=digest,
                stateResetOffsets=[0x1d, 0x1c],
                boundActorSlots=[0x810, 0x814, 0x838, 0x5a0, 0x844],
                copiedThingSlot=0xd30,
                limits='API callee destruction and engine effects require separate validation')
