#pragma once
// Ego_r.pdb layout (0x1C); trivially destructible.
class CDateAndTime
{
public:
    // FableWin ??0CDateAndTime@@QAE@XZ, retail 0x009E9AB0 (zeroes all seven
    // fields; out of line: CDiskFileWin32::GetFileInformation 0x0099A5A0).
    CDateAndTime();
    CDateAndTime(long year, unsigned long month, unsigned long day,
                 unsigned long hour, unsigned long minute,
                 unsigned long second, unsigned long millisecond);
    // FableWin ?Initialise@CDateAndTime@@QAEXABU_FILETIME@@@Z, retail 0x009E9CB0.
    void Initialise(const struct _FILETIME& fileTime);

    explicit CDateAndTime(const struct _FILETIME& fileTime);
    bool operator==(const CDateAndTime& other) const;
    // FableWin ??9/??M/??O CDateAndTime; retail 009E9B20 / 009E9B80 / 009E9BE0.
    bool operator!=(const CDateAndTime& other) const;
    bool operator<(const CDateAndTime& other) const;
    bool operator>(const CDateAndTime& other) const;
    // Ego_r ??P / ??N CDateAndTime (>= and <=); retail 009E9C40 / 009E9E10.
    bool operator>=(const CDateAndTime& other) const;
    bool operator<=(const CDateAndTime& other) const;
    // Ego_r ?Initialise@CDateAndTime@@QAEXABU_SYSTEMTIME@@@Z, retail 009E9C70, and
    // ?Initialise@CDateAndTime@@QAEXABV1@@Z, retail 009E9D10.
    void Initialise(const struct _SYSTEMTIME& systemTime);
    void Initialise(const CDateAndTime& date_and_time);
    // Ego_r ?ToString@CDateAndTime@@QBE?AVCCharString@@XZ, retail 009E9E30.
    class CCharString ToString() const;
    // FableWin ??0CDateAndTime@@QAE@ABU_SYSTEMTIME@@@Z, retail 009E9A30.
    explicit CDateAndTime(const struct _SYSTEMTIME& systemTime);
    // FableWin ?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ, retail 009E9D40.
    struct _FILETIME ToFileTime() const;
    // Ego_r ?IsValid@CDateAndTime@@QBE_NXZ: a zero month marks an unset date.
    bool IsValid() const;
    long Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};
