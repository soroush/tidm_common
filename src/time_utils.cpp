/*
 * libtidm_common - Common tools and utilities of the TIDM/OI
 * Copyright (C) 2017 TIDM Co.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "time_utils.hpp"
#include <iomanip>
#include <vector>

using namespace std::chrono;

using days = std::chrono::duration<int, std::ratio<60 * 60 * 24>>;

std::string tidm::time_utils::duration_to_string(system_clock::time_point prior,
        system_clock::time_point latter)
{
    return tidm::time_utils::duration_to_string(duration_cast<nanoseconds>(latter - prior));
}

std::string tidm::time_utils::duration_to_string(nanoseconds ns)
{
    std::stringstream out;
    bool printed_earlier = false;
    auto days_count = duration_cast<days>(ns).count();
    auto hours_count = duration_cast<hours>(ns).count() - days_count * 24;
    auto minutes_count = duration_cast<minutes>(ns).count() - hours_count * 60;
    auto seconds_count = duration_cast<seconds>(ns).count() - minutes_count * 60;
    auto ms_count = duration_cast<milliseconds>(ns).count() - seconds_count * 1000;
    if(days_count >= 1) {
        printed_earlier = true;
        out << days_count << (1 != days_count ? " days" : " day") << ' ';
    }
    if(printed_earlier || hours_count >= 1) {
        printed_earlier = true;
        out << hours_count << (1 != hours_count ? " hours" : " hour") << ' ';
    }
    if(printed_earlier || minutes_count >= 1) {
        printed_earlier = true;
        out << minutes_count << (1 != minutes_count ? " minutes" : " minute") << ' ';
    }
    if(printed_earlier || seconds_count >= 1) {
        printed_earlier = true;
        out << seconds_count << (1 != seconds_count ? " seconds" : " second") << ' ';
    }
    if(printed_earlier || ms_count >= 1) {
        printed_earlier = true;
        out << ms_count << (1 != ms_count ? " milliseconds" : " millisecond");
    }
    return out.str();
}

std::string tidm::time_utils::timet_to_string(time_t t)
{
    struct tm my_tm;
    localtime_r(&t , &my_tm);
    char buff[70] = {0};
    strftime(buff, sizeof(buff), "%Y/%m/%d %H:%M:%S", &my_tm);
    return std::string(buff);
}

time_t tidm::time_utils::string_to_timet(std::string str)
{
    time_t t = 0;
    struct tm my_tm;
    strptime(str.c_str(), "%Y/%m/%d %H:%M:%S", &my_tm);
    my_tm.tm_isdst = -1;
    t = mktime(&my_tm);
    return(t);
}

std::string tidm::time_utils::get_elapsed_time(timespec start)
{
    timespec end;
    clock_gettime(CLOCK_REALTIME, &end);
    double res = ((end.tv_sec * 1000 - start.tv_sec * 1000) +
                  (end.tv_nsec / 1000000.0 - start.tv_nsec / 1000000.0)) / 1000.0;
    std::stringstream sstr;
    sstr << std::setprecision(2) << res ;
    return sstr.str();
}

std::string tidm::time_utils::get_current_time(const std::string &format)
{
    return tidm::time_utils::get_time_str(std::chrono::system_clock::now(), false, format);
}


std::string tidm::time_utils::get_time_str(const std::chrono::system_clock::time_point &tp,
        bool is_utc,
        const std::string &format)
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    time_t seconds = std::chrono::duration_cast<std::chrono::seconds>(ms).count();
    std::string time;
#ifdef STD_PUT_TIME
    if(is_utc) {
        time = put_time(gmtime(&seconds), "%Y-%m-%d_%H:%M:%S");
    } else {
        time = put_time(localtime(&seconds), "%Y-%m-%d_%H:%M:%S");
    }
#else
    std::vector<char> buffer(50);
    if(is_utc) {
        if(strftime(buffer.data(),
                    buffer.size(),
                    "%Y-%m-%d_%H_%M_%S",
                    gmtime(&seconds))) {
            time = buffer.data();
        } else {
            time = "2000-00-00 12:00:00";
        }
    } else {
        if(strftime(buffer.data(),
                    buffer.size(),
                    "%Y-%m-%d_%H_%M_%S",
                    localtime(&seconds))) {
            time = buffer.data();
        } else {
            time = "2000-00-00 12:00:00";
        }
    }
#endif
    return time;
}

std::string tidm::time_utils::get_time_str(struct tm t)
{
    std::stringstream s;
    s << ((t.tm_hour < 10) ? "0" : "") << t.tm_hour << ":"
      << ((t.tm_min < 10) ? "0" : "") << t.tm_min << ":"
      << ((t.tm_sec < 10) ? "0" : "") << t.tm_sec;
    return s.str();
}


std::string tidm::time_utils::get_date_str(struct tm t)
{
    std::stringstream s;
    s << t.tm_year + 1900
      << ((t.tm_mon < 9) ? "0" : "") << t.tm_mon + 1
      << ((t.tm_mday < 10) ? "0" : "") << t.tm_mday;
    return s.str();
}

// ostream operators

template <typename clock>
std::ostream &insert_time(std::ostream &os, const typename clock::time_point &time_point)
{
    std::time_t t = clock::to_time_t(time_point);
    tm tmm;
    gmtime_r(&t, &tmm);
    char buf[100] = {0};
    char buf1[100] = {0};
    strftime(buf, 100, "%Y/%m/%d %H:%M:%S", &tmm);
    strftime(buf1, 100, "%z(%Z)", &tmm);
    int temp_us = std::chrono::duration_cast<std::chrono::microseconds>(time_point.time_since_epoch()).count() % 1000000;
    std::string str_us = std::to_string(temp_us);
    size_t len = str_us.size();
    if(len < 6) {
        str_us.insert(0, 6 - len, '0');
    }
    os  << buf << "." << str_us << " " << buf1 ;
    return os;
}


std::ostream &operator<<(std::ostream &os, const std::chrono::high_resolution_clock::time_point &time_point)
{
    return insert_time<std::chrono::high_resolution_clock>(os, time_point);
}

std::ostream &operator<<(std::ostream &os, const tm &t)
{
    char dt[512] = {0};
    strftime(dt, 512, "%Y-%m-%d %H:%M:%S", &t);
    os << dt;
    return os;
}

std::ostream &operator<<(std::ostream &os, const timespec &ts)
{
    time_t t = static_cast<time_t>(ts.tv_sec);
    tm tt;
    localtime_r(&t, &tt);
    std::stringstream sstr;
    sstr << (uint64_t)(ts.tv_nsec / 1000.0);
    std::string str = sstr.str().substr(0, 6);
    os << tt;
    os << "." << str;
    return os;
}

