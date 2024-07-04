wireless-info-icon
=======

Tray icon with Wi-Fi signal power level information and control (rx and tx).

Allows to change tx power between predefined steps with a single click.

![Screenshot](/screenshot.png?raw=true)

Copyright (C) 2023-2024  Konrad Kosmatka

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# Information

This application supports Wi-Fi cards which do not obey the fixed tx power setting in long-term.
After a predefined refresh interval, the tx power will be changed by a 1 dB forth and back.

For detailed configuration options see wireless-info-icon -h.

# Build
In order to build wireless-info-icon you will need:

- CMake
- C++ compiler
- Qt6 library

Once you have all the necessary dependencies, you can use scripts available in the `build` directory.

Warning: custom capabilities are requried to set the tx power (setcap cap_net_admin=ep wireless-info-icon).

# Installation
After a successful build, just use:
```sh
$ sudo make install
```
in the `build` directory.
