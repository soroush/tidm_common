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


#include "../src/historical_stat.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void check(tidm::historical_stat<float>& st , int c, int sec, int over_last) {
    for(int i=0; i< c; i++) {
        st.update(10.0);
    }
    tidm::historical_stat<float>::stat stt;
    tidm::historical_stat<float>::stat agg;

    st.get_all_stat( stt, agg, over_last);

    std::cerr << stt;
    std::cerr << "\n  stat (" << over_last << ") ";
    std::cerr << stt;
    std::cerr << "\n  aggr (" << over_last << ") ";
    std::cerr << agg;
    std::cerr << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    tidm::historical_stat<float> st(1,7,tidm::historical_stat<float>::unit_type::SEC);
    int over_last= 3;
    check(st, 2, 1, over_last);
    check(st, 1, 1, over_last);
    check(st, 1, 1, over_last);
    check(st, 1, 1, over_last);
    check(st, 3, 1, over_last);
    check(st, 1, 1, over_last);
    check(st, 3, 1, over_last);
    check(st, 1, 1, over_last);
    check(st, 4, 1, over_last);
    check(st, 3, 1, over_last);
    check(st, 2, 1, over_last);
    check(st, 2, 1, over_last);
    return 0;
}
