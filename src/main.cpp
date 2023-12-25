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

#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include "Icon.hpp"

#define WIRELESS_INFO_ICON_VERSION "1.0"

template<typename T>
std::ostream& operator<<(std::ostream& stream, std::vector<T> v)
{
    for (std::size_t i = 0; i < v.size(); i++)
    {
        stream << v[i] << (i == v.size() - 1 ? "" : "/");
    }

    return stream;
}

void
printUsage()
{
    Config defConf;

    std::cout << "wireless-info-icon " WIRELESS_INFO_ICON_VERSION << std::endl;
    std::cout << "usage: wireless-info-icon [-i wlan] [-f font] [-c color] [-h size]" << std::endl;
    std::cout << "                          [-s size] [-u interval] [-p txp] [-r refresh]" << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "-i  wlan interface name (default: " << defConf.name << ")" << std::endl;
    std::cout << "-f  font name (default: " << defConf.font << ")" << std::endl;
    std::cout << "-c  font color (default: " << defConf.color << ")" << std::endl;
    std::cout << "-S  font size [px] (default: " << defConf.fontSize << " (auto))" << std::endl;
    std::cout << "-s  icon size [px] (default " << defConf.iconSize << " px)" << std::endl;
    std::cout << "-u  icon update interval [ms] (default: " << defConf.interval << " ms)" << std::endl;
    std::cout << "-p  tx power step [dBm] (default " << defConf.txPowerSteps << " dBm)" << std::endl;
    std::cout << "    (firstly added step is the initial value)" << std::endl;
    std::cout << "-r  tx power refresh interval [ms] (default " << defConf.txPowerRefreshInterval << " ms)" << std::endl;
    std::cout << "    (for cards that do not obey the fixed tx power in the long-term)" << std::endl;
    std::cout << std::endl;
    std::cout << "example: wireless-info-icon -i wlp2s0 -c white -p 22 -p 12 -p 3" << std::endl;
    std::cout << "important: setting the tx power requires setcap cap_net_admin=ep" << std::endl;

}

Config
parseConfig(int   argc,
            char *argv[])
{
    Config conf;
    bool defaultTxPowerSteps = true;

    int c;
    while ((c = getopt(argc, argv, "i:f:c:S:s:u:p:r:h")) != -1)
    {
        switch (c)
        {
            case 'i':
                conf.name = std::string{optarg};
                break;
            
            case 'f':
                conf.font = std::string{optarg};
                break;

            case 'c':
                conf.color = std::string{optarg};
                break;
                
            case 'S':
                conf.fontSize = atoi(optarg);
                break;
                
            case 's':
                conf.iconSize = atoi(optarg);
                break;
                
            case 'u':
                conf.interval = atoi(optarg);
                break;
                
            case 'p':
                if (defaultTxPowerSteps)
                {
                    conf.txPowerSteps.clear();
                    defaultTxPowerSteps = false;
                }
                conf.txPowerSteps.push_back(atoi(optarg));
                break;

            case 'r':
                conf.txPowerRefreshInterval = atoi(optarg);
                break;
                
            case 'h':
                printUsage();
                exit(EXIT_SUCCESS);

            default:
                printUsage();
                exit(EXIT_FAILURE);
        }
    }

    return conf;
}

int
main(int   argc,
     char *argv[])
{
    Config conf = parseConfig(argc, argv);
    Controller ctrl{conf.name};

    if (!ctrl.isAvailable())
    {
        std::cerr << "ERROR: Interface " << conf.name << " is unavailable." << std::endl;
        printUsage();
        return -1;
    }

    QApplication app{argc, argv};
    app.setQuitOnLastWindowClosed(false);

    Icon icon{ctrl, conf};
    return app.exec();
}
