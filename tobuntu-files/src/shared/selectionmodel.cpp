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

#include "selectionmodel.h"

SelectionModel::SelectionModel(QObject *parent) :
    QStandardItemModel(parent)
{
    m_roleNames[SelectionRoles::NameRole] = "name";
    m_roleNames[SelectionRoles::ValueRole] = "value";
#if QT_VERSION < 0x050000
    this->setRoleNames(m_roleNames);
#endif
}

SelectionModel::~SelectionModel() {}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> SelectionModel::roleNames() const {
    return m_roleNames;
}
#endif

QVariant SelectionModel::data(const QModelIndex &index, int role) const {
    return QStandardItemModel::data(index, role);
}

QVariant SelectionModel::data(int row, const QByteArray &role) const {
    return QStandardItemModel::data(QStandardItemModel::index(row, 0), this->roleNames().key(role));
}

void SelectionModel::addItem(const QString &name, const QVariant &value) {
    QStandardItem *item = new QStandardItem(name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(value, SelectionRoles::ValueRole);
    this->appendRow(item);
    emit countChanged(this->rowCount());
}
