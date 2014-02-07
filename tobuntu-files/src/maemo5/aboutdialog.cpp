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

#include "aboutdialog.h"
#include "../shared/definitions.h"
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>

AboutDialog::AboutDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_icon(new QLabel(this)),
    m_title(new QLabel("toBuntu " + VERSION_NUMBER, this)),
    m_details(new QLabel(tr("A client for Ubuntu One. toBuntu uses the libqubuntuone shared library.") + "<br><br> &copy; Stuart Howarth 2012-2014", this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_donateButton(m_buttonBox->addButton(tr("Donate"), QDialogButtonBox::ActionRole)),
    m_bugButton(m_buttonBox->addButton(tr("Report bug"), QDialogButtonBox::ActionRole)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("About"));
    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    m_icon->setPixmap(QPixmap("/usr/share/icons/hicolor/64x64/apps/tobuntu.png"));
    m_icon->setFixedWidth(m_icon->pixmap()->width());
    QFont font;
    font.setBold(true);
    font.setPixelSize(32);
    m_title->setFont(font);
    m_details->setWordWrap(true);

    this->connect(m_donateButton, SIGNAL(clicked()), this, SLOT(donate()));
    this->connect(m_bugButton, SIGNAL(clicked()), this, SLOT(reportBug()));

    this->onOrientationChanged();
}

void AboutDialog::setLandscapeLayout() {
    m_buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_icon, 0, 0);
        m_grid->addWidget(m_title, 0, 1);
        m_grid->addWidget(m_details, 1, 0, 1, 2, Qt::AlignBottom);
        m_grid->addWidget(m_buttonBox, 1, 2);
    }
    else {
        m_grid->setAlignment(m_details, Qt::AlignBottom);
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 1, 2);
    }
}

void AboutDialog::setPortraitLayout() {
    m_buttonBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (m_grid->isEmpty()) {
        m_grid->addWidget(m_icon, 0, 0);
        m_grid->addWidget(m_title, 0, 1);
        m_grid->addWidget(m_details, 1, 0, 1, 2, Qt::AlignHCenter);
        m_grid->addWidget(m_buttonBox, 2, 0, 1, 2);
    }
    else {
        m_grid->setAlignment(m_details, Qt::AlignHCenter);
        m_grid->removeWidget(m_buttonBox);
        m_grid->addWidget(m_buttonBox, 2, 0, 1, 2);
    }
}

void AboutDialog::donate() {
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=stuhowarth77@gmail.com&lc=GB&item_name=toBuntu%20for%20Maemo5&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted"));
    this->close();
}

void AboutDialog::reportBug() {
    QDesktopServices::openUrl(QUrl(QString("mailto:showarth@marxoft.co.uk?subject=toBuntu %1 for Maemo5").arg(VERSION_NUMBER)));
    this->close();
}
