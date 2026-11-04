# Retired identity at 009FC440

The two preserved files were a Ghidra-labeled `CTCActivationTrigger::OnKill` placeholder and a test that compiled its own substitute body. The original routine is `CResource::RemoveFromResourceList`, forwarding to `CResourceList::RemoveResource` at 009FC020.

The unchanged canonical inline provider in `CResourceList_CResourceList_009fc570.cpp` matches the original 16 bytes and its child relocation. The permanent native resource-removal gate executes that provider with the actual 80-byte child against an independent model. No new strong definition or helper coverage is counted. Archived files are not active build inputs.
