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

#include "mainwindow.h"
#include "authentication.h"
#include "../shared/settings.h"
#include "../shared/nodemodel.h"
#include "../shared/transfermanager.h"
#include "../shared/definitions.h"
#include "../shared/storage.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("toBuntu");
    app.setApplicationName("toBuntu");
    app.setApplicationVersion(VERSION_NUMBER);

    QScopedPointer<TransferManager> transferManager(TransferManager::instance());
    QScopedPointer<Settings> settings(Settings::instance());
    QScopedPointer<NodeModel> nodeModel(NodeModel::instance());

    QTranslator translator;

    if (translator.load(QString("/opt/tobuntu/translations/%1.qm").arg(Settings::instance()->language()))) {
        app.installTranslator(&translator);
    }
    else {
        qWarning() << "No translation found for chosen language";
    }

    Storage::init();
    Acc account = Storage::getAccount("toBuntu");
    Authentication::setCredentials(account.consumerKey,
                                   account.consumerSecret,
                                   account.tokenKey,
                                   account.tokenSecret);

    QTimer::singleShot(3000, TransferManager::instance(), SLOT(restoreStoredTransfers()));

    MainWindow window;
    window.show();

    return app.exec();
}
