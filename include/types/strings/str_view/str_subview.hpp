#pragma once

#include <cstdint>
#include <cstring>

namespace gan {

    /** Allows you to compare small portions of a str_view **/
    class str_subview {
    private:
        const char* data;
        uint32_t len;

    public:
        constexpr str_subview(const char* d, uint32_t l) : data(d), len(l) {}

        [[nodiscard]] constexpr uint32_t length() const { return len; }
        [[nodiscard]] constexpr const char* c_str() const { return data; }

        template<uint32_t N>
        bool operator==(const char (&str)[N]) const {
            if (len != N - 1) return false;
            return std::memcmp(data, str, len) == 0;
        }

        bool operator==(const char* str) const {
            for (uint32_t i = 0; i < len; ++i) {
                if (data[i] != str[i]) return false;
            }
            return str[len] == '\0';
        }

        bool operator==(const str_subview& other) const {
            if (len != other.len) return false;
            return std::memcmp(data, other.data, len) == 0;
        }
    };
}