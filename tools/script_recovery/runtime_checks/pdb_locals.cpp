// Read original function-local symbols through DIA without registering a COM server.
#include <windows.h>
#include <dia2.h>
#include <atlbase.h>
#include <iostream>
#include <string>

static void require(HRESULT result, const char* operation) {
    if (FAILED(result)) {
        std::cerr << operation << " failed: 0x" << std::hex << result << '\n';
        throw result;
    }
}

static std::wstring nameOf(IDiaSymbol* symbol) {
    CComBSTR name;
    if (symbol && symbol->get_name(&name) == S_OK && name) return std::wstring(name, name.Length());
    return L"";
}

static void dump(IDiaSymbol* scope, unsigned depth) {
    CComPtr<IDiaEnumSymbols> symbols;
    require(scope->findChildren(SymTagNull, nullptr, nsNone, &symbols), "findChildren");
    CComPtr<IDiaSymbol> child;
    ULONG fetched = 0;
    while (symbols->Next(1, &child, &fetched) == S_OK && fetched) {
        DWORD tag = 0, location = 0, dataKind = 0, reg = 0, rva = 0, base = 0;
        LONG offset = 0;
        ULONGLONG length = 0;
        child->get_symTag(&tag);
        child->get_locationType(&location);
        child->get_dataKind(&dataKind);
        child->get_registerId(&reg);
        child->get_offset(&offset);
        child->get_relativeVirtualAddress(&rva);
        CComPtr<IDiaSymbol> type;
        child->get_type(&type);
        if (type) { type->get_length(&length); type->get_baseType(&base); }
        if (tag == SymTagBlock) child->get_length(&length);
        if (tag == SymTagData || tag == SymTagBlock)
            std::wcout << depth << L'\t' << tag << L'\t' << nameOf(child) << L'\t'
                   << nameOf(type) << L'\t' << base << L'\t' << length << L'\t'
                   << dataKind << L'\t' << location << L'\t' << reg << L'\t'
                   << offset << L'\t' << rva << L'\n';
        if (tag == SymTagBlock) dump(child, depth + 1);
        child.Release();
    }
}

int wmain(int argc, wchar_t** argv) {
    if (argc != 4) { std::cerr << "usage: pdb-locals DIA_DLL PDB FUNCTION\n"; return 2; }
    try {
        require(CoInitializeEx(nullptr, COINIT_MULTITHREADED), "CoInitializeEx");
        HMODULE library = LoadLibraryW(argv[1]);
        if (!library) throw HRESULT_FROM_WIN32(GetLastError());
        auto getClass = reinterpret_cast<HRESULT (STDAPICALLTYPE *)(REFCLSID, REFIID, LPVOID*)>(
            GetProcAddress(library, "DllGetClassObject"));
        if (!getClass) throw E_NOINTERFACE;
        CComPtr<IClassFactory> factory;
        require(getClass(__uuidof(DiaSource), IID_IClassFactory, reinterpret_cast<void**>(&factory)), "class factory");
        CComPtr<IDiaDataSource> source;
        require(factory->CreateInstance(nullptr, __uuidof(IDiaDataSource), reinterpret_cast<void**>(&source)), "DIA source");
        require(source->loadDataFromPdb(argv[2]), "load PDB");
        CComPtr<IDiaSession> session;
        require(source->openSession(&session), "open session");
        CComPtr<IDiaSymbol> global;
        require(session->get_globalScope(&global), "global scope");
        CComPtr<IDiaEnumSymbols> matches;
        const DWORD flags = nsfCaseSensitive | (wcschr(argv[3], L'*') ? nsfRegularExpression : 0);
        require(global->findChildren(SymTagFunction, argv[3], flags, &matches), "find function");
        CComPtr<IDiaSymbol> function;
        ULONG fetched = 0;
        unsigned count = 0;
        std::wcout << L"depth\ttag\tname\ttype\tbaseType\tsize\tdataKind\tlocation\tregister\toffset\trva\n";
        while (matches->Next(1, &function, &fetched) == S_OK && fetched) {
            DWORD rva = 0;
            ULONGLONG length = 0;
            function->get_relativeVirtualAddress(&rva);
            function->get_length(&length);
            std::wcout << L"FUNCTION\t" << nameOf(function) << L'\t' << rva << L'\t' << length << L'\n';
            dump(function, 0);
            function.Release();
            ++count;
        }
        std::wcout << L"MATCHES\t" << count << L'\n';
        return count ? 0 : 3;
    } catch (HRESULT error) {
        std::cerr << "DIA query failed: 0x" << std::hex << error << '\n';
        return 1;
    }
}
