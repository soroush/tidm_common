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

#ifndef TIDM_HISTORICAL_STAT_HPP
#define TIDM_HISTORICAL_STAT_HPP

#include <valarray>
#include <chrono>
#include <ostream>
#include <mutex>
#include <atomic>
#include <thread>
#include <cstdint>

namespace tidm {

template <typename TYPE = float>
class historical_stat {
        template <typename T> friend
        std::ostream &operator<<(std::ostream &os, const historical_stat<T> &h);
    public:
        class stat {
            public:
                stat():
                    sum{0},
                    max{0},
                    min{0},
                    avg{0.0} {
                }
                TYPE sum;
                TYPE max;
                TYPE min;
                double avg;
        };
        enum class unit_type {
            SEC = 0,
            MIN = 1,
            HOUR = 2
        };
    private:
        std::atomic<bool> _is_alive;
        std::thread _shift_thread;
        std::mutex  _lock;

        std::valarray<TYPE> _data;
        std::valarray<uint64_t> _count;
        std::valarray<TYPE> _min;
        std::valarray<TYPE> _max;

        int _unit_size;
        unit_type _type;

        void shift_worker();
    public:
        historical_stat();
        historical_stat(int unit_s, size_t data_size, unit_type t);
        void init(int unit_size, size_t data_size, unit_type t);
        void reset(size_t t);
        void update(TYPE val);
        typename historical_stat<TYPE>::stat get_aggregate_stat(uint16_t units,
                uint16_t start = 0);
        typename historical_stat<TYPE>::stat get_stat(uint16_t units, uint16_t start = 0);
        void get_all_stat(
            historical_stat<TYPE>::stat &count_stat,
            historical_stat<TYPE>::stat &aggr_stat,
            uint16_t units, uint16_t start = 0);
        std::valarray<TYPE> get_data(int units);
};

template <typename T = float>
std::ostream & operator<<(std::ostream &os, const tidm::historical_stat<T> &h);
template <typename T = float>
std::ostream & operator<<(std::ostream &os,
                          const typename tidm::historical_stat<T>::unit_type &h);
template <typename T = float>
std::ostream & operator<<(std::ostream &os,
                          const typename tidm::historical_stat<T>::stat &s);
}

#include "historical_stat.tpp"

#endif // header guard
