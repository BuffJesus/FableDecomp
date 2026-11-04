#pragma once

#include <stddef.h>
#include <stdlib.h>

#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_cbase.h"

// Retail links STLport, whose list is a single pointer to a malloc'd
// circular sentinel node (no size member). Only what retail uses is here.
namespace std
{
    template<class _Tp>
    struct _List_node
    {
        _List_node* _M_next;
        _List_node* _M_prev;
        _Tp _M_data;
    };

    template<class _Tp>
    class list
    {
    public:
        list() : _M_node(0)
        {
            _Node* node = static_cast<_Node*>(malloc(sizeof(_Node)));
            node->_M_next = node;
            node->_M_prev = node;
            _M_node = node;
        }

        list(const list& other)
        {
            _Node* node = static_cast<_Node*>(malloc(sizeof(_Node)));
            node->_M_next = node;
            node->_M_prev = node;
            _M_node = node;
            for (_Node* cur = other._M_node->_M_next; cur != other._M_node; cur = cur->_M_next)
            {
                _Node* copy = static_cast<_Node*>(malloc(sizeof(_Node)));
                copy->_M_data = cur->_M_data;
                copy->_M_next = node;
                copy->_M_prev = node->_M_prev;
                node->_M_prev->_M_next = copy;
                node->_M_prev = copy;
            }
        }

        ~list();

        // STLport's list iterator, as the comment queries walk it
        // (CStringParser::IsPosInComments 0x009BA830).
        class iterator
        {
        public:
            explicit iterator(_List_node<_Tp>* node) : _M_node(node) {}
            _Tp& operator*() const { return _M_node->_M_data; }
            _Tp* operator->() const { return &_M_node->_M_data; }
            iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
            bool operator==(const iterator& other) const { return _M_node == other._M_node; }
            bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
            _List_node<_Tp>* _M_node;
        };
        iterator begin() const { return iterator(_M_node->_M_next); }
        iterator end() const { return iterator(_M_node); }

        void clear()
        {
            _Node* cur = _M_node->_M_next;
            while (cur != _M_node)
            {
                _Node* tmp = cur;
                cur = cur->_M_next;
                free(tmp);
            }
            _M_node->_M_next = _M_node;
            _M_node->_M_prev = _M_node;
        }

    private:
        typedef _List_node<_Tp> _Node;
        _Node* _M_node;
    };

    // Out of class, so not inline: callers reach the COMDAT (0x004046B0).
    template<class _Tp>
    list<_Tp>::~list()
    {
        clear();
        if (_M_node)
        {
            free(_M_node);
        }
    }
}

class CStringParserCommentBoundaries
{
public:
    char* PStart;
    char* PEnd;
};

// engine/CStringParser.h is Ego_r's layout (0x30, 12-byte list); retail's
// STLport list is one pointer, so every later member sits 8 bytes lower.
enum EParsedItemType { PARSED_SYMBOL = 1, PARSED_STRING = 2, PARSED_INTEGER = 3, PARSED_FLOAT = 4 };

class CParsedItem
{
public:
    // Numeric tags confirmed by retail InitAsInteger/InitAsFloat/ReadNextSymbol.
    typedef ::EParsedItemType EParsedItemType;
    static const EParsedItemType SYMBOL = PARSED_SYMBOL;
    static const EParsedItemType STRING = PARSED_STRING;
    static const EParsedItemType INTEGER = PARSED_INTEGER;
    static const EParsedItemType FLOAT = PARSED_FLOAT;
    const CCharString& GetSymbol() const;
    const CCharString& GetString() const;
    long GetInteger() const;
    float GetFloat() const;
    CCharString ConvertToString() const;
    void InitAsInteger(const char* text, unsigned long length);
    void InitAsFloat(const char* text, unsigned long length);
    // FableWin ?InitAsString@CParsedItem@@QAEXABVCCharString@@@Z: inline for callers
    // (definition after the class), out of line at 0x009B9850.
    void InitAsString(const CCharString& text);
    // Ego_r, retail 0x009B96E0 / 0x009B9830.
    void InitAsSymbol(const char* text, unsigned long length);
    void InitAsString(const char* text, unsigned long length);
    EParsedItemType Type;
    union { long Integer; float Float; } Value;
    CCharString String;
};
FABLE_STATIC_ASSERT(sizeof(CParsedItem) == 12);

#ifndef FABLE_PARSED_ITEM_OUT_OF_LINE
inline void CParsedItem::InitAsString(const CCharString& text)
{
    String = text;
    Type = PARSED_STRING;
}
#endif
FABLE_STATIC_ASSERT(offsetof(CParsedItem, String) == 8);

