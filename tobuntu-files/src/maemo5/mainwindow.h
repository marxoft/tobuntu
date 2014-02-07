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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stackedwindow.h"

class ListView;
class QMaemo5ValueButton;
class QPushButton;
class QLabel;
class QMenu;
class QLineEdit;
class QGridLayout;
class QModelIndex;

class MainWindow : public StackedWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void onLoadingChanged(bool loading);
    void onCountChanged();
    void onRootIndexChanged(const QModelIndex &index);
    void onItemClicked(const QModelIndex &index);
    void showContextMenu(const QPoint &pos);
    void moveUp();
    void goHome();
    void showFolderList();
    void setRootIndexFromResourcePath(const QString &resourcePath);
    void refreshFolder();
    void showNewFolderDialog();
    void createNewFolder(const QString &name);
    void showUploadDialog();
    void showMoveNodeDialog();
    void moveNode(const QModelIndex &destination);
    void showRenameDialog();
    void renameNode(const QString &name);
    void showConfirmDeleteDialog();
    void deleteNode();
    void downloadFile();
    void toggleFilePublished();
    void showTransfers();
    void showSettingsDialog();
    void showAccountDialog();
    void showAboutDialog();
    
private:
    ListView *m_view;

    QAction *m_uploadAction;
    QAction *m_refreshAction;
    QAction *m_newFolderAction;
    QAction *m_transfersAction;
    QAction *m_settingsAction;
    QAction *m_accountAction;
    QAction *m_aboutAction;

    QMenu *m_contextMenu;
    QAction *m_moveContextAction;
    QAction *m_renameContextAction;
    QAction *m_deleteContextAction;
    QAction *m_downloadContextAction;
    QAction *m_publishContextAction;

    QPushButton *m_upButton;
    QPushButton *m_homeButton;
    QMaemo5ValueButton *m_folderButton;

    QLabel *m_noResultsLabel;

    QGridLayout *m_grid;
};

#endif // MAINWINDOW_H
