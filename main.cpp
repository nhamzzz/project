#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>

class RomanToNumber {
private:
    std::string num;

    // Helper function: convert integer back to Roman
    std::string toRoman(int number) {
        std::pair<int, std::string> values[] = {
            {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
            {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
            {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
        };

        std::string result;
        for (auto& p : values) {
            while (number >= p.first) {
                result += p.second;
                number -= p.first;
            }
        }
        return result;
    }

public:
    RomanToNumber() {} // Default constructor

    void set(const std::string& value) {
        num = value;
    }

    int toInteger() {
        // Map Roman numerals to values
        std::map<char, int> roman = {
            {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50},
            {'C', 100}, {'D', 500}, {'M', 1000}
        };

        std::string upperNum = num;
        std::transform(upperNum.begin(), upperNum.end(), upperNum.begin(), ::toupper);

        // Validate characters first
        for (char c : upperNum) {
            if (roman.find(c) == roman.end()) {
                std::cout << "Invalid character in Roman numeral: " << num << "\n";
                return -1;
            }
        }

        int int_value = 0;
        for (size_t i = 0; i < upperNum.length(); i++) {
            int current = roman[upperNum[i]];
            int next = 0;

            if (i + 1 < upperNum.length()) {
                next = roman[upperNum[i + 1]];
            }

            if (current < next)
                int_value -= current;
            else
                int_value += current;
        }

        // Validate by converting back
        std::string reconverted = toRoman(int_value);

        if (reconverted != upperNum) {
            std::cout << "Invalid Roman numeral sequence: " << num << "\n";
            return -1;
        }

        return int_value;
    }
};

int main() {
    RomanToNumber converter;
    std::string input;

    std::cout << "=== Roman Numeral Converter ===\n";
    std::cout << "Enter a Roman numeral (or type 'exit' to quit)\n\n";

    while (true) {
        std::cout << "Enter Roman numeral: ";
        if (!std::getline(std::cin, input)) {
            std::cout << "\nInput stream closed. Exiting...\n";
            break;
        }

        // Remove spaces
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

        if (input == "exit" || input == "EXIT")
            break;

        if (input.empty())
            continue;

        converter.set(input);
        int result = converter.toInteger();

        if (result > 0)
            std::cout << "→ " << result << "\n\n";
    }

    return 0;
}
