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

#ifndef TIDM_STAT_INFO_HPP
#define TIDM_STAT_INFO_HPP

#include <cstdint>
#include <ostream>

namespace tidm {
class stat_info {
    public:
        std::ostream &operator<<(std::ostream &os);
        stat_info();
        stat_info(const stat_info &) = default;
        virtual ~stat_info() = default;
        double avg() const;
        uint64_t min() const;
        uint64_t max() const;
        double std() const;
        uint64_t count() const;
        virtual stat_info operator+(const stat_info &st);
        virtual void reset();
        virtual void set(uint64_t val);
    protected:
        uint64_t m_min;
        uint64_t m_max;
        long double m_sum;
        long double m_sum_2;
        uint64_t m_count;
};
}

#endif
