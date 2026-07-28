#!/usr/bin/env python3
"""Small typed view over the shipped frontend.bin widget records."""

from parse_frontend import decode_entry, load_all


class FrontendLayoutOracle:
    def __init__(self, game_root, schema_path):
        entries, schema, _ = load_all(game_root, schema_path)
        self._schema = schema
        self._by_index = {entry["index"]: entry for entry in entries}
        self._by_name = {
            entry["name"]: entry for entry in entries if entry["name"]
        }
        self._decoded = {}

    def entry(self, key):
        source = (
            self._by_index.get(key)
            if isinstance(key, int)
            else self._by_name.get(key)
        )
        if source is None:
            raise ValueError("frontend layout entry is absent: %s" % key)
        return source

    def decoded(self, key):
        source = self.entry(key)
        index = source["index"]
        if index not in self._decoded:
            result = decode_entry(source, self._schema)
            if result is None:
                raise ValueError(
                    "frontend layout entry has no schema: %s" % key)
            values, leftover = result
            if leftover:
                raise ValueError(
                    "frontend layout entry has %d undecoded bytes: %s" %
                    (leftover, key))
            self._decoded[index] = values
        return self._decoded[index]

    def child_indices(self, key):
        return tuple(self.decoded(key)["Children"])

    def child_names(self, key):
        return tuple(
            self.entry(index)["name"] for index in self.child_indices(key))

    def table_sprites(self, key):
        return tuple(
            (
                sprite["value"],
                sprite["component"],
                self.entry(sprite["component"])["name"],
            )
            for sprite in self.decoded(key)["Sprites"])

    def initial_position(self, key):
        states = self.decoded(key)["States"]
        if not states:
            raise ValueError(
                "frontend layout entry has no serialized state: %s" % key)
        return states[0]["PositionX"], states[0]["PositionY"]


def require_equal(label, actual, expected):
    if actual != expected:
        raise ValueError(
            "%s does not match frontend.bin: expected %r, got %r" %
            (label, expected, actual))


def require_position(label, actual, expected):
    require_equal(
        label,
        tuple(float(value) for value in actual),
        tuple(float(value) for value in expected))
