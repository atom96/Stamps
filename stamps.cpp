#include <iostream>
#include <vector>
#include <tuple>
#include <regex>

namespace {
    typedef std::tuple<unsigned int, std::string, double, std::string, std::string> stamp;
    typedef std::pair<unsigned int, unsigned int> query;

    static const std::regex stampPattern(
            "^[\\s]*(.+)[\\s]+([\\d]*[\\.|,]?[\\d]+)[\\s]+([1-9][\\d]{0,3})[\\s]+(.+)$");

    static const unsigned int year = 0;
    static const unsigned int postOffice = 1;
    static const unsigned int stampName = 3;
    static const unsigned int priceInputFormat = 4;

    bool checkLineIfStamp(const std::string &line) {
        return std::regex_match(line, stampPattern);
    }

    bool checkLineIfQuery(const std::string &line) {
        static const std::regex queryPattern(
                "^[\\s]*([\\d]{1,4})[\\s]+([\\d]{1,4})[\\s]*");

        return std::regex_match(line, queryPattern);
    }

    void prepareStringToParse(std::string &line) {
        static const std::regex whiteCharsPattern("\\s+");
        static const std::regex spaceOnTheEnd("^(.*) $");

        line = std::regex_replace(line, whiteCharsPattern, " ");
        line = std::regex_replace(line, spaceOnTheEnd, "$1");
    }

    std::string changeComaToDot(const std::string &line) {
        static const std::regex pattern(",");
        return std::regex_replace(line, pattern, ".");
    }

    stamp parseStampLine(std::string &line) {
        std::smatch matchResult;
        static const unsigned int matchedStampName = 1;
        static const unsigned int matchedPrice = 2;
        static const unsigned int matchedYear = 3;
        static const unsigned int matchedPostOffice = 4;

        prepareStringToParse(line);

        std::regex_search(line, matchResult, stampPattern);

        return std::forward_as_tuple(
                std::stoi(std::string(matchResult[matchedYear])),
                matchResult[matchedPostOffice],
                std::stod(changeComaToDot(matchResult[matchedPrice])),
                matchResult[matchedStampName], matchResult[matchedPrice]);
    }

    query parseQueryLine(const std::string &line) {
        static const unsigned int firstYear = 1;
        static const unsigned int secondYear = 2;
        static const std::regex queryPattern(
                "^[\\s]*([1-9][\\d]{0,3})[\\s]+([1-9][\\d]{0,3})[\\s]*");
        std::smatch matchResult;

        std::regex_search(line, matchResult, queryPattern);

        return std::make_pair(std::stoi(matchResult[firstYear]),
                              std::stoi(matchResult[secondYear]));
    }

    void printStamp(const stamp &stampToPrint) {
        std::cout << std::get<year>(stampToPrint) << " "
                  << std::get<postOffice>(stampToPrint) << " "
                  << std::get<priceInputFormat>(stampToPrint) <<
                  " " << std::get<stampName>(stampToPrint) << '\n';
    }

    bool stampComparator(const stamp &lhs, const stamp &rhs) {
        return std::get<year>(lhs) < std::get<year>(rhs);
    }

    void followQuery(const std::vector<stamp> &stampCollection,
                     query queryToFollow) {
        stamp lowerValue = stamp(queryToFollow.first, "", 0.0, "", "");
        stamp upperValue = stamp(queryToFollow.second, "", 0.0, "", "");
        auto iteratorFirst = std::lower_bound(stampCollection.begin(),
                                              stampCollection.end(), lowerValue,
                                              stampComparator);
        auto iteratorLast = std::upper_bound(stampCollection.begin(),
                                             stampCollection.end(), upperValue,
                                             stampComparator);

        while (iteratorFirst != iteratorLast) {
            printStamp(*iteratorFirst);
            ++iteratorFirst;
        }
    }

    void printError(const std::string &line, unsigned int lineNumber) {
        std::cerr << "Error in line " << lineNumber << ":" << line << std::endl;
    }

    void sortStamps(std::vector<stamp> &stampCollection) {
        std::sort(stampCollection.begin(), stampCollection.end());
    }
}

int main(void) {
    std::vector<stamp> stampCollection;
    bool allStampsLoaded = false;
    std::string line;
    unsigned int processedLines = 0;

    while (!allStampsLoaded) {
        if (!std::getline(std::cin, line)) {
            return 0;
        }

        ++processedLines;

        if (checkLineIfStamp(line)) {
            stampCollection.push_back(parseStampLine(line));
        } else if (checkLineIfQuery(line)) {
            allStampsLoaded = true;
        } else {
            printError(line, processedLines);
        }
    }

    sortStamps(stampCollection);

    do {
        if (!checkLineIfQuery(line)) {
            printError(line, processedLines);
        } else {
            query queryToFollow = parseQueryLine(line);

            if (queryToFollow.first > queryToFollow.second) {
                printError(line, processedLines);
            } else {
                followQuery(stampCollection, parseQueryLine(line));
            }
        }

        ++processedLines;
    } while (std::getline(std::cin, line));

    return 0;
}