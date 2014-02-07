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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "screenorientation.h"
#include <QSettings>
#include <qplatformdefs.h>

class Settings : public QSettings
{
    Q_OBJECT

    Q_PROPERTY(QString defaultDownloadDirectory
               READ defaultDownloadDirectory
               WRITE setDefaultDownloadDirectory
               NOTIFY defaultDownloadDirectoryChanged)
    Q_PROPERTY(bool useDefaultDownloadDirectory
               READ useDefaultDownloadDirectory
               WRITE setUseDefaultDownloadDirectory
               NOTIFY useDefaultDownloadDirectoryChanged)
    Q_PROPERTY(ScreenOrientation::Orientation screenOrientation
               READ screenOrientation
               WRITE setScreenOrientation
               NOTIFY screenOrientationChanged)
    Q_PROPERTY(QString language
               READ language
               WRITE setLanguage
               NOTIFY languageChanged)
#if (defined MEEGO_EDITION_HARMATTAN) || (defined Q_OS_SYMBIAN)
    Q_PROPERTY(bool useDarkTheme
               READ useDarkTheme
               WRITE setUseDarkTheme
               NOTIFY useDarkThemeChanged)
#endif

public:
    ~Settings();

    static Settings* instance();

    Q_INVOKABLE QVariant setting(const QString &key) const;

    QString defaultDownloadDirectory() const;

    bool useDefaultDownloadDirectory() const;

    ScreenOrientation::Orientation screenOrientation() const;

    QString language() const;
#if (defined MEEGO_EDITION_HARMATTAN) || (defined Q_OS_SYMBIAN)
    bool useDarkTheme() const;
#endif

public slots:
    void setSetting(const QString &key, const QString &value);

    void setDefaultDownloadDirectory(const QString &directory);

    void setUseDefaultDownloadDirectory(bool useDefault);

    void setScreenOrientation(ScreenOrientation::Orientation orientation);

    void setLanguage(const QString &language);
#if (defined MEEGO_EDITION_HARMATTAN) || (defined Q_OS_SYMBIAN)
    void setUseDarkTheme(bool dark);
#endif

signals:
    void defaultDownloadDirectoryChanged(const QString &directory);
    void useDefaultDownloadDirectoryChanged(bool useDefault);
    void screenOrientationChanged(ScreenOrientation::Orientation orientation);
    void languageChanged(const QString &language);
#if (defined MEEGO_EDITION_HARMATTAN) || (defined Q_OS_SYMBIAN)
    void useDarkThemeChanged(bool dark);
#endif

private:
    Settings();

    static Settings *m_instance;
};

#endif // SETTINGS_H
