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
import Qt.labs.folderlistmodel 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

MySheet {
    id: root

    property bool showFiles: false
    property alias folder: folderModel.folder
    property string fileName

    rejectButtonText: qsTr("Cancel")
    acceptButtonText: qsTr("Done")
    content: Item {
        anchors.fill: parent

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
                text: root.showFiles ? qsTr("Choose file") : qsTr("Choose folder")
            }

            Label {
                width: parent.width
                color: "#ef5906"
                elide: Text.ElideRight
                text: root.showFiles ? !root.fileName ? qsTr("None chosen") : root.fileName.slice(root.fileName.lastIndexOf("/") + 1) : root.folder.toString().slice(root.folder.toString().lastIndexOf("/") + 1)
            }

            SeparatorLabel {
                width: parent.width
                text: root.showFiles ? qsTr("Files") : qsTr("Folders")
            }
        }

        ListView {
            id: view

            anchors {
                left: parent.left
                right: parent.right
                top: column.bottom
                topMargin: UI.PADDING_DOUBLE
                bottom: backButton.top
                bottomMargin: UI.PADDING_DOUBLE
            }
            clip: true
            model: FolderListModel {
                id: folderModel

                showDirs: true
                showDotAndDotDot: false
                showOnlyReadable: true
                nameFilters: root.showFiles ? ["*.*"] : ["*.folders_only"]
            }
            delegate: FolderListDelegate {
                onClicked: folderModel.isFolder(index) ? folderModel.folder = filePath
                                                       : root.fileName = filePath.toString().slice(7)
            }
        }

        ScrollDecorator {
            flickableItem: view
        }

        Label {
            anchors.centerIn: view
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 40
            font.bold: true
            color: theme.inverted ? UI.COLOR_INVERTED_SECONDARY_FOREGROUND : UI.COLOR_SECONDARY_FOREGROUND
            text: qsTr("Folder empty")
            visible: folderModel.count == 0
        }

        ToolIcon {
            id: backButton

            z: 1000
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            platformIconId: "toolbar-up"
            onClicked: folderModel.folder = folderModel.parentFolder
        }
    }
}
