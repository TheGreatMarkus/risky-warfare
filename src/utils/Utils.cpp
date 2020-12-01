#include "Utils.h"

using std::find;
using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::end;
using std::endl;

namespace {
    bool seedSet = false;

    void ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
    }

    void rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), s.end());
    }
}

/**
 * Splits a string given a delimiter
 *
 * Implementation taken from
 * https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 *
 * @param str
 * @param delimiter
 * @return vector of tokens
 */
vector<string> cris_utils::strSplit(string str, const string &delimiter) {
    vector<string> result;
    size_t pos;
    while ((pos = str.find(delimiter)) != string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    result.push_back(str);
    return result;
}

/**
 * Trims string of whitespace
 *
 * Implementation taken from
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 *
 * @param s string to trim
 */
void cris_utils::trim(string &s) {
    ltrim(s);
    rtrim(s);
}

/**
 * Checks if string can be converted to a number
 *
 * Implementation for isNumber taken from
 * https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
 *
 * @param str
 * @return whether string can be converted to a number
 */

bool cris_utils::isNumber(const string &str) {
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

/**
 * Prompts the user for a number
 * @param prompt
 * @param min
 * @param max
 * @return the int given by the user
 */
int cris_utils::getIntInput(string prompt, int min, int max) {
    int answer = 0;
    bool error = false;

    do {
        if (error) {
            cout << prompt << " (Please enter a number between " << min << " and " << max << "): ";
        } else {
            cout << prompt << " (" << min << "-" << max << "): ";
        }

        cin >> answer;
        cin.clear();
        cin.ignore(1000, '\n');
        error = !cin || answer < min || answer > max;
    } while (error);

    return answer;
}

/**
 * Prompts the user for a boolean
 * @param prompt
 * @return the boolean given by the user
 */
bool cris_utils::getBoolInput(string prompt) {
    int answer = 0;
    bool error = false;

    do {
        if (error) {
            cout << prompt << " (Please answer with either \"1\" (yes) or \"2\" (no)): ";
        } else {
            cout << prompt << " (1 = yes, 2 = no): ";
        }

        cin >> answer;
        cin.clear();
        cin.ignore(1000, '\n');
        error = !cin || (answer != 1 && answer != 2);
    } while (error);

    if (answer == 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * Wait for user key press
 */
void cris_utils::getContinueInput() {
    cout << endl << "Press Enter to continue: ";
    getchar();
}

void cris_utils::printTitle(string title) {
    cout << endl
         << "==========================================================" << endl
         << title << endl
         << "==========================================================" << endl << endl;
}

void cris_utils::printSubtitle(string title) {
    cout << endl
         << "============== " <<
         title
         << " ==============" << endl << endl;
}

/**
 * Generate random number between min and max
 */
int cris_utils::randInt(int min, int max) {
    if (!seedSet) {
        srand((unsigned int) time(nullptr));
        seedSet = true;
    }
    return (rand() % (max - min + 1)) + min;
}
