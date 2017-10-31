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

#include "time_tracker.hpp"
#include <chrono>
#include <sstream>

tidm::time_tracker::time_tracker(bool enable): _enable(enable)
{
    _count = 0;
    _init = false;
}

void tidm::time_tracker::update(const tidm::time_tracker::tracker &t)
{
    if(!_enable) {
        return;
    }
    std::lock_guard<std::mutex> lk(_lock);
    if(_init == false) {
        _times = t._times;
        _points = t._points;
        _init = true;

    } else {
        _times += t._times;
    }
    _count++;
}

void tidm::time_tracker::enable()
{
    _enable = true;
}

void tidm::time_tracker::disable()
{
    _enable = false;
}

std::map<std::string, uint64_t>  tidm::time_tracker::get_stat(bool reset)
{
    std::map<std::string, uint64_t> res;
    if(_count == 0) {
        return res;
    }
    {
        std::lock_guard<std::mutex> lk(_lock);
        for(size_t i = 0; i < _times.size() ; i++) {
            res[_points[i]] = _times[i] / (double)_count;
            _times[i] = 0;
        }
        if(reset) {
            _times = 0;
        }
        _count = 0;
    }
    return res;
}

std::string tidm::time_tracker::get_all_stat(bool reset)
{
    if(_count == 0) {
        return "";
    }
    std::stringstream sstr;
    auto st = get_stat(reset);
    std::pair<std::string, uint64_t> max("", st.begin()->second);
    std::pair<std::string, uint64_t> min("", st.begin()->second);
    uint64_t total = 0;
    for(const auto &c : st) {
        if(c.second > max.second) {
            max = c;
        } else if(c.second < min.second) {
            min = c;
        }
        total += c.second;
        sstr << c.first << ":" << c.second << "us  ";
    }
    sstr << "  MIN:" << min.first <<  ":" << min.second << "us ";
    sstr << "  MAX:" << max.first <<  ":" << max.second  << "us ";
    sstr << "  TOTAL:" << total << "us " ;
    return sstr.str();
}

tidm::time_tracker::tracker::tracker(int size, bool enable): _times(uint64_t(0), size), _points(size, ""), _enable(enable), _counter(0)
{
    _last = std::chrono::high_resolution_clock::now();
}

void tidm::time_tracker::tracker::enable()
{
    _enable = true;
}

void tidm::time_tracker::tracker::disable()
{
    _enable = false;
}

void tidm::time_tracker::tracker::set_point(const std::string &pnt)
{
    auto nw = std::chrono::high_resolution_clock::now();
    if(_times.size() == _counter) {
        return;
    }
    _times[_counter] = std::chrono::duration_cast<std::chrono::microseconds>(nw - _last).count();
    _points[_counter] = pnt;
    _counter++;
    _last = nw;
}
