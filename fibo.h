#include <vector>
#include <assert.h>
#include <string>

class Fibo {
    std::vector<bool> bits; // unormowana postać
    bool is_zero() const;

public:
    explicit Fibo() : bits(1, 0) {};

    explicit Fibo(const std::string &);

    Fibo(int);

    template<typename T,
            typename = std::enable_if_t<!std::is_same<T, int>::value && !std::is_same<T, const char *>::value>>
    Fibo(T) = delete;

    Fibo(const Fibo &other_fib) : bits(other_fib.bits) {};

    Fibo(Fibo &&other_fib) : bits(std::move(other_fib.bits)) {};

    friend bool operator<(const Fibo& a, const Fibo& b);

    friend bool operator>(const Fibo& a, const Fibo& b);

    friend bool operator==(const Fibo& a, const Fibo& b);

    friend bool operator>=(const Fibo& a, const Fibo& b);

    friend bool operator<=(const Fibo& a, const Fibo& b);

    friend bool operator!=(const Fibo& a, const Fibo& b);

    Fibo &operator=(const Fibo &);

    Fibo &operator=(Fibo &&);

    Fibo &operator+=(const Fibo &);

    Fibo &operator&=(const Fibo &);

    Fibo &operator|=(const Fibo &);

    Fibo &operator^=(const Fibo &);

    Fibo &operator<<=(const int &);

    friend const Fibo operator+(const Fibo &, const Fibo &);

    friend const Fibo operator&(const Fibo &, const Fibo &);

    friend const Fibo operator|(const Fibo &, const Fibo &);

    friend const Fibo operator^(const Fibo &, const Fibo &);

    friend const Fibo operator<<(const Fibo &, const int &);

    friend std::ostream &operator<<(std::ostream &, const Fibo &);

    size_t length() const;

private:
    bool isBitSet(int pozition) const {
        return pozition >= 0 && pozition < (int) bits.size() && bits[bits.size() - 1 - pozition];
    }

    void setBit(int pozition) {
        if (pozition < 0) return;
        assert(pozition >= 0 && (pozition >= (int) bits.size() || !bits[bits.size() - 1 - pozition]));
        if (pozition >= (int) bits.size())
            addTrailingZeros(pozition + 1 - bits.size());
        bits[bits.size() - 1 - pozition] = 1;
    }

    void unsetBit(int pozition) {
        assert(isBitSet(pozition));
        bits[bits.size() - 1 - pozition] = false;
        clearZeros();
    }

    void correct(int pozition) {
        while (true) {
            if (!isBitSet(pozition) || !isBitSet(pozition + 1))
                break;
            unsetBit(pozition);
            unsetBit(pozition + 1);
            setBit(pozition + 2);
            pozition += 2;
        }
    }

    void addTrailingZeros(const int &n) {
        bits.insert(bits.begin(), n, false);
    }

    void clearZeros() {
        size_t numberOfTrailingZeros = 0;
        while (numberOfTrailingZeros < bits.size() && !bits[numberOfTrailingZeros])
            numberOfTrailingZeros++;
        bits.erase(bits.begin(), bits.begin() + numberOfTrailingZeros);
        if (bits.empty()) bits.push_back(0);
    }
};

const Fibo &Zero();

const Fibo &One();