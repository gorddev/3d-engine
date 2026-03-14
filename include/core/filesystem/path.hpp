#pragma once
#include <filesystem>

#include "../debug/gan_err.hpp"

namespace gan {


    class path {
    private:
        /// Working directory of the filesystem
        inline static std::filesystem::path working_directory
            = std::filesystem::path("@null");
        /// Folder assets are contained in.
        inline static std::filesystem::path assets_folder
            = std::filesystem::path("..")/"assets";

        /// The internal filesystem path this represents.
        std::filesystem::path filepath;

        friend class files; ///< Filesystem is a friend class

        void verifyInputString(const char path[]) const {
            const auto len = strlen(path);
            for (size_t i = 0; i < len; i++) {
                if (path[i] == '/' || path[i] == '\\') {
                    err::panic("path::operator/()",
                        "Cannot initialize path \"", (filepath/path).c_str(), "\" with '\\' or '/'. Please use the notation:\n"
                           "\tgan::dir(\"directory\")/\"subdir\"/\"file.ext\"");
                }
            }
        }

    public:
        explicit path(std::filesystem::path filepath)
            : filepath(std::move(filepath)) {}

        /// Add files onto the current path.
        path operator/(const char p[]) {
            verifyInputString(p);
            return path(filepath / p);
        }

        /// Automatic decay into a constant character for conversion purposes.
        // ReSharper disable once CppNonExplicitConversionOperator
        operator const char*() const { // NOLINT(*-explicit-constructor)
            return filepath.c_str();
        }

        path& operator=(const char path[]) {
            verifyInputString(path);
            filepath = std::filesystem::path(path);
            return *this;
        }

        /// Converts into a c_str
        [[nodiscard]] const char* c_str() const {
            return filepath.c_str();
        }

        /// Expands the path including the assets folder
        [[nodiscard]] path expand() const {
            return path(assets_folder/filepath);
        }

        /// Verify path existence.
        [[nodiscard]] bool exists() const {
            const path expanded = expand();
            if (!std::filesystem::exists(expanded.c_str())) {
                GAN_WriteLog("path::exists()",
                    "The file ", expanded.c_str(), " doesn't exist.");
                return false;
            } if (!std::filesystem::is_regular_file(expanded.c_str())) {
                GAN_WriteLog("path::exits()",
                    "The file ", expanded.c_str(), " is not a regular file.");
                return false;
            }
            return true;
        }

        /// Adds a subdirectory to the current path
        path subdir(const char path[]) const {
            return gan::path(filepath / path);
        }
    };
}
