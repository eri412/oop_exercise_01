/**
 * Ватулин М8О-206Б-19
 * Создать класс BitString для работы с 96-битовыми строками. Битовая строка должна быть представлена двумя полями: 
 * старшая часть unsigned long long, младшая часть unsigned int. Должны быть реализованы все традиционные операции 
 * для работы с битами: and, or, xor, not. Реализовать сдвиг влево shiftLeft и сдвиг вправо shiftRight на заданное 
 * количество битов. Реализовать операцию вычисления количества единичных битов, операции сравнения по количеству 
 * единичных битов. Реализовать операцию проверки включения.
 */

/**
 * Usage: <operand1> <operator> <operand2>
 * -- if operator is unary, operand2 is ignored
 * -- EOF to end the input
 * -- any symbols other that '1' in operands are considered to be '0'
 * -- one test file can contain any number of tests
 * -- tests can be delimited by blank line
 */

#include <bitset>
#include <iostream>
#include <string>
#include <limits>
#include <ios>

/**
 * class that stores 96-bit string
 */
class BitString {
private:
    unsigned long long _head; // most significant
    unsigned int _tail; // least significant
    unsigned int _set_bits;
    unsigned int _count_set_bits(unsigned long long number) const {
        unsigned int count = 0;
        while (number != 0) {
            count += number & 1;
            number >>= 1;
        }
        return count;
    }
public:
    static const unsigned int bits_in_head = sizeof(unsigned long long) * 8;
    static const unsigned int bits_in_tail = sizeof(unsigned int) * 8;
    // constructor that takes two numbers as arguments and assigns them 'as is'
    BitString(unsigned long long head = 0, unsigned int tail = 0){
        _head = head;
        _tail = tail;
        _set_bits = _count_set_bits(_head) + _count_set_bits(_tail);
    }
    // constructor that parses string, at that anything except '1' will be considered 0
    BitString(const std::string& sample) {
        _head = 0;
        _tail = 0;
        for (int i = sample.size() - 1; i >= 0; i--) {
            if (sample[i] == '1') {
                if (sample.size() - i - 1 < bits_in_tail) {
                    _tail |= 1u << (sample.size() - i - 1);
                }
                else {
                    _head |= 1ull << (sample.size() - bits_in_tail - i - 1);
                }
            }
        }
        _set_bits = _count_set_bits(_head) + _count_set_bits(_tail);
    }
    BitString operator&(const BitString& other) const {
        BitString temp(_head & other._head, _tail & other._tail);
        return temp;
    }
    unsigned int operator&(const unsigned int other) const {
        return _tail & other;
    }
    BitString operator|(const BitString& other) const {
        BitString temp(_head | other._head, _tail | other._tail);
        return temp;
    }
    BitString operator^(const BitString& other) const {
        BitString temp(_head ^ other._head, _tail ^ other._tail);
        return temp;
    }
    BitString operator~() const {
        BitString temp(~_head, ~_tail);
        return temp;
    }

