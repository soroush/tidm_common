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

#include <algorithm>
#include <functional>
#include <chrono>
#include <tidm/utils/exception/exception.hpp>

template <typename TYPE>
tidm::historical_stat<TYPE>::historical_stat()
{
    _unit_size = 0;
    _type = tidm::historical_stat<TYPE>::unit_type::SEC;
}

template <typename TYPE>
tidm::historical_stat<TYPE>::historical_stat(int unit_s, size_t  data_size,
        tidm::historical_stat<TYPE>::unit_type t)
{
    init(unit_s, data_size, t);
}

template <typename TYPE>
void tidm::historical_stat<TYPE>::init(int unit_size, size_t data_size,
                                       tidm::historical_stat<TYPE>::unit_type t)
{
    _unit_size = unit_size;
    _type = t;
    reset(data_size);
    _is_alive = true;
    _shift_thread = std::thread(&tidm::historical_stat<TYPE>::shift_worker, this);
    _shift_thread.detach();
}

template <typename TYPE>
void tidm::historical_stat<TYPE>::shift_worker()
{
    uint64_t sleep_time = 0;
    switch(_type) {
    case tidm::historical_stat<TYPE>::unit_type::SEC:
        sleep_time = _unit_size;
        break;
    case tidm::historical_stat<TYPE>::unit_type::MIN:
        sleep_time = _unit_size * 60;
        break;
    case tidm::historical_stat<TYPE>::unit_type::HOUR:
        sleep_time = _unit_size * 60 * 60;
        break;
    };
    //    std::this_thread::sleep_until(begin + std::chrono::microseconds(first_time_u));
    //    begin = std::chrono::high_resolution_clock::now();
    while(_is_alive) {
        /*
         * this is not a good method for sleeping. Because if the unit type is HOUR
         * and some finalize method sets `is_alive' flag to true, then this loop
         * will end about some minutes later! It needs some sort of signaling machanisms
         * or a smarter sleep(periodic check!)
         * */
        auto begin = std::chrono::system_clock::now();
        std::time_t tp = std::chrono::system_clock::to_time_t(begin);
        tp += sleep_time;
        std::chrono::system_clock::time_point target_tp = std::chrono::system_clock::from_time_t(tp);
        std::this_thread::sleep_until(target_tp);
        {
            std::lock_guard<std::mutex> l(_lock) ;
            _data = _data.shift(-1);
            _count = _count.shift(-1);
            _min = _min.shift(-1);
            _max = _max.shift(-1);
        }
    }
}

template <typename TYPE>
void tidm::historical_stat<TYPE>::reset(size_t len)
{
    std::lock_guard<std::mutex> l(_lock) ;
    TYPE default_val = 0;
    uint64_t def_val = 0;
    _data = std::valarray<TYPE>(default_val, len);
    _count = std::valarray<uint64_t>(def_val, len);
    _min = std::valarray<TYPE>(default_val, len);
    _max = std::valarray<TYPE>(default_val, len);
}

template <typename TYPE>
void tidm::historical_stat<TYPE>::update(TYPE val)
{
    std::lock_guard<std::mutex> l(_lock) ;
    _data[0] += val;
    if(_count[0] == 0) {
        _min[0] = val;
        _max[0] = val;
    } else {
        _min[0] = std::min(val, _min[0]);
        _max[0] = std::max(val, _max[0]);
    }
    _count[0]++;
}


template <typename TYPE>
typename tidm::historical_stat<TYPE>::stat tidm::historical_stat<TYPE>::get_aggregate_stat(uint16_t units,
        uint16_t start)
{
    tidm::historical_stat<TYPE>::stat res;
    if(units == 0) {
        return res;
    }
    if(start < 0) {
        throw std::runtime_error("tidm::historical_stat<TYPE>::get_aggregate_stat: invalid value for `start'(negative value)");
    }
    {
        std::lock_guard<std::mutex> l(_lock);
        units = std::min(units, static_cast<uint16_t>(_data.size()));
        res.sum = static_cast<std::valarray<TYPE>>(_data[std::slice(start, units, 1)]).sum();
        uint64_t count = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).sum();
        res.avg = count != 0 ? res.sum / static_cast<double>(count) : 0 ;
        res.min = static_cast<std::valarray<TYPE>>(_min[std::slice(start, units, 1)]).min();
        res.max = static_cast<std::valarray<TYPE>>(_max[std::slice(start, units, 1)]).max();
    }
    return res;
}


