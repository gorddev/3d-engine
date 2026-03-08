#pragma once

#include "types/strings/fstring/fstring.hpp"
#include <OpenGL.h>
#include <type_traits>

// made by gordie feb 20th

namespace gan {

    /// Holds the error buffer and number of functions called before the program is meant to error.
    /// Contains an internal "call_chain" num (number of functions called) and a character buffer of size 1024.
    class internal_error_handler {
    public:
        constexpr static uint16_t buff_size = 1024;
    private:
        inline static fstring<buff_size> error_buffer;   ///< "Fixed String" of size 1024 that contains the errors.
        inline static uint16_t call_chain;          ///< Number of functions called.

        // friends
        friend inline const char* GAN_GetError();           //
        friend inline void GAN_ClearError();                //
        friend inline fstring<buff_size>& GAN_RawAppendError();  //
        // friends


        static const char* getError_c_str() {
            return error_buffer.c_str();
        }

        static fstring<buff_size>& get_raw_buffer() {
            return error_buffer;
        }

    public:

        // deleted constructors
        internal_error_handler(const internal_error_handler&) = delete;             //
        internal_error_handler& operator=(const internal_error_handler&) = delete;  //
        internal_error_handler(const internal_error_handler&&) = delete;            //
        internal_error_handler& operator=(const internal_error_handler&&) = delete; //
        // deleted constructors

        static void write_error(const char err[]) {
            error_buffer = err;
        }

        static uint16_t get_call_chain_count() {
            return call_chain;
        }

        static void increment_call_chain_count() {
            call_chain++;
        }

        // ReSharper disable once CppDFAConstantFunctionResult
        static bool has_error() {
            return !error_buffer.empty();
        }

        static void clear_call_chain_count() {
            call_chain = 0;
        }
    };

    /// Clears the current error in the error buffer & resets the call chain.
    inline void GAN_ClearError() {
        internal_error_handler::error_buffer.clear();
        internal_error_handler::call_chain = 0;
    }

    /// Gets the raw underlying fstring of the error buffer.
    inline fstring<internal_error_handler::buff_size>& GAN_RawAppendError() {
        return internal_error_handler::get_raw_buffer();
    }

    /// Appends an error to the current buffer, increasing the call chain count by one.
    template<typename... Args>
    void GAN_AppendError(const char src[], Args&&... args) {
        auto& buffer = GAN_RawAppendError();
        auto chain = internal_error_handler::get_call_chain_count();
        if (chain) {
            buffer << "\n::+" << chain << " \t=> {" << src << "}\n";
        }
        else {
            buffer << "::src" << " \t=> {" << src << "}\n";
        }
        (..., (buffer << args));
        internal_error_handler::increment_call_chain_count();
    }

    template<typename T>
    concept GAN_can_error =
        std::is_convertible_v<T, const char*>
        || std::is_fundamental_v<T>
        || std::is_convertible_v<T, unsigned>
        || std::is_unsigned_v<T>;
    /// Writes an error to the error buffer, overwriting the current error & resetting the call chain count.
    template<typename... Args>
        requires (GAN_can_error<Args> && ...)
    void GAN_WriteError(const char src[], Args&&... args) {
        internal_error_handler::clear_call_chain_count();

        internal_error_handler::write_error("::src \t=> {");

        GAN_RawAppendError() << src << "}\n";
        (..., (GAN_RawAppendError() << args));
        GAN_RawAppendError() << "\n";

        internal_error_handler::increment_call_chain_count();
    }

    /// Gets the current error stored in the error buffer. If no error is stored, checks OpenGL for an error.
    /// Otherwise returns blank.
    inline const char* GAN_GetError() {
        if (internal_error_handler::has_error()) {
            return internal_error_handler::getError_c_str();
        }

        auto err = glGetError();
        if (err) {
            GAN_WriteError("OpenGL::GL", "Open GL with error code: ", glGetError());
            return internal_error_handler::getError_c_str();
        }
        return "";
    }
}