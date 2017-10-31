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

#include "tracer.hpp"
#include <iostream>
#include <sstream>

void tidm::tracer::update(uint64_t msg_id, int line, int state) {
    std::map<uint64_t, tracer::stat>::iterator it ;
    std::lock_guard<std::mutex> m(_trans_guard);
    it = _trans.find(msg_id);
    if(it == _trans.end()) {
        tracer::stat s;
        s.last_line = line;
        s.state = state;
        s.start = std::chrono::high_resolution_clock::now();
        _trans[msg_id] = s;
    }
    else {
        it->second.last_line = line;
        it->second.state= state;
    }
}

void tidm::tracer::del(uint64_t msg_id) {
    std::lock_guard<std::mutex> m(_trans_guard);
    _trans.erase(msg_id);
}

std::string tidm::tracer::get_stat(std::chrono::seconds duration) {
    std::stringstream sstr;
    auto nw =  std::chrono::high_resolution_clock::now();
    std::map<uint64_t, tracer::stat> trans;
    {
        std::lock_guard<std::mutex> m(_trans_guard);
        trans  =_trans;
    }
    std::map<uint64_t, tracer::stat>::iterator it ;
    for(it = trans.begin(); it != trans.end(); it++) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(nw - it->second.start);
        if(diff > duration) {
            sstr << "id:" << it->first << " line:" << it->second.last_line << " state:" << it->second.state <<
                 " elapsed_time:"<< diff.count() << "s " << std::endl;
        }
    }
    return sstr.str();
}
