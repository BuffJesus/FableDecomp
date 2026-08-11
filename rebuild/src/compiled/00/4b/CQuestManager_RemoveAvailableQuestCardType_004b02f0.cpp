// CQuestManager::RemoveAvailableQuestCardType  (retail 0x004b02f0)
class CCharString;

struct SubList {
	// method at member offset +0x58; __fastcall (self in ecx), arg pushed
	void Remove(CCharString *name);
};

struct CQuestManager {
	char pad0[0x58];
	SubList list;          // +0x58 (1 byte)
	char pad1[0x8f - 0x58 - 1];
	unsigned char dirty;   // +0x8f

	void RemoveAvailableQuestCardType(CCharString *name);
};

void CQuestManager::RemoveAvailableQuestCardType(CCharString *name)
{
	this->list.Remove(name);
	this->dirty = 1;
}