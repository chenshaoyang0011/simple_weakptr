#include <memory>

namespace cik {

template <typename T>
class WeakPtrFactory;

template <typename T>
class WeakPtr {
public:
    WeakPtr(){};
    WeakPtr(std::nullptr_t){};

    // Allow conversion from U to T provided U "is a" T. Note that this
    // is separate from the (implicit) copy and move constructors.
    template <typename U>
    WeakPtr(const WeakPtr<U> &other) {
        ref_ = other.ref_;
        raw_ptr_ = reinterpret_cast<T *>(other.raw_ptr_);
    };

    template <typename U>
    WeakPtr(WeakPtr<U> &&other) noexcept {
        ref_ = std::move(other.ref_);
        raw_ptr_ = reinterpret_cast<T *>(other.raw_ptr_);
        other.raw_ptr_ = nullptr;
    }

    WeakPtr(const WeakPtr &) = default;
    WeakPtr &operator=(const WeakPtr &) = default;
    WeakPtr(WeakPtr &&) noexcept = default;
    WeakPtr &operator=(WeakPtr &&) noexcept = default;
    ~WeakPtr() = default;

    T *get() const noexcept {
        if (ref_ == nullptr || *ref_ == false) {
            return nullptr;
        }
        return raw_ptr_;
    };

    T &operator*() const {
        // ASSERT(raw_ptr_);
        return *raw_ptr_;
    };

    T *operator->() const noexcept {
        return get();
    };

    explicit operator bool() const {
        return *ref_;
    };

private:
    WeakPtr(T *ptr, std::shared_ptr<bool> ptr_live) :
        raw_ptr_{ptr}, ref_{ptr_live} {};

private:
    template <typename U>
    friend class WeakPtr;
    friend class WeakPtrFactory<T>;
    T *raw_ptr_{nullptr};
    std::shared_ptr<bool> ref_;
};

template <typename T>
class WeakPtrFactory {
public:
    WeakPtrFactory(T *t) :
        ptr_{t}, ptr_checker_{std::make_shared<bool>(true)} {};
    WeakPtrFactory(const WeakPtrFactory &) = delete;
    WeakPtrFactory &operator=(const WeakPtrFactory &) = delete;
    WeakPtrFactory(WeakPtrFactory &&) = delete;
    WeakPtrFactory &operator=(WeakPtrFactory &&) = delete;
    ~WeakPtrFactory() {
        *ptr_checker_ = false;
    };

    WeakPtr<T> GetWeakPtr() const {
        return std::move(WeakPtr<T>(ptr_, ptr_checker_));
    };

private:
    T *ptr_;
    std::shared_ptr<bool> ptr_checker_;
};

} // namespace cik
