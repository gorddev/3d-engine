#pragma once
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <charconv>

/** @brief Throws an exception for str_view errors */
static void fstr_view_throw_err(const char err[]) {
    throw std::runtime_error(err);
}

namespace gan {

// **************************************
// Construction
// **************************************
/** @brief Construct a str_view referencing an existing buffer
 *  @param data Pointer to the buffer
 *  @param len Reference to the current length
 *  @param cap Maximum capacity of the buffer (excluding null terminator)
 */
inline str_view::str_view(char* data, uint32_t& len, uint32_t cap)
    : data(data), len(len), cap(cap) {}

// **************************************
// Operations
// **************************************

constexpr uint32_t str_view::length() const { return len; }
constexpr uint32_t str_view::capacity() const { return cap; }

/** @brief Get C-style null-terminated string */
constexpr const char* str_view::c_str() const {
    if (len < cap) data[len] = '\0';
    return data;
}

/** @brief Get a subview of the string */
inline str_subview str_view::subview(uint32_t start, uint32_t sub_len) const {
    if (start + sub_len > len)
        fstr_view_throw_err("str_view::subview, out-of-bounds access");
    return {data + start, sub_len};
}

// **************************************
// Element Access
// **************************************
inline char& str_view::operator[](uint32_t index) const {
    if (index >= len)
        fstr_view_throw_err("str_view::operator[], access out of bounds memory.");
    return data[index];
}

// **************************************
// modification
// ***********************************
template <uint32_t N>
inline void str_view::emplace(uint32_t index, const char(&str)[N]) {
    if (index + (N - 1) > cap)
        fstr_view_throw_err("str_view::emplace, out-of-bounds memory");
    std::memcpy(data + index, str, N - 1);
    if (index + (N - 1) > len) len = index + (N - 1);
    data[len] = '\0';
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void str_view::emplace(const uint32_t index, const char* str, const uint32_t count) {
    if (!str) return;
    if (index + count > cap)
        fstr_view_throw_err("str_view::emplace, out-of-bounds memory");
    std::memcpy(data + index, str, count);
    if (index + count > len) len = index + count;
    data[len] = '\0';
}

inline int str_view::find(char c, uint32_t pos) const {
    while (pos < len) {
        if (data[pos] == c)
            return pos;
        pos++;
    }
    return -1;
}

// **************************************
// Equality tests
// **************************************
template<uint32_t N>
inline bool str_view::operator==(const char(&str)[N]) {
    if (len != N - 1) return false;
    return std::memcmp(data, str, len) == 0;
}

inline bool str_view::operator==(const char* str) const {
    for (uint32_t i = 0; i < len; ++i) {
        if (data[i] != str[i]) return false;
    }
    return str[len] == '\0';
}

// **************************************
// Append / stream operators
// **************************************
template<uint32_t N>
inline str_view& str_view::operator<<(const char(&str)[N]) {
    const uint32_t bytes = std::min<uint32_t>(N - 1, cap - len);
    std::memcpy(data + len, str, bytes);
    len += bytes;
    data[len] = '\0';
    return *this;
}

inline str_view& str_view::operator<<(const char* str) {
    if (!str) return (*this) << "nullptr";
    const uint32_t bytes = std::min<uint32_t>(std::strlen(str), cap - len);
    std::memcpy(data + len, str, bytes);
    len += bytes;
    data[len] = '\0';
    return *this;
}

inline str_view& str_view::operator<<(char c) {
    if (len < cap) data[len++] = c;
    data[len] = '\0';
    return *this;
}

inline str_view& str_view::operator<<(int num) {
    auto* start = data + len;
    auto* end = data + cap;
    auto res = std::to_chars(start, end, num);
    len += res.ptr - start;
    data[len] = '\0';
    return *this;
}

inline str_view& str_view::operator<<(float num) {
    auto* start = data + len;
    auto* end = data + cap;
    auto res = std::to_chars(start, end - 1, num, std::chars_format::fixed, prec);
    len += res.ptr - start;
    data[len] = '\0';
    return *this;
}

template<uint8_t P>
inline str_view& str_view::operator<<(precision<P>) {
    prec = P;
    return *this;
}

inline str_view& str_view::operator<<(bool b) {
    return *this << (b ? "true" : "false");
}

inline str_view& str_view::operator<<(const void* ptr) {
    if (!ptr) return (*this) << "nullptr";
    auto addr = reinterpret_cast<uintptr_t>(ptr);
    *this << "0x";
    auto* start = data + len;
    auto* end = data + cap;
    auto res = std::to_chars(start, end - 1, addr, 16);
    len += res.ptr - start;
    data[len] = '\0';
    return *this;
}

}