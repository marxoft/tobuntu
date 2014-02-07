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

#ifndef TRANSFERLISTWINDOW_H
#define TRANSFERLISTWINDOW_H

#include "stackedwindow.h"

class TransferModel;
class ListView;
class QLabel;
class QMenu;

class TransferListWindow : public StackedWindow
{
    Q_OBJECT

public:
    explicit TransferListWindow(QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void showContextMenu(const QPoint &pos);
    void pauseOrResumeTransfer();
    void cancelTransfer();
    void onCountChanged(int count);
    void onScreenLockStateChanged(bool locked);
    
private:
    TransferModel *m_model;
    ListView *m_view;

    QLabel *m_noResultsLabel;

    QMenu *m_contextMenu;
    QAction *m_pauseResumeAction;
    QAction *m_cancelAction;
};

#endif // TRANSFERLISTWINDOW_H
