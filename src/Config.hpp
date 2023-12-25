/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  wireless-info-icon
 *  Copyright (C) 2023  Konrad Kosmatka
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 3
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef WIRELESS_INFO_CONFIG_H
#define WIRELESS_INFO_CONFIG_H
#include <string>
#include <vector>

struct Config
{
    std::string name{"wlan0"};
    std::string font{"DejaVu Sans Mono"};
    std::string color{"gray"};
    int fontSize{0};
    int iconSize{32};
    int interval{1000};
    std::vector<int> txPowerSteps{20, 10, 0};
    int txPowerRefreshInterval{30000};
};

#endif
