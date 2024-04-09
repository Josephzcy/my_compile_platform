#ifndef COMMON_BASE_STRING_ALGORITHM_HPP
#define COMMON_BASE_STRING_ALGORITHM_HPP

#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <vector>
#include <string>

#include "common/base/stdint.h"
#include "common/string/string_piece.h"

inline bool IsOctDigit(char c)
{ return c >= '0' && c <= '7'; }
inline bool IsCharInString(char c, const StringPiece& str)
{
    size_t len = str.size();
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] == c) return true;
    }
    return false;
}

inline bool StringStartsWith(const StringPiece& str, const StringPiece& prefix)
{
    return str.starts_with(prefix);
}

bool StringStartsWithIgnoreCase(const StringPiece& str, const StringPiece& prefix);

inline std::string StringRemovePrefix(const StringPiece& str, const StringPiece& prefix)
{
    if (StringStartsWith(str, prefix))
    {
        return str.substr(prefix.size()).as_string();
    }
    return str.as_string();
}

inline bool StringEndsWith(const StringPiece& str, const StringPiece& suffix)
{
    return str.ends_with(suffix);
}

inline std::string StringRemoveSuffix(const StringPiece& str, const StringPiece& suffix)
{
    if (StringEndsWith(str, suffix))
    {
        return str.substr(0, str.size() - suffix.size()).as_string();
    }
    return str.as_string();
}

std::string ReplaceString(const StringPiece& s, const StringPiece& from, const StringPiece& to);
std::string ReplaceAll(const StringPiece& s, const StringPiece& from, const StringPiece& to);
size_t ReplaceAllChars(std::string* s, const StringPiece& from, char to);
std::string ReplaceAllChars(const StringPiece& s, const StringPiece& from, char to);

std::string StripString(const StringPiece& s, const char* remove, char replacewith);
inline std::string StripString(const StringPiece& s, const std::string& remove, char replacewith)
{
    return StripString(s, remove.c_str(), replacewith);
}

void StripString(std::string* s, const char* remove, char replacewith);
inline void StripString(std::string* s, const std::string& remove, char replacewith)
{
    return StripString(s, remove.c_str(), replacewith);
}

std::string RemoveContinuousBlank(const StringPiece& str);
void RemoveContinuousBlank(std::string* str);

//    These methods concatenate a vector of strings into a C++ string, using
//    the C-string "delim" as a separator between components.
//
//    There are two flavors of the function, one flavor returns the concatenated string,
//    another takes a pointer to the target string. In the latter case the
//    target string is cleared and overwritten.
//
//    There are also interfaces for STL-like iterator range, the typical usage is to join
//    repeated string field in protocol buffer message.
//    See also: thirdparty/protobuf/repeated_field.h
std::string JoinStrings(const std::vector<std::string>& components, const StringPiece& delim);
void JoinStrings(const std::vector<std::string>& components, const StringPiece& delim, std::string* res);

template <class InputIterator>
void JoinStrings(InputIterator begin_iter,
                 InputIterator end_iter,
                 const StringPiece& delim,
                 std::string* result) {
    for (InputIterator iter = begin_iter; iter != end_iter; ++iter)
    {
        if (iter != begin_iter)
        {
            result->append(delim.data(), delim.length());
        }
        result->append(iter->data(), iter->size());
    }
}

template <class InputIterator>
std::string JoinStrings(
  InputIterator begin_iter, InputIterator end_iter, const StringPiece& delim) {
    std::string result;
    JoinStrings<InputIterator>(begin_iter, end_iter, delim, &result);
    return result;
}

// Split a string using a character delimiter. fill the components into
// 'result'.  If there are consecutive delimiters, this function skips over all
// of them.
// Note: For multi-character delimiters, this routine will split on *ANY* of
// the characters in the string, not the entire string as a single delimiter.
// So it's NOT the reverse function of JoinStrings.
void SplitStringByAnyOf(const StringPiece& full, const char* delim, std::vector<std::string>* res);

// The 'delim' is a delimiter string, it's the reverse function of JoinStrings.
void SplitString(const StringPiece& full,
                            const char* delim,
                            std::vector<std::string>* result);

void SplitStringKeepEmpty(
    const StringPiece& full,
    char delim,
    std::vector<std::string>* result
);

void SplitStringKeepEmpty(
    const StringPiece& full,
    const StringPiece& delim,
    std::vector<std::string>* result
);

std::string StringTrimLeft(const StringPiece& str, const StringPiece& trim_value);
std::string StringTrimLeft(const StringPiece& str);
std::string StringTrimRight(const StringPiece& str, const StringPiece& trim_value);
std::string StringTrimRight(const StringPiece& str);
std::string StringTrim(const StringPiece& str, const StringPiece& trim_value);
std::string StringTrim(const StringPiece& str);

void StringTrimLeft(std::string* str, const StringPiece& trim_value);
void StringTrimLeft(std::string* str);
void StringTrimRight(std::string* str, const StringPiece& trim_value);
void StringTrimRight(std::string* str);
void StringTrim(std::string* str, const StringPiece& trim_value);
void StringTrim(std::string* str);

std::string RemoveSubString(
    const StringPiece& str,
    const StringPiece& sub,
    bool fill_blank = false
);

std::string RemoveAllSubStrings(
    const StringPiece& str,
    const StringPiece& sub,
    bool fill_blank = false
);

int CEscapeString(const char* src, int src_len, char* dest, int dest_len);
std::string CEscapeString(const StringPiece& src);

inline void StringToUpper(std::string* s)
{
    std::string::iterator end = s->end();
    for (std::string::iterator i = s->begin(); i != end; ++i)
        *i = toupper(static_cast<unsigned char>(*i));
}

inline void StringToLower(std::string* s)
{
    std::string::iterator end = s->end();
    for (std::string::iterator i = s->begin(); i != end; ++i)
        *i = tolower(static_cast<unsigned char>(*i));
}

inline std::string UpperString(const StringPiece& s)
{
    std::string res = s.as_string();
    StringToUpper(&res);
    return res;
}

inline std::string LowerString(const StringPiece& s)
{
    std::string res = s.as_string();
    StringToLower(&res);
    return res;
}

#endif // COMMON_BASE_STRING_ALGORITHM_HPP
