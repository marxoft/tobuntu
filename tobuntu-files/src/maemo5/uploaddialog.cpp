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

#include "uploaddialog.h"
#include "folderselectiondialog.h"
#include "../shared/nodemodel.h"
#include "../shared/transfermanager.h"
#include <QFileDialog>
#include <QMaemo5ValueButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>

UploadDialog::UploadDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_folderDialog(0),
    m_folderButton(new QMaemo5ValueButton(tr("Destination folder"), this)),
    m_fileButton(new QMaemo5ValueButton(tr("File"), this)),
    m_publicCheckbox(new QCheckBox(tr("Publish file"), this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_doneButton(m_buttonBox->addButton(tr("Done"), QDialogButtonBox::ActionRole)),
    m_grid(new QGridLayout(this)),
    m_contentPath("/")
{
    this->setWindowTitle(tr("Upload file"));
    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    m_folderButton->setValueText("/");
    m_fileButton->setValueText(tr("None chosen"));
    m_doneButton->setEnabled(false);

    this->connect(m_folderButton, SIGNAL(clicked()), this, SLOT(showFolderDialog()));
    this->connect(m_fileButton, SIGNAL(clicked()), this, SLOT(showFileDialog()));
    this->connect(m_doneButton, SIGNAL(clicked()), this, SLOT(uploadFile()));

    this->onOrientationChanged();
}

void UploadDialog::setLandscapeLayout() {
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_fileButton, 0, 0);
        m_grid->addWidget(m_folderButton, 1, 0);
        m_grid->addWidget(m_publicCheckbox, 2, 0);
        m_grid->addWidget(m_buttonBox, 2, 1);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 2, 1);
    }
}

void UploadDialog::setPortraitLayout() {
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_fileButton, 0, 0);
        m_grid->addWidget(m_folderButton, 1, 0);
        m_grid->addWidget(m_publicCheckbox, 2, 0);
        m_grid->addWidget(m_buttonBox, 3, 0);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 3, 0);
    }
}

void UploadDialog::showFolderDialog() {
    if (!m_folderDialog) {
        m_folderDialog = new FolderSelectionDialog(this);
        m_folderDialog->setAttribute(Qt::WA_DeleteOnClose, false);
        m_folderDialog->setRootIndex(QModelIndex());
        this->connect(m_folderDialog, SIGNAL(folderChosen(QModelIndex)), this, SLOT(setFolder(QModelIndex)));
    }

    m_folderDialog->open();
}

void UploadDialog::setFolder(const QModelIndex &index) {
    m_folderButton->setValueText(index.data(NodeRoles::ResourcePathRole).toString());
    m_contentPath = index.data(NodeRoles::ContentPathRole).toString();
}

void UploadDialog::showFileDialog() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose file"), "/home/user/MyDocs/");

    if (!filePath.isEmpty()) {
        m_fileButton->setValueText(filePath.section('/', -1));
        m_filePath = filePath;
        m_doneButton->setEnabled(true);
    }
    else {
        m_doneButton->setEnabled(false);
    }
}

void UploadDialog::uploadFile() {
    TransferManager::instance()->addUploadTransfer(m_filePath, "application/octet-stream", m_contentPath, m_publicCheckbox->isChecked());
    this->accept();
}
