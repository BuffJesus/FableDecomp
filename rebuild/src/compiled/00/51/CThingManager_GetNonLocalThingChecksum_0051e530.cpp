#include "fable_world.h"
#include "fable_thing_manager.h"

struct CThingChecksumNode
{
    CThingChecksumNode* Next;
    CThingChecksumNode* Previous;
    CThing* Thing;
};

// Retail dispatches the checksum through CThing's vtable slot 23.
class CThingChecksumView
{
public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02();
    virtual void Slot03(); virtual void Slot04(); virtual void Slot05();
    virtual void Slot06(); virtual void Slot07(); virtual void Slot08();
    virtual void Slot09(); virtual void Slot10(); virtual void Slot11();
    virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17();
    virtual void Slot18(); virtual void Slot19(); virtual void Slot20();
    virtual void Slot21(); virtual void Slot22();
    virtual unsigned long GetChecksum() const;
};

unsigned long CThingManager::GetNonLocalThingChecksum() const
{
    CThingChecksumNode* node = (*reinterpret_cast<CThingChecksumNode* const*>(
        reinterpret_cast<const unsigned char*>(this) + 0x18))->Next;
    unsigned long result = 0;
    for (; node != *reinterpret_cast<CThingChecksumNode* const*>(
               reinterpret_cast<const unsigned char*>(this) + 0x18); node = node->Next)
    {
        CThing* thing = node->Thing;
        if (!thing->IsDead())
        {
            result += reinterpret_cast<const CThingChecksumView*>(thing)->GetChecksum();
        }
    }
    return result;
}
