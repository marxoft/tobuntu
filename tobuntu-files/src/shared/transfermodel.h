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

#ifndef TRANSFERMODEL_H
#define TRANSFERMODEL_H

#include "filetransfer.h"
#include <QAbstractListModel>

using namespace QtUbuntuOne;

class TransferRoles : public QObject
{
    Q_OBJECT

    Q_ENUMS(Role)

public:
    enum Role {
        TransferTypeRole = Qt::UserRole + 1,
        FileNameRole,
        IdRole,
        UrlRole,
        ContentPathRole,
        FilePathRole,
        ContentTypeRole,
        SizeRole,
        PositionRole,
        ProgressRole,
        IsPublicRole,
        StatusRole,
        StatusStringRole,
        ErrorRole,
        ErrorStringRole
    };

private:
    TransferRoles() {}
    ~TransferRoles() {}
};

class TransferModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool signalsConnected
               READ signalsConnected
               WRITE setSignalsConnected)

public:
    explicit TransferModel(QObject *parent = 0);
    ~TransferModel();

#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QVariant data(int row, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Q_INVOKABLE bool setData(int row, const QVariant &value, int role);

    QMap<int, QVariant> itemData(const QModelIndex &index) const;
    Q_INVOKABLE QMap<int, QVariant> itemData(int row) const;

    FileTransfer* get(const QModelIndex &index) const;
    Q_INVOKABLE FileTransfer* get(int row) const;

    bool signalsConnected() const;

public slots:
    void setSignalsConnected(bool connected);

private slots:
    void onCountChanged();
    void onTransferDataChanged();
    
private:
    QHash<int, QByteArray> m_roleNames;

    bool m_signalsConnected;
};

#endif // TRANSFERMODEL_H
