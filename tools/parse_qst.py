#!/usr/bin/env python3
"""Parser for Fable: TLC .qst quest-registry files (plain-text script format).

Format spec: docs/QST_FORMAT.md. A .qst is ASCII text containing
    AddQuest("<Name>", TRUE|FALSE);
    AddTestQuest("<Name>", "<StartHSP>", <int>, "<Display>", "<Ini>", "<End>", "<CardDef>");
statements with free-form whitespace, tolerated stray junk, and (per the
SilverChest cross-check) optional // and /* */ comments.

The parser is loss-less: every byte of the input is captured either as a
decoded statement or as an inter-statement gap, so re-serialization is
byte-identical by construction *and* the statement decoder is validated
against 100% of statements found.

Usage:
    python parse_qst.py parse <file.qst> [--json]
    python parse_qst.py roundtrip <file.qst>
    python parse_qst.py diff <vanilla.qst> <modded.qst>
"""

import json
import re
import sys
from dataclasses import dataclass, field

KEYWORDS = ("AddTestQuest", "AddQuest")  # longest first
IDENT_CHARS = re.compile(r"[A-Za-z0-9_]")


@dataclass
class Statement:
    keyword: str            # "AddQuest" | "AddTestQuest"
    args: list              # decoded argument strings (quotes stripped)
    raw: str                # exact source text of the whole call incl. ';'
    offset: int             # byte offset in file
    line: int               # 1-based line number

    # --- decoded views -------------------------------------------------
    @property
    def name(self):
        return self.args[0] if self.args else ""

    def decoded(self):
        if self.keyword == "AddQuest":
            return {"type": "AddQuest", "name": self.name,
                    "active": self.args[1].strip().upper() == "TRUE",
                    "line": self.line}
        return {"type": "AddTestQuest", "name": self.name,
                "start_hsp": self.args[1], "mode": int(self.args[2]),
                "display": self.args[3], "ini": self.args[4],
                "end_script": self.args[5], "card": self.args[6],
                "line": self.line}


@dataclass
class QstFile:
    # elements: alternating list of ("gap", str) and ("stmt", Statement)
    elements: list = field(default_factory=list)

    @property
    def statements(self):
        return [e[1] for e in self.elements if e[0] == "stmt"]

    def quests(self):
        return [s for s in self.statements if s.keyword == "AddQuest"]

    def test_quests(self):
        return [s for s in self.statements if s.keyword == "AddTestQuest"]

    def serialize(self):
        out = []
        for kind, val in self.elements:
            out.append(val.raw if kind == "stmt" else val)
        return "".join(out)


class ParseError(Exception):
    pass


def _line_of(text, idx):
    return text.count("\n", 0, idx) + 1


def parse(text):
    """Tokenize a .qst. Everything that is not a recognized call is a 'gap'
    (whitespace, blank lines, comments, stray junk like vanilla line 213)."""
    qst = QstFile()
    i, n = 0, len(text)
    gap_start = 0

    def at_keyword(pos):
        for kw in KEYWORDS:
            if text.startswith(kw, pos):
                before_ok = pos == 0 or not IDENT_CHARS.match(text[pos - 1])
                after = pos + len(kw)
                after_ok = after >= n or not IDENT_CHARS.match(text[after])
                if before_ok and after_ok:
                    return kw
        return None

    while i < n:
        c = text[i]
        # skip comments/strings inside gaps so a quoted "AddQuest" or a
        # commented-out call is not decoded (grammar per SilverChest parser)
        if c == "/" and text.startswith("//", i):
            j = text.find("\n", i)
            i = n if j < 0 else j + 1
            continue
        if c == "/" and text.startswith("/*", i):
            j = text.find("*/", i + 2)
            i = n if j < 0 else j + 2
            continue
        kw = at_keyword(i)
        if not kw:
            i += 1
            continue
        stmt, end = _parse_call(text, i, kw)
        if stmt is None:          # keyword not followed by a valid call
            i += len(kw)
            continue
        if gap_start < i:
            qst.elements.append(("gap", text[gap_start:i]))
        qst.elements.append(("stmt", stmt))
        i = end
        gap_start = end
    if gap_start < n:
        qst.elements.append(("gap", text[gap_start:]))
    _validate(qst)
    return qst


