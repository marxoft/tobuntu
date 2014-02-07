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

#include "transfermodel.h"
#include "transfermanager.h"

TransferModel::TransferModel(QObject *parent) :
    QAbstractListModel(parent),
    m_signalsConnected(false)
{
    m_roleNames[TransferRoles::TransferTypeRole] = "transferType";
    m_roleNames[TransferRoles::FileNameRole] = "fileName";
    m_roleNames[TransferRoles::IdRole] = "id";
    m_roleNames[TransferRoles::UrlRole] = "url";
    m_roleNames[TransferRoles::ContentPathRole] = "contentPath";
    m_roleNames[TransferRoles::FilePathRole] = "filePath";
    m_roleNames[TransferRoles::ContentTypeRole] = "contentType";
    m_roleNames[TransferRoles::SizeRole] = "size";
    m_roleNames[TransferRoles::PositionRole] = "position";
    m_roleNames[TransferRoles::ProgressRole] = "progress";
    m_roleNames[TransferRoles::IsPublicRole] = "isPublic";
    m_roleNames[TransferRoles::StatusRole] = "status";
    m_roleNames[TransferRoles::StatusStringRole] = "statusString";
    m_roleNames[TransferRoles::ErrorRole] = "error";
    m_roleNames[TransferRoles::ErrorRole] = "errorString";
#if QT_VERSION < 0x050000
    this->setRoleNames(m_roleNames);
#endif
}

TransferModel::~TransferModel() {}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> TransferModel::roleNames() const {
    return m_roleNames;
}
#endif

int TransferModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)

    return TransferManager::instance()->count();
}

QVariant TransferModel::data(const QModelIndex &index, int role) const {
    if (FileTransfer *transfer = this->get(index)) {
        switch (role) {
        case TransferRoles::TransferTypeRole:
            return transfer->transferType();
        case TransferRoles::FileNameRole:
            return transfer->filePath().mid(transfer->filePath().lastIndexOf('/') + 1);
        case TransferRoles::IdRole:
            return transfer->id();
        case TransferRoles::UrlRole:
            return transfer->url();
        case TransferRoles::ContentPathRole:
            return transfer->contentPath();
        case TransferRoles::FilePathRole:
            return transfer->filePath();
        case TransferRoles::ContentTypeRole:
            return transfer->contentType();
        case TransferRoles::SizeRole:
            return transfer->size();
        case TransferRoles::PositionRole:
            return transfer->position();
        case TransferRoles::ProgressRole:
            return transfer->progress();
        case TransferRoles::IsPublicRole:
            return transfer->isPublic();
        case TransferRoles::StatusRole:
            return transfer->status();
        case TransferRoles::StatusStringRole:
            return transfer->statusString();
        case TransferRoles::ErrorRole:
            return transfer->error();
        case TransferRoles::ErrorStringRole:
            return transfer->errorString();
        default:
            break;
        }
    }

    return QVariant();
}

QVariant TransferModel::data(int row, int role) const {
    return this->data(this->index(row), role);
}

bool TransferModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    switch (role) {
    case TransferRoles::IsPublicRole:
        if (FileTransfer *transfer = this->get(index)) {
            transfer->setPublic(value.toBool());
            return true;
        }

        break;
    case TransferRoles::StatusRole:
        switch (value.toInt()) {
        case FileTransfer::Paused:
            if (FileTransfer *transfer = this->get(index)) {
                transfer->pause();
                return true;
            }

            break;
        case FileTransfer::Queued:
            if (FileTransfer *transfer = this->get(index)) {
                transfer->queue();
                return true;
            }

            break;
        case FileTransfer::Cancelled:
            if (FileTransfer *transfer = this->get(index)) {
                transfer->cancel();
                return true;
            }

            break;
        default:
            return false;
        }

        break;
    default:
        break;
    }

    return false;
}

bool TransferModel::setData(int row, const QVariant &value, int role) {
    return this->setData(this->index(row), value, role);
}

QMap<int, QVariant> TransferModel::itemData(const QModelIndex &index) const {
    QMap<int, QVariant> map;

    for (int role = TransferRoles::TransferTypeRole; role <= TransferRoles::ErrorStringRole; role++) {
        map[role] = this->data(index, role);
    }

    return map;
}

QMap<int, QVariant> TransferModel::itemData(int row) const {
    return this->itemData(this->index(row));
}

FileTransfer* TransferModel::get(const QModelIndex &index) const {
    if ((index.row() >= 0) && (index.row() < TransferManager::instance()->count())) {
        return TransferManager::instance()->transfers().at(index.row());
    }

    return 0;
}

FileTransfer* TransferModel::get(int row) const {
    return this->get(this->index(row));
}

bool TransferModel::signalsConnected() const {
    return m_signalsConnected;
}

void TransferModel::setSignalsConnected(bool connected) {
    if (connected != this->signalsConnected()) {
        m_signalsConnected = connected;

        if (connected) {
            this->onCountChanged();
            this->connect(TransferManager::instance(), SIGNAL(countChanged(int)), this, SLOT(onCountChanged()));

            foreach (FileTransfer *transfer, TransferManager::instance()->transfers()) {
                this->connect(transfer, SIGNAL(progressChanged(int)), this, SLOT(onTransferDataChanged()));
                this->connect(transfer, SIGNAL(sizeChanged(qint64)), this, SLOT(onTransferDataChanged()));
                this->connect(transfer, SIGNAL(statusChanged(FileTransfer::Status)), this, SLOT(onTransferDataChanged()));
            }
        }
        else {
            this->disconnect(TransferManager::instance(), SIGNAL(countChanged(int)), this, SLOT(onCountChanged()));

            foreach (FileTransfer *transfer, TransferManager::instance()->transfers()) {
                this->disconnect(transfer, 0, this, SLOT(onTransferDataChanged()));
            }
        }
    }
}

void TransferModel::onCountChanged() {
    this->beginResetModel();
    this->endResetModel();
}

void TransferModel::onTransferDataChanged() {
    if (FileTransfer *transfer = qobject_cast<FileTransfer*>(this->sender())) {
        int row = TransferManager::instance()->transfers().indexOf(transfer);
        emit dataChanged(this->index(row), this->index(row));
    }
}
