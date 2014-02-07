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

    width: parent.width
    content: Item {
        height: column.height
        anchors {
            left: parent.left
            right: parent.right
            margins: UI.PADDING_DOUBLE
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
                source: "file:///usr/share/icons/hicolor/80x80/apps/tobuntu.png"
            }

            Label {
                id: titleLabel

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: 32
                color: UI.COLOR_INVERTED_FOREGROUND
                text: "toBuntu " + VERSION_NUMBER
            }

            Label {
                id: aboutLabel

                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                color: UI.COLOR_INVERTED_FOREGROUND
                text: qsTr("A client for Ubuntu One. toBuntu uses the libqubuntuone shared library and the libqubuntuone-declarative plugin.<br><br>&copy; Stuart Howarth 2012-2014<br>")
            }

            Label {
                width: parent.width
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: "<font color='white'>" + qsTr("Contact") + ": </font><a href='mailto:showarth@marxoft.co.uk?subject=toBuntu " + VERSION_NUMBER + " for N9'>showarth@marxoft.co.uk</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}
