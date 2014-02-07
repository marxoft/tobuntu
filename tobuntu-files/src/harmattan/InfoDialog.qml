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

    function displayInfo(info) {
        root.state = "info";
        infoLabel.text = info;
        root.open();
    }

    function displayError(errorString) {
        root.state = "error";
        infoLabel.text = errorString;
        root.open();
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
            spacing: UI.PADDING_DOUBLE

            Image {
                id: icon

                x: Math.floor((parent.width / 2) - (width / 2))
            }

            Label {
                id: infoLabel

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                color: UI.COLOR_INVERTED_FOREGROUND
            }
        }
    }

    states: [
        State {
            name: "info"
            PropertyChanges { target: icon; source: "image://theme/icon-l-user-guide" }
        },

        State {
            name: "error"
            PropertyChanges { target: icon; source: "image://theme/icon-l-error" }
        }
    ]
}
