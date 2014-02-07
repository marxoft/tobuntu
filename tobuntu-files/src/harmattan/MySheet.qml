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

Sheet {
    id: root

    platformStyle: SheetStyle {
        background: window.platformStyle.background
    }

    buttons: [
        SheetButton {
            anchors.left: parent.left
            anchors.leftMargin: root.platformStyle.rejectButtonLeftMargin
            anchors.verticalCenter: parent.verticalCenter
            text: root.rejectButtonText
            visible: text != ""
            onClicked: reject()
        },

        SheetButton {
            anchors.right: parent.right
            anchors.rightMargin: root.platformStyle.acceptButtonRightMargin
            anchors.verticalCenter: parent.verticalCenter
            platformStyle: SheetButtonAccentStyle {
                background: "image://theme/color14-meegotouch-sheet-button-accent"+__invertedString+"-background"
                pressedBackground: "image://theme/color14-meegotouch-sheet-button-accent"+__invertedString+"-background-pressed"
                disabledBackground: "image://theme/color14-meegotouch-sheet-button-accent"+__invertedString+"-background-disabled"
            }
            text: root.acceptButtonText
            visible: text != ""
            onClicked: accept()
        }
    ]

    MouseArea {
        z: -1
        anchors.fill: parent
    }
}
