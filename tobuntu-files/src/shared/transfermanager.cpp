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

#include "transfermanager.h"
#include "definitions.h"
#include "storage.h"
#include "notifications.h"
#include <QCoreApplication>

TransferManager* TransferManager::m_instance = 0;

TransferManager::TransferManager() :
    QObject()
{
    if (!m_instance) {
        m_instance = this;
    }

    m_queueTimer.setInterval(1000);
    m_queueTimer.setSingleShot(true);

    this->connect(&m_queueTimer, SIGNAL(timeout()), this, SLOT(startNextTransfers()));
    this->connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(storeAndDeleteTransfers()));
}

TransferManager::~TransferManager() {}

TransferManager* TransferManager::instance() {
    return !m_instance ? new TransferManager : m_instance;
}

int TransferManager::count() const {
    return m_transfers.size();
}

QList<FileTransfer*> TransferManager::transfers() const {
    return m_transfers;
}

void TransferManager::addDownloadTransfer(const QString &contentPath, const QString &localPath) {
    FileTransfer *transfer = new FileTransfer(FileTransfer::Download, contentPath, localPath + (localPath.endsWith('/') ? contentPath.mid(contentPath.lastIndexOf('/') + 1)
                                                                                                                        : contentPath.mid(contentPath.lastIndexOf('/'))));
    this->connect(transfer, SIGNAL(statusChanged(FileTransfer::Status)), this, SLOT(onTransferStatusChanged(FileTransfer::Status)));

    m_transfers.append(transfer);
    m_queueTimer.start();

    emit countChanged(this->count());

    Notifications::showInformation(tr("Download added to transfer queue."));
}

void TransferManager::addUploadTransfer(const QString &filePath, const QString &contentType, const QString &contentPath, bool isPublic) {
    FileTransfer *transfer = new FileTransfer(FileTransfer::Upload, contentPath + filePath.mid(filePath.lastIndexOf('/')), filePath);
    transfer->setContentType(contentType);
    transfer->setPublic(isPublic);
    this->connect(transfer, SIGNAL(statusChanged(FileTransfer::Status)), this, SLOT(onTransferStatusChanged(FileTransfer::Status)));

    m_transfers.append(transfer);
    m_queueTimer.start();

    emit countChanged(this->count());

    Notifications::showInformation(tr("Upload added to transfer queue."));
}

void TransferManager::storeTransfers() {
    if (m_transfers.isEmpty()) {
        Storage::clearStoredTransfers();
    }
    else {
        Storage::storeTransfers(m_transfers);
    }
}

void TransferManager::storeAndDeleteTransfers() {
    Storage::storeTransfers(m_transfers);
    qDeleteAll(m_transfers);
    m_transfers.clear();
}

void TransferManager::restoreStoredTransfers() {
    foreach (FileTransfer *transfer, Storage::restoreTransfers()) {
        this->connect(transfer, SIGNAL(statusChanged(FileTransfer::Status)), this, SLOT(onTransferStatusChanged(FileTransfer::Status)));
        m_transfers.append(transfer);
    }

    m_queueTimer.start();

    emit countChanged(this->count());
}

void TransferManager::startNextTransfers() {
    int active = m_activeTransfers.size();
    int i = 0;

    while ((active < MAX_CONCURRENT_TRANSFERS) && (i < m_transfers.size())) {
        if (m_transfers.at(i)->status() == FileTransfer::Queued) {
            m_transfers.at(i)->start();
            active++;
        }

        i++;
    }
}

void TransferManager::onTransferStatusChanged(FileTransfer::Status status) {
    if (FileTransfer *transfer = qobject_cast<FileTransfer*>(this->sender())) {
        switch (status) {
        case FileTransfer::Connecting:
        case FileTransfer::Downloading:
        case FileTransfer::Uploading:
            m_activeTransfers.append(transfer);
            return;
        case FileTransfer::Paused:
        case FileTransfer::Failed:
            m_activeTransfers.removeOne(transfer);
            m_queueTimer.start();
            return;
        case FileTransfer::Completed:
        case FileTransfer::Cancelled:
            m_activeTransfers.removeOne(transfer);
            m_transfers.removeOne(transfer);
            transfer->deleteLater();

            m_queueTimer.start();

            emit countChanged(this->count());

            QMetaObject::invokeMethod(this, "storeTransfers", Qt::QueuedConnection);
            return;
        default:
            return;
        }
    }
}
