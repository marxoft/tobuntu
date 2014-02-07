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

Image {
    id: root

    property string text
    property bool showBusyIndicator: false

    z: 1000
    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
    }
    source: "image://theme/color14-meegotouch-view-header-fixed"
    fillMode: Image.Stretch

    Label {
        id: title

        anchors {
            left: parent.left
            leftMargin: UI.PADDING_DOUBLE
            verticalCenter: parent.verticalCenter
            right: root.showBusyIndicator ? busyIndicator.left : parent.right
            rightMargin: UI.PADDING_DOUBLE
        }
        font.bold: true
        maximumLineCount: 2
        elide: Text.ElideRight
        color: UI.COLOR_INVERTED_FOREGROUND
        text: root.text
    }

    BusyIndicator {
        id: busyIndicator

        anchors {
            right: parent.right
            rightMargin: UI.PADDING_DOUBLE
            verticalCenter: parent.verticalCenter
        }
        running: visible
        visible: root.showBusyIndicator
        platformStyle: BusyIndicatorStyle {
            inverted: true
        }
    }

    MouseArea {
        z: -1
        anchors.fill: parent
    }
}
