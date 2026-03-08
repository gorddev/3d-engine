#pragma once
#include <algorithm>
#include <concepts>
#include "../str_view/str_view.hpp"


namespace gan {

    /** @brief @code fstring@endcode is a custom string class that is much faster for comparison than std::string, but cannot be resized.
     * Create an fstring with @code fstring<size> varName@endcode.
     * @details @code fstring@endcode only has two member variables: @code uint_16 len@endcode and @code char arr[len]@endcode.
     * Note that this means the maximum length for any fstring is 65534 characters (+1 for null terminator)
     */
    template<uint32_t C>
    class fstring {
        static_assert(C >= 2 && C <= 65533, "fstring capacity must be at least 2, and less than 65533");
    private:
        /// Current length of the fstring
        uint32_t len;
        /// Array containing the information of the fstring
        char arr[C + 1];
        /// Current precision of the array
        uint8_t prec = 10;

    public:
        // **************
        // Constructors
        // **************
        /// Constructor for constant characters
        template<uint16_t N>
        constexpr fstring(const char (&str)[N]); // NOLINT(*-explicit-constructor)
        /// Constructor for constant character without length info
        constexpr fstring(const char* str); // NOLINT(*-explicit-constructor)
        /// Constructor for a singular character
        constexpr explicit fstring(char c);
        /// Nothing in the constructor
        constexpr fstring();

        // **************
        // Utility functions
        // **************
        // <><><> Getters <><><>
        /// Returns the current length of the fstring
        [[nodiscard]] constexpr uint32_t length() const;
        /// Returns the capacity of the fstring
        [[nodiscard]] static constexpr uint32_t capacity();
        /// Returns the data containing the fstring
        [[nodiscard]] const char* c_str();
        /// Returns true if the fstring is empty
        [[nodiscard]] bool empty() const;
        // <><><> Modifiers <><><>
        /// Clears out all the data in the fstring
        void clear();
        /// Pops the entire fstring, setting length to 0 and returning a copy
        fstring<C> pop();
        /// Wraps up a view for the current fstring
        gan::str_view wrap();

        // <><><> Emplacing <><><>
        /// Emplace text at a position within the string
        template<uint32_t N>
        void emplace(uint32_t index, const char (&str)[N]) requires(index + N - 1 <= C);
        /// Does the same but with a specified count
        void emplace(uint32_t index, const char *str,uint32_t count)requires(index + count <= C);

        /// Gets the substring with @code <N>@endcode elements at index @code uint32_t index@endcode
        /// @return @code fstring<N>@endcode, an fstring with @code N@endcode elements.
        template<uint32_t N>
        [[nodiscard]] constexpr fstring<N> substr(uint32_t index) const;

        // **************
        // Operator overloads
        // **************

        // <><><> Equality <><><>
        /// Returns the character at the requested position.
        constexpr char& operator[](uint32_t index);
        /// Checks if an fstring and a predefined const character array are equal
        template<uint32_t N>
        bool operator==(const char (&str)[N]) const;
        /// Checks if an fstring and a const char of undefined length are equal
        bool operator==(const char* str) const;

        // <><><> Piping Into String <><><>
        /// Adds another fstring into the current fstring
        template<uint32_t N>
        fstring<C>& operator<<(fstring<N>& other);
        /// Adds a constant character with a literal into the fstring
        template<uint32_t N>
        fstring<C>& operator<<(const char (&str)[N]);
        /// Adds a constant character of unknown length into the current fstring
        fstring<C>& operator<<(const char* str);

        // <><><> Specific types <><><>
        /// Adds an integer into the current fstring
        fstring<C>& operator<<(int num);
        /// Adds doubles into the current fstring. Static casts to float first.
        fstring<C>& operator<<(double num) { return *this << static_cast<float>(num); }
        /// Lets us set the precision for floats
        template<uint8_t P>
        fstring<C>& operator<<(precision<P> precision);
        /// Lets us accept floats as well
        fstring<C>& operator<<(float num);
        /// Lets us take unsigned integers
        template<typename T>
            requires(std::is_unsigned_v<T>)
        fstring<C>& operator<<(T num) { return *this << static_cast<int>(num); }
        /// Also characters, because why not
        fstring<C>& operator<<(char c);
        /// Hexadecimals as well
        fstring<C>& operator<<(const void*);
        /// Also boolean reading
        fstring<C>& operator<<(bool b);
    };

}

#include "fstring.inl"
