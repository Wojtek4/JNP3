

#include <vector>
#include <assert.h>
#include <ostream>
#include "fibo.h"

const Fibo &Zero() {
    static const Fibo &zero = Fibo();
    return zero;
};

const Fibo &One() {
    static const Fibo &one = Fibo("1");
    return one;
};

bool Fibo::is_zero() const {
    return bits.empty();
}

Fibo::Fibo(const std::string &str) {
    if(str == "0") {
        bits.push_back(false);
        return;
    }
    assert(*str.begin() == '1');
    int zeros_to_push = 0;
    for (auto it = str.begin(); it != str.end(); ++it) {
        assert(*it == '0' || *it == '1');
        auto curr_bit = (*it == '1') ? true : false;
        bits.push_back(curr_bit);
        while (bits.size() > 1 && bits.back() && bits[bits.size() - 2]) {
            bits.pop_back();
            bits.pop_back();
            zeros_to_push += 2;
            if (!bits.empty()) bits.pop_back();
            bits.push_back(true);
        }
        while (zeros_to_push > 0) {
            bits.push_back(false);
            --zeros_to_push;
        }
    }
}

Fibo::Fibo(int n) {
    if (n == 0) {
        bits.push_back(0);
        return;
    }
    assert(n > 0);
    int f1 = 1, f2 = 1; // fib numbers
    while (f2 <= n) {
        auto t = f1;
        f1 = f2;
        f2 += t;
    }
    while (n > 0 || f1 > 0) {
        if (f1 <= n) {
            n -= f1;
            bits.push_back(true);
        } else bits.push_back(false);

        auto t = f1;
        f1 = f2 - f1;
        f2 = t;
    }
    bits.pop_back();
}


bool operator>(const Fibo &a, const Fibo &b) {
    if (a.is_zero()) return false;
    if (b.is_zero()) return true;
    if (a.bits.size() != b.bits.size()) {
        return a.bits.size() > b.bits.size();
    }
    for (size_t i = 0; i < a.bits.size(); i++) {
        if (a.bits[i] && !b.bits[i]) return true;
        if (!a.bits[i] && b.bits[i]) return false;
    }
    return false;
}

bool operator==(const Fibo &a, const Fibo &b) {
    return !(a > b) && !(a < b);
}

bool operator<(const Fibo &a, const Fibo &b) {
    return b > a;
}

bool operator<=(const Fibo &a, const Fibo &b) {
    return a < b || a == b;
}

bool operator!=(const Fibo &a, const Fibo &b) {
    return !(a == b);
}

bool operator>=(const Fibo &a, const Fibo &b) {
    return b <= a;
}

Fibo &Fibo::operator=(const Fibo &that) {
    if (this != (&that))
        bits = that.bits;
    return *this;
}

Fibo &Fibo::operator=(Fibo &&that) {
    if (this != (&that))
        swap(bits, that.bits);
    return *this;
}

Fibo &Fibo::operator+=(const Fibo &that) {
    if (this == (&that))
        return (*this) += Fibo(that);
    for (size_t i = 0; i < that.bits.size(); i++) {
        if (!that.isBitSet(i))
            continue;
        if (!isBitSet(i)) {
            setBit(i);
            correct(i);
            correct(i - 1);
        } else {
            int it = i;
            while (true) {
                setBit(it + 1);
                unsetBit(it);
                correct(it + 1);
                if (it == 0) break; // przypadek specjalny: ...01 + ...01 = ...10
                if (it == 1) { // przypadek specjalny: ...010 + ...010 = ...101
                    setBit(it - 1);
                    break;
                }
                if (isBitSet(it - 2)) {
                    it -= 2;
                } else {
                    setBit(it - 2);
                    correct(it - 2);
                    correct(it - 3);
                    break;
                }
            }
        }
    }
    return *this;
}

Fibo &Fibo::operator&=(const Fibo &that) {
    if (this == (&that))
        return *this;

    if (bits.size() > that.bits.size())
        bits.erase(bits.begin(), bits.begin() + bits.size() - that.bits.size());

    if (bits.size() < that.bits.size())
        addTrailingZeros(that.bits.size() - bits.size());

    for (size_t i = 1; i <= that.bits.size(); i++)
        bits[bits.size() - i] = bits[bits.size() - i] & that.bits[that.bits.size() - i];

    clearZeros();

    return *this;
}

Fibo &Fibo::operator|=(const Fibo &that) {
    if (this == (&that))
        return *this;
    if (bits.size() < that.bits.size())
        addTrailingZeros(that.bits.size() - bits.size());

    for (size_t i = 0; i < that.bits.size(); i++)
        bits[bits.size() - that.bits.size() + i] = that.bits[i] | bits[bits.size() - that.bits.size() + i];

    for (int i = (int) bits.size() - 2; i >= 0; i--)
        correct(i);

    return *this;
}

Fibo &Fibo::operator^=(const Fibo &that) {

    if (this == (&that))
        return (*this) ^= Fibo(that);

    if (bits.size() < that.bits.size())
        addTrailingZeros(that.bits.size() - bits.size());

    for (size_t i = 0; i < that.bits.size(); i++)
        bits[bits.size() - that.bits.size() + i] = that.bits[i] ^ bits[bits.size() - that.bits.size() + i];

    for (int i = (int) bits.size() - 2; i >= 0; i--)
        correct(i);

    clearZeros();

    return *this;
}

Fibo &Fibo::operator<<=(const int &n) {
    bits.resize(bits.size() + n);
    return *this;
}

const Fibo operator+(const Fibo &a, const Fibo &b) {
    return Fibo(a) += b;
}

const Fibo operator&(const Fibo &a, const Fibo &b) {
    return Fibo(a) &= b;
}

const Fibo operator|(const Fibo &a, const Fibo &b) {
    return Fibo(a) |= b;
}

const Fibo operator^(const Fibo &a, const Fibo &b) {
    return Fibo(a) ^= b;
}

const Fibo operator<<(const Fibo &a, const int &n) {
    return Fibo(a) <<= n;
}


std::ostream &operator<<(std::ostream &os, const Fibo &fib) {
    for (size_t i = 0; i < fib.bits.size(); i++) {
        os << fib.bits[i];
    }
    return os;
}

size_t Fibo::length() const {
    return bits.size();
}
