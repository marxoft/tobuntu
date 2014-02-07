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

#include "filenamedialog.h"
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

FileNameDialog::FileNameDialog(const QString &hint, const QString &name, QWidget *parent) :
    AbstractDialog(parent),
    m_hintLabel(new QLabel(hint, this)),
    m_nameEdit(new QLineEdit(name, this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_doneButton(m_buttonBox->addButton(tr("Done"), QDialogButtonBox::AcceptRole)),
    m_grid(new QGridLayout(this))
{
    this->connect(m_nameEdit, SIGNAL(textChanged(QString)), this, SLOT(onFileNameChanged(QString)));
    this->connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    this->onOrientationChanged();
    this->onFileNameChanged(m_nameEdit->text());
}

void FileNameDialog::setLandscapeLayout() {
    this->setFixedHeight(150);
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_hintLabel, 0, 0);
        m_grid->addWidget(m_nameEdit, 1, 0);
        m_grid->addWidget(m_buttonBox, 1, 1);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 1, 1);
    }
}

void FileNameDialog::setPortraitLayout() {
    this->setFixedHeight(220);
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_hintLabel, 0, 0);
        m_grid->addWidget(m_nameEdit, 1, 0);
        m_grid->addWidget(m_buttonBox, 2, 0);
    }
    else {
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 2, 0);
    }
}

void FileNameDialog::onFileNameChanged(const QString &name) {
    m_doneButton->setEnabled(!name.isEmpty());
}

void FileNameDialog::accept() {
    emit fileNameChosen(m_nameEdit->text());
    AbstractDialog::accept();
}
