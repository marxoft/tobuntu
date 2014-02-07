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

#ifndef STORAGE_H
#define STORAGE_H

#include "filetransfer.h"
#include <QObject>

using namespace QtUbuntuOne;

struct Acc {
    QString tokenName;
    QString consumerKey;
    QString consumerSecret;
    QString tokenKey;
    QString tokenSecret;
};

class Storage : public QObject
{
    Q_OBJECT

public:
    explicit Storage(QObject *parent = 0);
    ~Storage();

    Q_INVOKABLE static void init();

    Q_INVOKABLE static bool storeAccount(const QString &tokenName,
                      const QString &consumerKey,
                      const QString &consumerSecret,
                      const QString &tokenKey,
                      const QString &tokenSecret);

    Q_INVOKABLE static bool storeAccount(Acc account);

    Q_INVOKABLE static bool removeAccount(const QString &tokenName);

    Q_INVOKABLE static Acc getAccount(const QString &tokenName);
    Q_INVOKABLE static QVariantMap getAccountMap(const QString &tokenName);

    Q_INVOKABLE static bool storeTransfers(QList<FileTransfer*> transfers);
    Q_INVOKABLE static QList<FileTransfer*> restoreTransfers();
    Q_INVOKABLE static bool clearStoredTransfers();
};

#endif // STORAGE_H
