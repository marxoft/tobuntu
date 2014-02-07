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

import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import com.marxoft.qubuntuone 1.0

PageStackWindow {
    id: window

    showStatusBar: true
    showToolBar: true
    platformStyle: PageStackWindowStyle {
        background: "image://theme/meegotouch-applicationpage-background" + (theme.inverted ? "-inverted" : "")
    }

    QUbuntuOneToken {
        id: token

        function load() {
            var account = Storage.getAccountMap("toBuntu");
            consumerKey = account.consumerKey;
            consumerSecret = account.consumerSecret;
            tokenKey = account.tokenKey;
            tokenSecret = account.tokenSecret;
        }

        onReady: {
            switch (error) {
            case QUbuntuOneToken.NoError:
                if (Storage.storeAccount("toBuntu", consumerKey, consumerSecret, tokenKey, tokenSecret)) {
                    banner.displayMessage(qsTr("You have added your Ubuntu One account."));
                    nodeModel.cdRoot();
                    nodeModel.reload();
                }
                else {
                    infoDialog.displayError(qsTr("Database error. Unable to add your Ubuntu One account"));
                }

                return;
            default:
                infoDialog.displayError(errorString);
                return;
            }
        }
    }

    QUbuntuOneNodeListModel {
        id: nodeModel

        onStatusChanged: {
            switch (status) {
            case QUbuntuOneNodeListModel.Error:
                infoDialog.displayError(errorString);
                return;
            default:
                return;
            }
        }
    }

    InfoBanner {
        id: banner

        function displayMessage(message) {
            text = message;
            show();
        }

        topMargin: 40
    }

    ProgressDialog {
        id: progressDialog
    }

    InfoDialog {
        id: infoDialog
    }

    Connections {
        target: Notifications
        onInformation: banner.displayMessage(message)
        onError: infoDialog.displayError(message)
    }

    Connections {
        target: Settings
        onUseDarkThemeChanged: theme.inverted = Settings.useDarkTheme
    }

    Connections {
        target: platformWindow
        onActiveChanged: if (!platformWindow.active) TransferManager.storeTransfers();
    }

    Component.onCompleted: {
        token.load();
        theme.inverted = Settings.useDarkTheme;
        window.pageStack.push(Qt.resolvedUrl("MainPage.qml"), undefined, true);
        TransferManager.restoreStoredTransfers();
    }
}
