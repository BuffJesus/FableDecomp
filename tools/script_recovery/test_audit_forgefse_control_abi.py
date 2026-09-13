import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_control_abi import audit


GOOD_SOURCE = r'''
bool LuaEntityAPI::AcquireControl(CScriptThing* pMe, sol::optional<int> priority) {
  if (m_pControlHandle) {
    int& depthSlot = m_ownedControlDepth[entityData];
    if (heldPriority != end && heldPriority->second == requestedPriority && depthSlot > 0) {
      return true;
    }
    const int depth = ++depthSlot;
    return true;
  }
  else if (entityData) {
    auto live = g_controlHandlesByEntityData.find(pMe->pImp.Data);
    m_borrowedControlHandles[pMe->pImp.Data] = live->second;
  }
  auto scriptPriority = priority.value_or(5);
  while (true) {
    if (StartScriptingEntity_API(m_pGameInterface, pMe, m_pControlHandle, scriptPriority)) {
      m_ownedControlDepth[m_pControlledEntityData] = 0;
      return true;
    }
    NewScriptFrame_API(m_pGameInterface);
    if (IsThreadTerminating()) {
      CSGROSTB_Destroy_API(m_pControlHandle);
      Game_free(m_pControlHandle);
      return false;
    }
  }
}
void LuaEntityAPI::ReleaseControl(CScriptThing* pMe) {
  auto borrowed = m_borrowedControlHandles.find(key);
  if (borrowed != end) {
    LogToFile("Borrowed control handle released without destroying its owner's resource.");
    return;
  }
  auto depth = m_ownedControlDepth.find(m_pControlledEntityData);
  if (depth != end && depth->second > 0) {
    --depth->second;
    return;
  }
  m_ownedControlDepth.erase(releasedEntityData);
  CSGROSTB_Destroy_API(m_pControlHandle);
  Game_free(m_pControlHandle);
}
void LuaEntityAPI::MakeBehavioral(CScriptThing* pMe) {}
'''


class ControlAbiAuditTests(unittest.TestCase):
    def write_tree(self, root: Path, source: str) -> None:
        folder = root / "FableScriptExtender"
        folder.mkdir()
        (folder / "LuaEntityAPI.cpp").write_text(source, encoding="utf-8")
        (folder / "LuaManager.cpp").write_text(
            'cscriptThing_type["AcquireControl"] = x;\n'
            'cscriptThing_type["ReleaseControl"] = y;\n', encoding="utf-8")

    def audit_source(self, source: str) -> dict:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self.write_tree(root, source)
            return audit(root)

    def test_accepts_required_control_lifecycle(self):
        result = self.audit_source(GOOD_SOURCE)
        self.assertTrue(result["ok"], result["checks"])

    def test_rejects_borrowed_handle_destruction(self):
        bad = GOOD_SOURCE.replace(
            'LogToFile("Borrowed control handle released without destroying its owner\'s resource.");\n    return;',
            'CSGROSTB_Destroy_API(m_pControlHandle);\n    return;')
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["borrowReleasePreservesOwner"])

    def test_rejects_second_scheduler_resource_for_owned_actor(self):
        # The 2026-09-11 hang: suspending the owned handle and requesting a second resource.
        bad = GOOD_SOURCE.replace(
            "    const int depth = ++depthSlot;\n    return true;\n",
            "    m_suspendedControlHandles[entityData].push_back(m_pControlHandle);\n")
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["nestedAcquireReusesOwnedHandle"])

    def test_rejects_borrow_attempt_over_owned_handle(self):
        bad = GOOD_SOURCE.replace("  else if (entityData) {", "  if (entityData) {")
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["nestedAcquireSkipsSelfBorrow"])

    def test_rejects_release_that_destroys_outer_handle_while_nested(self):
        # The Affair Man walk-home failure: conversation release destroyed the Main handle.
        bad = GOOD_SOURCE.replace(
            "  if (depth != end && depth->second > 0) {\n    --depth->second;\n    return;\n  }\n", "")
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["nestedReleaseRetainsOuterHandle"])

    def test_rejects_unbounded_depth_on_same_priority_reacquire(self):
        # v11 regression: per-frame Main-loop re-acquires inflated depth into the hundreds.
        bad = GOOD_SOURCE.replace(
            "    if (heldPriority != end && heldPriority->second == requestedPriority && depthSlot > 0) {\n"
            "      return true;\n    }\n", "")
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["samePriorityReacquireSaturates"])

    def test_rejects_fresh_acquire_without_depth_reset(self):
        bad = GOOD_SOURCE.replace("      m_ownedControlDepth[m_pControlledEntityData] = 0;\n", "")
        result = self.audit_source(bad)
        self.assertFalse(result["ok"])
        self.assertFalse(result["checks"]["freshAcquireResetsDepth"])


if __name__ == "__main__":
    unittest.main()
