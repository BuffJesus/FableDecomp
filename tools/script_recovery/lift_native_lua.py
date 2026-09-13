#!/usr/bin/env python3
"""Lift native script decompiles (Ghidra) into first-draft ForgeFSE Lua.

Two modes:
  quest mode   --script V_MazeResearch                (reads refs/script_recovery/native_clusters)
  entity mode  --tu <translation_unit.json> --entity NAME --init 0xADDR --main 0xADDR

This is a converter, not an oracle. It emits Lua only where the native shape is recognised
(interface calls through the proven slot table, string temporaries and .rdata string pointers,
persistence transfers, state field writes, entity bindings, the frame/terminate idiom, CScriptThing
slot calls through the proven thing vtable with receiver aliases (`me`, things returned by the
interface), structured control flow) and marks everything else with `-- TODO(native): ...` so a human
finishes the script
against the trace/fixture harness. `--compare` scores the draft against a hand port (Aeon's or ours)
by interface-call recall/precision.
"""
from __future__ import annotations

import argparse
import copy
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "script_recovery"))
from annotate_interface_slots import RETAIL_EXE, annotate, load_slots, load_thing_slots  # noqa: E402
from native_literal_loops import fold_literal_comparisons  # noqa: E402
from native_random import fold_retail_random, fold_book_trader_random, fold_affair_wife_random  # noqa: E402
from native_arguments import split_arguments, strip_template_pointer_casts  # noqa: E402
from native_thing_predicates import recover_reviewed_thing_predicates  # noqa: E402
from native_call_operands import recover_reviewed_call_operands  # noqa: E402
from native_self_wrapper import fold_self_wrapper_arguments  # noqa: E402
from native_deeds import recover_deed_operands  # noqa: E402
from native_goto_scopes import supported_jumps  # noqa: E402
from native_subregisters import fold_literal_slices, fold_unsigned_three_byte_casts  # noqa: E402
from native_constant_conditions import fold_decisive_condition  # noqa: E402
from native_conditions import conditional_call_assignment  # noqa: E402
from native_termination_expressions import fold_inline_termination  # noqa: E402
from native_presented_items import recover_presented_item_comparison  # noqa: E402
from native_teddy_distance import recover_teddy_distance, recover_theresa_distance, recover_barrel_distance  # noqa: E402
from native_teddy_distance import recover_post_attack_distance  # noqa: E402
from native_entry_timer import recover_entry_timer  # noqa: E402
from native_loop_timer import recover_loop_timer  # noqa: E402
from native_colours import colour_literal, recover_barrel_colours  # noqa: E402
from native_barrel_update import recover_barrel_update  # noqa: E402
from native_barrel_creation import recover_barrel_creation  # noqa: E402
from native_quest_markers import recover_quest_markers  # noqa: E402
from native_marker_cleanup import recover_marker_cleanup, OWNERSHIP_LIMITATION  # noqa: E402
from native_post_attack_cleanup import recover_post_attack_cleanup  # noqa: E402
from native_post_attack_scalars import recover_post_attack_scalars  # noqa: E402
from native_post_attack_scalars import recover_post_attack_movie_flags  # noqa: E402
from native_post_attack_limbo import recover_post_attack_limbo  # noqa: E402
from native_post_attack_resources import map_post_attack_resources, map_book_trader_resources  # noqa: E402
from native_post_attack_movie_lifetime import recover_post_attack_movie_lifetime  # noqa: E402
from native_barrel_position import map_barrel_position, recover_first_barrel_actors  # noqa: E402
from native_guard_cleanup import recover_guard_cleanup  # noqa: E402
from native_teddy_distance import recover_affair_man_distance, recover_affair_man_woman_distance  # noqa: E402
from native_sequence_conditions import expand_sequence_conditions  # noqa: E402
from native_cached_thing_queries import recover_cached_thing_queries  # noqa: E402
from native_affair_facing import recover_affair_facing  # noqa: E402
from native_affair_pause import recover_affair_pause, recover_book_trader_pause, recover_affair_wife_pause  # noqa: E402
from native_book_trader_timer import recover_book_trader_timer, recover_affair_wife_timer  # noqa: E402
from native_book_trader_line import recover_book_trader_line  # noqa: E402
from native_book_trader_health import recover_book_trader_health, recover_affair_wife_health, recover_affair_man_health  # noqa: E402
from native_book_trader_allies import recover_book_trader_allies, recover_affair_wife_allies  # noqa: E402
from native_affair_movies import recover_book_trader_movies, recover_affair_wife_movies  # noqa: E402
from native_book_trader_cleanup import recover_book_trader_cleanup, recover_affair_wife_cleanup  # noqa: E402
from native_book_trader_hits import recover_book_trader_hits, recover_affair_wife_hits, recover_affair_wife_argument_hits  # noqa: E402
from native_book_trader_termination import recover_book_trader_termination, recover_affair_wife_termination  # noqa: E402
from native_book_trader_acquisition import recover_book_trader_acquisition  # noqa: E402
from native_speech_vectors import recover_init_vector_construction  # noqa: E402
from native_init_scalars import recover_init_scalars, recover_init_bad_deeds  # noqa: E402
from native_affair_wife_route import recover_affair_wife_route  # noqa: E402
from native_teddy_distance import recover_affair_wife_distance  # noqa: E402
from native_hero_distance import recover_affair_wife_hero_distances  # noqa: E402
from native_affair_wife_mask import recover_affair_wife_mask  # noqa: E402
from native_affair_wife_facing import recover_affair_wife_facing  # noqa: E402
from native_affair_wife_acquisition import recover_affair_wife_acquisition  # noqa: E402
from native_affair_wife_conversation import recover_affair_wife_conversation, recover_affair_wife_running_line  # noqa: E402
from native_affair_wife_argument_conversation import recover_affair_wife_argument_conversation  # noqa: E402
from native_affair_wife_text_sequence import recover_affair_wife_text_sequence  # noqa: E402
from native_affair_wife_reply import recover_affair_wife_reply  # noqa: E402
from native_affair_wife_animation_random import recover_affair_wife_animation_random  # noqa: E402
from native_affair_wife_animation_operands import recover_affair_wife_animation_operands  # noqa: E402
from native_affair_wife_position import map_affair_wife_position  # noqa: E402
from native_book_trader_hits import recover_affair_man_hits  # noqa: E402
from native_affair_wife_mask import recover_affair_man_mask  # noqa: E402
from native_affair_man_conversation import recover_affair_man_conversation, recover_affair_man_near_lines, recover_affair_man_ranged_lines  # noqa: E402
from native_affair_man_conversation import recover_affair_man_affection_setup  # noqa: E402
from native_affair_man_conversation import recover_affair_man_animation_operands  # noqa: E402
from native_hero_distance import recover_affair_man_hero_distances  # noqa: E402
from native_affair_man_conversation import recover_affair_man_question  # noqa: E402
from native_affair_man_conversation import recover_affair_man_question_cleanup  # noqa: E402
from native_affair_man_conversation import recover_affair_man_home  # noqa: E402
from native_affair_man_conversation import recover_affair_man_question_entry  # noqa: E402
from native_affair_man_conversation import recover_affair_man_talk_continuation  # noqa: E402
from native_affair_man_speech import map_affair_man_speech  # noqa: E402
from native_post_attack_resources import map_affair_man_resources  # noqa: E402
from native_affair_movies import recover_affair_movies  # noqa: E402
from native_post_attack_logbook import recover_post_attack_logbook  # noqa: E402
from native_post_attack_teleport import recover_post_attack_teleport  # noqa: E402
from native_readable_names import readable_names, rename_lua_locals  # noqa: E402

CLUSTERS = ROOT / "refs" / "script_recovery" / "native_clusters"
MANIFEST = ROOT / "refs" / "fse_api_manifest.json"
ENTITY_INVENTORIES = ROOT / "refs" / "script_recovery" / "new_oakvale_intro" / "entities"

# Retail callees whose decompiler label is unreliable (bsim similarity names); keyed by address.
CALLEE_ALIASES = {
    0x005BC746: "IsActiveThreadTerminating", 0x006A8BE0: "IsActiveThreadTerminating",
    0x00F35B30: "IsActiveThreadTerminating",
    0x00CBE9EE: "AddLogbookTutorialEntry",
}
TERMINATING_NAMES = ("IsActiveThreadTerminating",)

TYPE_MAP = {"signed_char": "Bool", "char": "Bool", "bool": "Bool", "int": "Int", "uint": "Int",
            "long": "Int", "float": "Float", "class_CCharString": "String", "CCharString": "String"}
LUA_DEFAULT = {"Bool": "false", "Int": "0", "Float": "0.0", "String": '""'}
STORE_TYPE = {"undefined1": "Bool", "byte": "Bool", "char": "Bool", "bool": "Bool", "undefined4": "Int",
              "int": "Int", "uint": "Int", "long": "Int", "float": "Float"}

RE_CSTR = re.compile(r'^\s*CCharString::CCharString\s*\(\s*(?:\(CCharString \*\))?&?(\w+|stack0x[0-9a-f]+)\s*,\s*"((?:[^"\\]|\\.)*)"\s*,\s*-1\s*\);\s*$')
RE_CSTR_DEFAULT = re.compile(
    r'^\s*CCharString::CCharString\s*\(\s*(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*\);\s*$')
RE_CSTR_COPY = re.compile(
    r'^\s*CCharString::CCharString\s*\(\s*(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*,\s*'
    r'(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*\);\s*$')
RE_CSTR_FROM_VALUE = re.compile(
    r'^\s*CCharString::CCharString\s*\(\s*(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*,\s*'
    r'(\w+|stack0x[0-9a-f]+)\s*,\s*-1\s*\);\s*$')
RE_CSTR_LOCAL_ASSIGN = re.compile(
    r'^\s*CCharString::operator=\s*\(\s*(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*,\s*'
    r'(?:\(CCharString \*\)\s*)?&?(\w+|stack0x[0-9a-f]+)\s*\);\s*$')
RE_CSTR_FIELD_ASSIGN = re.compile(
    r'^\s*CCharString::operator=\s*\(\s*\(CCharString \*\)\s*\((?:this|param_1)\s*\+\s*'
    r'(0x[0-9a-fA-F]+)\)\s*,\s*"((?:[^"\\]|\\.)*)"\s*\);\s*$')
RE_CSTR_MASTER_ASSIGN = re.compile(
    r'^\s*CCharString::operator=\s*\(\(CCharString \*\)\(\*\(int \*\)\((?:this|param_1) \+ 0x18\) \+ '
    r'(0x[0-9a-fA-F]+)\),"((?:[^"\\]|\\.)*)"\);\s*$')
RE_SLOT_ASSIGN = re.compile(r'^\s*((?:[pu]|pu|pC|pi|pf)?[a-zA-Z]*Stack_\w+|local_\w+) = (?:\([^)]*\))?(.+);\s*$')
RE_LOCAL_ASSIGN = re.compile(r'^\s*([A-Za-z]{1,3}Var\d+(?:_\d+)?|native_arg_\w+) = (.+);\s*$')
RE_ADDR_OF = re.compile(r'^(?:\([^)]*\))?\s*&(\w+|stack0x[0-9a-f]+)$')
RE_STRING_PARAM = re.compile(r'string|CCharString|char', re.I)
RE_TRANSFER = re.compile(
    r'CPersistContext::Transfer<(\w+)>\s*\(\s*\w+\s*,\s*"(\w+)"\s*,\s*\([^)]*\)\(\w+ \+ (0x[0-9a-fA-F]+)\)')
RE_BINDING = re.compile(
    r'(?P<var>pCVar\d+) = (?:::)?operator_new\(0x1c\);.*?'
    r'CCharString::CCharString\((?:\(CCharString \*\))?&?\w+,"(?P<name>[^"]+)",-1\);.*?'
    r'CScriptBase::AddEntityScriptBinding\([^,]+,(?P=var)\);\s*if \([^{]*\) \{[^}]*?_Cons_val[^}]*?\}', re.S)
RE_CONS_VAL = re.compile(r'std::\s*_Cons_val<[^;]*?;', re.S)
RE_GSI = re.compile(r'^\s*(?:(\w+) = )?(?:\([^;]*?\)\s*)?GSI->(\w+)\s*\((.*)\);\s*$')
RE_NAMED_CALL = re.compile(r'^\s*(?:(\w+) = )?(?:\([^;]*?\)\s*)?([\w:~]+)\s*\((.*)\);\s*$')
RE_IF_GOTO = re.compile(r'^\s*if \((.*)\) goto (LAB_[0-9a-f]+);\s*$')
RE_IF_BREAK = re.compile(r'^\s*if\s*\((.*)\)\s*break;\s*$')
RE_THING_CALL = re.compile(r'^\s*(?:(\w+) = )?(?:\([^;]*?\)\s*)?CScriptThing::(\w+)\s*\(\s*([^,]+?)\s*(?:,(.*))?\);\s*$')
RE_THREAD = re.compile(
    r'(\w+) = (?:::)?operator_new\(0x3c\);.*?CCharString::CCharString\((?:\(CCharString \*\))?&?\w+,"([^"]+)",-1\);'
    r'.*?CSpawnedFunc<[^>]*>::\s*CSpawnedFunc<[^>]*>\s*\([^;]*;.*?\+ 0x34\) = &?([\w:]+);.*?\}'
    r'(?:\s*CGuiVarTransferStruct::Add\([^;]*;)?', re.S)
RE_GUI_TRANSFER_ADD = re.compile(
    r'CGuiVarTransferStruct::Add\([^,]+,\s*\(CGuiVarTransferBase \*\)(\w+)\s*\);')
RE_STORE = re.compile(r'^\s*\*\((\w+) \*\)\((?:this|param_1) \+ (0x[0-9a-fA-F]+)\) = ([^;]+);\s*$')
RE_STORE_IDX = re.compile(r'^\s*(?:this|param_1)\[(0x[0-9a-fA-F]+)\] = (?:\([^)]*\))?([^;]+);\s*$')
RE_LOAD = re.compile(r'\*\((\w+) \*\)\((?:this|param_1) \+ (0x[0-9a-fA-F]+)\)|(?:this|param_1)\[(0x[0-9a-fA-F]+)\]')
RE_PARENT_LOAD = re.compile(
    r'\*\((\w+) \*\)\(\*\(int \*\)\((?:this|param_1) \+ 0x14\) \+ (0x[0-9a-fA-F]+)\)')
RE_IF = re.compile(r'^\s*if \((.*)\) \{\s*$')
RE_IF_RECHECK_TERMINATING = re.compile(
    r'^\s*if \(\(\s*!\s*(?P<var>\w+)\s*\)\s*&&\s*\(\s*(?P=var)\s*=\s*'
    r'CScriptBase::IsActiveThreadTerminating\(.*?\)\s*,\s*!\s*(?P=var)\s*\)\)\s*\{\s*$')
RE_ELSE_IF = re.compile(r'^\s*else if \((.*)\) \{\s*$')
RE_WHILE = re.compile(r'^\s*while\s*\((.*)\)\s*\{\s*$')
RE_DO_WHILE_END = re.compile(r'^\s*\}\s*while\s*\(\s*(.*?)\s*\)\s*;\s*$')
RE_LABEL = re.compile(r'^\s*(LAB_[0-9a-f]+):\s*(.*)$')
RE_GOTO = re.compile(r'^\s*goto (LAB_[0-9a-f]+);\s*$')
RE_CAST = re.compile(r'\((?:undefined\d?|byte|uint|int|float(?:10)?|bool|char|long|short|ushort|code|[A-Za-z_]\w*(?:::\w+)* \*+|[A-Z]\w+)\)')
RE_DECL = re.compile(r'^\s*[\w:<>,\*\s]+\s+\*{0,3}\w+(\s*\[\d+\])?;\s*$')
RE_EXTRAOUT = re.compile(r"extraout_AL(?:_\d+)?\s*(==|!=)\s*'\\0'")
RE_PAIRED_TERMINATION_ALIAS = re.compile(
    r'CCreatureAction_TrollWhackGroundBase::Initialise\s*\([^;]*\);\s*'
    r'(?=if\s*\(\s*extraout_AL(?:_\d+)?\s*(?:==|!=)\s*\'\\0\')', re.S)
