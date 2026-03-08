#pragma once
#include <atomic>



namespace gan::mem {
    /// @code rc@endcode stands for "Reference Counter."
    /// Allows for objects to manually count the number of references
    /// that have been made to them over time.
    class reference_counter {
        /// Contains a pointer to the number of references.
        std::atomic<uint32_t>* num_refs;
    public:
        /// Constructor for an RC makes a new reference counter.
        reference_counter() : num_refs(new std::atomic<uint32_t>(1)) {}
        /// Constructor given another rc adds one to the reference count.
        reference_counter(const reference_counter& other) noexcept : num_refs(other.num_refs) {
            num_refs->fetch_add(1, std::memory_order_relaxed);
        }
        /// Move constructs: no addition and no removal.
        reference_counter(reference_counter&& other) noexcept : num_refs(other.num_refs) {
            other.num_refs = nullptr;
        }
        /// Adds a count to the reference counter & updates accordingly.
        reference_counter& operator=(const reference_counter& other) noexcept {
            if (this != &other) {
                release();
                num_refs = other.num_refs;
                if (num_refs)
                    num_refs->fetch_add(1, std::memory_order_relaxed);
            }
            return *this;
        }
        /// Move constructs a new rc in place. Does not update the reference count.
        reference_counter& operator=(reference_counter&& other) noexcept {
            if (this != &other) {
                release();
                num_refs = other.num_refs;
                other.num_refs = nullptr;
            }
            return *this;
        }
        /// Returns the reference count.
        [[nodiscard]] uint32_t ref_count() const {
            if (num_refs)
                return num_refs->load();
            return 0;
        }

        /// Releases the RC upon destruction.
        ~reference_counter() {
            release();
        }

    private:
        /// On release, make sure it isn't null, and then subtract one from the reference counter.
        void release() {
            if (!num_refs) return;
            if (num_refs->fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete num_refs;
            }
            num_refs = nullptr;
        }
    };
} //namespace gan::mem


