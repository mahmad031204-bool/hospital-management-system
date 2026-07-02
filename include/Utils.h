#pragma once

#include <string>
#include <vector>
#include <climits>

// Small collection of helper functions used throughout the project:
// safe console input, string manipulation and date/time formatting.
namespace Utils {

    // Prompts the user until a valid integer within [minVal, maxVal] is entered.
    int readInt(const std::string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX);

    // Prompts the user until a valid double within [minVal, maxVal] is entered.
    double readDouble(const std::string& prompt, double minVal = -1e18, double maxVal = 1e18);

    // Reads a full line of text (may be empty).
    std::string readLine(const std::string& prompt);

    // Reads a full line of text, re-prompting until it is non-empty.
    std::string readNonEmptyLine(const std::string& prompt);

    // Asks a yes/no question. Returns true for 'y'/'Y'.
    bool readYesNo(const std::string& prompt);

    // Returns the current date as YYYY-MM-DD.
    std::string currentDate();

    // Returns the current date and time as YYYY-MM-DD HH:MM:SS.
    std::string currentDateTime();

    // Splits a string on a delimiter character.
    std::vector<std::string> split(const std::string& s, char delim);

    // Joins a vector of strings using a delimiter character.
    std::string join(const std::vector<std::string>& parts, char delim);

    // Returns an upper-case copy of the string.
    std::string toUpper(const std::string& s);

    // Trims leading/trailing whitespace.
    std::string trim(const std::string& s);

    // Removes characters that would break our pipe-delimited file format
    // ('|', ';' and ':') from user-supplied text, and trims whitespace.
    std::string sanitize(const std::string& s);

    // Case-insensitive substring search.
    bool containsIgnoreCase(const std::string& haystack, const std::string& needle);

    // Waits for the user to press Enter before continuing.
    void pause();

    // Clears any leftover characters (including the newline) from std::cin.
    void clearInputBuffer();
}