def _parse_call(text, start, kw):
    """Parse '<kw> ( args ) ;' beginning at `start`. Returns (Statement, end)
    or (None, None) if it is not a well-formed call."""
    n = len(text)
    i = start + len(kw)
    while i < n and text[i] in " \t\r\n":
        i += 1
    if i >= n or text[i] != "(":
        return None, None
    i += 1
    args, cur, depth, in_str = [], [], 1, False
    while i < n:
        c = text[i]
        if in_str:
            if c == "\\" and i + 1 < n:
                cur.append(text[i + 1]); i += 2; continue
            if c == '"':
                in_str = False; i += 1; continue
            cur.append(c); i += 1; continue
        if c == '"':
            in_str = True; i += 1; continue
        if c == "(":
            depth += 1; cur.append(c); i += 1; continue
        if c == ")":
            depth -= 1
            if depth == 0:
                args.append("".join(cur).strip()); i += 1; break
            cur.append(c); i += 1; continue
        if c == "," and depth == 1:
            args.append("".join(cur).strip()); cur = []; i += 1; continue
        cur.append(c); i += 1
    else:
        return None, None
    while i < n and text[i] in " \t":
        i += 1
    if i < n and text[i] == ";":
        i += 1
    return Statement(kw, args, text[start:i], start, _line_of(text, start)), i


def _validate(qst):
    for s in qst.statements:
        if s.keyword == "AddQuest":
            if len(s.args) != 2 or s.args[1].strip().upper() not in ("TRUE", "FALSE"):
                raise ParseError(f"line {s.line}: bad AddQuest args {s.args!r}")
        else:
            if len(s.args) != 7:
                raise ParseError(f"line {s.line}: AddTestQuest needs 7 args, got {len(s.args)}")
            int(s.args[2])  # must be an integer
        s.decoded()  # must decode cleanly


# ---------------------------------------------------------------- commands

def cmd_parse(path, as_json=False):
    text = open(path, newline="").read()
    qst = parse(text)
    dec = [s.decoded() for s in qst.statements]
    strays = [(kind, val) for kind, val in qst.elements
              if kind == "gap" and val.strip()]
    if as_json:
        print(json.dumps({"statements": dec,
                          "stray_text": [v.strip() for _, v in strays]}, indent=2))
        return
    print(f"{path}: {len(qst.quests())} AddQuest, {len(qst.test_quests())} AddTestQuest")
    for d in dec:
        if d["type"] == "AddQuest":
            print(f"  L{d['line']:>4} AddQuest      {d['name']:<40} "
                  f"{'ACTIVE' if d['active'] else 'dormant'}")
        else:
            print(f"  L{d['line']:>4} AddTestQuest  {d['name']:<40} mode={d['mode']} "
                  f"hsp={d['start_hsp'] or '-'} ini={d['ini'] or '-'} "
                  f"end={d['end_script'] or '-'} card={d['card'] or '-'} "
                  f"\"{d['display']}\"")
    for _, v in strays:
        print(f"  [stray junk tolerated] {v.strip()!r}")


def cmd_roundtrip(path):
    data = open(path, "rb").read()
    text = data.decode("ascii")
    qst = parse(text)
    out = qst.serialize().encode("ascii")
    ok = out == data
    print(f"{path}: {'byte-identical' if ok else 'MISMATCH'} "
          f"({len(qst.quests())} quests, {len(qst.test_quests())} test quests, "
          f"{len(data)} bytes)")
    return 0 if ok else 1


def cmd_diff(path_a, path_b):
    qa = parse(open(path_a, newline="").read())
    qb = parse(open(path_b, newline="").read())
    rc = 0
    for label, sel in (("AddQuest", "quests"), ("AddTestQuest", "test_quests")):
        # key on (name, occurrence-index) so duplicate names diff positionally
        def keyed(q):
            seen, out = {}, {}
            for s in getattr(q, sel)():
                k = (s.name, seen.setdefault(s.name, 0))
                seen[s.name] += 1
                out[k] = s
            return out
        da, db = keyed(qa), keyed(qb)
        for k in sorted(set(da) | set(db)):
            name = k[0] if k[1] == 0 else f"{k[0]}#{k[1]+1}"
            if k not in da:
                print(f"+ {label} {name}: added -> {db[k].decoded()}"); rc = 1
            elif k not in db:
                print(f"- {label} {name}: removed (was {da[k].decoded()})"); rc = 1
            else:
                a, b = da[k].decoded(), db[k].decoded()
                changed = {f: (a[f], b[f]) for f in a
                           if f != "line" and a[f] != b[f]}
                if changed:
                    rc = 1
                    for f, (va, vb) in changed.items():
                        print(f"~ {label} {name}: {f} {va!r} -> {vb!r}")
    if rc == 0:
        print("no decoded differences")
    return rc


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    cmd = argv[1]
    if cmd == "parse":
        cmd_parse(argv[2], "--json" in argv)
        return 0
    if cmd == "roundtrip":
        return cmd_roundtrip(argv[2])
    if cmd == "diff" and len(argv) >= 4:
        return cmd_diff(argv[2], argv[3])
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
