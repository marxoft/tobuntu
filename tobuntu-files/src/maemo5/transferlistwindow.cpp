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

#include "transferlistwindow.h"
#include "transferlistdelegate.h"
#include "listview.h"
#include "screen.h"
#include "../shared/transfermodel.h"
#include "../shared/transfermanager.h"
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMaemo5Style>

TransferListWindow::TransferListWindow(QWidget *parent) :
    StackedWindow(parent),
    m_model(new TransferModel(this)),
    m_view(new ListView(this)),
    m_noResultsLabel(new QLabel(QString("<p style='color:%1;font-size:40px;'><center><b>%2</b></center></p>").arg(QMaemo5Style::standardColor("SecondaryTextColor").name()).arg(tr("No transfers queued")), this)),
    m_contextMenu(new QMenu(this)),
    m_pauseResumeAction(m_contextMenu->addAction(tr("Pause"), this, SLOT(pauseOrResumeTransfer()))),
    m_cancelAction(m_contextMenu->addAction(tr("Remove"), this, SLOT(cancelTransfer())))
{
    this->setWindowTitle(tr("Transfers"));
    this->setCentralWidget(m_view);

    m_view->setModel(m_model);
    m_view->setItemDelegate(new TransferListDelegate(m_view));
    m_model->setSignalsConnected(true);

    this->connect(m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    this->connect(TransferManager::instance(), SIGNAL(countChanged(int)), this, SLOT(onCountChanged(int)));
    this->connect(Screen::instance(), SIGNAL(screenLockStateChanged(bool)), this, SLOT(onScreenLockStateChanged(bool)));

    this->onCountChanged(m_model->rowCount());
    this->onOrientationChanged();
}

void TransferListWindow::setLandscapeLayout() {
    m_noResultsLabel->setFixedSize(800, 400);
}

void TransferListWindow::setPortraitLayout() {
    m_noResultsLabel->setFixedSize(480, 700);
}

void TransferListWindow::showContextMenu(const QPoint &pos) {
    if (m_view->currentIndex().isValid()) {
        switch (m_view->currentIndex().data(TransferRoles::StatusRole).toInt()) {
        case FileTransfer::Paused:
        case FileTransfer::Failed:
            m_pauseResumeAction->setText(tr("Start"));
            break;
        default:
            m_pauseResumeAction->setText(tr("Pause"));
            break;
        }

        m_contextMenu->popup(pos, m_pauseResumeAction);
    }
}

void TransferListWindow::pauseOrResumeTransfer() {
    switch (m_view->currentIndex().data(TransferRoles::StatusRole).toInt()) {
    case FileTransfer::Paused:
    case FileTransfer::Failed:
        m_model->setData(m_view->currentIndex(), FileTransfer::Queued, TransferRoles::StatusRole);
        break;
    default:
        m_model->setData(m_view->currentIndex(), FileTransfer::Paused, TransferRoles::StatusRole);
        break;
    }
}

void TransferListWindow::cancelTransfer() {
    m_model->setData(m_view->currentIndex(), FileTransfer::Cancelled, TransferRoles::StatusRole);
}

void TransferListWindow::onCountChanged(int count) {
    m_noResultsLabel->setVisible(count == 0);
}

void TransferListWindow::onScreenLockStateChanged(bool locked) {
    m_model->setSignalsConnected(!locked);
}
