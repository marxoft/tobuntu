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

#include "settings.h"
#include <qplatformdefs.h>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

Settings* Settings::m_instance = 0;

Settings::Settings() :
    QSettings("toBuntu", "toBuntu")
{
    if (!m_instance) {
        m_instance = this;
    }
}

Settings::~Settings() {}

Settings* Settings::instance() {
    return !m_instance ? new Settings : m_instance;
}

QVariant Settings::setting(const QString &key) const {
    return this->value(key);
}

void Settings::setSetting(const QString &key, const QString &value) {
    this->setValue(key, value);
}

QString Settings::defaultDownloadDirectory() const {
#if (defined Q_WS_MAEMO_5) || (defined MEEGO_EDITION_HARMATTAN) || (defined SAILFISH_OS)
    return this->value("Transfers/defaultDownloadDirectory", "/home/user/MyDocs/toBuntu/").toString();
#elif (defined Q_OS_SYMBIAN)
    return this->value("Transfers/defaultDownloadDirectory", "E:/toBuntu/").toString();
#elif QT_VERSION >= 0x050000
    return this->value("Transfers/defaultDownloadDirectory", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/toBuntu/").toString();
#else
    return this->value("Transfers/defaultDownloadDirectory", QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/toBuntu/").toString();
#endif
}

void Settings::setDefaultDownloadDirectory(const QString &directory) {
    if (directory != this->defaultDownloadDirectory()) {
        this->setValue("Transfers/defaultDownloadDirectory", directory.endsWith('/') ? directory : directory + '/');
        emit defaultDownloadDirectoryChanged(directory);
    }
}

bool Settings::useDefaultDownloadDirectory() const {
    return this->value("Transfers/useDefaultDownloadDirectory", true).toBool();
}

void Settings::setUseDefaultDownloadDirectory(bool useDefault) {
    if (useDefault != this->useDefaultDownloadDirectory()) {
        this->setValue("Transfers/useDefaultDownloadDirectory", useDefault);
        emit useDefaultDownloadDirectoryChanged(useDefault);
    }
}

ScreenOrientation::Orientation Settings::screenOrientation() const {
    return static_cast<ScreenOrientation::Orientation>(this->value("General/screenOrientation",
                                                                   ScreenOrientation::Orientation(ScreenOrientation::Automatic)).toInt());
}

void Settings::setScreenOrientation(ScreenOrientation::Orientation orientation) {
    if (orientation != this->screenOrientation()) {
        this->setValue("General/screenOrientation", ScreenOrientation::Orientation(orientation));
        emit screenOrientationChanged(orientation);
    }
}

QString Settings::language() const {
    return this->value("General/language", "en").toString();
}

void Settings::setLanguage(const QString &language) {
    if (language != this->language()) {
        this->setValue("General/language", language);
        emit languageChanged(language);
    }
}

#if (defined MEEGO_EDITION_HARMATTAN) || (defined Q_OS_SYMBIAN)
bool Settings::useDarkTheme() const {
    return this->value("General/useDarkTheme", false).toBool();
}

void Settings::setUseDarkTheme(bool dark) {
    if (dark != this->useDarkTheme()) {
        this->setValue("General/useDarkTheme", dark);
        emit useDarkThemeChanged(dark);
    }
}
#endif
