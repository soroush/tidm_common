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

#include "stat_info.hpp"
#include <algorithm>
#include <cmath>

tidm::stat_info::stat_info()
{
    reset();
}

void tidm::stat_info::reset()
{
    m_min = 0;
    m_max = 0;
    m_sum = 0;
    m_sum_2 = 0;
    m_count = 0;
}

void tidm::stat_info::set(uint64_t val)
{
    if(m_count == 0) {
        m_min = val;
    } else {
        m_min = std::min(val, m_min);
    }
    m_max   = std::max(val, m_max);
    m_sum   += val;
    m_sum_2 += val * val;
    m_count++;
}

tidm::stat_info  tidm::stat_info::operator+(const stat_info &st)
{
    stat_info res;
    if(m_min == 0 || st.m_min == 0) {
        res.m_min = st.m_min + m_min;
    } else {
        res.m_min = std::min(st.m_min , m_min);
    }
    res.m_max  = std::max(st.m_max , m_max);
    res.m_sum  = st.m_sum   + m_sum;
    res.m_sum_2 = st.m_sum_2 + m_sum_2;
    res.m_count = st.m_count + m_count;
    return res;
}

double tidm::stat_info::avg() const
{
    if(m_count == 0) {
        return 0;
    }
    return m_sum / m_count;
}

uint64_t tidm::stat_info::min()const
{
    return m_min;
}

uint64_t tidm::stat_info::max()const
{
    return m_max;
}

double tidm::stat_info::std()const
{
    if(m_count == 0) {
        return 0;
    }
    double a = avg();
    return sqrt(m_sum_2 / m_count - a * a);
}

uint64_t tidm::stat_info::count()const
{
    return m_count;
}

// IO operators

std::ostream &tidm::stat_info::operator<<(std::ostream &os)
{
    os << " count: " << this->m_count
       << " avg: " << this->avg()
       << " min: " << this->min()
       << " max: " << this->max()
       << " std: " << this->std()
       << " sum: " << this->m_sum
       << " sum_2: " << this->m_sum_2;
    return os;
}

