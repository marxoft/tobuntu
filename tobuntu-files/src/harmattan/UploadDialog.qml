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

    property string fileName
    property alias isPublic: checkbox.checked

    rejectButtonText: qsTr("Cancel")
    acceptButtonText: !fileName ? "" : qsTr("Done")
    content: Item {
        anchors.fill: parent

        Flickable {
            id: flicker

            anchors.fill: parent
            contentHeight: column.height + UI.PADDING_DOUBLE

            Column {
                id: column

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
                spacing: UI.PADDING_DOUBLE

                ValueListItem {
                    width: parent.width
                    title: qsTr("File")
                    subTitle: !root.fileName ? qsTr("None chosen") : root.fileName

                    onClicked: {
                        loader.sourceComponent = folderDialog;
                        loader.item.open();
                    }
                }

                MyCheckBox {
                    id: checkbox

                    x: UI.PADDING_DOUBLE
                    width: parent.width - UI.PADDING_DOUBLE * 2
                    text: qsTr("Publish file")
                }
            }
        }

        ScrollDecorator {
            flickableItem: flicker
        }
    }

    Loader {
        id: loader
    }

    Component {
        id: folderDialog

        FolderListDialog {
            showFiles: true
            onAccepted: root.fileName = fileName
        }
    }
}
