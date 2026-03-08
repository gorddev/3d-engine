#pragma once

#include "path.hpp"

namespace gan {

    class Engine;

    /// Thin wrapper around the filesystem to allow for easy management
    /// of
    class files {
        /// Initializes the filesystem with the current path.
        static void init_filesystem() {
            path::working_directory = (std::filesystem::current_path());
        }

        friend class Engine;


    public:
        /// constructor that initializes the filesystem
        files() { init_filesystem();}

        /// Sets the assets folder of the engine
        static void set_assets_folder(const char path[]) {
            path::assets_folder = gan::path(std::filesystem::path(path)).c_str();
        }

        /// Gets the assets folder of the filesystem
        static path assets() {
            return path(path::assets_folder);
        }

        /// Gets the working directory of the filesystem
        static path working_dir() {
            return path(path::working_directory);
        }

    };

    /// Gets a subdirectory of the assets folder
    inline path dir(const char path[]) {
        const auto len = strlen(path);
        for (size_t i = 0; i < len; i++) {
            if (path[i] == '/' || path[i] == '\\') {
                err::panic("core::dir(const char path[])"
                    , "Cannot initialize a path with '\\' or '/'. Please use the notation:\n"
                       "\tgan::dir(\"directory\")/\"subdir\"/\"file.ext\"");
            }
        }
        return gan::path(std::filesystem::path(path));
    }

    inline files internal_filesystem_setup;

}
