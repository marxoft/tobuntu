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

#include "mainwindow.h"
#include "nodelistdelegate.h"
#include "filenamedialog.h"
#include "querydialog.h"
#include "filedetailsdialog.h"
#include "folderselectiondialog.h"
#include "uploaddialog.h"
#include "folderlistdialog.h"
#include "listview.h"
#include "authentication.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "accountdialog.h"
#include "authenticationdialog.h"
#include "transferlistwindow.h"
#include "../shared/nodemodel.h"
#include "../shared/settings.h"
#include "../shared/transfermanager.h"
#include "../shared/notifications.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMaemo5ValueButton>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QLineEdit>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMaemo5Style>

MainWindow::MainWindow(QWidget *parent) :
    StackedWindow(parent),
    m_view(new ListView(this)),
    m_uploadAction(this->menuBar()->addAction(tr("Upload file"), this, SLOT(showUploadDialog()))),
    m_refreshAction(this->menuBar()->addAction(tr("Refresh folder"), this, SLOT(refreshFolder()))),
    m_newFolderAction(this->menuBar()->addAction(tr("New volume"), this, SLOT(showNewFolderDialog()))),
    m_transfersAction(this->menuBar()->addAction(tr("Transfers"), this, SLOT(showTransfers()))),
    m_settingsAction(this->menuBar()->addAction(tr("Settings"), this, SLOT(showSettingsDialog()))),
    m_accountAction(this->menuBar()->addAction(tr("Account"), this, SLOT(showAccountDialog()))),
    m_aboutAction(this->menuBar()->addAction(tr("About"), this, SLOT(showAboutDialog()))),
    m_contextMenu(new QMenu(this)),
    m_moveContextAction(m_contextMenu->addAction(tr("Move"), this, SLOT(showMoveNodeDialog()))),
    m_renameContextAction(m_contextMenu->addAction(tr("Rename"), this, SLOT(showRenameDialog()))),
    m_deleteContextAction(m_contextMenu->addAction(tr("Delete"), this, SLOT(showConfirmDeleteDialog()))),
    m_downloadContextAction(m_contextMenu->addAction(tr("Download"), this, SLOT(downloadFile()))),
    m_publishContextAction(m_contextMenu->addAction(tr("Publish"), this, SLOT(toggleFilePublished()))),
    m_upButton(new QPushButton(QIcon::fromTheme("filemanager_folder_up"), "", this)),
    m_homeButton(new QPushButton(QIcon::fromTheme("general_presence_home"), "", this)),
    m_folderButton(new QMaemo5ValueButton(this)),
    m_noResultsLabel(new QLabel(QString("<p style='color:%1;font-size:40px;'><center><b>%2</b></center></p>").arg(QMaemo5Style::standardColor("SecondaryTextColor").name()).arg(tr("Folder empty")), m_view)),
    m_grid(0)
{
    this->setWindowTitle("toBuntu");
    this->setAttribute(Qt::WA_DeleteOnClose, false);
    this->setCentralWidget(new QWidget(this));

    m_view->setModel(NodeModel::instance());
    m_view->setItemDelegate(new NodeListDelegate(m_view));

    m_uploadAction->setShortcut(Qt::CTRL + Qt::Key_U);
    m_refreshAction->setShortcut(Qt::CTRL + Qt::Key_R);
    m_newFolderAction->setShortcut(Qt::CTRL + Qt::Key_N);

    m_upButton->setFixedWidth(80);
    m_homeButton->setFixedWidth(80);

    m_grid = new QGridLayout(this->centralWidget());
    m_grid->setContentsMargins(11, 11, 11, 11);
    m_grid->addWidget(m_upButton, 0, 0);
    m_grid->addWidget(m_homeButton, 0, 1);
    m_grid->addWidget(m_folderButton, 0, 2);
    m_grid->addWidget(m_view, 1, 0, 1, 3);

    m_noResultsLabel->hide();

    this->connect(NodeModel::instance(), SIGNAL(loadingChanged(bool)), this, SLOT(onLoadingChanged(bool)));
    this->connect(NodeModel::instance(), SIGNAL(loadingChanged(bool)), this, SLOT(toggleBusy(bool)));
    this->connect(NodeModel::instance(), SIGNAL(countChanged()), this, SLOT(onCountChanged()));
    this->connect(m_view, SIGNAL(rootIndexChanged(QModelIndex)), this, SLOT(onRootIndexChanged(QModelIndex)));
    this->connect(m_view, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    this->connect(m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    this->connect(m_upButton, SIGNAL(clicked()), this, SLOT(moveUp()));
    this->connect(m_homeButton, SIGNAL(clicked()), this, SLOT(goHome()));
    this->connect(m_folderButton, SIGNAL(clicked()), this, SLOT(showFolderList()));

    this->onOrientationChanged();
    this->onRootIndexChanged(QModelIndex());
}

void MainWindow::setLandscapeLayout() {
    m_noResultsLabel->setFixedSize(780, 300);
}

void MainWindow::setPortraitLayout() {
    m_noResultsLabel->setFixedSize(460, 600);
}

void MainWindow::onLoadingChanged(bool loading) {
    this->toggleBusy(loading);
    m_noResultsLabel->setVisible((!loading) && (NodeModel::instance()->rowCount(m_view->rootIndex()) == 0));
}

void MainWindow::onCountChanged() {
    m_noResultsLabel->setVisible((!NodeModel::instance()->loading()) && (NodeModel::instance()->rowCount(m_view->rootIndex()) == 0));
}

void MainWindow::onRootIndexChanged(const QModelIndex &index) {
    if (index.isValid()) {
        m_newFolderAction->setText(tr("New folder"));
        m_upButton->setEnabled(true);
        m_folderButton->setText(index.data(NodeRoles::NameRole).toString());

        switch (index.data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::Volume:
            m_folderButton->setValueText(index.data(NodeRoles::NodePathRole).toString().section('/', 0, -2));
            break;
        default:
            m_folderButton->setValueText(index.data(NodeRoles::ParentPathRole).toString());
            break;
        }
    }
    else {
        m_newFolderAction->setText(tr("New volume"));
        m_upButton->setEnabled(false);
        m_folderButton->setText("/");
        m_folderButton->setValueText("");
    }

    m_noResultsLabel->setVisible((!NodeModel::instance()->loading()) && (NodeModel::instance()->rowCount(m_view->rootIndex()) == 0));
}

void MainWindow::onItemClicked(const QModelIndex &index) {
    if (NodeModel::instance()->loading()) {
        return;
    }

    switch (index.data(NodeRoles::NodeTypeRole).toInt()) {
    case Node::File:
    {
        FileDetailsDialog *dialog = new FileDetailsDialog(index, this);
        dialog->open();
        return;
    }
    default:
        m_view->setRootIndex(index);

        if (NodeModel::instance()->rowCount(index) == 0) {
            NodeModel::instance()->listDirectory(index);
        }

        return;
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    if (NodeModel::instance()->loading()) {
        return;
    }

    if (m_view->currentIndex().isValid()) {
        switch (m_view->currentIndex().data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::File:
            m_moveContextAction->setVisible(true);
            m_renameContextAction->setVisible(true);
            m_publishContextAction->setVisible(true);
            m_downloadContextAction->setVisible(true);
            m_publishContextAction->setText(m_view->currentIndex().data(NodeRoles::IsPublicRole).toBool() ? tr("Unpublish") : tr("Publish"));
            break;
        case Node::Volume:
            m_moveContextAction->setVisible(false);
            m_renameContextAction->setVisible(false);
            m_publishContextAction->setVisible(false);
            m_downloadContextAction->setVisible(false);
            break;
        default:
            m_moveContextAction->setVisible(true);
            m_renameContextAction->setVisible(true);
            m_publishContextAction->setVisible(false);
            m_downloadContextAction->setVisible(false);
            break;
        }

        m_contextMenu->popup(pos, m_moveContextAction);
    }
}

void MainWindow::moveUp() {
    if (!NodeModel::instance()->loading()) {
        m_view->setRootIndex(m_view->rootIndex().parent());
    }
}

void MainWindow::goHome() {
    if (!NodeModel::instance()->loading()) {
        m_view->setRootIndex(QModelIndex());
    }
}

void MainWindow::showFolderList() {
    if (NodeModel::instance()->loading()) {
        return;
    }

    FolderListDialog *dialog = new FolderListDialog(m_view->rootIndex(), this);
    dialog->open();

    this->connect(dialog, SIGNAL(folderChosen(QString)), this, SLOT(setRootIndexFromResourcePath(QString)));
}

void MainWindow::setRootIndexFromResourcePath(const QString &resourcePath) {
    QModelIndex index = m_view->rootIndex();

    while ((index.isValid()) && (index.data(NodeRoles::ResourcePathRole).toString() != resourcePath)) {
        index = index.parent();
    }

    m_view->setRootIndex(index);
}

void MainWindow::refreshFolder() {
    NodeModel::instance()->reload(m_view->rootIndex());
}

void MainWindow::showNewFolderDialog() {
    bool isVolume = !m_view->rootIndex().isValid();

    FileNameDialog *dialog = new FileNameDialog(isVolume ? tr("Volume name") : tr("Folder name"), "", this);
    dialog->setWindowTitle(isVolume ? tr("New volume") : tr("New folder"));
    dialog->open();

    this->connect(dialog, SIGNAL(fileNameChosen(QString)), this, SLOT(createNewFolder(QString)));
}

void MainWindow::createNewFolder(const QString &name) {
    NodeModel::instance()->makeDirectory(m_view->rootIndex(), name);
}

void MainWindow::showUploadDialog() {
    UploadDialog *dialog = new UploadDialog(this);
    dialog->open();
}

void MainWindow::showMoveNodeDialog() {
    FolderSelectionDialog *dialog = new FolderSelectionDialog(this);
    dialog->setRootIndex(m_view->rootIndex());
    dialog->open();

    this->connect(dialog, SIGNAL(folderChosen(QModelIndex)), this, SLOT(moveNode(QModelIndex)));
}

void MainWindow::moveNode(const QModelIndex &destination) {
    NodeModel::instance()->moveNode(m_view->currentIndex(), destination);
}

void MainWindow::showRenameDialog() {
    FileNameDialog *dialog = new FileNameDialog(tr("New name"), m_view->currentIndex().data(NodeRoles::NameRole).toString(), this);
    dialog->setWindowTitle(tr("Rename"));
    dialog->open();

    this->connect(dialog, SIGNAL(fileNameChosen(QString)), this, SLOT(renameNode(QString)));
}

void MainWindow::renameNode(const QString &name) {
    NodeModel::instance()->renameNode(m_view->currentIndex(), name);
}

void MainWindow::showConfirmDeleteDialog() {
    QString name(m_view->currentIndex().data(NodeRoles::NameRole).toString());
    QueryDialog *dialog = new QueryDialog(QString("%1 '%2'?").arg(tr("Do you wish to delete")).arg(name), this);
    dialog->setWindowTitle(tr("Delete?"));
    dialog->open();

    this->connect(dialog, SIGNAL(accepted()), this, SLOT(deleteNode()));
}

void MainWindow::deleteNode() {
    NodeModel::instance()->deleteNode(m_view->currentIndex());
}

void MainWindow::downloadFile() {
    QString downloadPath;

    if (Settings::instance()->useDefaultDownloadDirectory()) {
        downloadPath = Settings::instance()->defaultDownloadDirectory();
    }
    else {
        downloadPath = QFileDialog::getExistingDirectory(this, tr("Download folder"), Settings::instance()->defaultDownloadDirectory());
    }

    if (!downloadPath.isEmpty()) {
        TransferManager::instance()->addDownloadTransfer(m_view->currentIndex().data(NodeRoles::ContentPathRole).toString(), downloadPath);
    }
    else {
        Notifications::showError(tr("Download path is empty."));
    }
}

void MainWindow::toggleFilePublished() {
    NodeModel::instance()->setFilePublic(m_view->currentIndex(), !m_view->currentIndex().data(NodeRoles::IsPublicRole).toBool());
}

void MainWindow::showTransfers() {
    TransferListWindow *window = new TransferListWindow(this);
    window->show();
}

void MainWindow::showSettingsDialog() {
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->open();
}

void MainWindow::showAccountDialog() {
    if (Authentication::tokenKey().isEmpty()) {
        AuthenticationDialog *dialog = new AuthenticationDialog(this);
        dialog->show();
    }
    else {
        AccountDialog *dialog = new AccountDialog(this);
        dialog->show();
        dialog->getAccount();
    }
}

void MainWindow::showAboutDialog() {
    AboutDialog *dialog = new AboutDialog(this);
    dialog->open();
}
