/*  SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  wireless-info-icon
 *  Copyright (C) 2023-2024  Konrad Kosmatka
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

#ifndef WIRELESS_INFO_CONTROLLER_H
#define WIRELESS_INFO_CONTROLLER_H
#include <string>

typedef struct iwreq iwreq;

class Controller
{
public:
    Controller(const std::string&);
    ~Controller();
    const std::string& getName() const;
    bool isAvailable();
    bool getRxPower(int&);
    bool getTxPower(int&);
    bool setTxPower(int);

private:
    void prepare(iwreq*);
    bool ioctl(iwreq*, int);
    std::string name;
    int fd;
};

#endif
