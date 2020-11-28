#include "Utils.h"

#include <algorithm>
#include <iostream>

using std::find;
using std::vector;
using std::sort;
using std::string;
using std::cin;
using std::cout;
using std::end;
using std::endl;
using std::getline;

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
 * @param str
 * @param delimiter
 * @return vector of tokens
 */
vector<string> cris_utils::strSplit(string str, const string &delimiter) {
    vector<string> result;
    size_t pos = 0;
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
 * @param str
 * @return whether string can be converted to a number
 */

bool cris_utils::isNumber(const string &str) {
    string::const_iterator it = str.begin();
    while (it != str.end() && isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

int cris_utils::getIntInput(string prompt, int min, int max) {
    int answer;
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

int cris_utils::getBoolInput(string prompt) {
    string answer = "";
    bool error = false;

    do {
        if (error) {
            cout << prompt << " (Please answer with either \"y\" or \"n\"): ";
        } else {
            cout << prompt << " (y/n): ";
        }

        getline(cin, answer);
        cin.clear();
        error = answer != "y" && answer != "n" && answer != "yes" && answer != "no";
    } while (error);

    if (answer == "y" || answer == "yes") {
        return true;
    } else {
        return false;
    }
}

void cris_utils::printTitle(string title) {
    cout << "==========================================================" << endl
         << title << endl
         << "==========================================================" << endl << endl;
}

void cris_utils::printSubtitle(string title) {
    cout << "============== " <<
         title
         << " ==============" << endl << endl;
}

int cris_utils::randInt(int min, int max) {
    if (!seedSet) {
        srand((unsigned int) time(nullptr));
        seedSet = true;
    }
    return (rand() % (max - min + 1)) + min;
}
