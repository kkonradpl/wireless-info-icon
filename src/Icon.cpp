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

#include "Icon.hpp"
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QTimer>
#include <QDebug>

Icon::Icon(Controller   &_ctrl,
           const Config &_conf,
           QObject      *_parent)
    : QSystemTrayIcon(_parent), ctrl(_ctrl), conf(_conf)
{
    const int fontSize = conf.fontSize ? conf.fontSize : conf.iconSize * 0.3125;
    this->font = new QFont(conf.font.c_str(), fontSize);
    this->color = new QColor(conf.color.c_str());
    
    this->txChangeTimer = new QTimer(this);
    txChangeTimer->setSingleShot(true);

    this->setToolTip(conf.name.c_str());
    this->forceUpdate();

    if (!this->conf.txPowerSteps.empty())
    {
        connect(this->txChangeTimer, &QTimer::timeout, this, &Icon::setTxPower);
        connect(this, &QSystemTrayIcon::activated, this, &Icon::onActivated);
        this->setTxPower();
    }
}

Icon::~Icon()
{
    killTimer(this->redrawTimerId);

    delete txChangeTimer;
    delete color;
    delete font;
}

void
Icon::forceUpdate()
{
    if (this->redrawTimerId)
    {
        killTimer(this->redrawTimerId);
    }

    this->update();
    this->redrawTimerId = startTimer(this->conf.interval);
}

void
Icon::timerEvent(QTimerEvent *event)
{
    this->update();
}

void
Icon::update()
{
    if (!this->ctrl.getRxPower(this->rx))
    {
        if (this->isVisible())
        {
            this->setVisible(false);
        }
        return;
    }

    if (!this->ctrl.getTxPower(this->tx))
    {
        this->tx = 0;
    }

    if(this->lastRx != this->rx ||
       this->lastTx != this->tx)
    {
        this->redraw();
        this->lastRx = this->rx;
        this->lastTx = this->tx;
    }

    if (!this->isVisible())
    {
        this->setVisible(true);
    }
}

void
Icon::redraw()
{
    QPixmap pixmap{conf.iconSize, conf.iconSize};
    pixmap.fill(Qt::transparent);

    QPainter painter{&pixmap};
    painter.setFont(*font);
    painter.setPen(*color);

    char text[16];
    snprintf(text, sizeof(text), "R%c%2d", (rx < 0 ? '-' : '+'), abs(rx));
    painter.drawText(1, 1, this->conf.iconSize, this->conf.iconSize / 2, 0, text);

    snprintf(text, sizeof(text), "T%c%2d", (tx < 0 ? '-' : '+'), abs(tx));
    painter.drawText(1, this->conf.iconSize / 2, this->conf.iconSize, this->conf.iconSize / 2, 0, text);

    this->setIcon(pixmap);
}

void
Icon::onActivated()
{
    this->txChangeTimer->stop();

    this->txPowerId = (this->txPowerId + 1) % this->conf.txPowerSteps.size();
    this->txChangeFlag = false;

    this->setTxPower();
}

void
Icon::setTxPower()
{
    int txPowerTarget = this->conf.txPowerSteps[this->txPowerId];
    int delay;

    if (txChangeFlag)
    {
        int newTarget = txPowerTarget - 1;
        ctrl.setTxPower(newTarget >= 0 ? newTarget : txPowerTarget + 1);

        constexpr int refreshDelay = 500;
        delay = refreshDelay;
    }
    else
    {
        ctrl.setTxPower(txPowerTarget);
        delay = conf.txPowerRefreshInterval;
    }

    txChangeTimer->setInterval(delay);
    txChangeTimer->start();

    this->txChangeFlag = !this->txChangeFlag;
    this->forceUpdate();
}
