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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "abstractdialog.h"

class QLabel;
class QDialogButtonBox;
class QGridLayout;

class AboutDialog : public AbstractDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);

private slots:
    void reportBug();
    void donate();

private:
    void setLandscapeLayout();
    void setPortraitLayout();

private:
    QLabel *m_icon;
    QLabel *m_title;
    QLabel *m_details;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_donateButton;
    QPushButton *m_bugButton;
    QGridLayout *m_grid;
};

#endif // ABOUTDIALOG_H