RE_ADDR_LITERAL = re.compile(r'^0x([0-9a-f]{6,7})$')
RE_RESOURCE_METHOD = re.compile(r'_?(\w+?)_CScriptGameResourceObjectScriptedThingBase')
# CScriptThing message getters take the hitter/talker script name; ForgeFSE only binds the
# hero-specialised forms, which the hand ports use (me:MsgIsHitByHero() etc.). Applied only when the
# recovered string operand is "SCRIPT_NAME_HERO"; any other name keeps the raw slot name and a TODO.
HERO_FORMS = {
    "MsgIsHitBy": "MsgIsHitByHero", "MsgIsTalkedToBy": "IsTalkedToByHero",
    "MsgIsHitByAnySpecialAbilityFrom": "MsgIsHitByAnySpecialAbilityFromHero",
    "MsgIsHitBySpecialAbilityFrom": "MsgIsHitByHeroSpecialAbility",
    "MsgIsTriggeredBy": "MsgIsTriggeredByHero", "MsgIsUsedBy": "MsgIsUsedByHero",
    "MsgIsKnockedOutBy": "MsgIsKnockedOutByHero", "MsgIsHitByWithFlourish": "MsgIsHitByHeroWithFlourish",
    "MsgIsHitByWithDecapitate": "MsgIsHitByHeroWithDecapitate", "MsgIsHitByWithWeapon": "MsgIsHitByHeroWithWeapon",
    "MsgIsHitByWithProjectileWeapon": "MsgIsHitByHeroWithProjectileWeapon",
}
HERO_NAME = '"SCRIPT_NAME_HERO"'
# MSVC decorated CScriptThing slot: `?Name@CScriptThing@@<conv><ret><params>@Z` / `...XZ`.
RE_THING_SIG = re.compile(r'^\?\w+@CScriptThing@@[UM][AB]E(?P<ret>\?AV1@|\?A[VW]4?[\w$?]*?@@|[AP][AB]V[\w$?]*?@@|_[A-Z]|[A-Z])'
                          r'(?P<params>.*?)@?Z$')
RE_SIG_TOKEN = re.compile(r'W4\w+?@@|A[AB]V\w+?@@|P[AB]V\w+?@@|A[AB]V\d@|A[AB][A-Z]|_[A-Z]|[A-Z]|\d')
# The hidden return-slot operand Ghidra shows for by-value class returns (`&stack0x..`, `&pos`, `auStack_48`).
RE_HIDDEN_SLOT = re.compile(r'^(?:\([^)]*\))?\s*&?(?:stack0x[0-9a-f]+|local_\w+|pos|\w*Stack_\w+)$')
# CBaseIntelligentPointer release: `if ((p != 0x0) && (*p = *p + -1, *p == 0x0)) { (*(code *)p[1])(); operator_delete(p); }`
RE_REFCOUNT_IF = re.compile(
    r'^\s*if \(\(?(?P<v>\w+) != \(.+?\)0x0\) &&\s*'
    r'\(\*(?P=v) = \*(?P=v) \+ -1, \*(?P=v) == (?:\(.+?\))?(?:0|0x0)\)\) \{\s*$')
# A numeric literal or an arithmetic/bitwise expression over locals and literals (`uVar4 | 1`).
RE_NUMERIC_EXPR = re.compile(r'-?(?:\d+(?:\.\d+)?|0x[0-9a-f]+)|[\w\s()]+[|&^+\-*/%<>]+[\w\s()|&^+\-*/%<>]+')
RE_NOISE = (
    re.compile(r'^\s*\(\*\(code \*\)\w+(?:\[\d+\])+\)\(\);\s*$'),                       # refcount release call
    re.compile(r'^\s*(?:iVar\d+|piVar\d+|iStack_[0-9a-f]+|ppuVar\d+) = \*{1,2}\(int \*\*\)\((?:this|param_\d+) \+ (?:4|0x40)\);\s*$'),
)
NOISE = ("extraout_EDX", "unaff_EDI", "uStack_4 = 0;", "CTCVillage::OnInitialActivate", "PTR_LAB_",
         "CTriangleStripifier", "operator_new(", "operator_delete(", "CCharString::~CCharString", "FUN_0",
         "**(int **)(param_1 + 0x40)", "**(int **)(this + 0x40)", "& 0xffffff00", "= param_1 & 0xffffff",
         "*DAT_0143e8f8", "CScriptThing::CScriptThing(", "AllocateMapThingUID", "~C3DClothPrimitive",
         "CScriptThing::~CScriptThing", "**(this + 4)", "CBaseIntelligentPointer::", "_scalar_deleting_destructor",
         "_vector_deleting_destructor", "CMemoryDataOutputStream::Clear", "= me;", "::~", "local_4 = 0;",
         # native_helper_operation_ir.json identifies 0x0099A2E0 as a complete opaque-vtable-token
         # initializer. Its only effect is native temporary object lifetime; Lua has no counterpart.
         "NHeroInformationScreens::CBase::CBase(")


class RData:
    """Read NUL-terminated strings from the installed retail image (for unresolved literal pointers)."""

    def __init__(self, exe: Path = RETAIL_EXE):
        self.ok = exe.exists()
        if not self.ok:
            return
        data = exe.read_bytes()
        pe = struct.unpack_from("<I", data, 0x3C)[0]
        nsec = struct.unpack_from("<H", data, pe + 6)[0]
        opt = struct.unpack_from("<H", data, pe + 20)[0]
        self.base = struct.unpack_from("<I", data, pe + 24 + 28)[0]
        self.data = data
        self.secs = []
        self.raw_secs = []
        for i in range(nsec):
            off = pe + 24 + opt + i * 40
            vsize, va, rsize, raw = struct.unpack_from("<IIII", data, off + 8)
            self.secs.append((va, max(vsize, rsize), raw))
            self.raw_secs.append((va, rsize, raw))

    def bytes_at(self, va: int, size: int) -> bytes | None:
        """Read a complete, file-backed region; virtual padding is not native byte evidence."""
        if not self.ok or size <= 0:
            return None
        rva = va - self.base
        for sva, raw_size, raw in self.raw_secs:
            if sva <= rva and rva + size <= sva + raw_size:
                offset = raw + rva - sva
                data = self.data[offset:offset + size]
                return data if len(data) == size else None
        return None

    def string_at(self, va: int) -> str | None:
        """Read a printable literal wholly contained in a file-backed section.

        Virtual padding is initialized by the loader and may change at runtime;
        its RVA must never be used as an offset into unrelated file bytes.
        """
        if not self.ok:
            return None
        rva = va - self.base
        for sva, size, raw in self.raw_secs:
            if sva <= rva < sva + size:
                off = raw + (rva - sva)
                end = self.data.find(b"\0", off, min(off + 512, raw + size, len(self.data)))
                if end < 0:
                    return None
                chunk = self.data[off:end]
                if chunk and all(32 <= b < 127 for b in chunk):
                    return chunk.decode("ascii")
                return None
        return None


def load_manifest() -> dict[str, dict]:
    data = json.loads(MANIFEST.read_text(encoding="utf-8-sig"))
    functions = data["functions"] if isinstance(data["functions"], list) else list(data["functions"].values())
    return converter_signatures({f["name"]: f for f in functions})


def converter_signatures(manifest: dict[str, dict]) -> dict[str, dict]:
    """Supply reviewed host signatures without modifying the shared SDK manifest.

    Retail engine_api.tsv at 0x008910D0 supplies thing,bool,bool. The current runtime's
    LuaQuestState::RemoveThing accepts optional immediate/removeFromWorld flags with false/true
    defaults. NewOakValeIntro's beetle explicitly passes true,true; dropping them changes behavior.
    A newer or differently shaped manifest entry is left intact for its own review.
    """
    result = copy.deepcopy(manifest)
    # LuaManager binds MoveToPosition to the existing-handle, nonblocking API.
    # Keep both native movement flags; the older SDK omits them.
    movement = result.get('MoveToPosition', {})
    params = movement.get('parameters', [])
    if (movement.get('scope') == 'Entity' and movement.get('returnType') == 'void'
            and [(p.get('name'), p.get('type')) for p in params] == [
                ('pMe', 'CScriptThing*'), ('position', 'sol::table'),
                ('radius', 'float'), ('moveType', 'int')]):
        params[1]['nativeKind'] = 'vector'
        params[3].update(type='sol::optional<int>', optional=True)
        params.extend({'name': name, 'type': 'sol::optional<bool>', 'optional': True}
                      for name in ('b1', 'b2'))
        movement['blocking'] = False
    question = result.get('GiveHeroYesNoQuestion', {})
    params = question.get('parameters', [])
    if (question.get('scope') == 'Quest' and question.get('returnType') == 'int'
            and [(p.get('name'), p.get('type')) for p in params] == [
                (name, 'const std::string&') for name in ('question','button1','button2','button3')]):
        question['returnType'] = 'void'
        params.append({'name':'b5','type':'sol::optional<bool>','optional':True})
    # LuaManager / LuaEntityAPI forward this query to retail 0x00CBE45C.
    # Retain native squared-distance and boundary semantics instead of Lua math.
    result.setdefault('IsDistanceFromPositionOver', {
        'scope': 'Entity', 'returnType': 'bool', 'blocking': False,
        'parameters': [{'name': 'pMe', 'type': 'CScriptThing*'},
                       {'name': 'position', 'type': 'sol::table', 'nativeKind': 'vector'},
                       {'name': 'distance', 'type': 'float'}]})
    centre = result.get('SetWanderCentrePoint', {})
    for parameter in centre.get('parameters', []):
        if parameter.get('name') == 'pos' and parameter.get('type') == 'sol::table':
            parameter['nativeKind'] = 'vector'
    result.setdefault('RetailRandModulo', {'scope': 'Quest', 'returnType': 'int',
                      'parameters': [{'name': 'modulus', 'type': 'int'}]})
    # LuaManager exposes the existing-handle, nonblocking action clear. This is
    # distinct from ClearCommands and takes no native arguments after receiver.
    result.setdefault('ClearAllActions', {'name': 'ClearAllActions', 'scope': 'Entity',
                      'returnType': 'void', 'blocking': False,
                      'parameters': [{'name': 'pMe', 'type': 'CScriptThing*'}]})
    # Reviewed Forge binding: LuaManager exposes LuaQuestState's bool query,
    # forwarding native vtable[394] / 0x628 (0x0088E980), with no explicit args.
    # The shared SDK manifest predates this binding. Preserve newer entries.
    result.setdefault('IsHeroControlledByPlayer', {'name': 'IsHeroControlledByPlayer',
                      'scope': 'Quest', 'returnType': 'bool', 'parameters': [], 'blocking': False})
    spec = result.get("RemoveThing", {})
    params = spec.get("parameters", [])
    if (spec.get("scope") == "Quest" and spec.get("returnType") == "void" and len(params) == 1
            and params[0].get("name") == "pThing" and params[0].get("type") == "CScriptThing*"):
        params.extend([
            {"name": "immediate", "type": "sol::optional<bool>", "optional": True},
            {"name": "removeFromWorld", "type": "sol::optional<bool>", "optional": True},
        ])
    teleport = result.get('EntityTeleportToThing', {})
    params = teleport.get('parameters', [])
    if (teleport.get('scope') == 'Quest' and teleport.get('returnType') == 'void'
            and [p.get('type') for p in params] == ['CScriptThing*', 'CScriptThing*']):
        params.append({'name': 'b3', 'type': 'sol::optional<bool>', 'optional': True})
    # LuaQuestState.cpp forwards snap.value_or(false) to native slot0x76C.
    facing = result.get('EntitySetFacingAngleTowardsThing', {})
    params = facing.get('parameters', [])
    if (facing.get('scope') == 'Quest' and facing.get('returnType') == 'void'
            and [(p.get('name'), p.get('type')) for p in params] ==
            [('pThing', 'CScriptThing*'), ('pTarget', 'CScriptThing*')]):
        params.append({'name': 'snap', 'type': 'sol::optional<bool>', 'optional': True})
    killable = result.get('EntitySetAsKillable', {})
    params = killable.get('parameters', [])
    if (killable.get('scope') == 'Quest' and killable.get('returnType') == 'void'
            and [(p.get('name'), p.get('type')) for p in params] ==
            [('pThing', 'CScriptThing*'), ('isKillable', 'bool')]):
        params.append({'name': 'forceDead', 'type': 'sol::optional<bool>', 'optional': True})
    information = result.get('SetThingHasInformation', {})
    params = information.get('parameters', [])
    if (information.get('scope') == 'Quest' and information.get('returnType') == 'void'
            and [(p.get('name'), p.get('type')) for p in params] ==
            [('pThing', 'CScriptThing*'), ('hasInfo', 'bool')]):
        information['parameters'] = [params[0]] + [
            {'name': name, 'type': 'sol::optional<bool>', 'optional': True}
            for name in ('wander', 'wave', 'continueAI')]
    # LuaManager binds me:PlayAnimation to PlayAnimation_NonBlocking, which
    # forwards all seven native flags using the existing control handle.
    animation = result.get('PlayAnimation', {})
    params = animation.get('parameters', [])
    if (animation.get('scope') == 'Entity' and animation.get('returnType') == 'void'
            and [(p.get('name'), p.get('type')) for p in params] == [
                ('pMe', 'CScriptThing*'), ('animName', 'const std::string&'),
                ('waitForFinish', 'sol::optional<bool>'),
                ('stayOnLastFrame', 'sol::optional<bool>'),
                ('allowLooking', 'sol::optional<bool>')]):
        animation['parameters'] = params[:2] + [
            {'name': 'b' + str(i), 'type': 'sol::optional<bool>', 'optional': True}
            for i in range(1, 8)]
        animation['blocking'] = False
    return result


def load_entity_state(entity: str, inventory_root: Path = ENTITY_INVENTORIES) -> dict[str, tuple[str, str]]:
    """Load byte-proven local entity fields; unsupported or non-field rows remain unnamed.

    The recovery inventories distinguish class-local fields from parent fields and stack locals.
    A Lua persistence kind is accepted only when an explicit native type maps through TYPE_MAP or
    when the JSON type of initValue proves it directly.
    """
    path = inventory_root / f"{entity}.json"
    if not path.is_file():
        return {}
    payload = json.loads(path.read_text(encoding="utf-8-sig"))
    state: dict[str, tuple[str, str]] = {}
    for field in payload.get("localFields", []):
        name, offset = field.get("name"), str(field.get("offset", "")).lower()
        if not isinstance(name, str) or not re.fullmatch(r"[A-Za-z_]\w*", name):
            continue
        if not re.fullmatch(r"0x[0-9a-f]+", offset):
            continue
        kind = TYPE_MAP.get(str(field.get("type", "")).replace(" ", "_"))
        if kind is None and "initValue" in field:
            value = field["initValue"]
            if isinstance(value, bool):
                kind = "Bool"
            elif isinstance(value, int):
                kind = "Int"
            elif isinstance(value, float):
                kind = "Float"
            elif isinstance(value, str):
                kind = "String"
        if kind is None:
            continue
        prior = state.get(offset)
        if prior is not None and prior != (name, kind):
            raise ValueError(f"conflicting local field evidence for {entity} {offset}: {prior} / {(name, kind)}")
        state[offset] = (name, kind)
    return state


