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

#include "abstracttab.h"
#include "windowstack.h"
#include "stackedwindow.h"
#include <QApplication>
#include <QDesktopWidget>

AbstractTab::AbstractTab(QWidget *parent) :
    QWidget(parent),
    m_portrait(false)
{
    this->connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(onOrientationChanged()));
}

void AbstractTab::onOrientationChanged() {
    QRect screen = QApplication::desktop()->screenGeometry();

    if (screen.width() > screen.height()) {
        m_portrait = false;
        this->setLandscapeLayout();
    }
    else {
        m_portrait = true;
        this->setPortraitLayout();
    }
}

void AbstractTab::toggleBusy(bool busy) {
    if (WindowStack::instance()->currentWindow()) {
        WindowStack::instance()->currentWindow()->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
    }
}
