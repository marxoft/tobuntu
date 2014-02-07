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

#include "filedetailsdialog.h"
#include "../shared/nodemodel.h"
#include "../shared/utils.h"
#include "../shared/notifications.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDesktopServices>
#include <QUrl>

FileDetailsDialog::FileDetailsDialog(const QModelIndex &file, QWidget *parent) :
    AbstractDialog(parent),
    m_file(file),
    m_nameEdit(new QLineEdit(this)),
    m_createdLabel(new QLabel(this)),
    m_modifiedLabel(new QLabel(this)),
    m_publicUrlLabel(new QLabel(this)),
    m_sizeLabel(new QLabel(this)),
    m_publishButton(new QPushButton(this)),
    m_renameButton(new QPushButton(tr("Rename"), this)),
    m_deleteButton(new QPushButton(tr("Delete"), this)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("File details"));

    m_renameButton->setMinimumWidth(150);
    m_publishButton->setMinimumWidth(150);
    m_deleteButton->setMinimumWidth(150);

    this->connect(m_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onFileNameChanged(QString)));
    this->connect(m_renameButton, SIGNAL(clicked()), this, SLOT(renameFile()));
    this->connect(m_publishButton, SIGNAL(clicked()), this, SLOT(toggleFilePublished()));
    this->connect(m_deleteButton, SIGNAL(clicked()), this, SLOT(deleteFile()));
    this->connect(m_publicUrlLabel, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));

    this->loadFileData();
    this->onOrientationChanged();
}

void FileDetailsDialog::setLandscapeLayout() {
    this->setFixedHeight(340);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_nameEdit, 0, 0);
        m_grid->addWidget(m_renameButton, 0, 1);
        m_grid->addWidget(m_publicUrlLabel, 1, 0);
        m_grid->addWidget(m_publishButton, 1, 1);
        m_grid->addWidget(m_createdLabel, 2, 0);
        m_grid->addWidget(m_modifiedLabel, 3, 0);
        m_grid->addWidget(m_sizeLabel, 4, 0);
        m_grid->addWidget(m_deleteButton, 4, 1);
    }
    else {
        m_grid->removeWidget(m_deleteButton);
        m_grid->addWidget(m_deleteButton, 4, 1);
    }
}

void FileDetailsDialog::setPortraitLayout() {
    this->setFixedHeight(450);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_nameEdit, 0, 0);
        m_grid->addWidget(m_renameButton, 0, 1);
        m_grid->addWidget(m_publicUrlLabel, 1, 0);
        m_grid->addWidget(m_publishButton, 1, 1);
        m_grid->addWidget(m_createdLabel, 2, 0);
        m_grid->addWidget(m_modifiedLabel, 3, 0);
        m_grid->addWidget(m_sizeLabel, 4, 0);
        m_grid->addWidget(m_deleteButton, 5, 0);
    }
    else {
        m_grid->removeWidget(m_deleteButton);
        m_grid->addWidget(m_deleteButton, 5, 0, 1, 2);
    }
}

void FileDetailsDialog::onFileNameChanged(const QString &name) {
    m_renameButton->setEnabled(!name.isEmpty());
}

void FileDetailsDialog::openLink(const QString &link) {
    if (!QDesktopServices::openUrl(QUrl(link))) {
        Notifications::showError(tr("Unable to open %1").arg(link));
    }
}

void FileDetailsDialog::renameFile() {
    NodeModel::instance()->renameNode(m_file, m_nameEdit->text());
    this->close();
}

void FileDetailsDialog::deleteFile() {
    NodeModel::instance()->deleteNode(m_file);
    this->close();
}

void FileDetailsDialog::toggleFilePublished() {
    NodeModel::instance()->setFilePublic(m_file, !m_file.data(NodeRoles::IsPublicRole).toBool());
    this->close();
}

void FileDetailsDialog::loadFileData() {
    m_nameEdit->setText(m_file.data(NodeRoles::NameRole).toString());
    m_publicUrlLabel->setText(QString("%1: %2").arg("Public url").arg(m_file.data(NodeRoles::IsPublicRole).toBool() ? QString("<a href='%1'>%1</a>").arg(m_file.data(NodeRoles::PublicUrlRole).toString()) : tr("Not published")));
    m_createdLabel->setText(QString("%1: %2").arg(tr("Created")).arg(m_file.data(NodeRoles::WhenCreatedRole).toDateTime().toString()));
    m_modifiedLabel->setText(QString("%1: %2").arg(tr("Modified")).arg(m_file.data(NodeRoles::LastModifiedRole).toDateTime().toString()));
    m_sizeLabel->setText(QString("%1: %2").arg(tr("Size")).arg(Utils::fileSizeFromBytes(m_file.data(NodeRoles::SizeRole).toLongLong())));
    m_publishButton->setText(m_file.data(NodeRoles::IsPublicRole).toBool() ? tr("Unpublish") : tr("Publish"));
    m_renameButton->setEnabled(false);
}
