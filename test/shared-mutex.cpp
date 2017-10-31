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

#include <queue>
#include "../src/semaphore.hpp"
#include "../src/shared_mutex.hpp"
#include <thread>
#include <vector>
#include <iostream>

std::string shared_str;
tidm::shared_mutex _lock;

void reader()
{
    std::cerr << "reader startted" << std::endl;
    while(1) {
        std::string str = "";
        _lock.lock_shared();
        {
            str = shared_str;
        }
        _lock.unlock_shared();
        std::cerr << str << std::endl;
    }

}

void writer()
{
    std::cerr << "writer startted" << std::endl;
    int i = 0;
    std::string str;//
    while(1) {
        str = std::to_string(i++);
        _lock.lock();
        {
            shared_str = str;
        }
        _lock.unlock();
    }

}

int main()
{
    std::vector<std::thread *> threads;
    for(size_t i = 0; i < 10; i++) {
        threads.push_back(new std::thread(reader));
    }
    for(size_t i = 0; i < 3; i++) {
        threads.push_back(new std::thread(writer));
    }
    for(size_t i = 0; i < threads.size(); i++) {
        threads[i]->join();
    }
    return 0;
}
