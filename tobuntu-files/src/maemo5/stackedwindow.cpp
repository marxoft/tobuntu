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

#include "stackedwindow.h"
#include "windowstack.h"
#include "../shared/settings.h"
#include <QApplication>
#include <QDesktopWidget>

StackedWindow::StackedWindow(QWidget *parent) :
    QMainWindow(parent),
    m_portrait(false)
{
    this->setAttribute(Qt::WA_Maemo5StackedWindow, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setOrientation(Settings::instance()->screenOrientation());

    this->connect(Settings::instance(), SIGNAL(screenOrientationChanged(ScreenOrientation::Orientation)), this, SLOT(setOrientation(ScreenOrientation::Orientation)));
    this->connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(onOrientationChanged()));
}

StackedWindow::~StackedWindow() {}

void StackedWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    WindowStack::instance()->addWindowToStack(this);
}

void StackedWindow::closeEvent(QCloseEvent *event) {
    WindowStack::instance()->removeWindowFromStack(this);
    QMainWindow::closeEvent(event);
}

void StackedWindow::setOrientation(ScreenOrientation::Orientation orientation) {
    switch (orientation) {
    case ScreenOrientation::Automatic:
        this->setAttribute(Qt::WA_Maemo5AutoOrientation, true);
        break;
    case ScreenOrientation::LockPortrait:
        this->setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        break;
    case ScreenOrientation::LockLandscape:
        this->setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
        break;
    default:
        this->setAttribute(Qt::WA_Maemo5LandscapeOrientation, true);
    }
}

void StackedWindow::onOrientationChanged() {
    QRect screen = QApplication::desktop()->screenGeometry();

    if ((screen.width()) > (screen.height())) {
        m_portrait = false;
        this->setLandscapeLayout();
    }
    else {
        m_portrait = true;
        this->setPortraitLayout();
    }
}

void StackedWindow::toggleBusy(bool busy) {
    this->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
}
