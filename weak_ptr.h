#include <memory>

namespace cik {

template <typename T>
class WeakPtrFactory;

template <typename T>
class WeakPtr {
public:
    WeakPtr(){};
    WeakPtr(std::nullptr_t){};

    WeakPtr(const WeakPtr &) = default;
    WeakPtr &operator=(const WeakPtr &) = default;
    WeakPtr(WeakPtr &&) = default;
    WeakPtr &operator=(WeakPtr &&) = default;
    ~WeakPtr() = default;

    T *get() const noexcept {
        if (!*ptr_live_) {
            return nullptr;
        }
        return raw_ptr_;
    };

    T &operator*() const {
        ASSERT(raw_ptr_);
        return *raw_ptr_;
    };

    T *operator->() const noexcept {
        return get();
    };

    explicit operator bool() const {
        return *ptr_live_;
    };

private:
    WeakPtr(T *ptr, std::shared_ptr<bool> ptr_live) :
        raw_ptr_{ptr}, ptr_live_{ptr_live} {};

private:
    friend class WeakPtrFactory<T>;
    T *raw_ptr_{nullptr};
    std::shared_ptr<bool> ptr_live_;
};

template <typename T>
class WeakPtrFactory {
public:
    WeakPtrFactory(T *t) :
        ptr_{t}, ptr_live_{std::make_shared<bool>(true)} {};
    WeakPtrFactory(const WeakPtrFactory &) = delete;
    WeakPtrFactory &operator=(const WeakPtrFactory &) = delete;
    WeakPtrFactory(WeakPtrFactory &&) = delete;
    WeakPtrFactory &operator=(WeakPtrFactory &&) = delete;
    ~WeakPtrFactory() {
        *ptr_live_ = false;
    };

    WeakPtr<T> GetWeakPtr() const {
        return std::move(WeakPtr<T>(ptr_, ptr_live_));
    };

private:
    T *ptr_;
    std::shared_ptr<bool> ptr_live_;
};

} // namespace cik
