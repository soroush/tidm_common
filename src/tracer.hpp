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

#ifndef TIDM_TRACER_HPP
#define TIDM_TRACER_HPP

#include <map>
#include <chrono>
#include <mutex>
#include <cstdint>

namespace tidm {
class tracer {
  private:
    struct stat {
        int last_line;
        int state;
        std::chrono::high_resolution_clock::time_point start;
    };
    std::map<uint64_t, tracer::stat> _trans;
    std::mutex _trans_guard;
  public:
    void update(uint64_t msg_id, int line, int state = 0);
    void del(uint64_t msg_id);
    std::string get_stat(std::chrono::seconds duration);
};
}
#endif

