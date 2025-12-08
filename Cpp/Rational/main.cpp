#include <fstream>
#include <numeric>
#include <iostream>
#include <typeinfo>
class Rational {
public:
    Rational () : numerator(0), denominator(1) {}
    Rational (int _a, int _b) : numerator(_a), denominator(_b) {
        if (denominator == 0) throw std::invalid_argument("Denominator can not be zero");
        normalize();
    }

    int Numerator() const { return numerator; }
    int Denominator() const { return denominator; }

    Rational operator+(const Rational& other) const {
        return Rational(numerator * other.denominator + other.numerator * denominator, denominator * other.denominator);
    }
    Rational operator-(const Rational& other) const {
        return Rational(numerator * other.denominator - other.numerator * denominator, denominator * other.denominator);
    }
    Rational operator*(const Rational& other) const {
        return Rational(numerator * other.numerator, denominator * other.denominator);
    }
    Rational operator/(const Rational& other) const {
        return Rational(numerator * other.denominator, denominator * other.numerator);
    }

    Rational operator+(const int& other) const {
        return Rational((other * denominator) + numerator, denominator);
    }
    Rational operator-(const int& other) const {
        return Rational(numerator - (other * denominator), denominator);
    }
    Rational operator*(const int& other) const {
        return Rational(numerator * other, denominator);
    }
    Rational operator/(const int& other) const {
        return Rational(numerator, denominator * other);
    }

    Rational operator+() const { return *this; }
    Rational operator-() const { return Rational(-numerator, denominator); }

    Rational& operator+=(const Rational& other) { return *this = *this + other; }
    Rational& operator-=(const Rational& other) { return *this = *this - other; }
    Rational& operator*=(const Rational& other) { return *this = *this * other; }
    Rational& operator/=(const Rational& other) { return *this = *this / other; }

    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }
    bool operator!=(const Rational& other) const {
        return !(*this == other);
    }

    Rational operator++() {
        return *this + 1;
    }

    Rational operator++(int) {
        Rational temp = *this;
        *this = *this + 1;
        return temp;
    }

private:
    int numerator;
    int denominator;

    void normalize() {
        int NOD = std::gcd(numerator, denominator);
        numerator /= NOD;
        denominator /= NOD;
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << r.Numerator() << "/" << r.Denominator() << std::endl;
    return os;
}

Rational operator+(int leftObj, const Rational& rightObj) {
    return Rational(leftObj * rightObj.Denominator() + rightObj.Numerator(),
                    rightObj.Denominator());
}
Rational operator-(int leftObj, const Rational& rightObj) {
    return Rational(leftObj * rightObj.Denominator() - rightObj.Numerator(),
                    rightObj.Denominator());
}
Rational operator*(int leftObj, const Rational& rightObj) {
    return Rational(leftObj * rightObj.Numerator(),
                    rightObj.Denominator());
}
Rational operator/(int leftObj, const Rational& rightObj) {
    return Rational(leftObj * rightObj.Denominator(),
                    rightObj.Numerator());
}

int main(){
    Rational y;
    Rational x(1, 3);
    Rational z(3, 6);
    std::cout << z;
    Rational sum = x + z;
    std::cout << sum;

    std::cout << x - z << " *: " << x * z << " /: " << x / z << std::endl;

    Rational r(3, 5);

    std::cout << std::endl;

    std::cout <<  r + 2;  // 3/5 + 2 = 13/5
    //std::cout <<  2 - r;
    //Rational res2 = 2 - r;  // 2 - 3/5 = 7/5
    //Rational res3 = r * 4;  // 3/5 * 4 = 12/5

    std::cout << 2 + r;

    //std::cout << ++r;
    std::cout << r++;
    std::cout << r;
}
