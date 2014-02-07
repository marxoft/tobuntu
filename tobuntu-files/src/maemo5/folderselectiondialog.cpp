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

#include "folderselectiondialog.h"
#include "nodelistdelegate.h"
#include "filenamedialog.h"
#include "listview.h"
#include "../shared/nodemodel.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMaemo5ValueButton>

FolderSelectionDialog::FolderSelectionDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_view(new ListView(this)),
    m_upButton(new QPushButton(QIcon::fromTheme("filemanager_folder_up"), "", this)),
    m_homeButton(new QPushButton(QIcon::fromTheme("general_presence_home"), "", this)),
    m_folderButton(new QMaemo5ValueButton(QIcon::fromTheme("general_folder"), "", this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_newButton(m_buttonBox->addButton(tr("New"), QDialogButtonBox::ActionRole)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("Choose folder"));

    m_view->setModel(NodeModel::instance());
    m_view->setItemDelegate(new NodeListDelegate(m_view));

    m_upButton->setFixedWidth(80);
    m_homeButton->setFixedWidth(80);
    m_folderButton->setIconSize(QSize(32, 32));

    this->connect(NodeModel::instance(), SIGNAL(loadingChanged(bool)), this, SLOT(onLoadingChanged(bool)));
    this->connect(NodeModel::instance(), SIGNAL(countChanged()), this, SLOT(onCountChanged()));
    this->connect(m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    this->connect(m_view, SIGNAL(rootIndexChanged(QModelIndex)), this, SLOT(onRootIndexChanged(QModelIndex)));
    this->connect(m_upButton, SIGNAL(clicked()), this, SLOT(moveUp()));
    this->connect(m_homeButton, SIGNAL(clicked()), this, SLOT(goHome()));
    this->connect(m_folderButton, SIGNAL(clicked()), this, SLOT(onFolderButtonClicked()));
    this->connect(m_newButton, SIGNAL(clicked()), this, SLOT(showNewFolderDialog()));

    this->onOrientationChanged();
    this->onRootIndexChanged(QModelIndex());
}

void FolderSelectionDialog::setLandscapeLayout() {
    this->setFixedHeight(340);
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_upButton, 0, 0);
        m_grid->addWidget(m_homeButton, 0, 1);
        m_grid->addWidget(m_folderButton, 0, 2);
        m_grid->addWidget(m_view, 1, 0, 1, 3);
        m_grid->addWidget(m_buttonBox, 1, 4, Qt::AlignBottom);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 1, 4, Qt::AlignBottom);
    }
}

void FolderSelectionDialog::setPortraitLayout() {
    this->setFixedHeight(600);
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_upButton, 0, 0);
        m_grid->addWidget(m_homeButton, 0, 1);
        m_grid->addWidget(m_folderButton, 0, 2);
        m_grid->addWidget(m_view, 1, 0, 1, 3);
        m_grid->addWidget(m_buttonBox, 2, 0, 1, 3, Qt::AlignBottom);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 2, 0, 1, 3, Qt::AlignBottom);
    }
}

void FolderSelectionDialog::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() == Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_N:
            event->accept();
            this->showNewFolderDialog();
            break;
        default:
            event->ignore();
            break;
        }
    }
    else {
        event->ignore();
    }
}

void FolderSelectionDialog::setRootIndex(const QModelIndex &index) {
    m_view->setRootIndex(index);
}

void FolderSelectionDialog::moveUp() {
    if (!NodeModel::instance()->loading()) {
        m_view->setRootIndex(m_view->rootIndex().parent());
    }
}

void FolderSelectionDialog::goHome() {
    if (!NodeModel::instance()->loading()) {
        m_view->setRootIndex(QModelIndex());
    }
}

void FolderSelectionDialog::onLoadingChanged(bool loading) {
    this->toggleBusy(loading);
}

void FolderSelectionDialog::onCountChanged() {
    this->hideFiles(m_view->rootIndex());
}

void FolderSelectionDialog::onRootIndexChanged(const QModelIndex &index) {
    if (index.isValid()) {
        m_upButton->setEnabled(true);
        m_folderButton->setText(index.data(NodeRoles::PathRole).toString().section('/', -1));
        m_folderButton->setValueText(index.data(NodeRoles::ResourcePathRole).toString().section('/', 0, -2));
    }
    else {
        m_upButton->setEnabled(false);
        m_folderButton->setText("/");
        m_folderButton->setValueText("");
    }

    this->hideFiles(index);
}

void FolderSelectionDialog::onItemClicked(const QModelIndex &index) {
    if (NodeModel::instance()->loading()) {
        return;
    }

    m_view->setRootIndex(index);

    if (NodeModel::instance()->rowCount(index) == 0) {
        NodeModel::instance()->listDirectory(index);
    }
}

void FolderSelectionDialog::onFolderButtonClicked() {
    if (!NodeModel::instance()->loading()) {
        emit folderChosen(m_view->rootIndex());
        this->accept();;
    }
}

void FolderSelectionDialog::showNewFolderDialog() {
    bool isVolume = !m_view->rootIndex().isValid();

    FileNameDialog *dialog = new FileNameDialog(isVolume ? tr("Volume name") : tr("Folder name"), "", this);
    dialog->setWindowTitle(isVolume ? tr("New volume") : tr("New folder"));
    dialog->open();

    this->connect(dialog, SIGNAL(fileNameChosen(QString)), this, SLOT(createNewFolder(QString)));
}

void FolderSelectionDialog::createNewFolder(const QString &name) {
    NodeModel::instance()->makeDirectory(m_view->rootIndex(), name);
}

void FolderSelectionDialog::hideFiles(const QModelIndex &parent) {
    for (int i = 0; i < NodeModel::instance()->rowCount(parent); i++) {
        switch (NodeModel::instance()->index(i, 0, parent).data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::File:
            m_view->setRowHidden(i, true);
            break;
        default:
            m_view->setRowHidden(i, false);
            break;
        }
    }
}
