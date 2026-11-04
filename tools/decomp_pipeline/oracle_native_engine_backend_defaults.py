"""PC engine backend defaults, compared using the shared full-state ABI oracle."""
import json
from pathlib import Path
from oracle_native_iengine_defaults import collect as _collect
SPECS=json.loads((Path(__file__).with_name('native_engine_backend_defaults.json')).read_text())
def collect():return _collect(SPECS)
