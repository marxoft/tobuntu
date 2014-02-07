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

#ifndef FOLDERSELECTIONDIALOG_H
#define FOLDERSELECTIONDIALOG_H

#include "abstractdialog.h"

class QMaemo5ValueButton;
class QDialogButtonBox;
class QGridLayout;
class QModelIndex;
class ListView;

class FolderSelectionDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit FolderSelectionDialog(QWidget *parent = 0);

    void setRootIndex(const QModelIndex &index);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void setLandscapeLayout();
    void setPortraitLayout();
    void hideFiles(const QModelIndex &parent);

private slots:
    void moveUp();
    void goHome();
    void onLoadingChanged(bool loading);
    void onCountChanged();
    void onItemClicked(const QModelIndex &index);
    void onRootIndexChanged(const QModelIndex &index);
    void onFolderButtonClicked();
    void showNewFolderDialog();
    void createNewFolder(const QString &name);

signals:
    void folderChosen(const QModelIndex &index);
    
private:
    ListView *m_view;

    QPushButton *m_upButton;
    QPushButton *m_homeButton;
    QMaemo5ValueButton *m_folderButton;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_newButton;

    QGridLayout *m_grid;
};

#endif // FOLDERSELECTIONDIALOG_H
