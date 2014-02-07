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

#include "folderlistdialog.h"
#include "../shared/selectionmodel.h"
#include "../shared/nodemodel.h"
#include "textlistdelegate.h"
#include <QListView>
#include <QGridLayout>

FolderListDialog::FolderListDialog(const QModelIndex &index, QWidget *parent) :
    AbstractDialog(parent),
    m_model(new SelectionModel(this)),
    m_view(new QListView(this)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("Choose folder"));

    m_grid->setContentsMargins(0, 0, 0 ,0);
    m_grid->addWidget(m_view);

    m_view->setModel(m_model);
    m_view->setItemDelegate(new TextListDelegate(SelectionRoles::NameRole, Qt::AlignCenter, m_view));

    this->connect(m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));

    this->setCurrentIndex(index);
    this->onOrientationChanged();
}

void FolderListDialog::setLandscapeLayout() {
    this->setFixedHeight(340);
}

void FolderListDialog::setPortraitLayout() {
    this->setFixedHeight(600);
}

void FolderListDialog::setCurrentIndex(QModelIndex index) {
    while (index.isValid()) {
        switch (index.data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::Volume:
            m_model->addItem(index.data(NodeRoles::NodePathRole).toString(), "");
            break;
        default:
            m_model->addItem(index.data(NodeRoles::ResourcePathRole).toString(), "");
            break;
        }

        index = index.parent();
    }

    m_model->addItem("/", "");
    m_view->setCurrentIndex(m_model->index(0, 0));
}

void FolderListDialog::onItemClicked(const QModelIndex &index) {
    emit folderChosen(index.data(SelectionRoles::NameRole).toString());
    this->accept();
}
