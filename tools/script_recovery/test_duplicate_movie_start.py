"""A lowered native movie constructor must not acquire the same sequence twice."""
from pathlib import Path
import pytest
from tools.script_recovery.native_evidence_lowering import fold_adjacent_movie_start, lower_after_annotate

START = '  movie = RESOURCE_StartMovie("");\n'
CALL = '  GSI->StartMovieSequence();\n'


@pytest.mark.parametrize('gap', [
    '',
    '  CCharString::CCharString(name,"",-1);\n',
    '  alias = movie;\n',
    '  CCharString::CCharString(name,"",-1);\n  alias = movie;\n',
    '  xStack_12c = (CCharString)0xe3bd3e;\n'
    '  CCharString::CCharString(name,"",-1);\n'
    '  xStack_12c = (CCharString)0xe3bd53;\n',
])
def test_scaffolding_preserved_and_only_second_acquisition_removed(gap):
    text = START + gap + CALL + '  GSI->PauseAllNonScriptedEntities(1);\n'
    assert fold_adjacent_movie_start(text) == text.replace(CALL, '')


@pytest.mark.parametrize('gap', [
    '  movie = (CCharString)0xe3bd3e;\n',
    '  movie = movie;\n',
    '  alias = otherMovie;\n',
    '  GSI->EndMovieSequence();\n',
    '  if (condition) {\n',
    'LABEL:\n',
    '  Notify(movie);\n',
    '  movie = RESOURCE_StartMovie("second");\n',
])
def test_unrelated_work_or_control_flow_is_not_crossed(gap):
    text = START + gap + CALL
    if 'RESOURCE_StartMovie("second")' in gap:
        # The second constructor owns the adjacent call; the first is kept.
        assert fold_adjacent_movie_start(text) == START + gap
    else:
        assert fold_adjacent_movie_start(text) == text


def test_standalone_start_is_kept():
    assert fold_adjacent_movie_start(CALL) == CALL


def test_explicit_different_movie_operand_is_kept():
    text = START + '  GSI->StartMovieSequence(&name, otherMovie);\n'
    assert lower_after_annotate(text) == text


@pytest.mark.parametrize('index', [1, 2])
def test_captured_bordello_patterns(index):
    root = Path(__file__).resolve().parents[2]
    path = root / f'work/codex_lua_movie_start_20261001/baseline/duplicate_movie_output_{index}.c'
    text = path.read_text()
    changed = fold_adjacent_movie_start(text)
    assert changed.count('RESOURCE_StartMovie(') == text.count('RESOURCE_StartMovie(')
    assert changed.count('GSI->StartMovieSequence();') < text.count('GSI->StartMovieSequence();')
    # No source line other than a redundant call is deleted or rewritten.
    assert ([l for l in changed.splitlines() if l.strip() != CALL.strip()] ==
            [l for l in text.splitlines() if l.strip() != CALL.strip()])
