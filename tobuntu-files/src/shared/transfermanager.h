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

#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

#include "filetransfer.h"
#include <QObject>
#include <QList>
#include <QTimer>

using namespace QtUbuntuOne;

class QTimer;

class TransferManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count
               READ count
               NOTIFY countChanged)

public:
    ~TransferManager();

    static TransferManager* instance();

    int count() const;

    QList<FileTransfer*> transfers() const;

public slots:
    void addDownloadTransfer(const QString &contentPath, const QString &localPath);
    void addUploadTransfer(const QString &filePath, const QString &contentType, const QString &contentPath, bool isPublic);

    void storeTransfers();
    void restoreStoredTransfers();

private slots:
    void storeAndDeleteTransfers();

    void startNextTransfers();

    void onTransferStatusChanged(FileTransfer::Status status);

signals:
    void countChanged(int count);

private:
    TransferManager();

    static TransferManager *m_instance;

    QTimer m_queueTimer;

    QList<FileTransfer*> m_transfers;
    QList<FileTransfer*> m_activeTransfers;
};

#endif // TRANSFERMANAGER_H
