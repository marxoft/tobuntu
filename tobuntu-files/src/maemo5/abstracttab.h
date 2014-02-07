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

#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QWidget>

class QApplication;
class QDesktopWidget;

class AbstractTab : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractTab(QWidget *parent = 0);

    inline bool inPortrait() const { return m_portrait; }

protected slots:
    void onOrientationChanged();
    void toggleBusy(bool busy);

protected:
    inline virtual void setLandscapeLayout() {}
    inline virtual void setPortraitLayout() {}

private:
    bool m_portrait;
};

#endif // ABSTRACTTAB_H
