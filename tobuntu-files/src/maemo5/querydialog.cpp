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

#include "querydialog.h"
#include <QLabel>
#include <QDialogButtonBox>
#include <QGridLayout>

QueryDialog::QueryDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_message(new QLabel(this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, Qt::Vertical, this)),
    m_grid(new QGridLayout(this))
{
    m_message->setWordWrap(true);

    this->connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    this->connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->onOrientationChanged();
}

QueryDialog::QueryDialog(const QString &message, QWidget *parent) :
    AbstractDialog(parent),
    m_message(new QLabel(message, this)),
    m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Yes | QDialogButtonBox::No, Qt::Vertical, this)),
    m_grid(new QGridLayout(this))
{
    m_message->setWordWrap(true);

    this->connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    this->connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    this->onOrientationChanged();
}

void QueryDialog::setMessage(const QString &message) {
    m_message->setText(message);
    this->onOrientationChanged();
}

void QueryDialog::setLandscapeLayout() {
    this->setFixedHeight(m_message->height() + 150);
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_message, 0, 0, Qt::AlignTop);
        m_grid->addWidget(m_buttonBox, 0, 1, Qt::AlignBottom);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 0, 1, Qt::AlignBottom);
    }
}

void QueryDialog::setPortraitLayout() {
    this->setFixedHeight(m_message->height() + 250);
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_message, 0, 0, Qt::AlignTop);
        m_grid->addWidget(m_buttonBox, 1, 0, Qt::AlignBottom);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 1, 0, Qt::AlignBottom);
    }
}
