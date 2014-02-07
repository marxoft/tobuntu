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

#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include "abstractdialog.h"
#include "useraccount.h"
#include "storagequota.h"

class QLabel;
class QDialogButtonBox;
class QScrollArea;
class QGridLayout;

using namespace QtUbuntuOne;

class AccountDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit AccountDialog(QWidget *parent = 0);

    void getAccount();
    
private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void onStorageQuotaReady(StorageQuota *quota);
    void onUserAccountReady(UserAccount *account);
    void signOut();

private:
    QLabel *m_totalBytesLabel;
    QLabel *m_usedBytesLabel;
    QLabel *m_planLabel;

    QDialogButtonBox *m_buttonBox;
    QPushButton *m_button;

    QScrollArea *m_scrollArea;
    QGridLayout *m_grid;
};

#endif // ACCOUNTDIALOG_H
