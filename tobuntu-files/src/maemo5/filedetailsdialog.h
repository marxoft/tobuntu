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

#ifndef FILEDETAILSDIALOG_H
#define FILEDETAILSDIALOG_H

#include "abstractdialog.h"
#include <QModelIndex>

class QLineEdit;
class QLabel;
class QGridLayout;

class FileDetailsDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit FileDetailsDialog(const QModelIndex &file, QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void loadFileData();
    void openLink(const QString &link);
    void onFileNameChanged(const QString &name);
    void renameFile();
    void deleteFile();
    void toggleFilePublished();

private:
    const QModelIndex m_file;

    QLineEdit *m_nameEdit;

    QLabel *m_createdLabel;
    QLabel *m_modifiedLabel;
    QLabel *m_publicUrlLabel;
    QLabel *m_sizeLabel;

    QPushButton *m_publishButton;
    QPushButton *m_renameButton;
    QPushButton *m_deleteButton;

    QGridLayout *m_grid;
};

#endif // FILEDETAILSDIALOG_H