class CStringParser : public CBaseClass
{
public:
    typedef void (FABLE_FASTCALL *ErrorCallbackFunction)(const CCharString& message);
    CStringParser() {}
    CStringParser(const CStringParser& other);
    CCharString ReadAsString();
    CCharString ReadAsSymbol();
    long ReadAsInteger();
    bool ReadNextItem(CParsedItem& item);
    bool PeekNextItem(CParsedItem& item);
    EParsedItemType PeekNextItemType();
    bool NextItemExists();
    long SkipWhitespace();
    long SkipUntilWhitespace();
    CCharString ReadAsStringUntilWhitespace();
    // Ego_r CStringParser readers, retail 0x009BA2D0, 0x009BA390, 0x009BA440, 0x009BA650.
    bool ReadAsStringUntilWhitespace(CParsedItem& item);
    CCharString ReadLineAsString();
    CCharString ReadAsStringUntilWholeString(const CCharString& text);
    float ReadAsFloat();
    // Ego_r ?SkipPastNewline@CStringParser@@QAEJXZ, retail 0x009B9D70.
    long SkipPastNewline();
    // Ego_r ?ReadAsUHUGE@CStringParser@@QAE_KXZ, retail 0x009BA5B0.
    unsigned __int64 ReadAsUHUGE();
    // Ego_r ?CountLinesParsed@CStringParser@@QBEJXZ, retail 0x009B95A0.
    long CountLinesParsed() const;
    // Ego_r, retail 0x009BA1D0.
    bool ReadNextItemAsQuotedString(CCharString& string);

protected:
    void Error(const CCharString& message);
    bool CalcInQuotes(bool seed, const char* start, const char* end) const;
    // Ego_r helpers, retail 0x009B95C0 / 0x009B96A0.
    CCharString ReadStringBetween(const char* start, const char* end);
    bool CharSeperatesString(char c);
    // Ego_r comment queries over Comments, retail 0x009BA830 / 0x009BA860 / 0x009BA890.
    bool IsPosInComments(const char* pos);
    const char* GetNextCommentStart(const char* pos);
    const char* GetNextCommentEnd(const char* pos);
    // FableWin ?MoveStringPos@CStringParser@@IAEXK@Z, retail 0x009B9980: advance,
    // tracking quotes.
    void MoveStringPos(unsigned long count);
    bool ReadNextNumber(CParsedItem& item);
    bool ReadNextString(CParsedItem& item);
    bool ReadNextSymbol(CParsedItem& item);
public:
    CStringParser(
        const CCharString& string,
        ErrorCallbackFunction errorCallback,
        const CCharString& filename,
        long baseLineOffset);
    // Ego_r ??0CStringParser@@QAE@PBDP6IXABVCCharString@@@Z0J@Z, retail 0x00414E30
    // (CConsole::RunScript 0x009EC890 parses the loaded buffer in place).
    CStringParser(
        const char* string,
        ErrorCallbackFunction errorCallback,
        const CCharString& filename,
        long baseLineOffset);
    // Implicit member/base cleanup, as in retail GFMain.

    void Init(
        const char* string,
        ErrorCallbackFunction errorCallback,
        const CCharString& filename,
        long baseLineOffset);

    // Retail 0x009B9940: recompute quote parity before this byte position.
    void SetStringPos(unsigned long position);
    bool SkipUntilString(const CCharString& text);
    bool SkipPastString(const CCharString& text);
    CCharString ReadAsStringUntilString(const CCharString& text);
    // Ego_r ?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z,
    // retail 0x009BA110 (CConsole::RunTextCommand 0x009EC710 splits on ";").
    bool ReadAsStringUntilString(const CCharString& text, CParsedItem& item);
    CCharString ReadAsStringUntilPastString(const CCharString& text);
    bool SkipUntilWholeString(const CCharString& text);
    bool SkipPastWholeString(const CCharString& text);
    bool SkipPastNextItemIfIs(const CCharString& text);

    const char* String;
    unsigned long BaseLineOffset;
    CCharString Filename;
    CCharString TempCharString;
    std::list<CStringParserCommentBoundaries> Comments;
    ErrorCallbackFunction ErrorCallback;
    bool InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};

FABLE_STATIC_ASSERT(sizeof(std::list<CStringParserCommentBoundaries>) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CStringParser, Filename) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CStringParser, TempCharString) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStringParser, Comments) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CStringParser, ErrorCallback) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CStringParser, InQuotes) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CStringParser, Line) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CStringParser) == 0x28);
