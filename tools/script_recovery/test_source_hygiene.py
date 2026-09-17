"""Shell-scripted patches have left literal control bytes in these tools (a BACKSPACE where `\\b` was meant
silently disables a regex; `\\1` turned into 0x01 corrupts a replacement). Every source stays printable."""
from pathlib import Path

HERE = Path(__file__).resolve().parent


def test_no_control_bytes_in_sources():
    bad = {}
    for path in sorted(HERE.glob('*.py')):
        data = path.read_bytes()
        hits = [i for i, c in enumerate(data) if c < 32 and c not in (9, 10, 13)]
        if hits:
            bad[path.name] = [repr(data[max(0, i - 30):i + 10]) for i in hits[:3]]
    assert not bad, bad
