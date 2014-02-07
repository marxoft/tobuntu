/*
 * Copyright (C) 2014 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "screen.h"
#include <QDBusConnection>

Screen* Screen::m_instance = 0;

Screen::Screen() :
    QObject()
{
    if (!m_instance) {
        m_instance = this;
    }

    QDBusConnection::systemBus().connect("",
                                         "/com/nokia/mce/signal",
                                         "com.nokia.mce.signal",
                                         "tklock_mode_ind",
                                         this,
                                         SLOT(onScreenLockStateChanged(QString)));
}

Screen::~Screen() {}

Screen* Screen::instance() {
    return !m_instance ? new Screen : m_instance;
}

void Screen::onScreenLockStateChanged(const QString &state) {
    if (state == "locked") {
        emit screenLockStateChanged(true);
    }
    else if (state == "unlocked") {
        emit screenLockStateChanged(false);
    }
}
