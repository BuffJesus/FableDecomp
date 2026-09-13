#!/usr/bin/env python3
"""Prove EndMovieSequence performs the same retail movie-resource destruction as native scripts."""
from __future__ import annotations

import json
from pathlib import Path


def audit(fable_api: Path, quest_source: Path, entity_dir: Path, api_gap_doc: Path | None = None) -> dict:
    api = fable_api.read_text(encoding="utf-8-sig")
    source = quest_source.read_text(encoding="utf-8-sig")
    end_movie = source[source.index("void LuaQuestState::EndMovieSequence"):
                       source.index("void LuaQuestState::AbandonMovieSequence")]
    manifests = [json.loads(p.read_text(encoding="utf-8-sig")) for p in entity_dir.glob("*.json")]
    movie_cleanup_notes = [
        op.get("note", "") for manifest in manifests for op in manifest.get("operations", [])
        if "~CScriptGameResourceObjectMovieBase" in op.get("native", "")
    ]
    gap_doc = api_gap_doc.read_text(encoding="utf-8-sig") if api_gap_doc else ""
    checks = {
        "forgeBindsRetailDerivedDestructor": (
            "MovieResource_Destroy_API = ASLR<tMovieResource_Destructor>(0x6E7B80);" in api),
        "endMovieCallsBoundDestructor": "MovieResource_Destroy_API(movieHandle);" in end_movie,
        "handleRemovedAfterDestructor": (
            end_movie.index("MovieResource_Destroy_API(movieHandle);")
            < end_movie.rindex("m_movieHandlesByLuaState.erase(movie);")),
        "allDocumentedMovieCleanupNotesAreExact": bool(movie_cleanup_notes)
            and all("inference" not in note.lower() for note in movie_cleanup_notes),
        "apiGapDocDoesNotContradictExactMapping": (
            not gap_doc or (
                "EndMovieSequence` is not equivalent" not in gap_doc
                and "exact retail derived movie-resource destructor at `0x006E7B80`" in gap_doc
            )
        ),
    }
    return {"schema": "forgefse-movie-destructor-audit/0.1", "ok": all(checks.values()),
            "checks": checks, "movieCleanupRecords": len(movie_cleanup_notes)}