    // next two operators may seem as an untidy code, but actually I did all this checks
    // to avoid undefined behaviour when bitshifting
    BitString operator<<(const unsigned int bits) const {
        unsigned long long temp_head;
        if (bits >= bits_in_head) {
            temp_head = 0;
        }
        else {
            temp_head = _head << bits;
        }       
        unsigned int temp_tail;
        if (bits >= bits_in_tail) {
            temp_head = (temp_head | _tail) << (bits - bits_in_tail);
            temp_tail = 0;
        }
        else {
            temp_head |= _tail >> (bits_in_tail - bits);
            temp_tail = _tail << bits;
        }
        BitString temp(temp_head, temp_tail);
        return temp;
    }
    BitString operator>>(const unsigned int bits) const {
        unsigned int temp_tail;
        if (bits >= bits_in_tail) {
            temp_tail = 0;
        }
        else {
            temp_tail = _tail >> bits;
        }
        unsigned long long temp_head;
        if (bits >= bits_in_head) {
            temp_tail = _head >> (bits_in_head/2) >> (bits - bits_in_head);
            temp_head = 0;
        }
        else {
            temp_tail |= _head << (bits_in_head - bits) >> bits_in_tail;
            temp_head = _head >> bits;
        }
        BitString temp(temp_head, temp_tail);
        return temp;
    }
    BitString& operator|=(const BitString &other) {
        return (*this = *this | other);
    }
    BitString& operator&=(const BitString &other) {
        return (*this = *this & other);
    }
    BitString& operator^=(const BitString &other) {
        return (*this = *this ^ other);
    }
    BitString& operator<<=(const unsigned int bits) {
        return (*this = *this << bits);
    }
    BitString& operator>>=(const unsigned int bits) {
        return (*this = *this >> bits);
    }
    // set bits getter
    unsigned int count_set_bits() const {
        return _set_bits;
    }
    bool operator>(const BitString& other) const {
        return this->count_set_bits() > other.count_set_bits();
    }
    bool operator<(const BitString& other) const {
        return this->count_set_bits() < other.count_set_bits();
    }
    // this operator DOESN'T mean bitstrings are the same, it means bitstrings have the same number of set bits
    bool operator==(const BitString& other) const {
        return this->count_set_bits() == other.count_set_bits();
    }
    bool operator<=(const BitString& other) const {
        return !(*this > other);
    }
    bool operator>=(const BitString& other) const {
        return !(*this < other);
    }
    bool operator!=(const BitString& other) const {
        return !(*this == other);
    }
    bool includes(const BitString& other) const {
        return ((this->_head & other._head) == other._head 
             && (this->_tail & other._tail) == other._tail);
    }
    std::string to_string() const {
        return std::bitset<bits_in_head>(_head).to_string() + std::bitset<bits_in_tail>(_tail).to_string();
    }
};

/**
 * execute functions take operands and operator and print the result of the operation
 */
void execute(const BitString& lhs, const std::string& op, const BitString& rhs) {
    if (op == "|") {
        std::cout << (lhs | rhs).to_string() << "\n" << std::endl;
    }
    else if (op == "&") {
        std::cout << (lhs & rhs).to_string() << "\n" << std::endl;
    }
    else if (op == "^") {
        std::cout << (lhs ^ rhs).to_string() << "\n" << std::endl;
    }
    else if (op == "<") {
        std::cout << (lhs < rhs) << "\n" << std::endl;
    }
    else if (op == "==") {
        std::cout << (lhs == rhs) << "\n" << std::endl;
    }
    else if (op == ">") {
        std::cout << (lhs > rhs) << "\n" << std::endl;
    }
    else if (op == "<=") {
        std::cout << (lhs <= rhs) << "\n" << std::endl;
    }
    else if (op == "!=") {
        std::cout << (lhs != rhs) << "\n" << std::endl;
    }
    else if (op == ">=") {
        std::cout << (lhs >= rhs) << "\n" << std::endl;
    }
    else if (op == "includes") {
        std::cout << lhs.includes(rhs) << "\n" << std::endl;
    }
    else if (op == "~") {
        std::cout << (~lhs).to_string() << "\n" << std::endl;
    }
    else if (op == "set_bits") {
        std::cout << lhs.count_set_bits() << "\n" << std::endl;
    }
    else {
        std::cout << "Bad input!" << "\n" << std::endl;
    }
}

void execute(const BitString& lhs, const std::string& op, const unsigned int rhs) {
    if (op == "<<") {
        std::cout << (lhs << rhs).to_string() << "\n" << std::endl;
    }
    else if (op == ">>") {
        std::cout << (lhs >> rhs).to_string() << "\n" << std::endl;
    }
    else {
        std::cout << "Bad input!" << "\n" << std::endl;
    }
}

/**
 * parse function takes input from stdin and calls execute function with corresponding arguments
 */
void parse() {
    std::cout << std::boolalpha;
    std::string lhs;
    std::string op;
    std::string rhs;
    while (true) {
        std::cin >> lhs >> op;
        // break out of loop in case input has ended
        if (std::cin.eof()) {
            break;
        }
        // if op is unary operation
        if (op == "~" || op == "set_bits") {
            execute(BitString(lhs), op, BitString(lhs));
        }
        // if i need to call overloaded function
        else if (op == ">>" || op == "<<") {
            std::cin >> rhs;
            execute(BitString(lhs), op, std::stoul(rhs));
        }
        // binary operation
        else {
            std::cin >> rhs;
            execute(BitString(lhs), op, BitString(rhs));
        }
        // clearing the buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    parse();
    
    return 0;
}
