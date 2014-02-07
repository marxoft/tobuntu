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
    }

    Connections {
        target: token
        onReady: {
            progressDialog.close();

            switch (token.error) {
            case QUbuntuOneToken.NoError:
                window.pageStack.pop(null);
                return;
            default:
                return;
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

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: UI.PADDING_DOUBLE
            }
            spacing: UI.PADDING_DOUBLE

            Label {
                width: parent.width
                text: qsTr("Email")
            }

            MyTextField {
                id: emailEdit

                width: parent.width
                inputMethodHints: Qt.ImhEmailCharactersOnly | Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                actionLabel: qsTr("Next")
                validator: RegExpValidator {
                    regExp: /.+@.+/
                }
                onAccepted: passwordEdit.forceActiveFocus()
            }

            Label {
                width: parent.width
                text: qsTr("Password")
            }

            MyTextField {
                id: passwordEdit

                width: parent.width
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                echoMode: TextInput.Password
                actionLabel: qsTr("Next")
                validator: RegExpValidator {
                    regExp: /.+/
                }
                onAccepted: otpEdit.forceActiveFocus()
            }

            Label {
                width: parent.width
                text: qsTr("One time password (optional)")
            }

            MyTextField {
                id: otpEdit

                width: parent.width
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                echoMode: TextInput.Password
            }

            MyButton {
                id: button

                x: parent.width / 2 - width / 2
                text: qsTr("Sign in")
                enabled: (emailEdit.acceptableInput) && (passwordEdit.acceptableInput) && (otpEdit.acceptableInput)
                onClicked: {
                    progressDialog.displayMessage(qsTr("Signing in"), 1);
                    token.authenticate(emailEdit.text, passwordEdit.text, otpEdit.text, "toBuntu (N9)");
                }
            }
        }
    }    

    ScrollDecorator {
        flickableItem: flicker
    }
}
