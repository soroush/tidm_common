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

#ifndef TIDM_SCHEDULER_HPP
#define TIDM_SCHEDULER_HPP

#include <ctime>
#include <string>

namespace tidm {
class scheduler {
    public:
        scheduler();
        scheduler(const std::string &cron_entry);
        ~scheduler() = default;
        void schedule(const std::string &cron_entry);
        uint32_t wait_time() const;
    private:
        std::tm difference(std::tm *now) const;
        bool has_day(const uint8_t &, const uint8_t &, const bool &) const;
        bool is_leap(const uint16_t &year) const;
        std::string minute;
        std::string hour;
        std::string day;
        std::string month;
        std::string weekday;
        static const uint8_t _days_in_month[12];
};
}

#endif // TIDM_SCHEDULER_HPP
