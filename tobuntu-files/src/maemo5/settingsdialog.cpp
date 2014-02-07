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

#include "settingsdialog.h"
#include "separatorlabel.h"
#include "valueselector.h"
#include "../shared/settings.h"
#include "../shared/languagemodel.h"
#include "../shared/screenorientationmodel.h"
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QMaemo5ValueButton>
#include <QCheckBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    AbstractDialog(parent),
    m_downloadCheckbox(new QCheckBox(tr("Use default download folder"), this)),
    m_downloadPathButton(new QMaemo5ValueButton(tr("Download folder"), this)),
    m_orientationSelector(new ValueSelector(tr("Screen orientation"), this)),
    m_languageSelector(new ValueSelector(tr("Language"), this)),
    m_buttonBox(new QDialogButtonBox(Qt::Vertical, this)),
    m_doneButton(m_buttonBox->addButton(tr("Save"), QDialogButtonBox::ActionRole)),
    m_scrollArea(new QScrollArea(this)),
    m_grid(new QGridLayout(this))
{
    this->setWindowTitle(tr("Settings"));

    QWidget *scrollWidget = new QWidget(m_scrollArea);
    QVBoxLayout *vbox = new QVBoxLayout(scrollWidget);

    vbox->addWidget(new SeparatorLabel(tr("Transfers"), this));
    vbox->addWidget(m_downloadCheckbox);
    vbox->addWidget(m_downloadPathButton);
    vbox->addWidget(new SeparatorLabel(tr("Other"), this));
    vbox->addWidget(m_languageSelector);
    vbox->addWidget(m_orientationSelector);

    m_orientationSelector->setModel(new ScreenOrientationModel(m_orientationSelector));
    m_languageSelector->setModel(new LanguageModel(m_languageSelector));

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    this->connect(m_doneButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
    this->connect(m_downloadCheckbox, SIGNAL(toggled(bool)), m_downloadPathButton, SLOT(setEnabled(bool)));
    this->connect(m_downloadPathButton, SIGNAL(clicked()), this, SLOT(showFileDialog()));

    this->loadSettings();
    this->onOrientationChanged();
}

void SettingsDialog::setLandscapeLayout() {
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

void SettingsDialog::setPortraitLayout() {
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

void SettingsDialog::showFileDialog() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Download folder"), m_downloadPath);

    if (!path.isEmpty()) {
        m_downloadPath = path;
        m_downloadPathButton->setValueText(m_downloadPath);
    }
}

void SettingsDialog::loadSettings() {
    m_downloadCheckbox->setChecked(Settings::instance()->useDefaultDownloadDirectory());
    m_downloadPath = Settings::instance()->defaultDownloadDirectory();
    m_downloadPathButton->setValueText(m_downloadPath);
    m_downloadPathButton->setEnabled(m_downloadCheckbox->isChecked());
    m_languageSelector->setValue(Settings::instance()->language());
    m_orientationSelector->setValue(Settings::instance()->screenOrientation());
}

void SettingsDialog::saveSettings() {
    Settings::instance()->setUseDefaultDownloadDirectory(m_downloadCheckbox->isChecked());
    Settings::instance()->setDefaultDownloadDirectory(m_downloadPath);
    Settings::instance()->setLanguage(m_languageSelector->currentValue().toString());
    Settings::instance()->setScreenOrientation(static_cast<ScreenOrientation::Orientation>(m_orientationSelector->currentValue().toInt()));

    this->accept();
}
