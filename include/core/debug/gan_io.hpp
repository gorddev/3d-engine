#pragma once

#include "gan_err.hpp"
#include <iostream>

namespace gan {


    template<typename MSG_F, typename... MSG_T>
    constexpr void print(MSG_F msg_f, MSG_T&&... msg) {
        std::cout << msg_f;
        if constexpr (sizeof...(MSG_T) > 0)
            print(std::forward<MSG_T>(msg)...);
    }

    constexpr void printerr(const char msg[]) {
        puts("\n-----ERROR-----");
        puts(msg);
        fflush(stdout);
    }

    /// Represents a namespace where runtime errors can be called
    /// by the functions.
    namespace err {
        /// Throws an error from a specific source.
        template<typename... MSGS>
        constexpr void panic(const char src[], MSGS&&... msgs) {
            std::cerr << ("\n--------------\n!!PANIC!! Generated from: [")
                << src << "]\n--------------\n";
            if constexpr (sizeof...(MSGS) > 0)
                GAN_AppendError(src, std::forward<MSGS>(msgs)...);
            std::cerr << GAN_GetError() << std::endl;
            GAN_ClearError();
            throw std::runtime_error("");
        }
        /// Unwraps an optional. If unsuccessful, throws.
        /// @param src The source of the error
        /// @param reason The reason for calling unwrap.
        template<typename T, typename... Args>
        constexpr T unwrap(std::optional<T>&& opt, const char* src, Args&&... reason) {
            if (!opt) {
                if constexpr (sizeof...(Args) == 0)
                    GAN_AppendError(src, "Found std::nullopt when unwrapping with err::unwrap(optional<T>).");
                else
                    GAN_AppendError(src, reason...);
                GAN_AppendError("err::unwrap(optional<T>)", "Called panic on std::nullopt.");
                panic(src);
                throw std::runtime_error("");
            }
            return opt.value();
        }
    }

}
