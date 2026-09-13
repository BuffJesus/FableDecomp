"""Reviewed local input/result effects for annotated script-interface calls."""
import re
from dataclasses import dataclass

from tools.script_recovery.native_arguments import split_arguments


_SCALAR = {'bool', 'int', 'uint', 'long', 'unsigned int', 'unsigned long', 'float', 'double'}
_ATOM = re.compile(r'(?:\((?:bool|int|uint|long|float|double)\)\s*)?'
                   r'(?:[A-Za-z_]\w*|-?(?:0x[0-9a-fA-F]+|\d+(?:\.\d+)?))')


@dataclass(frozen=True)
class ProducedValue:
    kind: str
    statement: int
    callee: str


class ScalarCallEffects:
    def __init__(self, manifest, native_calls=()):
        self.signatures = {}
        self.results = {}
        self.const_distance_names = set()
        targets = {}
        for call in native_calls:
            targets.setdefault(call.get('currentName', ''), set()).add(str(call.get('target', '')).lower())
        distance = manifest.get('IsDistanceBetweenThingsUnder', {})
        if ([p.get('type') for p in distance.get('parameters', [])] ==
                ['const std::shared_ptr<CScriptThing>&', 'const std::shared_ptr<CScriptThing>&', 'float']
                and distance.get('returnType') == 'bool'):
            self.const_distance_names = {name for name, addresses in targets.items()
                                        if addresses == {'0x00cbe2ff'} and re.fullmatch(r'\w+', name)}
        lookup = manifest.get('GetThingWithScriptName', {})
        self.entity_lookup = (lookup.get('scope') == 'Quest'
            and lookup.get('returnType') == 'std::shared_ptr<CScriptThing>'
            and lookup.get('parameters') == [{'name': 'scriptName', 'type': 'const std::string&', 'optional': False}])
        for name, spec in manifest.items():
            params = spec.get('parameters', [])
            if (spec.get('scope') == 'Quest' and spec.get('returnType') in _SCALAR | {'void'}
                    and all(p.get('type') in _SCALAR and not p.get('optional') for p in params)):
                self.signatures[name] = len(params)
                result = spec['returnType']
                if result != 'void':
                    self.results[name] = ('bool' if result == 'bool' else
                                          'float' if result in ('float', 'double') else 'integer')
        # The reviewed interface GetHero slot returns a CScriptThing pointer
        # directly, with no hidden by-value output or explicit arguments.
        hero = manifest.get('GetHero', {})
        if (hero.get('scope') == 'Quest' and hero.get('parameters') == []
                and hero.get('returnType') == 'std::shared_ptr<CScriptThing>'):
            self.signatures['GetHero'] = 0
            self.results['GetHero'] = 'thing'

    def outputs(self, source, statement):
        """An explicit native hidden output receives a new entity wrapper value."""
        if not self.entity_lookup:
            return {}
        match = re.fullmatch(r'\s*GSI->GetThingWithScriptName\(&((?:\w*Stack_|local_)\w+),\s*"[^"\\]+"\);\s*', source)
        if not match:
            return {}
        return {match[1]: ProducedValue('thing', statement, 'GetThingWithScriptName')}

    def output_address_only(self, source, name):
        # This reviewed call fills the caller-owned return slot; its address is
        # not retained. Other address uses still count as escape.
        return name in self.outputs(source, 0)

    def result(self, expression, statement):
        expression = re.sub(r'^\(CScriptThing\s*\*\)\s*', '', expression.strip())
        match = re.fullmatch(r'GSI->(\w+)\((.*)\)', expression)
        if match and match[1] in self.results and self(expression + ';'):
            return ProducedValue(self.results[match[1]], statement, match[1])
        return None

    def __call__(self, source):
        """True proves input locals are passed by value; says nothing about world effects.

        Explicit entity outputs and the verified const-reference distance helper
        have separate profiles. Other pointer/reference calls remain unknown.
        """
        if self.outputs(source, 0):
            return True
        native = re.fullmatch(r'\s*(?:\w+\s*=\s*)?(\w+)\((.*)\);\s*', source)
        if native and native[1] in self.const_distance_names:
            try:
                args = split_arguments(native[2])
            except ValueError:
                return False
            return (len(args) == 3 and all(re.fullmatch(r'\w+', a) for a in args[:2])
                    and _ATOM.fullmatch(args[2]) is not None)
        match = re.fullmatch(r'\s*(?:\w+\s*=\s*)?GSI->(\w+)\((.*)\);\s*', source)
        if not match or match[1] not in self.signatures:
            return False
        try:
            args = split_arguments(match[2])
        except ValueError:
            return False
        return len(args) == self.signatures[match[1]] and all(_ATOM.fullmatch(arg) for arg in args)
