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

#include "authenticationdialog.h"
#include "authentication.h"
#include "../shared/storage.h"
#include "../shared/nodemodel.h"
#include "../shared/notifications.h"
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>

AuthenticationDialog::AuthenticationDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_emailEdit(new QLineEdit(this)),
    m_passwordEdit(new QLineEdit(this)),
    m_otpEdit(new QLineEdit(this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_button(m_buttonBox->addButton(tr("Done"), QDialogButtonBox::ActionRole)),
    m_scrollArea(new QScrollArea(this)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("Account"));

    QWidget *scrollWidget = new QWidget(m_scrollArea);
    QVBoxLayout *vbox = new QVBoxLayout(scrollWidget);
    vbox->addWidget(new QLabel(tr("Email address"), this));
    vbox->addWidget(m_emailEdit);
    vbox->addWidget(new QLabel(tr("Password"), this));
    vbox->addWidget(m_passwordEdit);
    vbox->addWidget(new QLabel(tr("One time password (optional)"), this));
    vbox->addWidget(m_otpEdit);

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_emailEdit->setValidator(new QRegExpValidator(QRegExp("[\\S]+@[\\S]+"), m_emailEdit));
    m_passwordEdit->setValidator(new QRegExpValidator(QRegExp("[\\S]+"), m_passwordEdit));
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_otpEdit->setValidator(new QRegExpValidator(QRegExp("([\\S]+|)"), m_passwordEdit));
    m_otpEdit->setEchoMode(QLineEdit::Password);

    m_button->setEnabled(false);

    this->connect(m_emailEdit, SIGNAL(textChanged(QString)), this, SLOT(onCredentialsChanged()));
    this->connect(m_passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(onCredentialsChanged()));
    this->connect(m_otpEdit, SIGNAL(textChanged(QString)), this, SLOT(onCredentialsChanged()));
    this->connect(m_button, SIGNAL(clicked()), this, SLOT(authenticate()));

    this->onOrientationChanged();
}

void AuthenticationDialog::setLandscapeLayout() {
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

void AuthenticationDialog::setPortraitLayout() {
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

void AuthenticationDialog::onCredentialsChanged() {
    m_button->setEnabled((m_emailEdit->hasAcceptableInput())
                                     && (m_passwordEdit->hasAcceptableInput())
                                     && (m_otpEdit->hasAcceptableInput()));
}

void AuthenticationDialog::authenticate() {
    this->toggleBusy(true);
    m_scrollArea->setEnabled(false);
    m_button->setEnabled(false);

    Token *token = Authentication::authenticate(m_emailEdit->text(),
                                                m_passwordEdit->text(),
                                                m_otpEdit->text(),
                                                "toBuntu");

    this->connect(token, SIGNAL(ready(Token*)), this, SLOT(onTokenReady(Token*)));
}

void AuthenticationDialog::onTokenReady(Token *token) {
    this->toggleBusy(false);
    m_scrollArea->setEnabled(true);
    m_button->setEnabled(true);

    switch (token->error()) {
    case Token::NoError:
        if (Storage::storeAccount("toBuntu",
                                          token->consumerKey(),
                                          token->consumerSecret(),
                                          token->tokenKey(),
                                          token->tokenSecret())) {

            Authentication::setCredentials(token->consumerKey(),
                                           token->consumerSecret(),
                                           token->tokenKey(),
                                           token->tokenSecret());

            Notifications::showInformation(tr("You have added your Ubuntu One account."));
            this->accept();
        }
        else {
            Notifications::showError(tr("Database error. Unable to add your Ubuntu One account."));
        }

        break;
    default:
        Notifications::showError(token->errorString());
        break;
    }

    token->deleteLater();
}
