#pragma once

#include <concepts>
#include "str_subview.hpp"

namespace gan {
    /** Allows for memoryless setting of the precision parameter of an fstring **/
    template<uint8_t C>
    struct precision {
        static_assert(C <= 10, "Maximum float precision is 10 digits.");
        precision<C>() = default;
    };

    /** Allows you to send an fstring along a buffer without template information. Very helpful**/
    class str_view final {
    private:
        friend str_view;
        /// Contains the data of the original fstring
        char* data;
        /// Contains the length of the original fstring
        uint32_t& len;
        /// Contains the capacity of the original fstring
        uint32_t cap;
        /// Contains precision information
        uint8_t prec;

    public:
        /// public constructor for str_view
        str_view(char* data, uint32_t& len, uint32_t cap);

        // **************
        // Utility functions
        // **************
        // <><><> Getters <><><>
        /// Returns the current length of the parent fstring
        [[nodiscard]] constexpr uint32_t length() const;
        /// Returns the capacity of the parent fstring
        [[nodiscard]] constexpr uint32_t capacity() const;
        /// Returns the data containing the parent fstring
        [[nodiscard]] constexpr const char* c_str() const;
        /// Createas a subview of the current view
        inline str_subview subview(uint32_t start, uint32_t sub_len) const;
        /// Emplaces a text at a position within the parent fstring.
        template <uint32_t N>
        void emplace(uint32_t index, const char (&str)[N]);
        ///  Emplaces a text at a position within the parent fstring.
        void emplace(uint32_t index, const char* str, uint32_t count);
        /// Finds a specific character starting from the provided position
        int find(char c, uint32_t pos = 0) const;

        // **************
        // Operator overloads
        // **************

        // <><><> Equality <><><>
        /// Returns the character at the requested position.
        char& operator[](uint32_t index) const;
        /// Checks if an fstring and a predefined const character are equal
        template<uint32_t N>
        bool operator==(const char (&str)[N]);
        /// Checks if an fst_view and a const char of undefined length are equal
        bool operator==(const char* str) const;

        // <><><> Piping Into String <><><>
        /// Adds a constant character with a literal into the parent fstring
        template<uint32_t N>
        str_view& operator<<(const char (&str)[N]);
        /// Adds a constant character of unknown length into the current fstring
        str_view& operator<<(const char* str);
        /// Adds an integer into the current fstring
        str_view& operator<<(int num);
        /// Converts from generic size to int
        str_view& operator<<(size_t num) { return *this << static_cast<int>(num); }
        /// Converts from generic type to int
        template<std::integral K>
        str_view& operator<<(K integer) { return *this << static_cast<int>(integer); }
        /// Lets us set the precision for floats
        template<uint8_t P>
        str_view& operator<<(precision<P> precision);
        /// Lets us accept floats as well
        str_view& operator<<(float num);
        /// Allows for doules as well
        str_view& operator<<(double num) { return *this << static_cast<float>(num); }
        /// Also characters, because why not
        str_view& operator<<(char c);
        /// Hexidecimals as well
        str_view& operator<<(const void*);
        /// Allows you to put in another str_view
        str_view& operator<<(const str_view &other);
        /// Also allows for booleans
        str_view& operator<<(bool b);

    public:

    };
}

#include "str_view.inl"
