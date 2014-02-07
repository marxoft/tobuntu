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

#ifndef FOLDERLISTDIALOG_H
#define FOLDERLISTDIALOG_H

#include "abstractdialog.h"
#include <QModelIndex>

class SelectionModel;
class QListView;
class QGridLayout;

class FolderListDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit FolderListDialog(const QModelIndex &index = QModelIndex(), QWidget *parent = 0);

    void setCurrentIndex(QModelIndex index);
    
private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void onItemClicked(const QModelIndex &index);

signals:
    void folderChosen(const QString &resourcePath);

private:
    SelectionModel *m_model;
    QListView *m_view;

    QGridLayout *m_grid;
};

#endif // FOLDERLISTDIALOG_H
