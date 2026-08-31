#!/usr/bin/env python3
"""Transactionally deploy or roll back a verified retail-shadow smoke package."""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

if __package__ in {None, ""}:
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.script_recovery.verify_shadow_smoke_package import sha256, verify


def ensure_under(path: Path, root: Path) -> Path:
    resolved = path.resolve()
    resolved.relative_to(root.resolve())
    return resolved


def fable_running() -> bool:
    result = subprocess.run(
        ["powershell", "-NoProfile", "-Command",
         "@(Get-Process -Name Fable -ErrorAction SilentlyContinue).Count"],
        check=True, capture_output=True, text=True)
    return int(result.stdout.strip() or "0") > 0


def deploy(package_root: Path, game_root: Path) -> dict[str, object]:
    package_result = verify(package_root)
    if not package_result["complete"]:
        raise ValueError(f"package verification failed: {package_result['failures']}")
    if fable_running():
        raise RuntimeError("refusing deployment while Fable.exe is running")
    if not (game_root / "Fable.exe").is_file():
        raise FileNotFoundError(f"Fable.exe not found under {game_root}")

    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    transaction_root = game_root / "FSE" / "backups" / f"RetailShadowSmoke-{stamp}"
    backup_root = transaction_root / "files"
    transaction_root.mkdir(parents=True, exist_ok=False)
    package_manifest = json.loads((package_root / "shadow_smoke_manifest.json").read_text(encoding="utf-8-sig"))
    records = []
    for row in package_manifest["files"]:
        relative = Path(row["path"])
        source = ensure_under(package_root / relative, package_root)
        destination = ensure_under(game_root / relative, game_root)
        existed = destination.is_file()
        previous_hash = None
        if existed:
            previous_hash = sha256(destination)
            backup = backup_root / relative
            backup.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(destination, backup)
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, destination)
        if sha256(destination) != row["sha256"]:
            raise RuntimeError(f"post-copy hash mismatch: {relative.as_posix()}")
        records.append({"path": relative.as_posix(), "existed": existed,
                        "previousSha256": previous_hash, "deployedSha256": row["sha256"]})

    transaction = {
        "schema": "forgefse-retail-shadow-deployment/0.1",
        "createdUtc": stamp,
        "package": str(package_root),
        "gameRoot": str(game_root),
        "safety": package_manifest["safety"],
        "files": records,
    }
    transaction_path = transaction_root / "deployment.json"
    transaction_path.write_text(json.dumps(transaction, indent=2) + "\n", encoding="utf-8")
    return {"deployed": len(records), "transaction": str(transaction_path),
            "backupRoot": str(backup_root)}


def rollback(transaction_path: Path) -> dict[str, object]:
    if fable_running():
        raise RuntimeError("refusing rollback while Fable.exe is running")
    transaction = json.loads(transaction_path.read_text(encoding="utf-8-sig"))
    game_root = Path(transaction["gameRoot"]).resolve()
    backup_root = transaction_path.parent / "files"
    restored = removed = 0
    for row in reversed(transaction["files"]):
        relative = Path(row["path"])
        destination = ensure_under(game_root / relative, game_root)
        if row["existed"]:
            backup = ensure_under(backup_root / relative, backup_root)
            if not backup.is_file() or sha256(backup) != row["previousSha256"]:
                raise RuntimeError(f"backup missing or corrupt: {relative.as_posix()}")
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(backup, destination)
            restored += 1
        elif destination.is_file():
            if sha256(destination) != row["deployedSha256"]:
                raise RuntimeError(f"refusing to remove modified deployed file: {relative.as_posix()}")
            destination.unlink()
            removed += 1
    return {"restored": restored, "removed": removed, "transaction": str(transaction_path)}


def main() -> int:
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="action", required=True)
    deploy_parser = sub.add_parser("deploy")
    deploy_parser.add_argument("--package", type=Path, required=True)
    deploy_parser.add_argument("--game-root", type=Path, required=True)
    rollback_parser = sub.add_parser("rollback")
    rollback_parser.add_argument("--transaction", type=Path, required=True)
    args = parser.parse_args()
    if args.action == "deploy":
        result = deploy(args.package.resolve(), args.game_root.resolve())
    else:
        result = rollback(args.transaction.resolve())
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
