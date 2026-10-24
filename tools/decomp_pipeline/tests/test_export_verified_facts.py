import csv
import json
import hashlib
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

from tools.decomp_pipeline import export_verified_facts as exporter


class VerifiedFactsExportTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.source_dir = self.root / "rebuild/src/compiled"
        self.source_dir.mkdir(parents=True)
        self.source = self.source_dir / "Example_Get_00401000.cpp"
        self.source.write_text(
            "#pragma pack(push,1)\nstruct Example { int value; };\n"
            "int Get(Example* self) { return self->value; }\n",
            encoding="utf-8",
        )
        self.manifest = [{
            "address": "00401000", "module": "Example", "name": "Get",
            "calling_convention": "__thiscall", "return_type": "int",
            "parameter_types": "", "compiled_source": str(self.source),
        }]
        self.audit = []
        self.queue = [{"address": "00401000", "api_names": "Example.Get"}]

    def write_table(self, relative, fields, rows):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("w", encoding="utf-8", newline="") as stream:
            writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t")
            writer.writeheader()
            writer.writerows(rows)

    def write_inputs(self):
        self.write_table("rebuild/manifest/functions.tsv", list(self.manifest[0]), self.manifest)
        self.write_table("rebuild/compile-gate/parity_audit.tsv", ["address", "verdict", "file"], self.audit)
        self.write_table("rebuild/backlog/forgefse-binding-queue.tsv", ["address", "api_names"], self.queue)

    def run_export(self):
        self.write_inputs()
        # Also bound the old implementation's globals, so the before-fix
        # regression cannot inspect or write the shared checkout or siblings.
        with patch.object(exporter, "ROOT", self.root), \
                patch.object(exporter.class_struct, "SRC", self.source_dir), \
                patch.object(sys, "argv", ["export_verified_facts.py", "--no-mirror"]):
            exporter.main()
        return {
            name: json.loads((self.root / "rebuild/sdk" / (name + ".json")).read_text(encoding="utf-8"))
            for name in ("verified_functions", "struct_schema", "forgefse_verified_bindings")
        }

    def test_missing_audit_row_never_becomes_verified(self):
        exported = self.run_export()
        for name, payload in exported.items():
            with self.subTest(export=name):
                self.assertEqual(payload["count"], 0)

    def matching_audit(self, verdict="EXACT"):
        self.audit = [{"address": "0x00401000", "verdict": verdict, "file": self.source.name}]

    def test_explicit_exact_and_relocation_rows_export_all_three_views(self):
        for verdict in ("EXACT", "RELOC"):
            with self.subTest(verdict=verdict):
                self.matching_audit(verdict)
                exported = self.run_export()
                for payload in exported.values():
                    self.assertEqual(payload["count"], 1)
                    self.assertIn("not established", payload["assurance"]["freshness"])
                fact = exported["verified_functions"]["functions"][0]
                self.assertEqual(fact["parity"], verdict)
                self.assertEqual(fact["source_sha256"], hashlib.sha256(self.source.read_bytes()).hexdigest())
                self.assertEqual(exported["struct_schema"]["classes"]["Example"]["fields"][0]["name"], "value")
                self.assertNotIn("behaviour byte-confirmed", exported["forgefse_verified_bindings"]["note"])

    def test_nonmatching_or_unknown_verdicts_are_excluded(self):
        for verdict in ("DIFFER", "NEAR_MATCH", "MATCH*", "", "PASS"):
            with self.subTest(verdict=verdict):
                self.matching_audit(verdict)
                for payload in self.run_export().values():
                    self.assertEqual(payload["count"], 0)

    def test_assembly_sources_are_excluded_from_every_view(self):
        self.matching_audit()
        original = self.source.read_text(encoding="utf-8")
        for marker in ("__asm int 3;", "__declspec(naked) void Bake();", "_emit 0x90;"):
            with self.subTest(marker=marker):
                self.source.write_text(original + marker, encoding="utf-8")
                for payload in self.run_export().values():
                    self.assertEqual(payload["count"], 0)

    def test_audit_and_manifest_must_identify_the_actual_source(self):
        self.matching_audit()
        self.audit[0]["file"] = "Other_Get_00401000.cpp"
        self.assertEqual(self.run_export()["verified_functions"]["count"], 0)
        self.matching_audit()
        self.manifest[0]["compiled_source"] = "Other_Get_00401000.cpp"
        self.assertEqual(self.run_export()["verified_functions"]["count"], 0)

    def test_normalized_duplicate_audit_rows_are_ambiguous(self):
        self.matching_audit()
        self.audit.append({"address": "401000", "verdict": "RELOC", "file": self.source.name})
        for payload in self.run_export().values():
            self.assertEqual(payload["count"], 0)

    def test_normalized_duplicate_manifest_rows_are_ambiguous(self):
        self.matching_audit()
        self.manifest.append({**self.manifest[0], "address": "0x401000"})
        for payload in self.run_export().values():
            self.assertEqual(payload["count"], 0)

    def test_duplicate_source_addresses_are_ambiguous(self):
        self.matching_audit()
        (self.source_dir / "Other_Get_00401000.cpp").write_text("int Get() { return 1; }", encoding="utf-8")
        self.assertEqual(self.run_export()["verified_functions"]["count"], 0)

    def test_schema_does_not_use_unaudited_members_of_an_eligible_class(self):
        self.matching_audit()
        other = self.source_dir / "Example_Set_00401010.cpp"
        other.write_text("#pragma pack(push,1)\nstruct Example { char pad[4]; int poison; };", encoding="utf-8")
        self.manifest.append({**self.manifest[0], "address": "00401010", "name": "Set", "compiled_source": str(other)})
        schema = self.run_export()["struct_schema"]["classes"]["Example"]
        self.assertEqual([field["name"] for field in schema["fields"]], ["value"])

    def test_duplicate_class_labels_do_not_create_layout_facts(self):
        self.matching_audit()
        self.manifest.append({**self.manifest[0], "address": "00401010"})
        exported = self.run_export()
        self.assertEqual(exported["verified_functions"]["count"], 1)
        self.assertTrue(exported["verified_functions"]["functions"][0]["label_trust"].startswith("MISLABELLED_FAMILY"))
        self.assertEqual(exported["struct_schema"]["count"], 0)

    def test_conflicting_field_declarations_are_not_exported_as_layout(self):
        self.matching_audit()
        other = self.source_dir / "Example_Set_00401010.cpp"
        other.write_text("#pragma pack(push,1)\nstruct Example { float other; };", encoding="utf-8")
        self.manifest.append({**self.manifest[0], "address": "00401010", "name": "Set", "compiled_source": str(other)})
        self.audit.append({"address": "00401010", "verdict": "EXACT", "file": other.name})
        exported = self.run_export()
        self.assertEqual(exported["verified_functions"]["count"], 2)
        self.assertEqual(exported["struct_schema"]["count"], 0)

    def test_class_size_must_cover_the_complete_field_extent(self):
        self.matching_audit()
        self.source.unlink()
        self.source = self.source_dir / "Example_GetSizeofClass_00401000.cpp"
        self.source.write_text(
            "#pragma pack(push,1)\nstruct Example { char pad[4]; int value; };\n"
            "int GetSizeofClass() { return 4; }\n", encoding="utf-8")
        self.manifest[0].update(name="GetSizeofClass", compiled_source=str(self.source))
        self.matching_audit()
        self.assertEqual(self.run_export()["struct_schema"]["count"], 0)

    def test_binding_signature_comes_from_the_selected_manifest_row(self):
        self.matching_audit()
        self.write_inputs()
        self.write_table("rebuild/backlog/forgefse-binding-queue.tsv",
                         ["address", "calling_convention", "return_type"],
                         [{"address": "00401000", "calling_convention": "wrong", "return_type": "wrong"}])
        binding = exporter.build_exports(self.root)["forgefse_verified_bindings.json"]["bindings"][0]
        self.assertEqual(binding["calling_convention"], "__thiscall")
        self.assertEqual(binding["return_type"], "int")

    def test_missing_required_audit_does_not_replace_existing_outputs(self):
        self.write_inputs()
        (self.root / "rebuild/compile-gate/parity_audit.tsv").unlink()
        output = self.root / "rebuild/sdk/verified_functions.json"
        output.parent.mkdir()
        output.write_text("previous output", encoding="utf-8")
        self.assertEqual(exporter.main(["--root", str(self.root)]), 1)
        self.assertEqual(output.read_text(encoding="utf-8"), "previous output")

    def test_root_is_explicit_and_mirroring_is_opt_in(self):
        self.matching_audit()
        self.write_inputs()
        with patch.object(exporter.shutil, "copy2") as copy:
            self.assertEqual(exporter.main(["--root", str(self.root)]), 0)
            copy.assert_not_called()
        destination = self.root / "explicit-mirror"
        destination.mkdir()
        self.assertEqual(exporter.main(["--root", str(self.root), "--mirror-dir", str(destination)]), 0)
        for name in ("verified_functions", "struct_schema", "forgefse_verified_bindings"):
            self.assertEqual((destination / (name + ".json")).read_bytes(),
                             (self.root / "rebuild/sdk" / (name + ".json")).read_bytes())

    def test_default_root_is_the_scripts_checkout(self):
        self.assertEqual(exporter.ROOT, Path(exporter.__file__).resolve().parents[2])

    def test_source_changes_during_inference_leave_existing_outputs_untouched(self):
        self.matching_audit()
        self.write_inputs()
        sdk = self.root / "rebuild/sdk"
        sdk.mkdir()
        output = sdk / "verified_functions.json"
        output.write_text("previous output", encoding="utf-8")
        original = exporter.class_struct.facts_for

        def changed_source(*args, **kwargs):
            self.source.write_text(self.source.read_text(encoding="utf-8") + "\n// changed\n", encoding="utf-8")
            return original(*args, **kwargs)

        with patch.object(exporter.class_struct, "facts_for", side_effect=changed_source):
            self.assertEqual(exporter.main(["--root", str(self.root)]), 1)
        self.assertEqual(output.read_text(encoding="utf-8"), "previous output")

    def test_missing_mirror_directory_is_rejected_before_export(self):
        self.matching_audit()
        self.write_inputs()
        with self.assertRaises(SystemExit) as raised:
            exporter.main(["--root", str(self.root), "--mirror-dir", str(self.root / "missing")])
        self.assertEqual(raised.exception.code, 2)
        self.assertFalse((self.root / "rebuild/sdk").exists())
