
#include "datetime_utils.h"

#include <iomanip>
#include <sstream>
#include "assert.h"
#include <vector>
#include "string_utils.h"


std::tm DatetimeUtils::stringToDatetime(std::string tm_str, std::string datetime_format){
    std::tm tm;
    std::memset(&tm, 0, sizeof(std::tm));
    std::istringstream ss(tm_str);
    ss >> std::get_time(&tm, datetime_format.c_str());
    return tm;
}

//
// SOURCE: https://stackoverflow.com/a/16358111/4547232
//
std::string DatetimeUtils::datetimeToString(std::tm datetime, std::string datetime_format){
    std::ostringstream oss;
    oss << std::put_time(&datetime, datetime_format.c_str());
    std::string str = oss.str();
    return str;
}

int DatetimeUtils::compareDates(std::tm date1, std::tm date2) {
    if(date1.tm_year < date2.tm_year){ return 1; }
    else if(date2.tm_year < date1.tm_year){ return -1; }

    if(date1.tm_yday < date2.tm_yday){ return 1; }
    else if(date2.tm_yday < date1.tm_yday){ return -1; }

    if(date1.tm_hour < date2.tm_hour){ return 1; }
    else if(date2.tm_hour < date1.tm_hour){ return -1; }

    if(date1.tm_min < date2.tm_min){ return 1; }
    else if(date2.tm_min < date1.tm_min){ return -1; }

    if(date1.tm_sec < date2.tm_sec){ return 1; }
    else if(date2.tm_sec < date1.tm_sec){ return -1; }

    return 0;
}

long int DatetimeUtils::mapDatetimeToSeconds(std::tm start_date, std::tm date){
    assert(compareDates(start_date, date) != -1);
    assert(start_date.tm_year == 0 && start_date.tm_mon == 0 && start_date.tm_mday == 1);
    assert(start_date.tm_hour == 0 && start_date.tm_min == 0 && start_date.tm_sec == 0);

    long int seconds = 0;
    for(int i=0; i < date.tm_year; i++){ seconds += secondsInYear(1900 + i); }
    for(int i=0; i < daysSinceJanuary1(date); i++){ seconds += secondsInDay(); }
    for(int i=0; i < date.tm_hour; i++){ seconds += secondsInHour(); }
    for(int i=0; i < date.tm_min; i++){ seconds += secondsInMinute(); }
    seconds += date.tm_sec;

    return seconds;
}

std::tm DatetimeUtils::mapSecondsToDatetime(std::tm start_date, long int seconds){
    assert(start_date.tm_year == 0 && start_date.tm_mon == 0 && start_date.tm_mday == 1);
    assert(start_date.tm_hour == 0 && start_date.tm_min == 0 && start_date.tm_sec == 0);

    long int whole_days = seconds / secondsInDay();
    long int remaining_seconds = seconds % secondsInDay();

    int year = start_date.tm_year + 1900;
    int month = start_date.tm_mon;
    int day = start_date.tm_mday;
    int days_in_month = daysInMonth(month, year);

    for(int i=0; i < whole_days; i++){
        if (day == days_in_month){
            day = 1; month ++;
            if (month == 12) { month = 0; year++; }
            days_in_month = daysInMonth(month, year);
        }
        else {
            day++;
        }
    }

    int hour = start_date.tm_hour;
    int min = start_date.tm_min;
    int sec = start_date.tm_sec;
    for(int i=0; i < remaining_seconds; i++){
        sec++;
        if (sec == 60){
            sec = 0; min++;
            if (min == 60) {
                min = 0; hour++;
                if (hour == 24){
                    hour = 0; day++;
                    if (day == days_in_month){
                        day = 1; month ++;
                        if (month == 12) { month = 0; year++; }
                    }
                }
            }
        }
    }
    int month_plus = month + 1;
    std::string timestamp_str = StringUtils::intToStringPos(year, 4) + "-" +
                                StringUtils::intToStringPos(month_plus, 2) + "-" +
                                StringUtils::intToStringPos(day, 2);
    timestamp_str += " " + StringUtils::intToStringPos(hour, 2) + ":" +
                           StringUtils::intToStringPos(min, 2) + ":" +
                           StringUtils::intToStringPos(sec, 2);
    std::tm timestamp_tm = stringToDatetime(timestamp_str, "%Y-%m-%d %H:%M:%S");
    return timestamp_tm;
}

void DatetimeUtils::printDateTime(std::tm date){
    std::cout << date.tm_year << "-" << date.tm_mon << "-" << date.tm_mday << " ";
    std::cout << date.tm_hour << "-" << date.tm_min << "-" << date.tm_sec << std::endl;
}

int DatetimeUtils::secondsInYear(int year){
    int day_count = 365;
    if (isLeapYear(year)){ day_count++; }
    return day_count*secondsInDay();
}

int DatetimeUtils::daysSinceJanuary1(std::tm date){
//    std::vector<int> days_per_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day_count = 0;
    for(int i=0; i < date.tm_mon; i++){
        day_count += daysInMonth(i, date.tm_year);
    }
    day_count += date.tm_mday - 1;
    return day_count;
}

int DatetimeUtils::secondsInDay(){
    return 24*secondsInHour();
}

int DatetimeUtils::secondsInHour(){
    return 60*secondsInMinute();
}

int DatetimeUtils::secondsInMinute(){
    return 60;
}

int DatetimeUtils::daysInMonth(int month, int year){
    std::vector<int> days_per_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days_in_month = days_per_month[month];
    if (month == 1 && isLeapYear(year)) { days_in_month++; }
    return days_in_month;
}

bool DatetimeUtils::isLeapYear(int year){
    if (year % 4 == 0){
        if (year % 100 == 0)
        {
            return (year % 400 == 0);
        }
        else
            return true;
    }
    else {
        return false;
    }
}