template <typename TYPE>
typename tidm::historical_stat<TYPE>::stat tidm::historical_stat<TYPE>::get_stat(uint16_t units, uint16_t start)
{
    tidm::historical_stat<TYPE>::stat res;
    if(units == 0) {
        return res;
    }
    if(start < 0) {
        throw std::runtime_error("tidm::historical_stat<TYPE>::get_aggregate_stat: invalid value for `start'(negative value)");
    }
    {
        std::lock_guard<std::mutex> l(_lock);
        units = std::min(units, static_cast<uint16_t>(_data.size()));
        res.sum = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).sum();
        res.avg = res.sum / static_cast<double>(units);
        res.min = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).min();
        res.max = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).max();
    }
    return res;
}

template <typename TYPE>
void tidm::historical_stat<TYPE>::get_all_stat(tidm::historical_stat<TYPE>::stat &count_stat,
        tidm::historical_stat<TYPE>::stat &aggr_stat,
        uint16_t units, uint16_t start)
{
    if(start < 0 or units == 0) {
        throw std::runtime_error("tidm::historical_stat<TYPE>::get_aggregate_stat: invalid value for `start'(negative value) or `units' == 0");
    }
    {
        std::lock_guard<std::mutex> l(_lock);
        units = std::min(units, static_cast<uint16_t>(_data.size()));
        count_stat.sum = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).sum();
        count_stat.avg = count_stat.sum / static_cast<double>(units);
        count_stat.min = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).min();
        count_stat.max = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).max();
        aggr_stat.sum = static_cast<std::valarray<TYPE>>(_data[std::slice(start, units, 1)]).sum();
        uint64_t count = static_cast<std::valarray<uint64_t>>(_count[std::slice(start, units, 1)]).sum();
        aggr_stat.avg = count != 0 ? aggr_stat.sum / static_cast<double>(count) : 0 ;
        aggr_stat.min = static_cast<std::valarray<TYPE>>(_min[std::slice(start, units, 1)]).min();
        aggr_stat.max = static_cast<std::valarray<TYPE>>(_max[std::slice(start, units, 1)]).max();
    }
}

template <typename TYPE>
std::valarray<TYPE> tidm::historical_stat<TYPE>::get_data(int units)
{
    std::valarray<TYPE> res;
    std::lock_guard<std::mutex> l(_lock) ;
    units = std::min(units, (int)_data.size());
    res = _data[std::slice(0, units, 1)];
    return res;
}





template <typename TYPE>
std::ostream &tidm::operator<<(std::ostream &os,
                               const typename tidm::historical_stat<TYPE>::unit_type &h)
{
    switch(h) {
    case tidm::historical_stat<TYPE>::unit_type::SEC:
        os << "SEC";
        break;
    case tidm::historical_stat<TYPE>::unit_type::MIN:
        os << "MIN";
        break;
    case tidm::historical_stat<TYPE>::unit_type::HOUR:
        os << "HOUR";
        break;
    default:
        os << "Invaild";
        break;
    };
    return os;
}

template <typename TYPE>
std::ostream &tidm::operator<<(std::ostream &os, const tidm::historical_stat<TYPE> &h)
{
    os << " unit_size:" << h._unit_size << " type: " ;
    typename tidm::historical_stat<TYPE>::unit_type x = h._type;
    tidm::operator<< <TYPE>(os, x);
    os << " len:" << h._data.size();
    os << " {";
    for(const auto &x : h._data) {
        os << x << " ";
    }
    os << "}";
    return os;
}

template <typename TYPE>
std::ostream &tidm::operator<<(std::ostream &os,
                               const typename tidm::historical_stat<TYPE>::stat &s)
{
    os << " sum:" << s.sum
       << " max:" << s.max
       << " min:" << s.min
       << " avg:" << s.avg ;
    return os;
}


