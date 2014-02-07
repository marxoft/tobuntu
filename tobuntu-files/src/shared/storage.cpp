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

#include "storage.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qplatformdefs.h>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

#if (defined Q_WS_MAEMO_5) || (defined MEEGO_EDITION_HARMATTAN)
static const QString DIRECTORY("/home/user/.toBuntu/");
#elif (defined Q_OS_SYMBIAN)
static const QString DIRECTORY("E:/.toBuntu/");
#elif QT_VERSION >= 0x050000
static const QString DIRECTORY(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.toBuntu/");
#else
static const QString DIRECTORY(QDesktopServices::storageLocation(QDesktopServices::HomeLocation) + "/.toBuntu/");
#endif

Storage::Storage(QObject *parent) :
    QObject(parent)
{
    this->init();
}

Storage::~Storage() {}

void Storage::init() {
    QDir dir;
    dir.mkpath(DIRECTORY);

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DIRECTORY + "toBuntu.db");

    if (database.open()) {
        database.exec("CREATE TABLE IF NOT EXISTS accounts (tokenName TEXT UNIQUE, consumerKey TEXT, consumerSecret TEXT, tokenKey TEXT, tokenSecret TEXT)");
    }
}

bool Storage::storeAccount(const QString &tokenName,
                           const QString &consumerKey,
                           const QString &consumerSecret,
                           const QString &tokenKey,
                           const QString &tokenSecret) {

    Acc account;
    account.tokenName = tokenName;
    account.consumerKey = consumerKey;
    account.consumerSecret = consumerSecret;
    account.tokenKey = tokenKey;
    account.tokenSecret = tokenSecret;

    return Storage::storeAccount(account);
}

bool Storage::storeAccount(Acc account) {
    if (!QSqlDatabase::database().open()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO accounts VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(account.tokenName);
    query.addBindValue(account.consumerKey);
    query.addBindValue(account.consumerSecret);
    query.addBindValue(account.tokenKey);
    query.addBindValue(account.tokenSecret);

    return query.exec();
}

bool Storage::removeAccount(const QString &tokenName) {
    if (!QSqlDatabase::database().isOpen()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM accounts WHERE tokenName = ?");
    query.addBindValue(tokenName);

    return query.exec();
}

Acc Storage::getAccount(const QString &tokenName) {
    Acc account;

    if (QSqlDatabase::database().open()) {
        QSqlQuery query;
        query.prepare("SELECT * FROM accounts WHERE tokenName = ?");
        query.addBindValue(tokenName);
        query.exec();

        if (query.record().count() > 0) {
            while (query.next()) {
                account.tokenName = query.value(0).toString();
                account.consumerKey = query.value(1).toString();
                account.consumerSecret = query.value(2).toString();
                account.tokenKey = query.value(3).toString();
                account.tokenSecret = query.value(4).toString();
            }
        }
    }

    return account;
}

QVariantMap Storage::getAccountMap(const QString &tokenName) {
    Acc account = Storage::getAccount(tokenName);
    QVariantMap map;
    map.insert("tokenName", account.tokenName);
    map.insert("consumerKey", account.consumerKey);
    map.insert("consumerSecret", account.consumerSecret);
    map.insert("tokenKey", account.tokenKey);
    map.insert("tokenSecret", account.tokenSecret);

    return map;
}

bool Storage::storeTransfers(QList<FileTransfer *> transfers) {
    if (transfers.isEmpty()) {
        return false;
    }

    QDir dir;

    if (!dir.mkpath(DIRECTORY)) {
        return false;
    }

    QFile file(DIRECTORY + "transfers.xml");

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("transfers");

    foreach (FileTransfer *transfer, transfers) {
        writer.writeStartElement("transfer");
        writer.writeTextElement("transferType", QString::number(FileTransfer::TransferType(transfer->transferType())));
        writer.writeTextElement("contentPath", transfer->contentPath());
        writer.writeTextElement("filePath", transfer->filePath());
        writer.writeTextElement("contentType", transfer->contentType());
        writer.writeTextElement("isPublic", transfer->isPublic() ? "true" : "false");
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    return true;
}

QList<FileTransfer*> Storage::restoreTransfers() {
    QList<FileTransfer*> transfers;

    if (!QFile::exists(DIRECTORY + "transfers.xml")) {
        return transfers;
    }

    QFile file(DIRECTORY + "transfers.xml");

    if (!file.open(QIODevice::ReadOnly)) {
        return transfers;
    }

    QXmlStreamReader reader(&file);

    while ((!reader.atEnd()) && (!reader.hasError())) {
        while ((!reader.atEnd()) && (!reader.hasError()) && (reader.name() != "transfer")) {
            while (reader.readNext() != QXmlStreamReader::Invalid) {
                if ((reader.isStartElement()) || (reader.isEndElement())) {
                    break;
                }
            }
        }

        if (reader.name() == "transfer") {
            FileTransfer *transfer = new FileTransfer;

            while (reader.readNext() != QXmlStreamReader::Invalid) {
                if ((reader.isStartElement()) || (reader.isEndElement())) {
                    break;
                }
            }

            while ((!reader.atEnd()) && (!reader.hasError()) && (reader.name() != "transfer")) {
                if (reader.name() == "transferType") {
                    transfer->setTransferType(static_cast<FileTransfer::TransferType>(reader.readElementText().toInt()));
                }
                else if (reader.name() == "contentPath") {
                    transfer->setContentPath(reader.readElementText());
                }
                else if (reader.name() == "filePath") {
                    transfer->setFilePath(reader.readElementText());
                }
                else if (reader.name() == "contentType") {
                    transfer->setContentType(reader.readElementText());
                }
                else if (reader.name() == "isPublic") {
                    transfer->setPublic(reader.readElementText() == "true");
                }

                while (reader.readNext() != QXmlStreamReader::Invalid) {
                    if ((reader.isStartElement()) || (reader.isEndElement())) {
                        break;
                    }
                }
            }

            while (reader.readNext() != QXmlStreamReader::Invalid) {
                if ((reader.isStartElement()) || (reader.isEndElement())) {
                    break;
                }
            }

            transfers.append(transfer);
        }
    }

    return transfers;
}

bool Storage::clearStoredTransfers() {
    return QFile::remove(DIRECTORY + "transfers.xml");
}
