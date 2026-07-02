#include "Utils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <limits>

namespace Utils {

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readInt(const std::string& prompt, int minVal, int maxVal) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            }
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a whole number";
            if (minVal != std::numeric_limits<int>::min() || maxVal != std::numeric_limits<int>::max()) {
                std::cout << " between " << minVal << " and " << maxVal;
            }
            std::cout << ".\n";
        }
    }

    double readDouble(const std::string& prompt, double minVal, double maxVal) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            }
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a valid number.\n";
        }
    }

    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        return trim(line);
    }

    std::string readNonEmptyLine(const std::string& prompt) {
        std::string line;
        do {
            line = readLine(prompt);
            if (line.empty()) {
                std::cout << "This field cannot be empty.\n";
            }
        } while (line.empty());
        return line;
    }

    bool readYesNo(const std::string& prompt) {
        while (true) {
            std::string answer = Utils::toUpper(readLine(prompt));
            if (!answer.empty() && (answer[0] == 'Y' || answer[0] == 'N')) {
                return answer[0] == 'Y';
            }
            std::cout << "Please answer with y or n.\n";
        }
    }

    std::string currentDate() {
        std::time_t t = std::time(nullptr);
        std::tm tmBuf{};
#if defined(_WIN32)
        localtime_s(&tmBuf, &t);
#else
        localtime_r(&t, &tmBuf);
#endif
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tmBuf);
        return std::string(buffer);
    }

    std::string currentDateTime() {
        std::time_t t = std::time(nullptr);
        std::tm tmBuf{};
#if defined(_WIN32)
        localtime_s(&tmBuf, &t);
#else
        localtime_r(&t, &tmBuf);
#endif
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tmBuf);
        return std::string(buffer);
    }

    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }
        // Preserve a trailing empty field (e.g. "a|" -> {"a", ""})
        if (!s.empty() && s.back() == delim) {
            result.emplace_back("");
        }
        return result;
    }

    std::string join(const std::vector<std::string>& parts, char delim) {
        std::string result;
        for (size_t i = 0; i < parts.size(); ++i) {
            result += parts[i];
            if (i + 1 < parts.size()) result += delim;
        }
        return result;
    }

    std::string toUpper(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(),
                        [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    std::string sanitize(const std::string& s) {
        std::string result;
        result.reserve(s.size());
        for (char c : s) {
            if (c != '|' && c != ';' && c != ':') result += c;
        }
        return trim(result);
    }

    bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
        std::string h = toUpper(haystack);
        std::string n = toUpper(needle);
        return h.find(n) != std::string::npos;
    }

    void pause() {
        std::cout << "\nPress Enter to continue...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }
}
