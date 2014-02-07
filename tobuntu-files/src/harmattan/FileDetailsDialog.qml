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

MySheet {
    id: root

    acceptButtonText: qsTr("Done")
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
                    margins: UI.PADDING_DOUBLE
                }
                spacing: UI.PADDING_DOUBLE

                Row {
                    spacing: UI.PADDING_DOUBLE

                    MyTextField {
                        id: nameEdit

                        width: column.width - renameButton.width - UI.PADDING_DOUBLE
                        validator: RegExpValidator {
                            regExp: /[^/]+/
                        }
                    }

                    MyButton {
                        id: renameButton

                        width: height
                        iconSource: "image://theme/icon-m-toolbar-done" + (theme.inverted ? "-white" : "")
                        enabled: nameEdit.acceptableInput
                        onClicked: {
                            nodeModel.renameNode(view.selectedIndex, nameEdit.text);
                            root.accept();
                        }
                    }
                }

                MyCheckBox {
                    id: publicCheckbox

                    width: parent.width
                    text: qsTr("Publish file")
                    onClicked: {
                        nodeModel.setFilePublic(view.selectedIndex, checked);
                        root.accept();
                    }
                }

                Label {
                    width: parent.width
                    elide: Text.ElideRight
                    text: qsTr("Public URL") + ": " + (nodeModel.data(view.selectedIndex, "isPublic") ? "<a href='" + nodeModel.data(view.selectedIndex, "publicUrl") + "'>" + nodeModel.data(view.selectedIndex, "publicUrl") + "</a>"
                                                                         : qsTr("Not published"))
                }

                Label {
                    width: parent.width
                    elide: Text.ElideRight
                    text: qsTr("Created") + ": " + Qt.formatDateTime(nodeModel.data(view.selectedIndex, "whenCreated"))
                }

                Label {
                    width: parent.width
                    elide: Text.ElideRight
                    text: qsTr("Modified") + ": " + Qt.formatDateTime(nodeModel.data(view.selectedIndex, "lastModified"))
                }

                Label {
                    width: parent.width
                    elide: Text.ElideRight
                    text: qsTr("Size") + ": " + Utils.fileSizeFromBytes(nodeModel.data(view.selectedIndex, "size"))
                }
            }
        }

        ScrollDecorator {
            flickableItem: flicker
        }
    }

    QtObject {
        id: internal

        function loadData() {
            nameEdit.text = nodeModel.data(view.selectedIndex, "name");
            publicCheckbox.checked = nodeModel.data(view.selectedIndex, "isPublic");
        }
    }

    onStatusChanged: if (status === DialogStatus.Opening) internal.loadData();
}
