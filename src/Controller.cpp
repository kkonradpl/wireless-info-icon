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

#include <string>
#include <cstring>
#include <cmath>
#include <linux/wireless.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Controller.hpp"

Controller::Controller(const std::string &_name)
    : name(_name)
{
    this->fd = socket(AF_INET, SOCK_DGRAM, 0);
}

Controller::~Controller()
{
    if (this->fd >= 0)
    {
        close(this->fd);
    }
}

const std::string&
Controller::getName() const
{
    return this->name;
}

bool
Controller::isAvailable()
{
    iwreq request;
    this->prepare(&request);

    const int ret = ioctl(this->fd, SIOCGIWNAME, &request);
    return ret >= 0;
}

bool
Controller::getRxPower(int &rxOut)
{
    iwreq request;
    this->prepare(&request);

    iw_statistics stats;
    request.u.data.pointer = &stats;
    request.u.data.length = sizeof(iw_statistics);

    if (ioctl(this->fd, SIOCGIWSTATS, &request) >= 0 &&
        stats.qual.updated & IW_QUAL_DBM)
    {
        rxOut = stats.qual.level - 256;
        return true;
    }

    return false;
}

bool
Controller::getTxPower(int &txOut)
{
    iwreq request;
    this->prepare(&request);
    request.u.txpower.flags = 0;
    if (ioctl(this->fd, SIOCGIWTXPOW, request) < 0)
    {
        return false;
    }

    if (request.u.txpower.flags & IW_TXPOW_MWATT)
    {
        txOut = (int)round(10.0 * log10(request.u.txpower.value));
    }
    else
    {
        txOut = request.u.txpower.value;
    }

    return true;
}

bool
Controller::setTxPower(int tx)
{   
    iwreq request;
    this->prepare(&request);
    request.u.txpower.value = -1;
    request.u.txpower.fixed = 1;
    request.u.txpower.disabled = 0;
    request.u.txpower.flags = IW_TXPOW_DBM;
    request.u.txpower.value = tx;

    const int ret = ioctl(this->fd, SIOCSIWTXPOW, request);
    return ret >= 0;
}

void
Controller::prepare(iwreq *request)
{
    memset(request, 0, sizeof(iwreq));
    snprintf(request->ifr_name, IFNAMSIZ, "%s", this->name.c_str());
}
