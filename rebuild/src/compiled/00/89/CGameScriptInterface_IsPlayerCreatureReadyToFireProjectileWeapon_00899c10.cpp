/* CGameScriptInterface::IsPlayerCreatureReadyToFireProjectileWeapon @ 0x00899c10
 * VC7.1 / __fastcall member modelled as free __fastcall(this in ecx).
 * Signed bool return in al; single out-parameter (float*) => ret 4.
 *
 * Register-alloc fix: retail passes &key to the map lookup as a PUSHED stack
 * argument with the map in ecx (a __fastcall member), NOT via edx (__fastcall).
 * Modelling the lookup as a real __fastcall member of the map struct makes
 * VC7.1 emit  lea eax,[esp+4] / push eax / mov ecx,esi  exactly as retail.
 */

/* --- self / object overlays (only the offsets retail touches) --- */
struct IsPCReady_Self
{
    char   pad00[0x14];
    void*  m_Field14;   /* [this+0x14] fed to the subsystem accessor */
};

struct IsPCReady_Creature
{
    char           pad00[0x30];
    unsigned char  m_Flags30;    /* [+0x30] : bit1 (0x2) must be set        */
    char           pad31[0x91 - 0x31];
    unsigned char  m_Flags91;    /* [+0x91] : bit0 (0x1) must be clear       */
};

/* std-style ordered map living at creature+0x44: begin at +0, end at +4. */
struct IsPCReady_MapNode
{
    int    m_Key;    /* [node+0x00] : projectile-weapon type key */
    void*  m_Value;  /* [node+0x04] : weapon object pointer       */
};

struct IsPCReady_Map
{
    IsPCReady_MapNode* m_Begin;  /* [+0x00] (unused by this fn) */
    IsPCReady_MapNode* m_End;    /* [+0x04] end sentinel        */

    /* __fastcall member: this(=map) in ecx, &key pushed on stack. */
    IsPCReady_MapNode* LowerBound(int* key);   /* call ffb9c9a0 */
};

/* --- engine callees / globals: relocation-masked, declared extern --- */
extern void*  __fastcall IsPCReady_GetSubsystem(void* thisField14);          /* call ffbafd60 */
extern void*  __fastcall IsPCReady_GetCreature(void* subsystem);             /* call ffbee1b0 */
extern float  __fastcall IsPCReady_GetReadiness(void* weapon);               /* call ffeefe50 */
extern const float IsPCReady_ReadyThreshold;                                 /* ds:0x129ba3c  */

bool __fastcall CGameScriptInterface_IsPlayerCreatureReadyToFireProjectileWeapon(
    void* thisptr, unsigned long /*edx*/, float* out)
{
    IsPCReady_Self* self = (IsPCReady_Self*)thisptr;

    void* subsystem = IsPCReady_GetSubsystem(self->m_Field14);
    IsPCReady_Creature* creature = (IsPCReady_Creature*)IsPCReady_GetCreature(subsystem);

    if (creature == 0)
        return false;
    if (creature->m_Flags91 & 0x1)
        return false;
    if (!(creature->m_Flags30 & 0x2))
        return false;

    IsPCReady_Map* map = (IsPCReady_Map*)((char*)creature + 0x44);

    int key = 0x81;
    IsPCReady_MapNode* node = map->LowerBound(&key);
    IsPCReady_MapNode* end = map->m_End;

    if (node != end && node->m_Key <= 0x81)
    {
        /* found: keep node */
    }
    else
    {
        node = end;
    }

    float readiness = IsPCReady_GetReadiness(node->m_Value);

    /* fcom threshold; taken (return false) when readiness <= threshold */
    if (readiness > IsPCReady_ReadyThreshold)
    {
        *out = readiness;
        return true;
    }
    return false;
}