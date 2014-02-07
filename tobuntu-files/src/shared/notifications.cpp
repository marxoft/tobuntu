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

#include "notifications.h"
#ifdef Q_WS_MAEMO_5
#include "../maemo5/windowstack.h"
#include "../maemo5/stackedwindow.h"
#include <QMaemo5InformationBox>
#endif

Notifications* Notifications::m_instance = 0;

Notifications::Notifications() :
    QObject()
{
    if (!m_instance) {
        m_instance = this;
    }
}

Notifications::~Notifications() {}

Notifications* Notifications::instance() {
    return !m_instance ? new Notifications : m_instance;
}

void Notifications::showInformation(const QString &message) {
#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox::information(WindowStack::instance()->currentWindow(), message);
#else
    if (m_instance) {
        emit m_instance->information(message);
    }
#endif
}

void Notifications::showError(const QString &message) {
#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox::information(WindowStack::instance()->currentWindow(), message, 0);
#else
    if (m_instance) {
        emit m_instance->error(message);
    }
#endif
}
