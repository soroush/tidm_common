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

#include "string_stubs.hpp"
#include <iostream>
#include <clocale>
#include <string>
#include <string.h>
#include <cstdlib>
#include <sstream>

std::wstring tidm::string_utils::to_wstring(const std::string &str)
{

    setlocale(LC_CTYPE, "");
    size_t len = strlen(str.c_str());
    wchar_t *dest = new wchar_t[len + 1];
    dest[len] = 0;
    mbstowcs(dest, str.c_str(), len);
    std::wstring res(dest);
    delete dest;
    return res;
}

std::string tidm::string_utils::to_string(const std::wstring &str)
{
    std::setlocale(LC_ALL, "en_US.utf8");
    size_t len = str.size();
    char *dest = new char[len * 6 + 1];
    memset(dest, 0 , len * 6 + 1);
    wcstombs(dest, str.c_str(), len * 6);
    std::string res(dest);
    delete dest;
    return res;
}
