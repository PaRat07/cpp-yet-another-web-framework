#pragma once

template<typename BaseT>
class Int {
 public:
    friend inline Int operator+(Int lhs, Int rhs) noexcept {
        return { lhs.base_ + rhs.base_ };
    }

    friend inline Int operator-(Int lhs, Int rhs) noexcept {
        return { lhs.base_ - rhs.base_ };
    }

    friend inline Int operator*(Int lhs, Int rhs) noexcept {
        return { lhs.base_ * rhs.base_ };
    }

    friend inline Int operator/(Int lhs, Int rhs) noexcept {
        return { lhs.base_ / rhs.base_ };
    }

    Int &operator=(Int rhs) noexcept {
        base_ = rhs.base_;
    }

 private:
    BaseT base_;
};