def load_entity_parent_state(entity: str, inventory_root: Path = ENTITY_INVENTORIES) -> dict[str, tuple[str, str]]:
    """Join an entity's declared parent fields to its owning quest's reviewed scalar layout.

    The native entity wrapper's +0x14 link holds the enclosing quest. Ownership must match the
    inventory's qualified native class; mere offset coincidence is not evidence of a parent field.
    Reviewed byte-bool arrays expand to the same indexed keys used by quest helpers.
    Other aggregates and string regions remain unresolved.
    """
    inventory_path = inventory_root / f"{entity}.json"
    manifest_path = inventory_root.parent / "persistence_manifest.json"
    if not inventory_path.is_file() or not manifest_path.is_file():
        return {}
    inventory = json.loads(inventory_path.read_text(encoding="utf-8-sig"))
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    owner = f"NScript::C{manifest.get('script', '')}Script::"
    if not str(inventory.get("nativeClass", "")).startswith(owner):
        return {}
    names = set(inventory.get("parentFieldsRead", [])) | set(inventory.get("parentFieldsWritten", []))
    result = {}
    for field in manifest.get("fields", []):
        name, offset = field.get("name"), str(field.get("retailOffset", "")).lower()
        array = re.fullmatch(r'bool\[(\d+)\]', str(field.get('type', '')))
        extent = re.fullmatch(r'(0x[0-9a-f]+)\.\.(0x[0-9a-f]+)', offset)
        if array and extent:
            count, start, end = int(array[1]), int(extent[1], 16), int(extent[2], 16)
            if (count > 0 and end - start + 1 == count
                    and (name in names or f'{name}[0..{count - 1}]' in names)):
                for index in range(count):
                    key, value = hex(start + index), (f'{name}_{index}', 'Bool')
                    if key in result and result[key] != value:
                        raise ValueError(f'conflicting parent array evidence for {entity} {key}')
                    result[key] = value
            continue
        kind = {"bool": "Bool", "int": "Int", "float": "Float"}.get(field.get("type"))
        if name not in names or kind is None or not re.fullmatch(r"0x[0-9a-f]+", offset):
            continue
        prior = result.get(offset)
        if prior is not None and prior != (name, kind):
            raise ValueError(f"conflicting parent field evidence for {entity} {offset}")
        result[offset] = (name, kind)
    return result


def load_entity_constants(entity: str, inventory_root: Path = ENTITY_INVENTORIES) -> dict[str, str]:
    """Use only reviewed numeric .rdata constants with one explicit Ghidra data-symbol anchor."""
    path = inventory_root / f"{entity}.json"
    if not path.is_file():
        return {}
    payload = json.loads(path.read_text(encoding="utf-8-sig"))
    result = {}
    for constant in payload.get("constants", []):
        value = constant.get("value")
        if type(value) not in (int, float) or "retail-rdata" not in constant.get("evidence", ""):
            continue
        symbols = set(re.findall(r'\b_?DAT_[0-9a-fA-F]{8}\b', constant.get("source", "")))
        if len(symbols) != 1:
            continue
        symbol = symbols.pop()
        literal = json.dumps(value, allow_nan=False)
        if symbol in result and result[symbol] != literal:
            raise ValueError(f"conflicting constant evidence for {entity} {symbol}")
        result[symbol] = literal
    return result


def strip_declarations(body: str) -> list[str]:
    """Drop the signature and local declarations, join wrapped statements, drop the closing brace."""
    lines = [l.rstrip() for l in body.splitlines()]
    try:
        first = lines.index("{")
    except ValueError:
        return lines
    rest = lines[first + 1:]
    while rest and (RE_DECL.match(rest[0]) and "(" not in rest[0] and "=" not in rest[0]
                    and not re.match(r'\s*return\b', rest[0])):
        rest.pop(0)
    joined: list[str] = []
    buffer = ""
    for line in rest:
        stripped = line.strip()
        if not stripped:
            continue
        buffer = (buffer + " " + stripped) if buffer else line
        if stripped.endswith((";", "{", "}")) or stripped.startswith("@@") or (
                stripped.endswith(":") and not stripped.endswith("::")):
            joined.append(re.sub(r'::\s+', '::', re.sub(r'\)\s+;', ');', buffer)))
            buffer = ""
    if buffer:
        joined.append(buffer)
    if joined and joined[-1].strip() == "}":
        joined.pop()
    if joined and joined[-1].strip() == "return;":
        joined.pop()
    return joined


def parse_thing_signature(mangled: str) -> tuple[str | None, list[str], bool] | None:
    """(result kind, operand kinds, returns a class by value) from a decorated CScriptThing slot name.

    Kinds: "string" (const CCharString&), "out" (non-const reference the callee fills; Forge returns
    it instead), "thing" (const CScriptThing&), "scalar" (bool/enum/int). A by-value class return
    (`?AV...`) travels through a hidden first operand that must be dropped. None when unparsed.
    """
    m = RE_THING_SIG.match(mangled or "")
    if not m:
        return None
    ret, params = m.group("ret"), m.group("params")
    if ret == "X":
        result = None
    elif ret == "?AV1@" or ret.startswith(("ABV1", "PAVCThing")):
        result = "thing"
    elif "CCharString" in ret:
        result = "string"
    elif "C3DVector" in ret:
        result = "vector"
    elif ret == "_N":
        result = "bool"
    elif ret.startswith("?AW4") or ret in ("M", "J", "_K", "H", "K", "N"):
        result = "number"
    else:
        result = "other"
    by_value = ret.startswith("?AV")
    kinds: list[str] = []
    if params != "X":
        tokens = RE_SIG_TOKEN.findall(params)
        if "".join(tokens) != params:
            return None
        for tok in tokens:
            if tok.isdigit():
                idx = int(tok)
                if idx >= len(kinds):
                    return None
                kinds.append(kinds[idx])
            elif tok.startswith("ABVCCharString"):
                kinds.append("string")
            elif tok.startswith("AA"):
                kinds.append("out")
            elif tok.startswith(("ABV1", "ABVCScriptThing", "PAVCScriptThing")):
                kinds.append("thing")
            else:
                kinds.append("scalar")
    return result, kinds, by_value


