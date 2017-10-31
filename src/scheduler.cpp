#include "scheduler.hpp"
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iterator>
//#include <iostream>
#include "prettyprint.hpp"
#include "string_formatter.hpp"

const uint8_t tidm::scheduler::_days_in_month[12] = {
    31, // Jan
    28, // Feb
    31, // Mar
    30, // Apr
    31, // May
    30, // Jun
    31, // Jul
    31, // Aug
    30, // Sep
    31, // Oct
    30, // Nov
    31, // Dec
};

tidm::scheduler::scheduler()
{
}

tidm::scheduler::scheduler(const std::string &cron_entry)
{
    this->schedule(cron_entry);
}

void tidm::scheduler::schedule(const std::string &entry)
{
    std::istringstream iss{entry};
    std::vector<std::string> tokens = tidm::string_utils::split(entry, ' ');
    this->minute = tokens[4];
    this->hour = tokens[3];
    this->day = tokens[2];
    this->month = tokens[1];
    this->weekday = tokens[0];
}

std::tm tidm::scheduler::difference(std::tm *now) const
{
    // Integer representation of cron entry
    uint8_t i_minute = std::atoi(minute.c_str());
    uint8_t i_hour = std::atoi(hour.c_str());
    uint8_t i_day = std::atoi(day.c_str());
    uint8_t i_month = std::atoi(month.c_str());
    uint8_t i_weekday = std::atoi(weekday.c_str());
    std::tm next = *now;
    // BUG
    next.tm_min += 1;
    bool done = false;
    while(!done) {
        if(minute != "*" and next.tm_min != i_minute) {
            if(next.tm_min > i_minute) {
                // Note: Don't worry about non-normalized BDTs (e.g. 25:60:61);
                // mktime does perform normalization on its values.
                next.tm_hour += 1;
            }
            next.tm_min = i_minute;
        }
        if(hour != "*" and next.tm_hour != i_hour) {
            if(next.tm_hour > i_hour) {
                next.tm_hour = i_hour;
                next.tm_mday++;
                next.tm_min = 0;
                continue;
            }
            next.tm_hour = i_hour;
            next.tm_min = 0;
            continue;
        }
        if(weekday != "*" and next.tm_wday != i_weekday) {
            uint8_t deltaDays = i_weekday -
                                next.tm_wday; //assume weekday is 0=sun, 1 ... 6=sat
            if(deltaDays < 0) {
                deltaDays += 7;
            }
            next.tm_wday += deltaDays;
            next.tm_hour = 0;
            next.tm_min = 0;
            continue;
        }
        if(day != "*" and next.tm_mday != i_day) {
            if(next.tm_mday > i_day or
                    !has_day(next.tm_mon, i_day, is_leap(next.tm_year + 1900))) {
                next.tm_mon++;
                next.tm_mday = 1; //assume days 1..31
                next.tm_hour = 0;
                next.tm_min = 0;
                continue;
            }
            next.tm_mday = i_day;
            next.tm_hour = 0;
            next.tm_min = 0;
            continue;
        }
        if(month != "*" and next.tm_mon != i_month) {
            if(next.tm_mon > i_month) {
                // next.tm_mon += (12 - next.tm_mon + i_month);
                next.tm_year++;
                next.tm_mon = i_month;
                next.tm_mday = 1; //assume days 1..31
                next.tm_hour = 0;
                next.tm_min = 0;
                continue;
            }
            next.tm_mon = i_month;
            next.tm_mday = 1;
            next.tm_hour = 0;
            next.tm_min = 0;
            continue;
        }
        done = true;
    }
    //next.tm_mon--;
    next.tm_sec = 0;
    return next;
}

uint32_t tidm::scheduler::wait_time() const
{
    std::chrono::system_clock::time_point now_point  =
        std::chrono::system_clock::now();
    std::time_t now = std::time(nullptr);
    std::tm *now_info = localtime(&now);
    std::tm then_info = difference(now_info);
    std::time_t then = std::mktime(&then_info);
    std::chrono::system_clock::time_point then_point =
        std::chrono::system_clock::from_time_t(then);
    int diff = std::chrono::duration_cast<std::chrono::seconds>
               (then_point - now_point).count();
    return static_cast<uint32_t>(std::abs(diff));
}

bool tidm::scheduler::has_day(const uint8_t &m, const uint8_t &d,
                              const bool &is_leap) const
{
    if(m != 2) {
        return d <= _days_in_month[m];
    } else {
        if(is_leap) {
            return d <= _days_in_month[m] + 1;
        } else {
            return d <= _days_in_month[m];
        }
    }
}

bool tidm::scheduler::is_leap(const uint16_t &year) const
{
    if(year % 4 != 0) {
        return false;
    } else if(year % 100 != 0) {
        return true;
    } else if(year % 400 != 0) {
        return false;
    } else {
        return true;
    }
}
