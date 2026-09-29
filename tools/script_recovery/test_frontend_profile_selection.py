from tools.script_recovery.autopilot import profile_row_from_ocr
import pytest


@pytest.mark.parametrize('text,row', [
    ('476 242 72 22\tOatlas', 254),
    ('476 287 72 22\t0atlas', 298),
    ('476 467 72 22\t0atlas', 478),
    ('476 242 72 22\t1\n476 287 72 22\t0atlas2', None),
    ('bad 242 72 22\t0atlas', None),
    ('10 242 72 22\t0atlas', None),
    ('476 198 72 22\t0atlas', None),
    ('476 242 72 22\t0atlas\n476 287 72 22\tOatlas', None),
])
def test_profile_label_position(text, row):
    assert profile_row_from_ocr(text) == row