class Lifter:
    def __init__(self, manifest: dict[str, dict], state: dict[str, tuple[str, str]], receiver: str,
                 entity: bool, package: str, rdata: RData, callee_names: dict[str, str] | None = None,
                 thing_sigs: dict[str, str] | None = None,
                 parent_state: dict[str, tuple[str, str]] | None = None,
                 native_constants: dict[str, str] | None = None, state_receiver: str | None = None,
                 live_termination: bool = False, execution_entity: bool = False,
                 state_arrays: dict[str, tuple[str, str]] | None = None,
                 master_strings: dict[str, str] | None = None, native_gotos: bool = False,
                 static_vectors: dict[str, list[str]] | None = None, readable_locals: bool = False):
        self.manifest = manifest
        self.state = state
        self.state_arrays = state_arrays or {}
        self.master_strings = master_strings or {}
        self.native_gotos = native_gotos
        self.readable_locals = readable_locals
        self.static_vectors = static_vectors or {}
        self.parent_state = parent_state or {}
        self.native_constants = native_constants or {}
        self.receiver = receiver          # "Quest" (quest mode) or "quest" (entity mode)
        self.state_receiver = state_receiver or receiver
        self.live_termination = live_termination
        self.entity = entity
        self.execution_entity = entity or execution_entity
        self.package = package
        self.rdata = rdata
        self.callee_names = callee_names or {}
        self.helper_names: set[str] = set()
        self.helper_parameters: dict[str, list[str]] = {}
        self.helper_return_kinds: dict[str, str] = {}
        self.parent_helpers: dict[str, dict] = {}
        self.thing_sigs = thing_sigs or {}   # CScriptThing method name -> decorated name (operand shape)
        self.reset()

    def reset(self) -> None:
        self.out: list[str] = []
        self.todo: list[str] = []
        self.entities: list[str] = []
        self.calls: list[str] = []
        self.temps: dict[str, str] = {}      # name -> Lua expression
        self.order: list[str] = []
        self.results = 0
        self.conditions = 0
        self.sequence_temporaries = set()
        self.source_names: set[str] = set()
        self.mutable_scalars: set[str] = set()
        self.hoisted_scalars: set[str] = set()
        self.staged_scalars: set[str] = set()
        self.literal_comparisons = 0
        self.indent = 1
        self.locals: set[str] = set()
        self.parameters: set[str] = set()
        self.used_alive = False
        self.last_timer: str | None = None
        self.threads: list[dict] = []
        self.kinds: dict[str, str] = {}      # local -> "thing" | "string" | "number" | "bool" | "vector"
        self.me_aliases: set[str] = set()    # locals that are `(CScriptThing *)(this + 8)`
        self.parent_aliases: set[str] = set()
        self.parent_alias_depth: dict[str, int] = {}
        self.parent_branch_groups: dict[int, dict] = {}
        self.last_parent_branch = None
        self.slot_alias: dict[str, str] = {}   # `ppVar7 = apStack_c;` -> the stack slot it points at
        self.slot_results: dict[str, str] = {}  # hidden return slot -> Lua result variable
        self.skip_depth = 0                  # inside a dropped block (refcount release idiom)
        self.cleanup_labels: set[str] = set()  # labels whose suffix is destructor-only then return
        self.thing_predicate_evidence: list[dict] = []
        self.operand_evidence: list[dict] = []
        self.random_evidence: list[dict] = []
        self.deed_evidence: list[dict] = []
        self.for_updates: dict[int, str] = {}
        self.lua_jumps: set[str] = set()
        self.lua_labels: set[str] = set()
        self.self_wrapper_arguments = 0
        self.presented_item_evidence = []
        self.teddy_distance_evidence = []
        self.theresa_distance_evidence = []
        self.entry_timer_evidence = []
        self.loop_timer_evidence = []
        self.barrel_distance_evidence = []
        self.barrel_colour_evidence = []
        self.barrel_update_evidence = []
        self.barrel_creation_evidence = []
        self.quest_marker_evidence = []
        self.marker_cleanup_evidence = []
        self.post_attack_cleanup_evidence = []
        self.post_attack_scalar_evidence = []
        self.post_attack_logbook_evidence = []
        self.post_attack_teleport_evidence = []
        self.post_attack_distance_evidence = []
        self.post_attack_movie_flag_evidence = []
        self.post_attack_limbo_evidence = []
        self.post_attack_resource_evidence = []
        self.book_trader_resource_evidence = []
        self.post_attack_movie_lifetime_evidence = []
        self.barrel_position_evidence = []
        self.speech_vector_init_evidence = []
        self.init_scalar_evidence = []
        self.init_bad_deed_evidence = []
        self.affair_wife_route_evidence = []
        self.affair_wife_distance_evidence = []
        self.affair_wife_hero_distance_evidence = []
        self.affair_wife_hit_evidence = []
        self.affair_wife_pause_evidence = []
        self.affair_wife_movie_evidence = []
        self.affair_wife_cleanup_evidence = []
        self.affair_wife_argument_hit_evidence = []
        self.affair_wife_mask_evidence = []
        self.affair_wife_facing_evidence = []
        self.affair_wife_ally_evidence = []
        self.affair_wife_acquisition_evidence = []
        self.affair_wife_termination_evidence = []
        self.affair_wife_timer_evidence = []
        self.affair_wife_random_evidence = []
        self.affair_wife_conversation_evidence = []
        self.affair_wife_running_line_evidence = []
        self.affair_wife_argument_conversation_evidence = []
        self.affair_wife_text_sequence_evidence = []
        self.affair_wife_reply_evidence = []
        self.affair_wife_health_evidence = []
        self.affair_wife_animation_random_evidence = []
        self.affair_wife_animation_operands_evidence = []
        self.affair_wife_position_evidence = []
        self.affair_man_health_evidence = []
        self.affair_man_hit_evidence = []
        self.affair_man_mask_evidence = []
        self.affair_man_conversation_evidence = []
        self.affair_man_near_lines_evidence = []
        self.affair_man_ranged_lines_evidence = []
        self.affair_man_affection_setup_evidence = []
        self.affair_man_animation_operands_evidence = []
        self.affair_man_hero_distance_evidence = []
        self.affair_man_question_evidence = []
        self.affair_man_question_cleanup_evidence = []
        self.affair_man_home_evidence = []
        self.affair_man_question_entry_evidence = []
        self.affair_man_talk_continuation_evidence = []
        self.affair_man_speech_evidence = []
        self.affair_man_resource_evidence = []
        self.guard_cleanup_evidence = []
        self.affair_man_distance_evidence = []
        self.affair_man_woman_distance_evidence = []
        self.cached_thing_query_evidence = []
        self.affair_facing_evidence = []
        self.affair_pause_evidence = []
        self.affair_movie_evidence = []
        self.book_trader_pause_evidence = []
        self.book_trader_random_evidence = []
        self.book_trader_timer_evidence = []
        self.book_trader_line_evidence = []
        self.book_trader_health_evidence = []
        self.book_trader_allies_evidence = []
        self.book_trader_movie_evidence = []
        self.book_trader_cleanup_evidence = []
        self.book_trader_hit_evidence = []
        self.book_trader_termination_evidence = []
        self.book_trader_acquisition_evidence = []
        self.readable_local_names = {}

    # ---- helpers -------------------------------------------------------------------------------
    def emit(self, line: str) -> None:
        self.out.append("    " * self.indent + line)

    def forget_value(self, name: str) -> None:
        """An unrepresented write must not leave an earlier operand value usable."""
        self.me_aliases.discard(name)
        self.parent_aliases.discard(name)
        self.parent_alias_depth.pop(name, None)
        self.kinds.pop(name, None)
        self.slot_alias.pop(name, None)
        self.staged_scalars.discard(name)
        if name in self.temps:
            self.temps.pop(name)
            self.order.remove(name)

    def declare(self, name: str) -> str:
        """Declare once; each assignment replaces the name's previous known value."""
        self.forget_value(name)
        if name in self.mutable_scalars:
            self.locals.add(name)
            if name not in self.parameters:
                self.hoisted_scalars.add(name)
            return name
        if name in self.locals:
            return name
        self.locals.add(name)
        if self.lua_labels or (self.indent > 1 and not name.startswith('__native_condition_')):
            self.hoisted_scalars.add(name)
            return name
        return f"local {name}"

    def slot_name(self, operand: str) -> str | None:
        """The stack slot an operand denotes (`&stack0x..`, `apStack_c`, or a local aliased to one)."""
        m = re.fullmatch(r'(?:\([^)]*\))?\s*&?(\w+|stack0x[0-9a-f]+)', operand.strip())
        if not m:
            return None
        name = self.slot_alias.get(m.group(1), m.group(1))
        return name if RE_HIDDEN_SLOT.match(name) else None

    def expr(self, text: str) -> str:
        text = text.strip()
        colour = colour_literal(text)
        if colour is not None:
            return colour
        for alias in self.parent_aliases:
            text = re.sub(r'\*\((\w+) \*\)\(' + re.escape(alias) + r' \+ (0x[0-9a-f]+)\)',
                          lambda m: f'*({m[1]} *)(*(int *)(this + 0x14) + {m[2]})', text)
        for offset, keys in self.static_vectors.items():
            def pointer(at):
                return re.escape(f'*(int *)(*(int *)(this + 0x14) + {at})')
            size = pointer(hex(int(offset, 16) + 4)) + r'\s*-\s*' + pointer(offset) + r'\s*>>\s*2'
            text = re.sub(size, str(len(keys)), text)
            text = re.sub(pointer(offset), f'__native_vectors[{offset}]', text)
        array_bools = []
        def array_load(match):
            field = self.state_arrays.get(match[1].lower())
            if not field:
                return match[0]
            name, kind = field
            value = f'{self.state_receiver}:GetState{kind}("{name}_" .. ({self.expr(match[2])}))'
            if kind == 'Bool':
                array_bools.append(value)
            return value
        text = re.sub(r'\*\(undefined1 \*\)\(this \+ (0x[0-9a-f]+) \+ (\w+)\)', array_load, text)
        mask = re.fullmatch(r"~-\(uint\)\((\w+\s*(?:!=|==|<=|>=|<|>)\s*(?:\w+|'\\0'))\)\s*&\s*(\w+)", text)
        if mask:
            condition, value = self.expr(mask[1]), self.expr(mask[2])
            return f'((not ({condition})) and {value} or 0)'
        if re.fullmatch(r'"(?:[^"\\]|\\.)*"', text):
            return text
        text = strip_template_pointer_casts(text)
        for symbol, value in self.native_constants.items():
            text = re.sub(rf'\b{re.escape(symbol)}\b', value, text)

        def load(match: re.Match) -> str:
            offset = (match.group(2) or match.group(3)).lower()
            if offset in self.state:
                name, kind = self.state[offset]
                return f'{self.state_receiver}:GetState{kind}("{name}")'
            self.todo.append(f"unnamed field {offset}")
            kind = STORE_TYPE.get(match.group(1) or "undefined1")
            if kind is not None:
                # Unknown names already use self_0xNN keys for stores. Loads must read the same
                # storage instead of an unrelated Lua global; the review marker remains.
                return f'{self.state_receiver}:GetState{kind}("self_{offset}")'
            return f"self_{offset}"

        if self.entity:
            def parent_load(match: re.Match) -> str:
                field = self.parent_state.get(match[2].lower())
                if field is None:
                    return match[0]
                name, kind = field
                return f'{self.receiver}:GetState{kind}("{name}")'

            text = RE_PARENT_LOAD.sub(parent_load, text)
            text = re.sub(r'\b(?:this|param_1)\s*\+\s*8\b', 'me', text)
            for alias in self.me_aliases:
                text = re.sub(rf'(?<![\w.]){alias}\b', "me", text)
        for slot, var in self.slot_results.items():
            text = re.sub(rf'(?<![\w.])&?{slot}\b', var, text)
        text = RE_LOAD.sub(load, text)
        text, inline_termination = fold_inline_termination(
            text, self.callee_names, self.receiver, self.live_termination)
        if inline_termination:
            self.used_alive = True
            if self.live_termination:
                self.calls.extend(['IsActiveThreadTerminating'] * inline_termination)
        text = RE_EXTRAOUT.sub(lambda m: "alive" if m.group(1) == "==" else "not alive", text)
        text = RE_CAST.sub("", text)
        for temp, value in self.temps.items():
            text = re.sub(rf'(?<![\w.]){re.escape(temp)}\b', value, text)
        text = fold_literal_slices(text)
        text = fold_unsigned_three_byte_casts(text)
        text = re.sub(r'CScriptThing::(\w+)\((\w+)(?:, ?([^()]*))?\)', self.inline_thing_call, text)
        text = text.replace("&&", " and ").replace("||", " or ").replace("!=", " ~= ")
        text = re.sub(r'(?<![=~<>!])!\s*', "not ", text)
        text = text.replace("'\\0'", "0")
        bool_atoms = [var for var, kind in self.kinds.items() if kind == "bool"]
        bool_atoms += array_bools
        bool_atoms += re.findall(r'\b\w+:GetStateBool\("\w+"\)', text)
        for var in bool_atoms:
            pattern = re.escape(var)
            text = re.sub(rf'\b{pattern}\s*==\s*(?:0|0x0)\b', f"not {var}", text)
            text = re.sub(rf'\b{pattern}\s*~=\s*(?:0|0x0)\b', lambda _m: var, text)
            text = re.sub(rf'\b{pattern}\s*==\s*(?:1|0x1)\b', lambda _m: var, text)
            text = re.sub(rf'\b{pattern}\s*~=\s*(?:1|0x1)\b', f"not {var}", text)
        text = re.sub(r'\b0x0\b', "nil", text) if re.search(r'(==|~=)\s+0x0\b', text) else text
        if "alive" in text:
            self.used_alive = True
        text = re.sub(r'\s+', " ", text).strip()
        text = fold_decisive_condition(text)
        return 'me' if self.entity and text == '(me)' else text

    def inline_thing_call(self, match: re.Match) -> str:
        """`CScriptThing::Name(recv, args)` inside an expression (already cast-stripped)."""
        name, recv, argtext = match.group(1), match.group(2), match.group(3) or ""
        sig = parse_thing_signature(self.thing_sigs.get(name, ""))
        args = self.arguments(argtext)
        if sig is not None:
            _, kinds, by_value = sig
            if by_value and args and RE_HIDDEN_SLOT.match(args[0]) and len(args) > len(kinds):
                args.pop(0)
            if not kinds:
                args = []
            args = [a for a in args if not a.startswith("&")]
        if name in HERO_FORMS and args and args[0] == HERO_NAME:
            name, args = HERO_FORMS[name], args[1:]
        self.calls.append(name)
        return f"{recv}:{name}({', '.join(args)})"

    def kind_of(self, arg: str) -> str | None:
        if arg == "me":
            return "thing"
        if arg.startswith('"'):
            return "string"
        return self.kinds.get(arg)

    def arguments(self, text: str) -> list[str]:
        try:
            return split_arguments(text)
        except ValueError as error:
            self.todo.append(f"unparsed call operands ({error}): {text}")
            return [text.strip()]

    def literal(self, value: str) -> str | None:
        """A stack-slot value: string pointer into .rdata, a number, or None when unknown."""
        value = value.strip()
        casted = re.fullmatch(r'\([^()]+\)\s*(.+)', value)
        if casted:
            # Local staging often carries a template-heavy pointer cast that RE_CAST intentionally
            # does not strip in arbitrary expressions. Accept it here only when the remainder is
            # independently a literal below.
            value = casted.group(1).strip()
        if re.fullmatch(r'"(?:[^"\\]|\\.)*"', value):
            return value
        m = RE_ADDR_LITERAL.match(value)
        if m:
            text = self.rdata.string_at(int(value, 16))
            return json.dumps(text) if text is not None else None
        if re.fullmatch(r'-?\d+(\.\d+)?', value) or re.fullmatch(r'0x[0-9a-f]+', value):
            return value
        if value in ("true", "false"):
            return value
        return None

    def push_temp(self, name: str, value: str) -> None:
        self.parent_aliases.discard(name)
        self.parent_alias_depth.pop(name, None)
        if value in self.me_aliases:
            value = 'me'
        if name in self.order:
            self.order.remove(name)
        self.temps[name] = value
        self.order.append(name)

    def place_args(self, params: list[dict], parsed: list[str], *, explicit_things: tuple[str, ...] = ()) -> list[str]:
        """Fill the manifest parameter slots by type.

        String parameters can only come from string literals / temporaries / interface results;
        a decompiler local in that position is a guessed operand. Non-string slots take the parsed
        non-string operands in order; any shortfall is filled from the temporaries, most recent
        first (push order is reversed argument order). The timer handle from the last RegisterTimer
        fills a missing `timerId`.
        """
        if not params:
            return []

        # The numeric/string overload has one explicit positional operand.
        # Do not classify its integer literal as a missing string and then
        # substitute a stale string temporary from an earlier native call.
        if (len(params) == 1 and params[0].get('type') == 'int|string' and len(parsed) == 1
                and re.fullmatch(r'-?(?:0x[0-9a-fA-F]+|[0-9]+)', parsed[0])):
            value = parsed[0]
            if re.fullmatch(r'0x[0-9a-fA-F]{8}', value) and int(value, 16) >= 0x80000000:
                value = str(int(value, 16) - 0x100000000)
            return [value]

        def is_stringy(a: str) -> bool:
            # a local of unknown kind may hold an interface string result (old behaviour); a local
            # known to hold a thing/number/bool/vector, or a numeric literal, is not a string operand
            if a in self.kinds or a == "me":
                return self.kind_of(a) == "string"
            if RE_NUMERIC_EXPR.fullmatch(a):
                return False
            return a.startswith('"') or a in self.locals or a in self.temps.values()

        def is_thing(a: str) -> bool:
            return a in explicit_things or self.kind_of(a) == "thing"

        string_slots = [i for i, p in enumerate(params) if RE_STRING_PARAM.search(p.get("type", ""))]
        thing_slots = [i for i, p in enumerate(params) if i not in string_slots and "CScriptThing" in p.get("type", "")]
        vector_slots = [i for i, p in enumerate(params) if p.get('nativeKind') == 'vector']
        other_slots = [i for i in range(len(params)) if i not in string_slots and i not in thing_slots and i not in vector_slots]
        vectors = [a for a in parsed if self.kind_of(a) == 'vector']
        things = [a for a in parsed if is_thing(a)]
        stringy = [a for a in parsed if not is_thing(a) and is_stringy(a)]
        others = [a for a in parsed if not is_thing(a) and not is_stringy(a) and a not in vectors]
        result: list[str | None] = [None] * len(params)
        # The timer handle is positional evidence: it goes into `timerId` before anything else.
        timer_slot = next((i for i, p in enumerate(params) if p["name"] == "timerId"), None)
        if timer_slot is not None and self.last_timer:
            result[timer_slot] = self.last_timer
            others = [a for a in others if a != self.last_timer]
        for slot, a in zip(thing_slots, things):
            result[slot] = a
        for slot, a in zip(vector_slots, vectors):
            result[slot] = a
        others += things[len(thing_slots):]       # a thing with no thing slot is an ordinary operand
        for slot, a in zip(string_slots, stringy):
            result[slot] = a
        for slot, a in zip([i for i in other_slots if result[i] is None], others):
            result[slot] = a
        # Shortfall from the temporaries: a thing slot takes a thing result or (as before) an
        # untyped non-literal operand; a string slot a string; a scalar slot an untyped operand.
        # Bool/number results are never guessed into a slot, and sol::optional parameters are never
        # filled from stale temporaries: absent means absent.
        parsed_values = tuple(a for a in result if a)
        for want, slots in (("thing", thing_slots), ("string", string_slots), ("vector", vector_slots), ("other", other_slots)):
            for slot in [i for i in slots if result[i] is None and "optional" not in params[i].get("type", "")]:
                # take_args consumes a distinct native slot each time. Equal values in two
                # separately constructed slots remain two operands (e.g. two empty regions).
                fill = self.take_args(1, parsed_values, want=want)
                result[slot] = fill[0] if fill else None
        # trailing sol::optional parameters may simply be absent
        while result and result[-1] is None and "optional" in params[len(result) - 1].get("type", ""):
            result.pop()
        if any(a is None for a in result):
            self.todo.append(f"{len([a for a in result if a])} args for {len(params)} params")
        for i, value in enumerate(result):
            if (params[i].get('type', '').strip() in ('int', 'long', 'signed int')
                    and value and re.fullmatch(r'0x[0-9a-fA-F]{8}', value)
                    and int(value, 16) >= 0x80000000):
                result[i] = str(int(value, 16) - 0x100000000)
            if re.fullmatch(r'bool|sol::optional<\s*bool\s*>', params[i].get("type", "").strip()):
                if value in ("0", "0x0", "'\\0'"):
                    result[i] = "false"
                elif value in ("1", "0x1", "'\\x01'"):
                    result[i] = "true"
                elif self.kinds.get(value) == "number":
                    result[i] = f"({value} ~= 0)"
        return [a if a is not None else "nil --[[missing]]" for a in result]

    def take_args(self, needed: int, exclude: tuple[str, ...] = (), want_string: bool | None = None,
                  want: str | None = None) -> list[str]:
        """Most recently constructed operands first (push order is reversed argument order).

        `want` narrows to "string" / "thing" / "other" operands (`want_string` is the older
        boolean spelling of string vs. other). An untyped temporary (a literal or an unknown local)
        keeps the old rules: quoted/local = string, anything else = other (a "thing" slot also
        accepts an untyped non-literal); a local known to hold a bool/number/vector result is never
        guessed into a slot."""
        if want is None and want_string is not None:
            want = "string" if want_string else "other"
        taken: list[str] = []
        skipped: list[str] = []
        while needed > 0 and self.order:
            name = self.order.pop()
            if name not in self.temps:
                continue
            value = self.temps.pop(name)
            kind = self.kind_of(value)
            if kind is None:
                kind = "string" if (value.startswith('"') or value in self.locals) else "other"
                if kind == "other" and want == "thing" and not RE_NUMERIC_EXPR.fullmatch(value):
                    kind = "thing"
            elif kind in ("number", "bool") and name in self.staged_scalars:
                kind = "other"
            elif kind not in ("string", "thing", "vector"):
                kind = "result"
            if value in exclude or (want is not None and kind != want):
                skipped.append(name)
                self.temps[name] = value
                continue
            taken.append(value)
            needed -= 1
        self.order.extend(reversed(skipped))
        return taken

    def result_kind(self, return_type: str) -> str | None:
        rt = return_type or "void"
        if rt == "void":
            return None
        if "CScriptThing" in rt:
            return "thing"
        if "string" in rt:
            return "string"
        if rt == "bool":
            return "bool"
        if rt in ("int", "float", "long", "double", "unsigned int"):
            return "number"
        return None

    def interface_call(self, target: str | None, name: str, argtext: str) -> bool:
        spec = self.manifest.get(name)
        if (not self.execution_entity
                and (name == 'StartScriptingEntity' or spec and spec.get('scope') == 'Entity')):
            # A quest owns no implicit entity receiver. Its native resource must
            # be connected to a proven actor before an entity method can be emitted.
            # In particular, a recent GetHero does not establish resource ownership
            # or justify substituting AcquireControl's retry behavior.
            reason = f'{name}: unresolved entity receiver/resource in quest context'
            self.todo.append(reason)
            self.emit(f'-- TODO(native): {reason}; arguments: {argtext}')
            if target:
                self.forget_value(target)
                self.emit(f'{self.declare(target)} = nil --[[unresolved native result]]')
            return True
        if name == "PostAddScriptedEntities":
            self.emit(f"{self.receiver}:FinalizeEntityBindings()")
            self.calls.append("FinalizeEntityBindings")
            return True
        if name in TERMINATING_NAMES:
            # CGameScriptInterface::IsActiveThreadTerminating is the same fact Forge reports
            # through NewScriptFrame's boolean, tracked here as `alive`.
            self.used_alive = True
            if self.live_termination:
                self.emit(f"alive = not {self.receiver}:IsActiveThreadTerminating()")
                self.calls.append('IsActiveThreadTerminating')
            if target:
                self.emit(f"{self.declare(target)} = not alive")
                self.kinds[target] = "bool"
            return True
        if name == "StartScriptingEntity":
            # Retail: while (!StartScriptingEntity(me, resource, priority)) NewScriptFrame();
            # Forge: me:AcquireControl(priority) blocks until the scheduler grants control.
            numbers = re.findall(r'\b(\d+)\s*\)?\s*$', argtext)
            priority = numbers[0] if numbers else "4"
            expr = f"me:AcquireControl({priority})"
            self.emit(f"{self.declare(target)} = {expr}" if target else expr)
            if target:
                self.kinds[target] = 'bool'
            self.calls.append("AcquireControl")
            self.todo.append("collapse the StartScriptingEntity retry loop around AcquireControl")
            return True
        spec = self.manifest.get(name)
        operands = self.arguments(argtext)
        return_slot = None
        if spec and operands and self.result_kind(spec.get("returnType", "void")) in ("thing", "string"):
            # A by-value CScriptThing/CCharString result travels through a hidden first operand;
            # remember which stack slot so later uses of that slot resolve to the result.
            slot = self.slot_name(operands[0])
            if slot and slot not in self.temps:
                return_slot = slot
                operands = operands[1:]
        raw_args = [a for a in operands if not a.startswith("&") and a not in ("this", "param_1")]
        params = None
        if spec:
            params = [p for p in spec["parameters"]
                      if p["name"] not in ("pMe", "pContext") and p.get("type") != "sol::this_state"]
            if len(raw_args) > len(params):
                # Ghidra guessed a prototype wider than the real one: the extras are usually the
                # string temporaries of the NEXT call. Shed those first, then trailing operands.
                raw_args = [a for a in raw_args if a not in self.temps] or raw_args
                raw_args = raw_args[:len(params)]
        args: list[str] = []
        explicit_things: list[str] = []
        for a in raw_args:
            if re.fullmatch(r'\(CScriptThing\s*\*\)\s*(?:0|0x0)', a.strip()):
                # A reviewed typed null is an explicit actor operand, not a
                # missing slot to fill from an unrelated staged actor.
                args.append('nil')
                explicit_things.append('nil')
            elif a in self.temps:
                value = self.temps.pop(a)
                self.order.remove(a)
                args.append("me" if value in self.me_aliases else value)
            else:
                args.append(self.expr(a))
        if spec:
            args = self.place_args(params, args, explicit_things=tuple(explicit_things))
            required = len([p for p in params if "optional" not in p.get("type", "")])
            if len(args) < required:
                self.todo.append(f"{name}: {len(args)} args for {len(params)} params")
            receiver = "me" if spec["scope"] == "Entity" else self.receiver
            if name == "NewScriptFrame":
                call = f"{self.receiver}:NewScriptFrame(me)" if self.execution_entity else f"{self.receiver}:NewScriptFrame()"
                self.emit(f"alive = {call}")
                self.used_alive = True
                self.calls.append(name)
                return True
            call = f"{receiver}:{name}({', '.join(args)})"
            if target:
                self.emit(f"{self.declare(target)} = {call}")
                kind = self.result_kind(spec.get("returnType", "void"))
                if kind:
                    self.kinds[target] = kind
                if return_slot:
                    self.slot_results[return_slot] = target
                if name == "RegisterTimer":
                    self.last_timer = target
                elif spec.get("returnType", "void") != "void":
                    # A named result may still be an operand Ghidra dropped from the next call.
                    self.push_temp(target, target)
            elif spec.get("returnType", "void") != "void":
                self.results += 1
                var = f"r{self.results}"
                self.emit(f"{self.declare(var)} = {call}")
                kind = self.result_kind(spec.get("returnType", "void"))
                if kind:
                    self.kinds[var] = kind
                if return_slot:
                    self.slot_results[return_slot] = var
                self.push_temp(var, var)
            else:
                self.emit(call)
        else:
            self.todo.append(f"{name}: not in FSE manifest")
            self.emit(f"-- TODO(native): {name} is not a ForgeFSE binding")
            self.emit(f"{self.receiver}:{name}({', '.join(args)})")
        self.calls.append(name)
        return True

    def thing_call(self, target: str | None, name: str, thing_text: str, rest: str) -> None:
        """`CScriptThing::Name(receiver, operands)` (annotated thing-vtable dispatch or a direct call).

        Operands are placed by the slot's decorated signature: the hidden return slot of a by-value
        class result is dropped, `XZ` slots take nothing (Ghidra's junk operands go), a
        `const CCharString&` operand comes from the parsed operands or the string temporaries in push
        order, and an out-parameter (`AAV...`) is dropped because Forge returns it instead. The
        hero-specialised binding replaces the raw slot when the recovered name is SCRIPT_NAME_HERO.
        """
        # Direct calls sometimes retain Ghidra's flattened decorated spelling instead of the
        # vtable annotator's clean method name (`_IsAlive_CScriptThing__UBE_NXZ`). The embedded
        # class/method pair is authoritative, and the same proven vtable signature supplies args.
        flattened = re.fullmatch(r'_?(\w+)_CScriptThing__.+', name)
        if flattened:
            name = flattened.group(1)
        thing = self.expr(thing_text)
        operands = self.arguments(rest)
        sig = parse_thing_signature(self.thing_sigs.get(name, ""))
        args: list[str | None]
        result = None
        vector_slot = None
        if sig is None:
            args = [self.expr(a) for a in operands]
        else:
            result, kinds, by_value = sig
            if result == 'vector' and by_value and operands:
                vector_slot = self.slot_name(operands[0])
                if vector_slot and not target:
                    self.results += 1
                    target = f'r{self.results}'
            if by_value and operands and RE_HIDDEN_SLOT.match(operands[0]) and len(operands) > len(kinds):
                operands.pop(0)
            if not kinds:
                operands = []

            def temp_name(a: str) -> str | None:
                m = RE_ADDR_OF.match(a) or re.fullmatch(r'(?:\([^)]*\))?\s*(\w+)', a)
                return m.group(1) if m and m.group(1) in self.temps else None

            strings = [a for a in operands if a.startswith('"') or temp_name(a)]
            others = [a for a in operands if a not in strings]
            args = []
            for kind in kinds:
                if kind == "string":
                    if strings:
                        a = strings.pop(0)
                        t = temp_name(a)
                        if t:
                            self.order.remove(t)
                            args.append(self.temps.pop(t))
                        else:
                            args.append(a)
                    else:
                        fill = self.take_args(1, want="string")
                        args.append(fill[0] if fill else None)
                elif kind == "out":
                    if others and others[0].startswith("&"):
                        others.pop(0)
                else:
                    args.append(self.expr(others.pop(0)) if others else None)
            if name in HERO_FORMS and "string" in kinds and args[kinds.index("string")] == HERO_NAME:
                del args[kinds.index("string")]
                name = HERO_FORMS[name]
            if any(a is None for a in args):
                self.todo.append(f"{name}: {len([a for a in args if a])} operands for {len(kinds)} params")
        call = f"{thing}:{name}({', '.join(a if a is not None else 'nil --[[missing]]' for a in args)})"
        # Retail CScriptThing::IsAlive (0x004AB130) returns false for an empty
        # implementation pointer. Forge lookups represent that wrapper as nil,
        # on which Lua cannot dispatch a method. Guard a typed local receiver;
        # bare locals are side-effect-free and the entity-thread `me` is bound.
        if (target and name == 'IsAlive' and result == 'bool' and not args
                and thing != 'me' and re.fullmatch(r'[A-Za-z_]\w*', thing)
                and self.kind_of(thing) == 'thing'):
            call = f"({thing} ~= nil and {call})"
        if name not in self.manifest:
            self.todo.append(f"{name}: CScriptThing method not in FSE manifest")
            self.emit(f"-- TODO(native): {name} is not a ForgeFSE binding")
        if target:
            self.emit(f"{self.declare(target)} = {call}")
            if result:
                self.kinds[target] = result
                self.push_temp(target, target)
                if vector_slot:
                    self.slot_results[vector_slot] = target
        else:
            self.emit(call)
        self.calls.append(name)

    # ---- main driver ---------------------------------------------------------------------------
    def lift(self, role: str, decompile: str, *, native_function: dict | None = None,
             parameters: dict[str, str] | None = None) -> list[str]:
        self.reset()
        self.parameters = set(parameters or {})
        self.locals.update(self.parameters)
        self.mutable_scalars.update(self.parameters)
        self.kinds.update(parameters or {})
        if self.entity:
            decompile, self.self_wrapper_arguments = fold_self_wrapper_arguments(decompile)
        if native_function is not None:
            decompile, self.affair_wife_route_evidence = recover_affair_wife_route(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_distance_evidence = recover_affair_wife_distance(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_hero_distance_evidence = recover_affair_wife_hero_distances(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_hit_evidence = recover_affair_wife_hits(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_pause_evidence = recover_affair_wife_pause(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_movie_evidence = recover_affair_wife_movies(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_cleanup_evidence = recover_affair_wife_cleanup(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_argument_hit_evidence = recover_affair_wife_argument_hits(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_mask_evidence = recover_affair_wife_mask(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_facing_evidence = recover_affair_wife_facing(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_ally_evidence = recover_affair_wife_allies(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_acquisition_evidence = recover_affair_wife_acquisition(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_termination_evidence = recover_affair_wife_termination(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_timer_evidence = recover_affair_wife_timer(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_random_evidence = fold_affair_wife_random(
                decompile, native_function, self.rdata.bytes_at)
            decompile, self.affair_wife_conversation_evidence = recover_affair_wife_conversation(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_running_line_evidence = recover_affair_wife_running_line(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_argument_conversation_evidence = recover_affair_wife_argument_conversation(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_text_sequence_evidence = recover_affair_wife_text_sequence(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_reply_evidence = recover_affair_wife_reply(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_health_evidence = recover_affair_wife_health(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_wife_animation_random_evidence = recover_affair_wife_animation_random(
                native_function, decompile, self.rdata)
            decompile, self.affair_wife_animation_operands_evidence = recover_affair_wife_animation_operands(
                native_function, decompile, self.rdata)
            self.affair_wife_position_evidence = map_affair_wife_position(native_function, self.rdata)
            decompile, self.speech_vector_init_evidence = recover_init_vector_construction(
                native_function, decompile, self.rdata, self.static_vectors)
            decompile, self.init_scalar_evidence = recover_init_scalars(
                native_function, decompile, self.rdata, self.state, self.manifest)
            decompile, self.init_bad_deed_evidence = recover_init_bad_deeds(
                native_function, decompile, self.rdata, self.state_arrays)
            self.barrel_position_evidence = map_barrel_position(native_function, self.rdata)
            if self.entity:
                decompile = recover_first_barrel_actors(decompile, self.barrel_position_evidence)
                decompile, self.guard_cleanup_evidence = recover_guard_cleanup(native_function, decompile, self.rdata)
                if self.guard_cleanup_evidence and self.guard_cleanup_evidence[0]['status'] == 'recovered':
                    self.todo.append(self.guard_cleanup_evidence[0]['ownershipLimitation'])
            self.post_attack_resource_evidence = map_post_attack_resources(native_function, self.rdata)
            self.book_trader_resource_evidence = map_book_trader_resources(native_function, self.rdata)
            if not self.entity:
                decompile, self.post_attack_cleanup_evidence = recover_post_attack_cleanup(native_function, decompile, self.rdata)
                if self.post_attack_cleanup_evidence and self.post_attack_cleanup_evidence[0]['status'] == 'recovered':
                    self.todo.append(self.post_attack_cleanup_evidence[0]['ownershipLimitation'])
                decompile, self.post_attack_scalar_evidence = recover_post_attack_scalars(native_function, decompile, self.rdata, self.manifest)
                decompile, self.post_attack_logbook_evidence = recover_post_attack_logbook(native_function, decompile, self.rdata, self.manifest)
                decompile, self.post_attack_teleport_evidence = recover_post_attack_teleport(native_function, decompile, self.rdata, self.manifest)
                decompile, self.post_attack_distance_evidence = recover_post_attack_distance(native_function, decompile, self.rdata)
                decompile, self.post_attack_movie_flag_evidence = recover_post_attack_movie_flags(native_function, decompile, self.rdata, self.manifest)
                decompile, self.post_attack_limbo_evidence = recover_post_attack_limbo(native_function, decompile, self.rdata, self.manifest)
                decompile, self.post_attack_movie_lifetime_evidence = recover_post_attack_movie_lifetime(native_function, decompile, self.rdata, self.manifest)
                decompile, self.quest_marker_evidence = recover_quest_markers(native_function, decompile, self.rdata)
                decompile, self.marker_cleanup_evidence = recover_marker_cleanup(native_function, decompile, self.rdata)
                if self.marker_cleanup_evidence and self.marker_cleanup_evidence[0]['status'] == 'recovered':
                    self.todo.append(OWNERSHIP_LIMITATION)
                decompile, self.entry_timer_evidence = recover_entry_timer(
                    native_function, decompile, self.rdata, self.manifest, self.state, strip_declarations)
                decompile, self.loop_timer_evidence = recover_loop_timer(
                    native_function, decompile, self.rdata, self.state)
                decompile, self.barrel_distance_evidence = recover_barrel_distance(
                    native_function, decompile, self.rdata)
                decompile, self.barrel_colour_evidence = recover_barrel_colours(
                    native_function, decompile, self.rdata, self.manifest)
                decompile, self.barrel_update_evidence = recover_barrel_update(
                    native_function, decompile, self.rdata, self.manifest)
                decompile, self.barrel_creation_evidence = recover_barrel_creation(
                    native_function, decompile, self.rdata, self.manifest)
            decompile, self.presented_item_evidence = recover_presented_item_comparison(
                native_function, decompile, self.rdata)
            decompile, self.teddy_distance_evidence = recover_teddy_distance(
                native_function, decompile, self.rdata)
            decompile, self.theresa_distance_evidence = recover_theresa_distance(
                native_function, decompile, self.rdata)
            decompile, self.affair_man_distance_evidence = recover_affair_man_distance(
                native_function, decompile, self.rdata)
            decompile, self.affair_man_woman_distance_evidence = recover_affair_man_woman_distance(
                native_function, decompile, self.rdata)
            decompile, self.deed_evidence = recover_deed_operands(
                native_function, decompile, self.rdata.bytes_at)
            decompile, self.random_evidence = fold_retail_random(
                decompile, native_function, self.rdata.bytes_at)
            decompile, self.cached_thing_query_evidence = recover_cached_thing_queries(
                native_function, decompile, self.rdata)
            decompile, self.affair_facing_evidence = recover_affair_facing(
                native_function, decompile, self.rdata)
            decompile, self.affair_pause_evidence = recover_affair_pause(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_movie_evidence = recover_affair_movies(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_health_evidence = recover_affair_man_health(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_hit_evidence = recover_affair_man_hits(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_mask_evidence = recover_affair_man_mask(
                native_function, decompile, self.rdata)
            decompile, self.affair_man_conversation_evidence = recover_affair_man_conversation(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_near_lines_evidence = recover_affair_man_near_lines(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_ranged_lines_evidence = recover_affair_man_ranged_lines(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_affection_setup_evidence = recover_affair_man_affection_setup(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_animation_operands_evidence = recover_affair_man_animation_operands(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_hero_distance_evidence = recover_affair_man_hero_distances(
                native_function, decompile, self.rdata)
            decompile, self.affair_man_question_evidence = recover_affair_man_question(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_question_cleanup_evidence = recover_affair_man_question_cleanup(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_home_evidence = recover_affair_man_home(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_question_entry_evidence = recover_affair_man_question_entry(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.affair_man_talk_continuation_evidence = recover_affair_man_talk_continuation(
                native_function, decompile, self.rdata, self.manifest)
            self.affair_man_speech_evidence = map_affair_man_speech(native_function, self.rdata, self.manifest)
            self.affair_man_resource_evidence = map_affair_man_resources(native_function, self.rdata)
            for evidence in self.affair_man_speech_evidence:
                self.todo.extend(evidence.get('issues', []))
            decompile, self.book_trader_pause_evidence = recover_book_trader_pause(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_random_evidence = fold_book_trader_random(
                decompile, native_function, self.rdata.bytes_at)
            decompile, self.book_trader_timer_evidence = recover_book_trader_timer(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_line_evidence = recover_book_trader_line(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_health_evidence = recover_book_trader_health(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_allies_evidence = recover_book_trader_allies(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_movie_evidence = recover_book_trader_movies(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_cleanup_evidence = recover_book_trader_cleanup(
                native_function, decompile, self.rdata)
            decompile, self.book_trader_hit_evidence = recover_book_trader_hits(
                native_function, decompile, self.rdata, self.manifest)
            decompile, self.book_trader_termination_evidence = recover_book_trader_termination(
                native_function, decompile, self.rdata)
            decompile, self.book_trader_acquisition_evidence = recover_book_trader_acquisition(
                native_function, decompile, self.book_trader_resource_evidence)
            decompile, self.operand_evidence = recover_reviewed_call_operands(
                native_function, decompile, self.rdata.bytes_at)
            decompile, self.thing_predicate_evidence = recover_reviewed_thing_predicates(
                native_function, decompile, self.rdata.bytes_at)
        decompile, self.literal_comparisons = fold_literal_comparisons(decompile, strip_declarations)
        # Exported worker functions can expose the implicit thiscall receiver as a typed ECX
        # input. Only the CScriptBase-typed, never-reassigned input is an alias of the quest.
        if (not self.entity and re.search(r'\bCScriptBase\s*\*\s*in_ECX\s*;', decompile)
                and not re.search(r'\bin_ECX\s*=(?!=)', decompile)):
            decompile = re.sub(r'\bin_ECX\b', "this", decompile)
        # Some exact-target termination calls are decompiled as void with their AL result copied
        # by the following statement. Recover only that immediate use, never unrelated AL values.
        terminating = {"CScriptBase::IsActiveThreadTerminating"}
        terminating.update(name for name, resolved in self.callee_names.items()
                           if resolved in TERMINATING_NAMES)
        for name in terminating:
            pattern = re.compile(re.escape(name) + r'\s*\([^;]*\)\s*;\s*'
                                 r'(\w+)\s*=\s*extraout_AL(?:_\d+)?;')
            decompile = pattern.sub(lambda m: f'{m[1]} = GSI->IsActiveThreadTerminating();', decompile)
        # Some exports lack call metadata for the known 0x00F35B30 bsim mislabel. Its result is
        # still locally proven when the very next statement tests the corresponding extraout_AL;
        # that test lowers to NewScriptFrame's `alive`, so the native call has no second Lua effect.
        if not self.live_termination:
            decompile = RE_PAIRED_TERMINATION_ALIAS.sub("", decompile)
        text = RE_BINDING.sub(
            lambda m: (self.entities.append(m.group("name")) or f'    @@BIND {m.group("name")}\n'),
            decompile)
        thread_objects: set[str] = set()

        def thread(match: re.Match) -> str:
            thread_objects.add(match.group(1))
            return f'    @@THREAD {match.group(2)} {match.group(3)}\n'

        text = RE_THREAD.sub(thread, text)
        # CreateThread is the Lua-side operation represented by the native spawned object. Remove
        # the ownership-container insertion only when its exact object variable was captured from
        # that same CSpawnedFunc allocation above; unrelated GUI transfers remain visible TODOs.
        text = RE_GUI_TRANSFER_ADD.sub(
            lambda m: "" if m.group(1) in thread_objects else m.group(0), text)
        text = RE_CONS_VAL.sub("", text)
        # MSVC funnels early exits through destructor epilogues.  An if-goto can be represented as
        # an ordinary Lua return without a review marker only when the target is the final label and
        # its suffix contains no script-interface call or state write.  This is intentionally
        # conservative: branch/join labels and any ambiguous suffix remain TODOs.
        labels = list(re.finditer(r'(?m)^\s*(LAB_[0-9a-f]+):', text))
        if labels:
            final = labels[-1]
            suffix = text[final.end():]
            has_effect = ("GSI->" in suffix or "@@" in suffix
                          or re.search(r'\(\*\*\(code \*\*\).*?\+ 0x[0-9a-f]+\)', suffix, re.S)
                          or re.search(r'\*\([^)]*\)\((?:this|param_1|in_ECX) \+ 0x[0-9a-f]+\)\s*=', suffix))
            if not has_effect and re.search(r'\breturn\s*;', suffix):
                self.cleanup_labels.add(final.group(1))
        self.source_names = set(re.findall(r'\b\w+\b', text))
        statements = strip_declarations(text)
        statements, sequence_assignments = expand_sequence_conditions(
            statements, self.source_names, preserve=RE_IF_RECHECK_TERMINATING.match)
        self.sequence_temporaries = sequence_assignments - self.source_names
        self.mutable_scalars.update(sequence_assignments)
        if self.native_gotos:
            self.lua_jumps, self.lua_labels = supported_jumps(statements)
        definitions = Counter(re.findall(r'\b([A-Za-z]{1,3}Var\d+(?:_\d+)?|native_arg_\w+)\s*=(?!=)', text))
        if self.lua_labels:
            self.mutable_scalars.update(definitions)
        for line in statements:
            assignment = RE_LOCAL_ASSIGN.match(line)
            if assignment and definitions[assignment[1]] > 1:
                value = self.literal(RE_CAST.sub("", assignment[2]).strip())
                if value is not None and not value.startswith('"'):
                    self.mutable_scalars.add(assignment[1])
        for index, line in enumerate(statements):
            overwritten = re.fullmatch(r'\s*(local_\w+) = (?:this|param_1);\s*', line)
            following = RE_CSTR.match(statements[index + 1]) if index + 1 < len(statements) else None
            if overwritten and following and overwritten[1] == following[1]:
                # Reused decompiler stack slot: this value is overwritten by the immediately
                # following literal string construction, with no intervening read or effect.
                continue
            self.statement(role, line)
        self.collapse_acquire_loops()
        self.legalize_nonterminal_returns()
        if self.used_alive:
            self.out.insert(0, "    local alive = true")
        if self.hoisted_scalars:
            self.out.insert(0, "    local " + ", ".join(sorted(self.hoisted_scalars)))
        if self.readable_locals:
            reserved = set(re.findall(r'\b\w+\b', '\n'.join(self.out))) | {self.receiver, 'me'}
            self.readable_local_names = readable_names(statements, self.manifest, reserved,
                                                       native_function.get('calls', []) if native_function else [])
            self.out = rename_lua_locals(self.out, self.readable_local_names)
        return self.out

    def legalize_nonterminal_returns(self) -> None:
        """Lua requires return to end its block; native labeled code may follow a return.

        A small do/end block preserves the unconditional return without dropping later native
        code or pretending its unconverted jump targets have been resolved.
        """
        for index, line in enumerate(self.out):
            if not re.match(r'^return(?:\s|$)', line.strip()):
                continue
            following = next((item.strip() for item in self.out[index + 1:]
                              if item.strip() and not item.lstrip().startswith("--")), None)
            if following is not None and not re.match(r'^(end|else|elseif|until)\b', following):
                self.out[index] = line[:len(line) - len(line.lstrip())] + 'do ' + line.strip() + ' end'

    def collapse_acquire_loops(self) -> None:
        """Collapse only a frame-only retry whose result is dead afterward.

        Other effects and explicit termination checks remain in their original loop.
        """
        text = "\n".join(self.out)
        pattern = re.compile(
            r'^(?P<i>\s*)local (?P<v>\w+) = me:AcquireControl\((?P<p>\d+)\)\n'
            r'(?P=i)while (?:not (?P=v)|(?P=v) == 0) do\n'
            r'(?P=i)    alive = \w+:NewScriptFrame\([^\n]*\)\n'
            r'(?P=i)    (?P=v) = me:AcquireControl\((?P=p)\)\n(?P=i)end\n?',
            re.M)
        def collapse(match):
            if re.search(r'\b' + re.escape(match['v']) + r'\b', text[match.end():]):
                return match[0]
            return f"{match['i']}if not me:AcquireControl({match['p']}) then return end\n"
        new = pattern.sub(collapse, text)
        n = new != text
        if n:
            self.out = new.split("\n")
            self.todo = [t for t in self.todo if not t.startswith("collapse the StartScriptingEntity")]

    def statement(self, role: str, line: str) -> None:
        stripped = line.strip()
        if not stripped:
            return
        structural = bool(re.match(r'^(?:if|else|while|for|do|return|goto)\b|^}', stripped))
        assigned = RE_LOCAL_ASSIGN.match(line) or RE_SLOT_ASSIGN.match(line)
        if assigned and (self.skip_depth or any(r.match(line) for r in RE_NOISE) or any(n in stripped for n in NOISE)):
            self.forget_value(assigned[1])
        if self.skip_depth:
            self.skip_depth += stripped.count("{") - stripped.count("}")
            return
        if RE_REFCOUNT_IF.match(line):
            self.skip_depth = 1           # CBaseIntelligentPointer release block: not script logic
            return
        if any(r.match(line) for r in RE_NOISE):
            return
        m = RE_LABEL.match(line)
        if m:
            if m[1] in self.lua_labels:
                self.emit(f'::{m[1]}::')
            else:
                self.emit(f"-- {m.group(1)}: (native jump target)")
                self.todo.append(f"label {m.group(1)}")
            if not m.group(2).strip():
                return
            line, stripped = m.group(2), m.group(2).strip()
        # The reviewed 0xCBE9EE helper consumes one CCharString in ECX. Ghidra's
        # similarity label claims ten arguments, some containing ordinary noise names.
        # Resolve its explicit string slot before generic noise removal can discard the call.
        direct = RE_NAMED_CALL.match(line)
        if direct and self.callee_names.get(direct[2]) == 'AddLogbookTutorialEntry':
            operands = self.arguments(direct[3])
            slot = self.slot_name(operands[0]) if operands else None
            value = self.temps.get(slot) if slot else None
            if value is None or not value.startswith('"'):
                self.todo.append('AddLogbookTutorialEntry: unresolved ECX string')
                self.emit(f'-- TODO(native): {stripped}')
            else:
                self.emit(f'{self.receiver}:AddLogbookTutorialEntry({value})')
                self.calls.append('AddLogbookTutorialEntry')
            return
        if any(n in stripped for n in NOISE) and structural:
            # A guessed register name or destructor operand does not prove that
            # its enclosing branch is bookkeeping. Keep both block boundaries.
            self.todo.append('unresolved native control operand: ' + stripped[:120])
        if any(n in stripped for n in NOISE) and not structural and not stripped.startswith("@@"):
            return
        if stripped.startswith("@@BIND "):
            name = stripped.split(" ", 1)[1]
            self.emit(f'{self.receiver}:AddEntityBinding("{name}", "{self.package}/Entities/{name}")')
            self.calls.append("AddEntityBinding")
            return
        if stripped.startswith("@@THREAD "):
            _, name, body = stripped.split(" ", 2)
            where = ("0x" + body[4:].upper().rjust(8, "0")
                     if body.startswith(("LAB_", "FUN_")) else body)
            self.threads.append({"name": name, "body": where})
            self.emit(f'{self.receiver}:CreateThread("{name}")  -- native thread body {where}: lift it as function {name}(quest)')
            self.calls.append("CreateThread")
            return
        m = RE_IF_GOTO.match(line)
        if m:
            cond = self.expr(m.group(1))
            if stripped in self.lua_jumps:
                self.emit(f'if {cond} then goto {m[2]} end')
            elif m.group(2) in self.cleanup_labels:
                self.emit(f"if {cond} then return end  -- native destructor epilogue {m.group(2)}")
            else:
                self.todo.append(f"verify cleanup at {m.group(2)} (lifted as early return)")
                self.emit(f"if {cond} then return end  -- TODO(native): goto {m.group(2)}")
            return
        m = RE_IF_BREAK.match(line)
        if m:
            self.emit(f"if {self.expr(m.group(1))} then break end")
            return
        m = RE_THING_CALL.match(line)
        if m and m.group(2) != "CScriptThing" and not m.group(2).startswith("~") \
                and not RE_RESOURCE_METHOD.search(m.group(2)):
            self.thing_call(m.group(1), m.group(2), m.group(3), m.group(4) or "")
            return
        m = RE_CSTR.match(line)
        if m:
            self.push_temp(m.group(1), json.dumps(m.group(2)))
            return
        indexed_string = re.fullmatch(r'\s*CCharString::operator=\((\w+),\(CCharString \*\)\((\w+) \+ (\w+) \* 4\)\);\s*', line)
        if indexed_string and self.kinds.get(indexed_string[2]) == 'string_vector':
            self.results += 1
            result = f'r{self.results}'
            self.emit(f'{self.declare(result)} = {indexed_string[2]}[{self.expr(indexed_string[3])} + 1]')
            self.kinds[result] = 'string'
            self.push_temp(indexed_string[1], result)
            return
        m = RE_CSTR_MASTER_ASSIGN.match(line)
        if self.entity and m and m[1].lower() in self.master_strings:
            self.emit(f'{self.receiver}:SetMasterGameState({json.dumps(self.master_strings[m[1].lower()])}, "{m[2]}")')
            self.calls.append('SetMasterGameState')
            return
        m = RE_CSTR_FIELD_ASSIGN.match(line)
        if m:
            offset = m.group(1).lower()
            if offset in self.state:
                name, kind = self.state[offset]
                if kind != "String":
                    self.todo.append(f"field {offset}: CCharString assignment conflicts with {kind}")
                    self.emit(f"-- TODO(native): field {offset} has conflicting recovered type {kind}")
                    return
            else:
                name = f"self_{offset}"
                self.todo.append(f"unnamed field {offset} written in {role}")
                self.emit(f"-- TODO(native): name field {offset} (CCharString)")
            self.emit(f'{self.receiver}:SetStateString("{name}", {json.dumps(m.group(2))})')
            self.calls.append("SetStateString")
            return
        m = RE_CSTR_DEFAULT.match(line)
        if m:
            # A default-constructed native CCharString is the empty Lua string. Keep it as an
            # operand temporary; construction/destruction itself has no script-side effect.
            self.push_temp(m.group(1), '""')
            return
        m = RE_CSTR_COPY.match(line) or RE_CSTR_FROM_VALUE.match(line) or RE_CSTR_LOCAL_ASSIGN.match(line)
        if m and m.group(2) in self.temps:
            # Copy construction/assignment preserves the exact recovered string operand. Do not
            # emit executable Lua until the copy is consumed by a script-interface call.
            self.push_temp(m.group(1), self.temps[m.group(2)])
            return
        if any(n in stripped for n in NOISE) and not structural:
            return
        if stripped == "return;":
            self.emit("return")
            return
        returned = re.fullmatch(r'return\s+(.+);', stripped)
        if returned:
            self.emit('return ' + self.expr(returned[1]))
            return
        if stripped == "break;":
            self.emit("break")
            return
        m = RE_GOTO.match(line)
        if m:
            if stripped in self.lua_jumps:
                self.emit(f'goto {m[1]}')
                return
            self.todo.append(f"goto {m.group(1)}")
            self.emit(f"-- TODO(native): goto {m.group(1)}")
            return
        if stripped == "}":
            branch = self.parent_branch_groups.pop(self.indent, None)
            if branch:
                branch['invalid'].update(set(branch['entry']) - self.parent_aliases)
                for alias in branch['invalid']:
                    self.parent_aliases.discard(alias)
                    self.parent_alias_depth.pop(alias, None)
            self.last_parent_branch = branch
            for alias, depth in list(self.parent_alias_depth.items()):
                if depth >= self.indent:
                    self.parent_aliases.discard(alias)
                    self.parent_alias_depth.pop(alias)
            update = self.for_updates.pop(self.indent, None)
            if update:
                self.statement(role, update + ';')
            self.indent = max(1, self.indent - 1)
            self.emit("end")
            return
        if stripped == "else {":
            if self.last_parent_branch:
                branch = self.last_parent_branch
                self.parent_alias_depth = dict(branch['entry'])
                self.parent_aliases = set(branch['entry'])
                self.parent_branch_groups[self.indent + 1] = branch
            if self.out and self.out[-1].strip() == "end":
                self.out.pop()
                self.indent += 1
            self.out.append("    " * (self.indent - 1) + "else")
            return
        m = RE_ELSE_IF.match(line)
        if m:
            if self.last_parent_branch:
                branch = self.last_parent_branch
                self.parent_alias_depth = dict(branch['entry'])
                self.parent_aliases = set(branch['entry'])
                self.parent_branch_groups[self.indent + 1] = branch
            if self.out and self.out[-1].strip() == "end":
                self.out.pop()
                self.indent += 1
            self.out.append("    " * (self.indent - 1) + f"elseif {self.expr(m.group(1))} then")
            return
        if stripped.startswith('if ') and stripped.endswith('{'):
            self.parent_branch_groups[self.indent + 1] = {'entry': dict(self.parent_alias_depth), 'invalid': set()}
        m = RE_IF_RECHECK_TERMINATING.match(line)
        if m:
            # The repeated pure scheduler query is the same fact Forge reports as `alive`.
            self.used_alive = True
            if self.live_termination:
                self.emit(f"alive = not {self.receiver}:IsActiveThreadTerminating()")
                self.calls.append('IsActiveThreadTerminating')
            self.emit("if alive then")
            self.indent += 1
            return
        m = conditional_call_assignment(line)
        if m:
            control, left, op, target, owner, name, args, right = m
            # C evaluates the left operand once, and assigns the RHS result only when needed.
            # Keep the assigned variable outside the guard so the body and subsequent statements
            # see it, including its old value when the call is skipped.
            while True:
                self.conditions += 1
                condition = f"__native_condition_{self.conditions}"
                if condition not in self.source_names and condition not in self.locals:
                    break
            # A while predicate executes at every loop head, including after the body changes its
            # operands. Keep the assigned result outside the loop so its final value survives.
            if control == "while":
                if target not in self.locals:
                    previous = self.temps.get(target)
                    declaration = self.declare(target)
                    if previous is not None:
                        self.emit(f"{declaration} = {previous}")
                    elif declaration.startswith('local '):
                        self.emit(declaration)
                self.emit("while true do")
                self.indent += 1
            self.emit(f"{self.declare(condition)} = {self.expr(left)}")
            if target not in self.locals:
                previous = self.temps.get(target)
                declaration = self.declare(target)
                if previous is not None:
                    self.emit(f"{declaration} = {previous}")
                elif declaration.startswith('local '):
                    self.emit(declaration)
            guard = condition if op == "&&" else f"not {condition}"
            self.emit(f"if {guard} then")
            self.indent += 1
            if owner == "GSI->":
                self.interface_call(target, name, args)
            elif owner == 'CCharString::':
                operands = self.arguments(args)
                if (name == 'NotEqual' and len(operands) == 2 and operands[0] == 'g_PresentedItemName'
                        and re.fullmatch(r'"[^"\\]+"', operands[1])):
                    self.emit(f'{self.declare(target)} = g_PresentedItemName ~= {operands[1]}')
                    self.kinds[target] = 'bool'
                else:
                    self.todo.append('unresolved conditional CCharString comparison: ' + args)
                    self.emit('-- TODO(native): unresolved conditional CCharString comparison')
            else:
                operands = self.arguments(args)
                if not operands:
                    self.todo.append(f"{name}: conditional thing call has no receiver")
                    self.emit(f"-- TODO(native): {name} has no receiver")
                else:
                    # Use the same dispatch as a standalone call, including reviewed
                    # controlled-resource methods which bind to me rather than a stack address.
                    self.statement(role, f'{target} = CScriptThing::{name}({args});')
            self.emit(f"{condition} = {self.expr(right)}")
            self.indent -= 1
            self.emit("end")
            if control == "while":
                self.emit(f"if not {condition} then break end")
            else:
                self.emit(f"if {condition} then")
                self.indent += 1
            return
        m = RE_IF.match(line)
        if m:
            condition = self.expr(m.group(1))
            if self.kind_of(condition) == 'number' or re.fullmatch(r'-?\d+(?:\.\d+)?', condition):
                condition = f'{condition} ~= 0'
            self.emit(f"if {condition} then")
            self.indent += 1
            return
        counted = re.fullmatch(r'\s*for\s*\(\s*([^;]*);\s*([^;]*);\s*([^;]*)\)\s*\{\s*', line)
        if counted:
            initial, condition, update = (part.strip() for part in counted.groups())
            initial_assignment = RE_LOCAL_ASSIGN.fullmatch(initial + ';') if initial else None
            update_assignment = RE_LOCAL_ASSIGN.fullmatch(update + ';')
            if (not initial or initial_assignment) and update_assignment:
                self.mutable_scalars.add(update_assignment[1])
                if initial_assignment:
                    self.mutable_scalars.add(initial_assignment[1])
                    self.statement(role, initial + ';')
                self.emit(f'while {self.expr(condition) if condition else "true"} do')
                self.indent += 1
                self.for_updates[self.indent] = update
                return
        m = RE_WHILE.match(line)
        if m:
            cond = m.group(1).strip()
            try:
                sequence = split_arguments(cond)
            except ValueError:
                sequence = []
            assignments = [RE_LOCAL_ASSIGN.fullmatch(part + ';') for part in sequence[:-1]]
            if len(sequence) > 1 and all(assignments):
                # C's comma operator evaluates every prefix before the final predicate,
                # including on the check that exits. Keep assignments inside the loop.
                for assignment in assignments:
                    self.mutable_scalars.add(assignment[1])
                self.emit('while true do')
                self.indent += 1
                for part in sequence[:-1]:
                    self.statement(role, part + ';')
                self.emit(f'if not ({self.expr(sequence[-1])}) then break end')
                return
            self.emit("while true do" if cond == "true" else f"while {self.expr(cond)} do")
            self.indent += 1
            return
        if stripped == "do {":
            self.emit("repeat")
            self.indent += 1
            return
        m = RE_DO_WHILE_END.match(line)
        if m:
            self.indent = max(1, self.indent - 1)
            cond = m.group(1).strip()
            self.emit("until false" if cond == "true" else f"until not ({self.expr(cond)})")
            return
        array_store = re.fullmatch(r'\*\(undefined1 \*\)\(this \+ (0x[0-9a-f]+) \+ (\w+)\)\s*=\s*(?:\(\w+\))?(0x[01]|[01]);', stripped)
        if array_store and array_store[1].lower() in self.state_arrays:
            name, kind = self.state_arrays[array_store[1].lower()]
            value = ('true' if int(array_store[3], 0) else 'false') if kind == 'Bool' else array_store[3]
            self.emit(f'{self.state_receiver}:SetState{kind}("{name}_" .. ({self.expr(array_store[2])}), {value})')
            self.calls.append('SetState' + kind)
            return
        parent_store = re.fullmatch(RE_PARENT_LOAD.pattern + r'\s*=\s*([^;]+);', stripped)
        if self.entity and parent_store and parent_store[2].lower() in self.parent_state:
            name, kind = self.parent_state[parent_store[2].lower()]
            value = parent_store[3].strip()
            if kind == "Bool" and value in ("0", "0x0", "1", "0x1", "'\\0'", "'\\x01'"):
                value = "false" if value in ("0", "0x0", "'\\0'") else "true"
            else:
                value = self.expr(value)
            self.emit(f'{self.receiver}:SetState{kind}("{name}", {value})')
            self.calls.append(f"SetState{kind}")
            return
        m = RE_STORE.match(line) or RE_STORE_IDX.match(line)
        if m:
            if m.re is RE_STORE:
                ctype, offset, value = m.group(1), m.group(2).lower(), m.group(3).strip()
            else:
                ctype, offset, value = "undefined1", m.group(1).lower(), m.group(2).strip()
            if offset in self.state:
                name, kind = self.state[offset]
            else:
                kind = STORE_TYPE.get(ctype, "Int")
                name = f"self_{offset}"
                self.todo.append(f"unnamed field {offset} written in {role}")
                self.emit(f"-- TODO(native): name field {offset} ({ctype})")
            if kind == "Bool" and value in ("0", "0x0", "1", "0x1", "'\\0'", "'\\x01'"):
                value = "false" if value in ("0", "0x0", "'\\0'") else "true"
            else:
                value = self.expr(value)
            self.emit(f'{self.state_receiver}:SetState{kind}("{name}", {value})')
            self.calls.append(f"SetState{kind}")
            return
        m = RE_GSI.match(line)
        if m:
            self.interface_call(m.group(1), m.group(2), m.group(3))
            return
        m = RE_NAMED_CALL.match(line)
        if m and not stripped.startswith("if ") and not stripped.startswith("while "):
            target, name, argtext = m.group(1), m.group(2), m.group(3)
            resolved = self.callee_names.get(name, name.split("::")[-1])
            if resolved == 'IsDistanceFromThingToPositionOver':
                operands = [self.expr(a) for a in self.arguments(argtext)]
                proven = False
                if len(operands) == 3:
                    actor, position, distance = operands
                    proven = (bool(re.fullmatch(r'[A-Za-z_]\w*', actor))
                              and self.kind_of(actor) == 'thing'
                              and (actor != 'me' or self.execution_entity)
                              and self.kind_of(position) == 'vector'
                              and (self.kind_of(distance) == 'number'
                                   or bool(re.fullmatch(r'-?\d+(?:\.\d+)?', distance))))
                if not proven:
                    self.todo.append(f'{resolved}: unresolved actor/vector/distance operands')
                    self.emit(f'-- TODO(native): {stripped}')
                    if target:
                        self.emit(f'{self.declare(target)} = nil --[[unresolved native result]]')
                        self.kinds.pop(target, None)
                    return
                call = f'{actor}:IsDistanceFromPositionOver({position}, {distance})'
                if target:
                    self.emit(f'{self.declare(target)} = ({actor} ~= nil and {call})')
                    self.kinds[target] = 'bool'
                else:
                    self.emit(f'if {actor} ~= nil then {call} end')
                self.calls.append('IsDistanceFromPositionOver')
                return
            parent_helper = self.parent_helpers.get(name)
            if parent_helper:
                operands = self.arguments(argtext)
                parent = re.fullmatch(r'\*\(\w+\s*\*\*\)\((?:this|param_1)\s*\+\s*0x14\)',
                                      operands[0] if operands else '')
                if not self.entity or not (parent or (operands and operands[0] in self.parent_aliases)) or len(operands) != parent_helper['arity'] + 1:
                    self.todo.append(f'{name}: unproven parent helper receiver or arity')
                    self.emit(f'-- TODO(native): {stripped}')
                    return
                args = [self.receiver, 'me'] + [self.expr(a) for a in operands[1:]]
                call = (f'require("{parent_helper["module"]}").{parent_helper["name"]}'
                        f'({", ".join(args)})')
                self.emit(f'{self.declare(target)} = {call}' if target else call)
                if target and parent_helper.get('returnKind'):
                    self.kinds[target] = parent_helper['returnKind']
                return
            mangled = RE_RESOURCE_METHOD.search(name)
            if mangled and mangled.group(1) in self.manifest:
                # CScriptGameResourceObjectScriptedThingBase::<Method>(resource, thing, ...) is the
                # controlled-entity API: Forge exposes it as me:<Method>(...) (pMe implied).
                operands = self.arguments(argtext)
                # Speak's second native operand is its target, not another implicit receiver. The
                # reviewed vtable typedef and LuaManager overload both retain it.
                drop = 1 if mangled.group(1) == "Speak" else 2
                operands = operands[drop:]
                if mangled.group(1) == "Speak":
                    # The retail typedef has target, key, method and three bools. Ghidra sometimes
                    # appends one phantom operand; trim by the reviewed ABI before generic temp
                    # placement so the exact key temporary is not mistaken for spillover.
                    operands = operands[:6]
                self.interface_call(target, mangled.group(1), ",".join(operands))
                return
            if resolved in TERMINATING_NAMES:
                self.interface_call(target, resolved, argtext)
                return
            if resolved in self.manifest or resolved == "StartScriptingEntity":
                self.interface_call(target, resolved, argtext)
                return
            if resolved in self.helper_names:
                operands = [self.expr(x) for x in self.arguments(argtext)]
                # Recovered script helpers are C++ member functions. Their first explicit Ghidra
                # operand is the native `this`; generated Lua helpers receive Quest directly.
                if operands and operands[0] in ("this", "param_1", "in_ECX"):
                    operands.pop(0)
                expected = self.helper_parameters.get(resolved)
                if expected is not None and len(operands) != len(expected):
                    self.todo.append(f'{resolved}: {len(operands)} helper operands for {len(expected)} parameters')
                context = [self.receiver] + (['me'] if self.execution_entity else [])
                call = f"{resolved}({', '.join(context + operands)})"
                self.emit(f'{self.declare(target)} = {call}' if target else call)
                if target and resolved in self.helper_return_kinds:
                    self.kinds[target] = self.helper_return_kinds[resolved]
                return
        m = RE_SLOT_ASSIGN.match(line)
        if m:
            value = m.group(2).strip()
            lit = self.literal(value)
            if lit is not None:
                self.push_temp(m.group(1), lit)
                return
            if RE_ADDR_LITERAL.match(value):
                # A code address (return-site / EH bookkeeping the decompiler spilled): not an operand.
                self.forget_value(m[1])
                return
            if value in self.temps or value in self.locals:
                self.push_temp(m.group(1), self.temps.get(value, value))
                return
        m = RE_LOCAL_ASSIGN.match(line)
        if m:
            value = m.group(2).strip()
            if self.entity and (value in self.parent_aliases or re.fullmatch(
                    r'\*\([\w:]+ \*{1,2}\)\((?:this|param_1) \+ 0x14\)', value)):
                self.declare(m[1])
                self.parent_aliases.add(m[1])
                self.parent_alias_depth[m[1]] = self.indent
                self.kinds[m[1]] = 'quest'
                return
            lit = self.literal(RE_CAST.sub("", value).strip())
            if lit is not None:
                # Ghidra stages const char* strings and scalar enum/bool values in locals before
                # calls. Pure literal assignment is exact operand evidence, including misleading
                # pointer casts such as `(CCharString *)0x1` on scalar ABI slots.
                var = m.group(1)
                if var in self.mutable_scalars:
                    self.emit(f"{self.declare(var)} = {lit}")
                    self.kinds[var] = ("string" if lit.startswith('"') else
                                       "bool" if lit in ("true", "false") else "number")
                    if var not in self.sequence_temporaries:
                        self.staged_scalars.add(var)
                        self.push_temp(var, var)
                else:
                    self.push_temp(var, lit)
                return
            ref = RE_ADDR_OF.match(value)
            if ref:
                referent = self.temps.get(ref[1])
                self.forget_value(m[1])
                if referent is not None:
                    # `local = &temporary`: an alias of a string operand, not a statement.
                    self.push_temp(m.group(1), referent)
                return
            if "::" in value or re.search(r'\b(?:FUN_|LAB_|DAT_)', value):
                self.forget_value(m[1])
                self.todo.append(f"unlifted: {stripped[:90]}")
                self.emit(f"-- TODO(native): {stripped}")
                return
            lifted = self.expr(value)
            var = m.group(1)
            if lifted == "me" and self.entity:
                # `pCVarN = (CScriptThing *)(this + 8);` is a receiver alias, not a statement
                self.declare(var)
                self.me_aliases.add(var)
                self.kinds[var] = "thing"
                if var in self.temps:            # it no longer holds an earlier result
                    self.temps.pop(var)
                    self.order.remove(var)
                return
            if RE_HIDDEN_SLOT.match(lifted) and lifted not in self.locals:
                # `ppVar7 = apStack_c;` points a local at a return slot: bookkeeping, not a statement
                self.declare(var)
                self.slot_alias[var] = lifted
                return
            kind = self.kinds.get(lifted) or ("number" if RE_NUMERIC_EXPR.fullmatch(lifted) else None)
            if re.fullmatch(r'__native_vectors\[0x[0-9a-f]+\]', lifted):
                kind = 'string_vector'
            state_getter = re.fullmatch(r'\w+:GetState(Bool|Int|Float)\("\w+"\)', lifted)
            if state_getter:
                kind = "bool" if state_getter[1] == "Bool" else "number"
            self.emit(f"{self.declare(var)} = {lifted}")
            if kind:
                self.kinds[var] = kind           # `a = b;` copies b's kind; `uVar15 = uVar4 | 1;` is a number
            return
        unresolved_slot = RE_SLOT_ASSIGN.match(line)
        if unresolved_slot:
            self.forget_value(unresolved_slot[1])
        if unresolved_slot and RE_ADDR_OF.match(unresolved_slot.group(2).strip()):
            return  # `slot = &returnSlot` bookkeeping
        self.todo.append(f"unlifted: {stripped[:90]}")
        self.emit(f"-- TODO(native): {stripped}")


def lift_persist(decompile: str, receiver: str) -> tuple[list[str], dict[str, tuple[str, str]], list[str]]:
    state: dict[str, tuple[str, str]] = {}
    out: list[str] = []
    calls: list[str] = []
    for ctype, name, offset in RE_TRANSFER.findall(decompile):
        kind = TYPE_MAP.get(ctype, "Int")
        state[offset.lower()] = (name, kind)
        var = name[0].lower() + name[1:]
        out.append(f'    local {var} = {receiver}:GetState{kind}("{name}") or {LUA_DEFAULT[kind]}')
        out.append(f'    {var} = {receiver}:PersistTransfer{kind}(context, "{name}", {var})')
        out.append(f'    {receiver}:SetState{kind}("{name}", {var})')
        calls += [f"GetState{kind}", f"PersistTransfer{kind}", f"SetState{kind}"]
    return out, state, calls


def thing_signatures(thing_slots: dict[int, tuple[str, str]]) -> dict[str, str]:
    return {name: mangled for name, mangled in thing_slots.values()}


def known_callee_aliases(fn: dict) -> dict[str, str]:
    """Map decompiler call labels only when their exact native target has a reviewed alias."""
    candidates: dict[str, set[str | None]] = {}
    for call in fn.get("calls", []):
        current = call.get("currentName")
        target = str(call.get("target") or call.get("address") or "")
        if not current:
            continue
        try:
            alias = CALLEE_ALIASES.get(int(target, 16))
        except ValueError:
            alias = None
        candidates.setdefault(current, set()).add(alias)
    return {name: next(iter(aliases)) for name, aliases in candidates.items()
            if len(aliases) == 1 and None not in aliases}


def load_thing_tables(manifest: dict[str, dict], slots: dict[int, str]) -> tuple[dict[int, tuple[str, str]], set[int]]:
    """The CScriptThing slot table plus the interface offsets whose result is a CScriptThing (from the
    Forge manifest return types: GetHero, GetThingWithScriptName, ...), which make their result
    variables thing receivers for the annotator."""
    thing_slots = load_thing_slots()
    returning = {off for off, name in slots.items()
                 if "CScriptThing" in str(manifest.get(name, {}).get("returnType", ""))}
    return thing_slots, returning


def lift_cluster(script: str) -> dict:
    cluster = json.loads((CLUSTERS / f"{script}.json").read_text(encoding="utf-8-sig"))
    manifest = load_manifest()
    slots = load_slots()
    thing_slots, thing_returning = load_thing_tables(manifest, slots)
    package = re.sub(r'^(Q_|V_|QS_|CS_)', "", script)
    receiver = "Quest"
    by_role = {l["role"]: annotate(l.get("decompile") or "", slots, thing_slots, thing_returning, entity=False)
               for l in cluster["lifecycle"]}
    persist_lines, state, persist_calls = lift_persist(by_role.get("OnPersist", ""), receiver)
    lifter = Lifter(manifest, state, receiver, False, package, RData(), thing_sigs=thing_signatures(thing_slots))
    report = {"script": script, "package": package, "state": state, "functions": {}, "entities": [], "threads": [],
              "helpers": [], "entityBodies": {}}
    lua = [f"-- Lifted draft of retail {script} (allocator {cluster.get('allocatorAddress')}).",
           "-- Generated by tools/script_recovery/lift_native_lua.py; every TODO(native) needs a human.",
           "Quest = nil", ""]
    for role in ("Init", "Main"):
        native_fn = next((f for f in cluster["lifecycle"] if f["role"] == role), None)
        body = lifter.lift(role, by_role.get(role, ""), native_function=native_fn)
        report["functions"][role] = {"lines": len(body), "todo": list(lifter.todo), "calls": list(lifter.calls),
                                      "literalByteComparisons": lifter.literal_comparisons}
        if lifter.thing_predicate_evidence:
            report["functions"][role]["thingPredicateEvidence"] = lifter.thing_predicate_evidence
        if lifter.operand_evidence:
            report["functions"][role]["operandEvidence"] = lifter.operand_evidence
        report["entities"] += lifter.entities
        report["threads"] += lifter.threads
        lua += [f"function {role}(questObject)", "    Quest = questObject"] + body + ["end", ""]
    # CreateThread records only a symbol/address in the lifecycle decompile. The read-only
    # native-thread export supplies the actual bodies. Feed those through the same annotator and
    # lifter instead of leaving the largest quest scripts as empty thread declarations.
    thread_path = ROOT / "refs" / "script_recovery" / "native_threads" / f"{script}.json"
    if report["threads"] and thread_path.is_file():
        thread_export = json.loads(thread_path.read_text(encoding="utf-8-sig"))
        exported = thread_export.get("functions", [])
        helpers = [f for f in exported if f.get("decompile") and
                   any(str(s).startswith("callee:") for s in f.get("selectedBy", []))]
        helper_counts = Counter(str(f.get("name", "")).split("::")[-1] for f in helpers)
        helper_by_address: dict[str, str] = {}
        for fn in helpers:
            terminal = str(fn.get("name", "")).split("::")[-1]
            address = str(fn.get("address", "")).upper()
            helper_by_address[address] = (f"{terminal}_{address[2:]}" if helper_counts[terminal] > 1 else terminal)
        lifter.helper_names = set(helper_by_address.values())

        def set_callee_names(fn: dict) -> None:
            mapped: dict[str, str] = {}
            for call in fn.get("calls", []):
                current = call.get("currentName")
                target = str(call.get("target") or call.get("address") or "").upper()
                if current and target in helper_by_address:
                    mapped[current] = helper_by_address[target]
                elif current and target:
                    try:
                        if int(target, 16) in CALLEE_ALIASES:
                            mapped[current] = CALLEE_ALIASES[int(target, 16)]
                    except ValueError:
                        pass
            lifter.callee_names = mapped

        emitted: set[str] = set()
        for thread in report["threads"]:
            name = thread["name"]
            if name in emitted:
                continue
            emitted.add(name)
            address = str(thread.get("body", "")).upper()
            fn = next((f for f in exported
                       if f"thread:{name}" in f.get("selectedBy", [])
                       or str(f.get("address", "")).upper() == address
                       or str(f.get("name", "")).split("::")[-1] == name), None)
            if not fn or not fn.get("decompile"):
                report["functions"][f"Thread:{name}"] = {
                    "lines": 1, "todo": [f"native thread body unavailable: {name}"], "calls": []}
                lua += [f"function {name}(questObject)", "    Quest = questObject",
                        f"    -- TODO(native): native thread body unavailable ({address})", "end", ""]
                continue
            set_callee_names(fn)
            annotated = annotate(fn["decompile"], slots, thing_slots, thing_returning, entity=False)
            body = lifter.lift(f"Thread:{name}", annotated, native_function=fn)
            report["functions"][f"Thread:{name}"] = {
                "lines": len(body), "todo": list(lifter.todo), "calls": list(lifter.calls),
                "literalByteComparisons": lifter.literal_comparisons}
            if lifter.thing_predicate_evidence:
                report["functions"][f"Thread:{name}"]["thingPredicateEvidence"] = lifter.thing_predicate_evidence
            lua += [f"function {name}(questObject)", "    Quest = questObject"] + body + ["end", ""]
            # A mission thread can itself create worker threads. Appending to this queue is
            # intentional: Python's list iterator will visit them in discovery order, using an
            # existing export immediately or emitting a precise stub for the next export pass.
            known_threads = {(t["name"], t.get("body")) for t in report["threads"]}
            for nested in lifter.threads:
                key = (nested["name"], nested.get("body"))
                if key not in known_threads:
                    report["threads"].append(nested)
                    known_threads.add(key)
        # Emit every available in-range helper reached by a lifecycle/thread body. Names are
        # address-qualified only when retail exports two different functions with the same terminal
        # symbol (Beardy Baldy's two GoTalkToBeardyBaldy bodies).
        for fn in helpers:
            helper_name = helper_by_address[str(fn.get("address", "")).upper()]
            report["helpers"].append({"name": helper_name, "address": fn.get("address"),
                                      "selectedBy": fn.get("selectedBy", [])})
            set_callee_names(fn)
            annotated = annotate(fn["decompile"], slots, thing_slots, thing_returning, entity=False)
            body = lifter.lift(f"Helper:{helper_name}", annotated)
            report["functions"][f"Helper:{helper_name}"] = {
                "lines": len(body), "todo": list(lifter.todo), "calls": list(lifter.calls),
                "literalByteComparisons": lifter.literal_comparisons}
            lua += [f"function {helper_name}(questObject)", "    Quest = questObject"] + body + ["end", ""]
    lua += ["function OnPersist(questObject, context)", "    Quest = questObject"] + persist_lines + ["end", ""]
    report["functions"]["OnPersist"] = {"lines": len(persist_lines), "todo": [], "calls": persist_calls}
    # Anchored native-entity exports are keyed by package + binding name. A one-function export is
    # the recovered entity Main body (the exporter deliberately anchors that body); Init remains
    # empty unless a separately named Init is present. Never select among multiple ambiguous bodies.
    native_entities = ROOT / "refs" / "script_recovery" / "native_entities"
    for entity in sorted(set(report["entities"])):
        entity_path = native_entities / f"{package}.{entity}.json"
        if not entity_path.is_file():
            continue
        payload = json.loads(entity_path.read_text(encoding="utf-8-sig"))
        functions = [f for f in payload.get("functions", []) if f.get("decompile")]
        mains = [f for f in functions if str(f.get("currentName", "")).split("::")[-1] == "Main"]
        main_fn = mains[0] if len(mains) == 1 else (functions[0] if len(functions) == 1 else None)
        if main_fn is None:
            continue
        entity_lifter = Lifter(manifest, {}, "quest", True, package, RData(),
                               thing_sigs=thing_signatures(thing_slots))
        entity_lifter.callee_names = known_callee_aliases(main_fn)
        body = entity_lifter.lift("Main", annotate(main_fn["decompile"], slots, thing_slots,
                                                    thing_returning, entity=True))
        entity_lua = [f"-- Lifted anchored retail entity {entity} ({main_fn.get('address')}).",
                      "-- Generated by tools/script_recovery/lift_native_lua.py; review TODO(native) markers.",
                      "", "function Init(quest, me)", "end", "", "function Main(quest, me)"]
        entity_lua += body + ["end", ""]
        report["entityBodies"][entity] = {
            "address": main_fn.get("address"), "lines": len(body), "todo": list(entity_lifter.todo),
            "calls": list(entity_lifter.calls), "lua": "\n".join(entity_lua),
            "literalByteComparisons": entity_lifter.literal_comparisons}
    report["lua"] = "\n".join(lua)
    return report


def lift_entity(tu_path: Path, entity: str, init_addr: str | None, main_addr: str | None) -> dict:
    tu = json.loads(tu_path.read_text(encoding="utf-8-sig"))
    by_addr = {f["address"].upper(): f for f in tu["functions"]}
    manifest = load_manifest()
    slots = load_slots()
    thing_slots, thing_returning = load_thing_tables(manifest, slots)
    lua = [f"-- Lifted draft of retail entity script {entity} (Init {init_addr}, Main {main_addr}).",
           "-- Generated by tools/script_recovery/lift_native_lua.py; every TODO(native) needs a human.",
           f'local MY_SCRIPT_NAME = "{entity}"', ""]
    state = load_entity_state(entity)
    parent_state = load_entity_parent_state(entity)
    constants = load_entity_constants(entity)
    report = {"entity": entity, "functions": {}, "state": state, "parentState": parent_state,
              "nativeConstants": constants}
    lifter = Lifter(manifest, state, "quest", True, "", RData(), thing_sigs=thing_signatures(thing_slots),
                    parent_state=parent_state, native_constants=constants)
    for role, addr in (("Init", init_addr), ("Main", main_addr)):
        if not addr:
            lua += [f"function {role}(quest, me)", "end", ""]
            continue
        fn = by_addr.get(addr.upper())
        if not fn:
            raise SystemExit(f"{addr} not in translation unit")
        # Rename similarity-labelled callees by their real address before lifting.
        lifter.callee_names = known_callee_aliases(fn)
        body = lifter.lift(role, annotate(fn.get("decompile") or "", slots, thing_slots, thing_returning, entity=True))
        report["functions"][role] = {"lines": len(body), "todo": list(lifter.todo), "calls": list(lifter.calls),
                                      "literalByteComparisons": lifter.literal_comparisons}
        lua += [f"function {role}(quest, me)"] + body + ["end", ""]
    report["lua"] = "\n".join(lua)
    return report


def calls_in_lua(text: str) -> Counter:
    return Counter(re.findall(r'\b(?:Quest|quest|me|Me):(\w+)\(', text))


def write_package(report: dict, out_dir: Path) -> None:
    pkg = report["package"]
    base = out_dir / "FSE" / pkg
    (base / "Entities").mkdir(parents=True, exist_ok=True)
    (base / f"{pkg}.lua").write_text(report["lua"] + "\n", encoding="utf-8")
    for entity in report["entities"]:
        recovered = report.get("entityBodies", {}).get(entity)
        if recovered:
            (base / "Entities" / f"{entity}.lua").write_text(recovered["lua"] + "\n", encoding="utf-8")
            continue
        stub = (f'-- TODO(native): entity script "{entity}" needs its translation-unit export; lift it with\n'
                f'-- lift_native_lua.py --tu <translation_unit.json> --entity {entity} --init 0x... --main 0x...\n'
                f'local MY_SCRIPT_NAME = "{entity}"\n\nfunction Init(quest, me)\nend\n\n'
                f'function Main(quest, me)\n    -- TODO(native): body not lifted\nend\n')
        (base / "Entities" / f"{entity}.lua").write_text(stub, encoding="utf-8")
    (out_dir / "FSE" / "quests.lua").write_text(
        f'-- Lifted draft; NOT registered by default (competes with the retail allocator if enabled).\n'
        f'Quests = {{}}\n-- Quests.{pkg} = {{ file = "{pkg}/{pkg}", enabled = false }}\n', encoding="utf-8")
    summary = copy.deepcopy({k: v for k, v in report.items() if k != "lua"})
    for body in summary.get("entityBodies", {}).values():
        body.pop("lua", None)
    (out_dir / "LIFT_REPORT.json").write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")


def compare(lua: str, oracle: Path) -> dict:
    files = [oracle] if oracle.is_file() else [p for p in oracle.rglob("*.lua")
                                                if "Entities" not in p.parts and p.name != "quests.lua"]
    oracle_calls = Counter()
    for p in files:
        oracle_calls += calls_in_lua(p.read_text(encoding="utf-8", errors="replace"))
    lifted = calls_in_lua(lua)
    ignore = {"Log"}
    o = {k: v for k, v in oracle_calls.items() if k not in ignore}
    l = {k: v for k, v in lifted.items() if k not in ignore}
    hit = sum(min(v, l.get(k, 0)) for k, v in o.items())
    return {"oracleCalls": sum(o.values()), "liftedCalls": sum(l.values()), "matched": hit,
            "recall": round(hit / max(1, sum(o.values())), 3),
            "precision": round(hit / max(1, sum(l.values())), 3),
            "missingFromLift": sorted(k for k in o if l.get(k, 0) < o[k]),
            "extraInLift": sorted(k for k in l if o.get(k, 0) < l[k])}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--script", help="native cluster name, e.g. V_MazeResearch (quest mode)")
    parser.add_argument("--tu", type=Path, help="translation_unit.json (entity mode)")
    parser.add_argument("--entity", help="entity script name (entity mode)")
    parser.add_argument("--init", help="entity Init address (entity mode)")
    parser.add_argument("--main", help="entity Main address (entity mode)")
    parser.add_argument("--out", type=Path, help="output dir (quest mode) or .lua path (entity mode)")
    parser.add_argument("--compare", type=Path, help="hand-port package dir or .lua file to score against")
    parser.add_argument("--print", action="store_true", help="print the lifted Lua")
    args = parser.parse_args()
    if args.script:
        report = lift_cluster(args.script)
        out_dir = args.out or (ROOT / "refs" / "script_recovery" / "lifted" / report["package"])
        write_package(report, out_dir)
        summary = {"script": args.script, "package": report["package"], "out": str(out_dir),
                   "entities": report["entities"], "state": report["state"]}
    elif args.tu and args.entity:
        report = lift_entity(args.tu, args.entity, args.init, args.main)
        out = args.out or (ROOT / "refs" / "script_recovery" / "lifted" / "entities" / f"{args.entity}.lua")
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(report["lua"] + "\n", encoding="utf-8")
        summary = {"entity": args.entity, "out": str(out)}
    else:
        parser.error("use --script (quest mode) or --tu/--entity (entity mode)")
        return 2
    summary["todoCount"] = sum(len(f["todo"]) for f in report["functions"].values())
    summary["perFunction"] = {k: {"lines": v["lines"], "todo": len(v["todo"])} for k, v in report["functions"].items()}
    summary["todo"] = [t for f in report["functions"].values() for t in f["todo"]]
    summary["compare"] = compare(report["lua"], args.compare) if args.compare else None
    print(json.dumps(summary, indent=2))
    if args.print:
        print(report["lua"])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
