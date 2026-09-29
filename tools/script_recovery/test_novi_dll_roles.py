"""Reject DLL role mistakes even when the payload manifest is self-consistent."""
import hashlib
import json
from unittest.mock import patch

import pytest

from tools.script_recovery import novi_local_test as launcher


def test_sidecar_in_stock_slot_rejected_with_matching_manifest(tmp_path):
    payload = b'sidecar with NoviCompatibilityStart'
    files = {'FableScriptExtender.dll': payload, 'NoviCompatibility.dll': payload}
    for name, data in files.items():
        (tmp_path/name).write_bytes(data)
    (tmp_path/'manifest.json').write_text(json.dumps({
        'files': {name:hashlib.sha256(data).hexdigest() for name,data in files.items()}}))
    # No game directory is needed: reject the role before accessing the game.
    with pytest.raises(ValueError, match='Stock FSE DLL role mismatch'):
        launcher.validate(tmp_path, tmp_path/'absent-game')


@pytest.mark.parametrize('addon,valid', [(b'stock DLL', False),
                                       (b'_NoviCompatibilityStart@4', True)])
def test_addon_entrypoint_required(tmp_path, addon, valid):
    (tmp_path/'NoviCompatibility.dll').write_bytes(addon)
    with patch.object(launcher, 'sha', return_value=
                      '36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea'):
        if valid:
            launcher.validate_dll_roles(tmp_path)
        else:
            with pytest.raises(ValueError, match='Add-on DLL lacks'):
                launcher.validate_dll_roles(tmp_path)
