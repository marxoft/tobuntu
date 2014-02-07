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

#include "accountdialog.h"
#include "account.h"
#include "authentication.h"
#include "separatorlabel.h"
#include "../shared/storage.h"
#include "../shared/utils.h"
#include "../shared/notifications.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>

AccountDialog::AccountDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_totalBytesLabel(new QLabel(this)),
    m_usedBytesLabel(new QLabel(this)),
    m_planLabel(new QLabel(this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_button(m_buttonBox->addButton(tr("Delete"), QDialogButtonBox::ActionRole)),
    m_scrollArea(new QScrollArea(this)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("Account"));

    QWidget *scrollWidget = new QWidget(m_scrollArea);
    QVBoxLayout *vbox = new QVBoxLayout(scrollWidget);
    vbox->addWidget(new SeparatorLabel(tr("Storage"), this));
    vbox->addWidget(m_totalBytesLabel);
    vbox->addWidget(m_usedBytesLabel);
    vbox->addWidget(new SeparatorLabel(tr("Plan"), this));
    vbox->addWidget(m_planLabel);

    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(scrollWidget);

    this->onOrientationChanged();
    this->connect(m_button, SIGNAL(clicked()), this, SLOT(signOut()));
}

void AccountDialog::setLandscapeLayout() {
    this->setFixedHeight(340);
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_scrollArea, 0, 0);
        m_grid->addWidget(m_buttonBox, 0, 1, Qt::AlignBottom);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 0, 1, Qt::AlignBottom);
    }
}

void AccountDialog::setPortraitLayout() {
    this->setFixedHeight(600);
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_scrollArea, 0, 0);
        m_grid->addWidget(m_buttonBox, 1, 0);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 1, 0);
    }
}

void AccountDialog::getAccount() {
    this->toggleBusy(true);

    StorageQuota *quota = Account::getStorageQuota();
    this->connect(quota, SIGNAL(ready(StorageQuota*)), this, SLOT(onStorageQuotaReady(StorageQuota*)));
}

void AccountDialog::onStorageQuotaReady(StorageQuota *quota) {
    switch (quota->error()) {
    case StorageQuota::NoError:
        m_totalBytesLabel->setText(tr("Total: %1").arg(Utils::fileSizeFromBytes(quota->totalStorage())));
        m_usedBytesLabel->setText(tr("Used: %1").arg(Utils::fileSizeFromBytes(quota->usedStorage())));
        break;
    default:
        break;
    }

    quota->deleteLater();

    UserAccount *account = Account::getAccount();
    this->connect(account, SIGNAL(ready(UserAccount*)), this, SLOT(onUserAccountReady(UserAccount*)));
}

void AccountDialog::onUserAccountReady(UserAccount *account) {
    this->toggleBusy(false);

    switch (account->error()) {
    case UserAccount::NoError:
        m_planLabel->setText(account->currentPlan());
        break;
    default:
        Notifications::showError(account->errorString());
        break;
    }

    account->deleteLater();    
}

void AccountDialog::signOut() {
    if (Storage::removeAccount("toBuntu")) {
        Authentication::clearCredentials();
        Notifications::showInformation(tr("You have deleted your Ubuntu One account."));
        this->accept();
    }
    else {
        Notifications::showError(tr("Database error. Unable to delete your Ubuntu One account."));
    }
}
