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
import com.marxoft.qubuntuone 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
    id: root

    orientationLock: Settings.screenOrientation
    tools: ToolBarLayout {

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: window.pageStack.pop()
        }

        ToolIcon {
            platformIconId: "toolbar-delete"
            onClicked: {
                if (Storage.removeAccount("toBuntu")) {
                    token.clear();
                    nodeModel.cdRoot();
                    nodeModel.clear();
                    window.pageStack.pop();
                    banner.displayMessage(qsTr("You have deleted your Ubuntu One account."));
                }
                else {
                    infoDialog.displayError(qsTr("Database error. Unable to delete your Ubuntu One account."));
                }
            }
        }
    }

    QUbuntuOneStorageQuota {
        id: quota

        property bool isReady: false

        onReady: {
            isReady = true;

            if (account.isReady) {
                root.state = "loaded";
            }
        }
    }

    QUbuntuOneAccount {
        id: account

        property bool isReady: false

        onReady: {
            isReady = true;

            if (quota.isReady) {
                root.state = "loaded";
            }
        }
    }

    PageHeader {
        id: header

        text: qsTr("Account")
    }

    Flickable {
        id: flicker

        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: parent.bottom
        }
        contentHeight: column.height + UI.PADDING_DOUBLE * 2
        visible: false

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: UI.PADDING_DOUBLE
            }
            spacing: UI.PADDING_DOUBLE

            SeparatorLabel {
                width: parent.width
                text: qsTr("Storage quota")
            }

            Label {
                width: parent.width
                text: qsTr("Total storage: ") + Utils.fileSizeFromBytes(quota.totalStorage)
            }

            Label {
                width: parent.width
                text: qsTr("Used storage: ") + Utils.fileSizeFromBytes(quota.usedStorage)
            }

            SeparatorLabel {
                width: parent.width
                text: qsTr("Account")
            }

            Label {
                width: parent.width
                text: qsTr("Id: ") + account.id
            }

            Label {
                width: parent.width
                text: qsTr("Username: ") + account.username
            }

            Label {
                width: parent.width
                text: qsTr("Nickname: ") + account.nickname
            }

            Label {
                width: parent.width
                text: qsTr("First name: ") + account.firstName
            }

            Label {
                width: parent.width
                text: qsTr("Last name: ") + account.lastName
            }

            Label {
                width: parent.width
                text: qsTr("Email address: ") + account.email
            }

            Label {
                width: parent.width
                text: qsTr("Open ID: ") + account.openId
            }

            Label {
                width: parent.width
                text: qsTr("Current plan: ") + account.currentPlan
            }

            Label {
                width: parent.width
                text: qsTr("Features: ") + account.features.join(", ")
            }
        }
    }

    states: State {
        name: "loaded"
        PropertyChanges { target: flicker; visible: true }
        PropertyChanges { target: busyIndicator; visible: false }
    }

    BusyIndicator {
        id: busyIndicator

        anchors.centerIn: flicker
        platformStyle: BusyIndicatorStyle {
            size: "large"
        }
        visible: true
        running: visible
    }

    ScrollDecorator {
        flickableItem: flicker
    }

    Component.onCompleted: {
        quota.load();
        account.load();
    }
}
