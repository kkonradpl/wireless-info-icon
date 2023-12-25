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

#ifndef WIRELESS_INFO_ICON_H
#define WIRELESS_INFO_ICON_H
#include <QSystemTrayIcon>
#include "Controller.hpp"
#include "Config.hpp"

class Icon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit Icon(Controller   &_ctrl,
                  const Config &_conf,
                  QObject      *_parent = 0);
    ~Icon();
    void forceUpdate();

private slots:
    void onActivated();

protected:
    void timerEvent(QTimerEvent *event);
    void update();
    void redraw();
    void setTxPower();
    void setTxPowerNear();

private:
    Controller &ctrl;
    const Config &conf;

    QFont *font;
    QColor *color;
    QTimer *txChangeTimer;

    int txPowerId = 0;
    int txChangeFlag = 0;

    int rx = 0;
    int tx = 0;
    int lastRx = 0;
    int lastTx = 0;

    int redrawTimerId = 0;
};

#endif
