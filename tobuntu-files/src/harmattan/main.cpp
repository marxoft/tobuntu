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

#include "authentication.h"
#include "../shared/settings.h"
#include "../shared/transfermanager.h"
#include "../shared/notifications.h"
#include "../shared/definitions.h"
#include "../shared/storage.h"
#include "../shared/utils.h"
#include "../shared/screenorientationmodel.h"
#include "../shared/languagemodel.h"
#include <QApplication>
#include <QTranslator>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <qdeclarative.h>
#include <QGLWidget>
#include <MDeclarativeCache>
#include <QDebug>

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    QScopedPointer<QApplication> app(MDeclarativeCache::qApplication(argc, argv));
    app.data()->setOrganizationName("toBuntu");
    app.data()->setApplicationName("toBuntu");
    app.data()->setApplicationVersion(VERSION_NUMBER);

    QScopedPointer<TransferManager> transferManager(TransferManager::instance());
    QScopedPointer<Settings> settings(Settings::instance());
    QScopedPointer<Notifications> notifications(Notifications::instance());

    Storage storage;
    Utils utils;

    QTranslator translator;

    if (translator.load(QString("/opt/tobuntu/translations/%1.qm").arg(Settings::instance()->language()))) {
        app.data()->installTranslator(&translator);
    }
    else {
        qWarning() << "No translation found for chosen language";
    }

    qRegisterMetaType<ScreenOrientation::Orientation>("ScreenOrientation::Orientation");
    qmlRegisterType<ScreenOrientationModel>("com.marxoft.models", 1, 0, "ScreenOrientationModel");
    qmlRegisterType<LanguageModel>("com.marxoft.models", 1, 0, "LanguageModel");
    qmlRegisterUncreatableType<ScreenOrientation>("com.marxoft.enums", 1, 0, "ScreenOrientation", "");

    QScopedPointer<QDeclarativeView> view(MDeclarativeCache::qDeclarativeView());
    QDeclarativeContext *context = view.data()->engine()->rootContext();
    context->setContextProperty("Settings", Settings::instance());
    context->setContextProperty("TransferManager", TransferManager::instance());
    context->setContextProperty("Notifications", Notifications::instance());
    context->setContextProperty("Storage", &storage);
    context->setContextProperty("Utils", &utils);
    context->setContextProperty("VERSION_NUMBER", VERSION_NUMBER);

    view.data()->setViewport(new QGLWidget);
    view.data()->setSource(QUrl("qrc:/main.qml"));
    view.data()->showFullScreen();

    return app.data()->exec();
}
