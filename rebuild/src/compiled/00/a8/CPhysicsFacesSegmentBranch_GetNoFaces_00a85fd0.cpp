// CPhysicsFacesSegmentBranch::GetNoFaces  @ 0x00a85fd0
// long __fastcall (this)
// Sums GetNoFaces() (virtual slot 2, vtable+8) of two child segment pointers.

struct CPhysicsFacesSegmentBase
{
	virtual void unk0();
	virtual void unk1();
	virtual long GetNoFaces();   // slot 2 -> [vtbl+8]
};

struct CPhysicsFacesSegmentBranch
{
	char pad[0x14];
	CPhysicsFacesSegmentBase* m14;  // +0x14
	CPhysicsFacesSegmentBase* m18;  // +0x18

	long GetNoFaces();
};

long CPhysicsFacesSegmentBranch::GetNoFaces()
{
	return this->m14->GetNoFaces() + this->m18->GetNoFaces();
}