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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "abstractdialog.h"

class QLineEdit;
class QCheckBox;
class QMaemo5ValueButton;
class QDialogButtonBox;
class QScrollArea;
class QGridLayout;
class ValueSelector;

class SettingsDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private slots:
    void showFileDialog();
    void loadSettings();
    void saveSettings();
    
private:
    QCheckBox *m_downloadCheckbox;
    QMaemo5ValueButton *m_downloadPathButton;

    ValueSelector *m_orientationSelector;
    ValueSelector *m_languageSelector;

    QDialogButtonBox *m_buttonBox;
    QPushButton *m_doneButton;

    QScrollArea *m_scrollArea;

    QGridLayout *m_grid;

    QString m_downloadPath;
};

#endif // SETTINGSDIALOG_H
