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

#include "binary_semaphore.hpp"

tidm::binary_semaphore::binary_semaphore(int init_count)
    : _count(init_count) {}

void tidm::binary_semaphore::wait()
{
    std::unique_lock<std::mutex> lk(_mutex);
    _cv.wait(lk, [ = ] { return 0 < _count; });
    --_count;
}

bool tidm::binary_semaphore::try_wait()
{
    std::lock_guard<std::mutex> _(_mutex);
    if(0 < _count) {
        --_count;
        return true;
    } else {
        return false;
    }
}

// V-operation / release
void tidm::binary_semaphore::signal()
{
    std::lock_guard<std::mutex> _(_mutex);
    if(_count < _count_max) {
        ++_count;
        _cv.notify_one();
    }
}

// Lockable requirements
void tidm::binary_semaphore::lock()
{
    wait();
}

bool tidm::binary_semaphore::try_lock()
{
    return try_wait();
}

void tidm::binary_semaphore::unlock()
{
    signal();
}
