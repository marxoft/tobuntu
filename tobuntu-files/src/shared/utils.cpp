/*
 * Copyright (C) 2014 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
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

#include "utils.h"
#include <QFileInfo>
#include <QFile>
#ifndef QML_USER_INTERFACE
#include <QPainter>
#include <QRect>
#include <QImage>
#endif

Utils::Utils(QObject *parent) :
    QObject(parent)
{
}

Utils::~Utils() {}

QString Utils::durationFromSecs(int secs) {
    QTime time(0, 0);
    QString format = secs >= 3600 ? "hh:mm:ss" : "mm:ss";

    return time.addSecs(secs).toString(format);
}

QString Utils::durationFromMSecs(int msecs) {
    QTime time(0, 0);
    QString format = msecs >= 3600000 ? "hh:mm:ss" : "mm:ss";

    return time.addMSecs(msecs).toString(format);
}

QString Utils::fileSizeFromPath(const QString &filePath) {
    QFileInfo file(filePath);
    return Utils::fileSizeFromBytes(file.size());
}

QString Utils::fileSizeFromBytes(double bytes) {
    double kb = 1024;
    double mb = kb * 1024;
    double gb = mb * 1024;

    QString size;

    if (bytes > gb) {
        size = QString::number(bytes / gb, 'f', 2) + "GB";
    }
    else if (bytes > mb) {
        size = QString::number(bytes / mb, 'f', 2) + "MB";
    }
    else if (bytes > kb){
        size = QString::number(bytes / kb, 'f', 2) + "KB";
    }
    else {
        size = QString::number(bytes) + "B";
    }

    return size;
}

QString Utils::dateFromSecs(qint64 secs, bool showTime) {
    return Utils::dateFromMSecs(secs * 1000, showTime);
}

QString Utils::dateFromMSecs(qint64 msecs, bool showTime) {
    QString date;

    if (showTime) {
        date = Utils::dateTimeFromMSecsSinceEpoch(msecs).toString("dd/MM/yyyy | HH:mm");
    }
    else {
        date = Utils::dateTimeFromMSecsSinceEpoch(msecs).toString("dd/MM/yyyy");
    }

    return date;
}

QString Utils::base64Id(const QString &artist, const QString &title) {
    return QString(artist.toUtf8().toBase64() + "-" + title.toUtf8().toBase64());
}

QString Utils::localDateTimeFromString(const QString &dateTimeString, Qt::DateFormat format) {
    QDateTime dt = QDateTime::fromString(dateTimeString, format);

    if (!dt.isValid()) {
        dt = QDateTime::currentDateTime();
    }

    return dt.toLocalTime().toString("dd/MM/yyyy | HH:mm");
}

qint64 Utils::currentMSecsSinceEpoch() {
#if QT_VERSION >= 0x040700
    return QDateTime::currentMSecsSinceEpoch();
#else
    return qint64(QDateTime::currentDateTime().toTime_t()) * 1000;
#endif
}

qint64 Utils::dateTimeToMSecsSinceEpoch(const QDateTime &dateTime) {
#if QT_VERSION >= 0x040700
    return dateTime.toMSecsSinceEpoch();
#else
    return qint64(dateTime.toTime_t()) * 1000;
#endif
}

QDateTime Utils::dateTimeFromMSecsSinceEpoch(qint64 msecs) {
#if QT_VERSION >= 0x040700
    return QDateTime::fromMSecsSinceEpoch(msecs);
#else
    return QDateTime::fromTime_t(msecs / 1000);
#endif
}

QString Utils::httpErrorString(int errorCode) {
    switch (errorCode) {
    case 400:
        return tr("Bad request");
    case 401:
        return tr("Request is unauthorised");
    case 403:
        return tr("Request is forbidden");
    case 404:
        return tr("Requested resource is unavailable");
    case 406:
        return tr("Requested resource is not accessible");
    case 422:
        return tr("Request cannot be processed");
    case 429:
        return tr("Request limit has been reached. Please try again later");
    case 500:
        return tr("Internal server error. Please try again later");
    case 503:
        return tr("Service unavailable. Please try again later");
    case 504:
        return tr("Request timed out. Please try again later");
    default:
        return tr("Unknown error. Please try again later");
    }
}

#ifdef Q_WS_MAEMO_5
QString Utils::iconPathFromFileSuffix(const QString &suffix) {
    if (suffix == "zip") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/application-zip.png");
    }
    else if (suffix == "pdf") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-application-pdf.png");
    }
    else if (suffix == "deb") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-application-x-deb.png");
    }
    else if (suffix == "swf") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-application-x-shockwave-flash.png");
    }
    else if (suffix == "mp2") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-mp2.png");
    }
    else if (suffix == "mp3") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-mp3.png");
    }
    else if (suffix == "ogg") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-ogg.png");
    }
    else if (suffix == "wav") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-wav.png");
    }
    else if (suffix == "flac") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-x-flac.png");
    }
    else if (suffix == "m4a") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-x-m4a.png");
    }
    else if (suffix == "wma") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-audio-x-ms-wma.png");
    }
    else if (suffix == "bmp") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-bmp.png");
    }
    else if (suffix == "gif") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-gif.png");
    }
    else if (suffix == "jpeg") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-jpeg.png");
    }
    else if (suffix == "png") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-png.png");
    }
    else if (suffix == "svg") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-svg+xml.png");
    }
    else if (suffix == "tiff") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-image-tiff.png");
    }
    else if (suffix == "css") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-text-css.png");
    }
    else if (suffix == "html") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-text-html.png");
    }
    else if (suffix == "txt") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-text-txt.png");
    }
    else if (suffix == "xml") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-text-xml.png");
    }
    else if (suffix == "avi") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-avi.png");
    }
    else if (suffix == "mp4") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-mp4.png");
    }
    else if (suffix == "mpeg") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-mpeg.png");
    }
    else if (suffix == "qt") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-quicktime.png");
    }
    else if (suffix == "wmv") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-x-ms-wmv.png");
    }
    else if (suffix == "asf") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-x-ms-asf.png");
    }
    else if (suffix == "rm") {
        return QString("/usr/share/icons/hicolor/48x48/mimetypes/gnome-mime-video-x-real.png");
    }
    else {
        return QString("/usr/share/icons/hicolor/48x48/hildon/filemanager_unknown_file.png");
    }
}
#endif

#ifdef MEEGO_EDITION_HARMATTAN
QString Utils::iconPathFromFileSuffix(const QString &suffix) {
    if (suffix == "zip") {
        return QString("icon-m-content-attachment");
    }
    else if (suffix == "pdf") {
        return QString("icon-m-content-pdf");
    }
    else if (suffix == "deb") {
        return QString("icon-m-content-application");
    }
    else if (suffix == "mp2") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "mp3") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "ogg") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "wav") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "flac") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "m4a") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "wma") {
        return QString("icon-m-content-audio");
    }
    else if (suffix == "bmp") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "gif") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "jpeg") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "png") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "svg") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "tiff") {
        return QString("icon-m-content-image");
    }
    else if (suffix == "css") {
        return QString("icon-m-content-document");
    }
    else if (suffix == "html") {
        return QString("icon-m-content-document");
    }
    else if (suffix == "txt") {
        return QString("icon-m-content-document");
    }
    else if (suffix == "xml") {
        return QString("icon-m-content-document");
    }
    else if (suffix == "avi") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "mp4") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "mpeg") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "qt") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "wmv") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "asf") {
        return QString("icon-m-content-video");
    }
    else if (suffix == "rm") {
        return QString("icon-m-content-video");
    }
    else {
        return QString("icon-m-content-file-unknown");
    }
}
#endif

void Utils::log(const QString &filePath, const QByteArray &message) {
    QFile lf(filePath);

    if (lf.open(QIODevice::Append)) {
        lf.write(QDateTime::currentDateTime().toString().toUtf8() + ": " + message + "\n");
    }

    lf.close();
}

#ifndef QML_USER_INTERFACE
void Utils::drawBorderImage(QPainter *painter, const QRect &rect, const QImage &image, int top, int right, int left, int bottom) {
    QRect imageRect = image.rect();

    // Top-left
    painter->drawImage(QRect(rect.left(), rect.top(), left, top),
                       image,
                       QRect(imageRect.left(), imageRect.top(), left, top));

    // Top-right
    painter->drawImage(QRect(rect.right() - right, rect.top(), right, top),
                       image,
                       QRect(imageRect.right() - right, imageRect.top(), right, top));

    // Bottom-left
    painter->drawImage(QRect(rect.left(), rect.bottom() - bottom, left, bottom),
                       image,
                       QRect(imageRect.left(), imageRect.bottom() - bottom, left, bottom));

    // Bottom-right
    painter->drawImage(QRect(rect.right() - right, rect.bottom() - bottom, bottom, right),
                       image,
                       QRect(imageRect.right() - right, imageRect.bottom() - bottom, right, bottom));

    // Top-middle
    painter->drawImage(QRect(rect.left() + left, rect.top(), rect.width() - (left + right), top),
                       image,
                       QRect(imageRect.left() + left, imageRect.top(), imageRect.width() - (left + right), top));

    // Bottom-middle
    painter->drawImage(QRect(rect.left() + left, rect.bottom() - bottom, rect.width() - (left + right), bottom),
                       image,
                       QRect(imageRect.left() + left, imageRect.bottom() - bottom, imageRect.width() - (left + right), bottom));

    // Left-middle
    painter->drawImage(QRect(rect.left(), rect.top() + top, left, rect.height() - (top + bottom)),
                       image,
                       QRect(imageRect.left(), imageRect.top() + top, left, imageRect.height() - (top + bottom)));

    // Right-middle
    painter->drawImage(QRect(rect.right() - right, rect.top() + top, right, rect.height() - (top + bottom)),
                       image,
                       QRect(imageRect.right() - right, imageRect.top() + top, right, imageRect.height() - (top + bottom)));

    // Center
    painter->drawImage(QRect(rect.left() + left, rect.top() + top, rect.width() - (left + right), rect.height() - (top + bottom)),
                       image,
                       QRect(imageRect.left() + left, imageRect.top() + top, imageRect.width() - (left + right), imageRect.height() - (top + bottom)));
}
#endif
