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

/**
 * @file
 * @author Ashkan Ghassemi <aghasemi@tidm.tosanltd.com>
 * @version 0.5
 *
 * @section LICENSE
 *
 * Copyright (C) 2012 Tosan Intelligent Data Miners
 *
 * @section DESCRIPTION
 *
 * OI server time utilities
 */


#ifndef TIDM_TIME_UTILS_HPP
#define TIDM_TIME_UTILS_HPP

#include <chrono>
#include <sstream>
#include <iostream>

namespace tidm {
namespace time_utils {

std::string duration_to_string(std::chrono::system_clock::time_point prior,
                               std::chrono::system_clock::time_point latter);
std::string duration_to_string(std::chrono::nanoseconds ns);
std::string timet_to_string(time_t t);
time_t string_to_timet(std::string str);
std::string get_elapsed_time(timespec start);
std::string get_current_time(const std::string &format = "");
std::string get_time_str(const std::chrono::system_clock::time_point &time,
                         bool utc = true,
                         const std::string &format = "");
std::string get_time_str(struct tm t);
std::string get_date_str(struct tm t);

}
}

std::ostream &operator<< (std::ostream &os, const std::chrono::high_resolution_clock::time_point &time_point) ;
std::ostream &operator<< (std::ostream &os, const timespec &ts);
std::ostream &operator<< (std::ostream &os, const tm &t);


#endif // TIDM_TIME_UTILS_HPP
