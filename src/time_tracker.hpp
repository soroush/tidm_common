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

#ifndef TIDM_TIME_TRACKER_HPP
#define TIDM_TIME_TRACKER_HPP

#include <mutex>
#include <string>
#include <vector>
#include <valarray>
#include <map>

namespace tidm {
class time_tracker {
    public:
        class tracker { //shoule be used as a local variable! NOT thread safe
                friend class time_tracker;
            private:
                std::chrono::high_resolution_clock::time_point _last;
                std::valarray<uint64_t> _times;
                std::vector<std::string> _points;
                bool _enable;
                size_t _counter;
                tracker() = delete;
            public:
                tracker(int size, bool enable = false);
                void set_point(const std::string &name);
                void enable();
                void disable();
        };
        time_tracker(bool enable = false);
        void update(const tracker &t);
        std::map<std::string, uint64_t>  get_stat(bool reset);
        std::string get_all_stat(bool reset);
        void enable();
        void disable();
    private:
        std::mutex _lock;
        int _count;
        bool _init;
        bool _enable;
        std::valarray<uint64_t> _times;
        std::vector<std::string> _points;
        time_tracker(const time_tracker &) = delete;
        time_tracker &operator=(const time_tracker &) = delete;

};
}

#endif // namespace tidm
