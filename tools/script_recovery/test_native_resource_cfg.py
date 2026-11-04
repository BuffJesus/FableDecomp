"""Resource receivers survive only when every native incoming path agrees."""
from types import SimpleNamespace

import pytest

from tools.script_recovery.native_member_resource_calls import resource_call_evidence


class Code:
    def __init__(self):
        self.data = bytearray()
        self.labels = {}
        self.fixups = []

    def emit(self, hex_bytes):
        self.data.extend(bytes.fromhex(hex_bytes))

    def label(self, name):
        self.labels[name] = len(self.data)

    def jump(self, opcode, target):
        self.emit(opcode + '00')
        self.fixups.append((len(self.data)-1, target))

    def bytes(self):
        for offset, target in self.fixups:
            self.data[offset] = (self.labels[target]-offset-1) & 255
        return bytes(self.data)


def proof(code, **options):
    raw = code.bytes()
    image = SimpleNamespace(bytes_at=lambda start, count: raw[start-0x1000:start-0x1000+count])
    fn = dict(address='0x1000', bodyEndExclusive=hex(0x1000+len(raw)))
    spec = SimpleNamespace(entity=True, parent_off=0x14, self_resources={0x34:'Speaker'}, parent_resources={})
    return resource_call_evidence(image,fn,spec,**options)


def getter(code):
    code.emit('8bcf 8b07 8d542410 52 ff5030 c3')


@pytest.mark.parametrize('spill', ['', '89442410'])
def test_resource_survives_retry_join_and_safe_stack_spill(spill):
    c=Code();c.emit('8bf1 8d7e34')
    c.label('retry');c.emit('8bce e800000000 85c0')
    c.jump('74','retry');c.emit(spill);getter(c)
    rows=list(proof(c).values())
    assert len(rows)==1 and rows[0]['family']=='self' and rows[0]['offset']==0x34


@pytest.mark.parametrize('other', ['', '8d7e38', 'bf00000000', '8937'])
def test_bypass_conflict_and_unknown_memory_write_do_not_inherit_receiver(other):
    c=Code();c.emit('8bf1 85c0');c.jump('74','alternative')
    c.emit('8d7e34');c.jump('eb','join');c.label('alternative')
    c.emit(other);c.label('join');getter(c)
    assert not proof(c)


def test_loop_backedge_overwrite_invalidates_initial_receiver():
    c=Code();c.emit('8bf1 8d7e34');c.label('loop');c.emit('85c0');c.jump('74','exit')
    c.emit('bf00000000');c.jump('eb','loop');c.label('exit');getter(c)
    assert not proof(c)


def test_volatile_receiver_is_not_preserved_by_a_call():
    c=Code();c.emit('8bf1 8d4634 e800000000 8bc8 8b10 ff5230 c3')
    assert not proof(c)


def test_different_vtable_or_unknown_write_invalidates_resource():
    for middle in ['8bcf 8b4638 ff5030 c3', '8907 8bcf 8b07 ff5030 c3']:
        c=Code();c.emit('8bf1 8d7e34 '+middle)
        assert not proof(c)


def test_same_resource_from_both_branches_is_kept():
    c=Code();c.emit('8bf1 85c0');c.jump('74','alternative')
    c.emit('8d7e34');c.jump('eb','join');c.label('alternative')
    c.emit('8d7e34');c.label('join');getter(c)
    assert len(proof(c))==1


@pytest.mark.parametrize('receiver,expected', [('8b5e04',True),('8d5e34',False),('8b5e08',False)])
def test_pause_requires_actual_saved_interface(receiver,expected):
    c=Code();c.emit('8bf1 '+receiver)
    c.label('retry');c.emit('8bce e800000000 85c0');c.jump('74','retry')
    c.emit('8b13 6a00 8bcb ff92ec050000 c3')
    rows=list(proof(c,include_pause=True).values())
    assert bool(rows)==expected
    if expected:
        assert rows==[dict(family='interface',offset=0,slot=0x5ec,pushes=[('imm',0)])]


def test_conflicting_interface_at_join_is_not_unpaused():
    c=Code();c.emit('8bf1 8b5e04 85c0');c.jump('74','join')
    c.emit('8d5e34');c.label('join');c.emit('8b13 6a00 8bcb ff92ec050000 c3')
    assert not proof(c,include_pause=True)
