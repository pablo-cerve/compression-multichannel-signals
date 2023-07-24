
#include "tests_datetime_utils.h"
#include <math.h>
#include <assert.h>

void TestsDatetimeUtils::runAll(){
    std::string start_date_str = "1900-01-01 00:00:00";
    std::string end_date_str = "2036-02-07 06:28:16";

    std::tm start_date = DatetimeUtils::stringToDatetime(start_date_str, DATE_FORMAT);
    std::tm end_date = DatetimeUtils::stringToDatetime(end_date_str, DATE_FORMAT);

    assert(DatetimeUtils::datetimeToString(start_date, DATE_FORMAT) == start_date_str);
    assert(DatetimeUtils::datetimeToString(end_date, DATE_FORMAT) == end_date_str);

    assert(DatetimeUtils::compareDates(start_date, start_date) == 0);
    assert(DatetimeUtils::compareDates(start_date, end_date) == 1);
    assert(DatetimeUtils::compareDates(end_date, start_date) == -1);

    std::tm start_date_plus_1_second = DatetimeUtils::stringToDatetime("1900-01-01 00:00:01", DATE_FORMAT);
    std::tm start_date_plus_50_years = DatetimeUtils::stringToDatetime("1950-01-01 00:00:00", DATE_FORMAT);
    std::tm start_date_plus_100_years = DatetimeUtils::stringToDatetime("2000-01-01 00:00:00", DATE_FORMAT);
    std::tm start_date_plus_110_years = DatetimeUtils::stringToDatetime("2010-01-01 00:00:00", DATE_FORMAT);
    std::tm start_date_plus_120_years = DatetimeUtils::stringToDatetime("2020-01-01 00:00:00", DATE_FORMAT);
    std::tm start_date_plus_136_years = DatetimeUtils::stringToDatetime("2036-01-01 00:00:00", DATE_FORMAT);
    std::tm example_date = DatetimeUtils::stringToDatetime("2015-01-03 09:15:00", DATE_FORMAT);

    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date) == 0);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_1_second) == 1);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_50_years) == 1577836800);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_100_years) == 3155673600);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_110_years) == 3471292800);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_120_years) == 3786825600);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, start_date_plus_136_years) == 4291747200);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, example_date) == 3629265300);
    assert(DatetimeUtils::mapDatetimeToSeconds(start_date, end_date) == pow(2,32));

    assert(DatetimeUtils::compareDates(start_date, DatetimeUtils::mapSecondsToDatetime(start_date, 0)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_1_second, DatetimeUtils::mapSecondsToDatetime(start_date, 1)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_50_years, DatetimeUtils::mapSecondsToDatetime(start_date, 1577836800)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_100_years, DatetimeUtils::mapSecondsToDatetime(start_date, 3155673600)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_110_years, DatetimeUtils::mapSecondsToDatetime(start_date, 3471292800)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_120_years, DatetimeUtils::mapSecondsToDatetime(start_date, 3786825600)) == 0);
    assert(DatetimeUtils::compareDates(start_date_plus_136_years, DatetimeUtils::mapSecondsToDatetime(start_date, 4291747200)) == 0);
    assert(DatetimeUtils::compareDates(example_date, DatetimeUtils::mapSecondsToDatetime(start_date, 3629265300)) == 0);
    assert(DatetimeUtils::compareDates(end_date, DatetimeUtils::mapSecondsToDatetime(start_date, pow(2,32))) == 0);
}

const std::string TestsDatetimeUtils::DATE_FORMAT = "%Y-%m-%d %H:%M:%S";
