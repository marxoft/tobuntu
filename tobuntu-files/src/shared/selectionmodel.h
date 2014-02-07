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

#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QStandardItemModel>

class SelectionRoles : public QObject
{
    Q_OBJECT

    Q_ENUMS(Role)

public:
    enum Role {
        NameRole = Qt::DisplayRole,
        ValueRole = Qt::UserRole + 1
    };
};

class SelectionModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY(int count
               READ rowCount
               NOTIFY countChanged)

public:
    explicit SelectionModel(QObject *parent = 0);
    ~SelectionModel();

#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QVariant data(int row, const QByteArray &role) const;
    Q_INVOKABLE void addItem(const QString &name, const QVariant &value);

signals:
    void countChanged(int count);

protected:
    QHash<int, QByteArray> m_roleNames;
};

#endif // SELECTIONMODEL_H
