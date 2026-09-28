"""Table-returning Forge bindings fill the stack slots retail's out-pointers named."""
import re
from pathlib import Path

from tools.script_recovery.lift_native_lua import OUT_TABLE_FIELDS

ROOT = Path(__file__).resolve().parents[2]
DRAFT = ROOT / 'refs/script_recovery/lifted/SickChild/draft/FSE/V_SickChild/Entities/SickChild.lua'


def test_sleeping_position_fields_follow_native_operand_order():
    # vtable 0xBC0 (pBed, pSleeper, &pos, &orient) -> {pos = ..., orient = ...}
    assert OUT_TABLE_FIELDS['GetSleepingPositionAndOrientationFromBed'] == ('pos', 'orient')


def test_sick_child_orientation_reaches_the_atan():
    # SickChild Main 0x00EC5DE0: pOutOrient = &fStack_24, then fpatan(fStack_24, fStack_20)
    lines = DRAFT.read_text(encoding='utf-8').splitlines()
    call = next(i for i, l in enumerate(lines) if 'GetSleepingPositionAndOrientationFromBed' in l)
    result = re.match(r'\s*(\w+) = quest:', lines[call])[1]
    after = '\n'.join(lines[call + 1:call + 4])
    assert f'fStack_24 = {result} and {result}.orient.x or 0.0' in after
    assert f'f_stk_20 = {result} and {result}.orient.y or 0.0' in after
    assert any('math.atan(fStack_24,f_stk_20)' in l for l in lines[call + 1:call + 6])
