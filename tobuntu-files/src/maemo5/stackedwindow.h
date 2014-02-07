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

#ifndef STACKEDWINDOW_H
#define STACKEDWINDOW_H

#include "../shared/screenorientation.h"
#include <QMainWindow>

class StackedWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StackedWindow(QWidget *parent = 0);
    ~StackedWindow();

    inline virtual bool inPortrait() const { return m_portrait; }

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

    inline virtual void setLandscapeLayout() {}
    inline virtual void setPortraitLayout() {}

protected slots:
    void setOrientation(ScreenOrientation::Orientation orientation);
    void onOrientationChanged();
    void toggleBusy(bool busy);

protected:
    bool m_portrait;
};

#endif // STACKEDWINDOW_H