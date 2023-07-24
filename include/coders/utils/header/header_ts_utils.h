
#ifndef CPP_PROJECT_HEADER_UTILS_H
#define CPP_PROJECT_HEADER_UTILS_H

#include <iostream>
#include "datetime_utils.h"


class HeaderTsUtils {

public:
    static std::string dateFormat() { return "%Y-%m-%d %H:%M:%S"; }; // "2010-10-01 00:00:00"
    static std::tm startDate() { return DatetimeUtils::stringToDatetime("1900-01-01 00:00:00", dateFormat()); };
    static std::tm endDate() { return DatetimeUtils::stringToDatetime("2036-02-07 06:28:16", dateFormat()); };

    static long int getSecondsFromTimestamp(std::string timestamp_str){
        std::tm timestamp_tm = DatetimeUtils::stringToDatetime(timestamp_str, dateFormat());
    #if CHECKS
        // check that the timestamp is within the expected range
        assert(DatetimeUtils::compareDates(startDate(), timestamp_tm) == 1);
        assert(DatetimeUtils::compareDates(timestamp_tm, endDate()) == 1);
    #endif
        long int seconds = DatetimeUtils::mapDatetimeToSeconds(startDate(), timestamp_tm);
        return seconds;
    }

    static std::string getTimestampFromSeconds(long int seconds){
        std::tm timestamp_tm = DatetimeUtils::mapSecondsToDatetime(HeaderTsUtils::startDate(), seconds);

        assert(DatetimeUtils::compareDates(HeaderTsUtils::startDate(), timestamp_tm) == 1);
        assert(DatetimeUtils::compareDates(timestamp_tm, HeaderTsUtils::endDate()) == 1);

        std::string timestamp_str = DatetimeUtils::datetimeToString(timestamp_tm, HeaderTsUtils::dateFormat());
        return timestamp_str;
    }
};

#endif //CPP_PROJECT_HEADER_UTILS_H
