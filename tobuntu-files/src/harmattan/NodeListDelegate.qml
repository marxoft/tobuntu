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

ListItem {
    id: root

    Image {
        id: icon

        width: 64
        height: 64
        anchors {
            left: parent.left
            leftMargin: UI.PADDING_DOUBLE
            verticalCenter: parent.verticalCenter
        }
        source: nodeType === QUbuntuOneNode.File ? "image://theme/" + Utils.iconPathFromFileSuffix(suffix) + (theme.inverted ? "-inverse" : "")
                                           : "image://theme/icon-m-content-document" + (theme.inverted ? "-inverse" : "")
    }

    Column {
        id: column

        anchors {
            left: icon.right
            leftMargin: UI.PADDING_DOUBLE
            right: parent.right
            rightMargin: UI.PADDING_DOUBLE
            verticalCenter: parent.verticalCenter
        }

        Label {
            width: parent.width
            elide: Text.ElideRight
            font.bold: true
            text: name
        }

        Row {
            Image {
                id: publicIcon

                width: 24
                height: 24
                smooth: true
                sourceSize.width: width
                sourceSize.height: height
                source: "image://theme/icon-m-toolbar-share" + (isPublic ? "" : "-dimmed") + (theme.inverted ? "-white" : "")
            }

            Label {
                width: column.width - publicIcon.width
                height: 24
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: UI.FONT_SMALL
                color: theme.inverted ? UI.COLOR_INVERTED_SECONDARY_FOREGROUND : UI.COLOR_SECONDARY_FOREGROUND
                elide: Text.ElideRight
                text: Qt.formatDateTime(whenCreated) + (nodeType === QUbuntuOneNode.File ? " - " + Utils.fileSizeFromBytes(size) : "")
            }
        }
    }
}
