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

#ifndef UPLOADDIALOG_H
#define UPLOADDIALOG_H

#include "abstractdialog.h"

class FolderSelectionDialog;
class QMaemo5ValueButton;
class QCheckBox;
class QGridLayout;
class QDialogButtonBox;
class QModelIndex;

class UploadDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit UploadDialog(QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void showFolderDialog();
    void setFolder(const QModelIndex &index);
    void showFileDialog();
    void uploadFile();
    
private:
    FolderSelectionDialog *m_folderDialog;

    QMaemo5ValueButton *m_folderButton;
    QMaemo5ValueButton *m_fileButton;

    QCheckBox *m_publicCheckbox;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_doneButton;

    QGridLayout *m_grid;

    QString m_contentPath;
    QString m_filePath;
};

#endif // UPLOADDIALOG_H
