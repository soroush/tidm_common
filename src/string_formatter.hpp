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


#ifndef TIDM_STRING_FORMATTER_HPP
#define TIDM_STRING_FORMATTER_HPP

#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <sstream>

namespace tidm {
class string_utils {
    private:
        string_utils();
    public:
        template<typename ... Args>
        static std::string format(const std::string &format, Args ... args) {
            size_t size = std::snprintf(nullptr, 0, format.c_str(),
                                        args ...) + 1;  // Extra space for '\0'
            std::unique_ptr<char[]> buf(new char[ size ]);
            std::snprintf(buf.get(), size, format.c_str(), args ...);
            return std::string(buf.get(),
                               buf.get() + size - 1);  // We don't want the '\0' inside
        }
        static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
        static std::vector<std::string> split(const std::string &s, char delim);
};
}

#endif
