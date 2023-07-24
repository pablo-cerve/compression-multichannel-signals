
#ifndef CPP_PROJECT_DATETIME_UTILS_H
#define CPP_PROJECT_DATETIME_UTILS_H

#include <iostream>
#include <cstring>
#include <ctime>

class DatetimeUtils {

public:
    static std::tm stringToDatetime(std::string tm_str, std::string datetime_format);
    static std::string datetimeToString(std::tm datetime, std::string datetime_format);

    static int compareDates(std::tm date1, std::tm date2);

    static long int mapDatetimeToSeconds(std::tm start_date, std::tm date);
    static std::tm mapSecondsToDatetime(std::tm start_date, long int seconds);

    static void printDateTime(std::tm date);

private:
    static int secondsInYear(int year);
    static int daysSinceJanuary1(std::tm date);
    static int secondsInDay();
//    static int secondsInMonth(int year, int month);
    static int secondsInHour();
    static int secondsInMinute();
    static int daysInMonth(int month, int year);
    static bool isLeapYear(int year);
};

#endif //CPP_PROJECT_DATETIME_UTILS_H
