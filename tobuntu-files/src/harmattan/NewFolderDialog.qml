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
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

MySheet {
    id: root

    property alias name: nameEdit.text

    rejectButtonText: qsTr("Cancel")
    acceptButtonText: nameEdit.acceptableInput ? qsTr("Done") : ""
    content: Item {
        anchors.fill: parent

        Flickable {
            id: flicker

            anchors.fill: parent
            contentHeight: column.height + UI.PADDING_DOUBLE * 2

            Column {
                id: column

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: UI.PADDING_DOUBLE * 2
                }
                spacing: UI.PADDING_DOUBLE * 2

                Label {
                    width: parent.width
                    text: nodeModel.resourcePath == "/" ? qsTr("Volume name") : qsTr("Folder name")
                }

                MyTextField {
                    id: nameEdit

                    width: parent.width
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    validator: RegExpValidator {
                        regExp: /[^//]+/
                    }
                    onAccepted: root.accept()
                }
            }
        }

        ScrollDecorator {
            flickableItem: flicker
        }
    }

    onStatusChanged: if (status === DialogStatus.Closed) nameEdit.text = "";
}
