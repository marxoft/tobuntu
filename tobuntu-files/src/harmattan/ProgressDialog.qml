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

Dialog {
    id: root

    property bool _close: false

    function displayMessage(info, numberOfOperations) {
        if (info !== "") {
            infoLabel.text = info;
            progressBar.value = 0;
            progressBar.maximumValue = numberOfOperations;
            root._close = false;
            root.open();
        }
        else {
            root.requestClosure();
        }
    }

    function requestClosure() {
        switch (root.status) {
        case DialogStatus.Open:
            root.close();
            return;
        default:
            root._close = true;
            return;
        }
    }

    content: Item {
        height: column.height
        anchors {
            left: parent.left
            right: parent.right
        }

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            spacing: UI.PADDING_DOUBLE * 2

            Label {
                id: infoLabel

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: UI.FONT_LARGE
                font.bold: true
                color: UI.COLOR_INVERTED_FOREGROUND
            }

            BusyIndicator {
                id: busyIndicator

                x: parent.width / 2 - width / 2
                visible: progressBar.maximumValue <= 1
                platformStyle: BusyIndicatorStyle {
                    inverted: true
                    size: "large"
                }
                running: (root.status === DialogStatus.Open) && (progressBar.maximumValue <= 1)
            }

            MyProgressBar {
                id: progressBar

                width: parent.width
                visible: maximumValue > 1
                onValueChanged: if (value == maximumValue) root.requestClosure();
            }
        }
    }

    Loader {
        id: mouseLoader

        sourceComponent: root.status === DialogStatus.Open ? mouseArea : undefined
    }

    Component {
        id: mouseArea

        MouseArea {
            // Prevents closing of the dialog via privateClicked() signal
            z: Number.MAX_VALUE + 1
            parent: window
            anchors {
                fill: parent
                topMargin: window.inPortrait ? 0 : 30
                leftMargin: window.inPortrait ? 30 : 0
            }
        }
    }

    onStatusChanged: if ((status === DialogStatus.Open) && (_close)) close();
}
