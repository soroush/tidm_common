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


#ifndef TIDM_BINARY_SEMAPHORE_HPP
#define TIDM_BINARY_SEMAPHORE_HPP

#include <mutex>
#include <condition_variable>

namespace tidm {

class binary_semaphore {
    public:
        explicit binary_semaphore(int init_count = _count_max);
        // P-operation / acquire
        void wait();
        bool try_wait();
        // V-operation / release
        void signal();
        // Lockable requirements
        void lock();
        bool try_lock();
        void unlock();
    private:
        constexpr static uint8_t _count_max = 1;
        uint8_t _count;
        std::mutex _mutex;
        std::condition_variable _cv;
};

}

#endif
