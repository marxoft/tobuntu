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

Page {
    id: root

    orientationLock: Settings.screenOrientation
    tools: ToolBarLayout {

        ToolIcon {
            platformIconId: "toolbar-home"
            enabled: nodeModel.status !== QUbuntuOneNodeListModel.Loading
            onClicked: nodeModel.cdRoot()
        }

        ToolIcon {
            platformIconId: "toolbar-up"
            enabled: (!nodeModel.activeNode.isRoot) && (nodeModel.status !== QUbuntuOneNodeListModel.Loading)
            onClicked: nodeModel.cdUp()
        }

        ToolIcon {
            platformIconId: "toolbar-refresh"
            enabled: (nodeModel.status !== QUbuntuOneNodeListModel.Loading) && (!token.isEmpty)
            onClicked: nodeModel.reload()
        }

        ToolIcon {
            platformIconId: "toolbar-add"
            enabled: (nodeModel.status !== QUbuntuOneNodeListModel.Loading) && (!token.isEmpty)
            onClicked: {
                loader.sourceComponent = newFolderDialog;
                loader.item.open();
            }
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            enabled: nodeModel.status !== QUbuntuOneNodeListModel.Loading
            onClicked: menu.open()
        }
    }

    Menu {
        id: menu

        MenuLayout {

            MenuItem {
                text: qsTr("Paste")
                visible: (!token.isEmpty) && (nodeModel.canPaste)
                onClicked: nodeModel.pasteNode()
            }

            MenuItem {
                text: qsTr("Upload file")
                visible: !token.isEmpty
                onClicked: {
                    loader.sourceComponent = uploadDialog;
                    loader.item.open();
                }
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: window.pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }

            MenuItem {
                text: qsTr("Account")
                onClicked: window.pageStack.push(Qt.resolvedUrl(token.tokenKey ? "AccountPage.qml" : "AuthenticationPage.qml"))
            }

            MenuItem {
                text: qsTr("About")
                onClicked: {
                    loader.sourceComponent = aboutDialog;
                    loader.item.open();
                }
            }
        }
    }

    PageHeader {
        id: header

        text: nodeModel.activeNode.isRoot ? "/" : nodeModel.activeNode.name + "\n" + (nodeModel.activeNode.nodeType === QUbuntuOneNode.Volume ? nodeModel.activeNode.nodePath.slice(0, nodeModel.activeNode.nodePath.lastIndexOf("/"))
                                                                                                          : nodeModel.activeNode.parentPath)
        showBusyIndicator: nodeModel.status === QUbuntuOneNodeListModel.Loading
    }

    ListView {
        id: view

        property int selectedIndex: -1

        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: parent.bottom
        }
        model: nodeModel
        delegate: NodeListDelegate {
            onClicked: {
                if (nodeModel.status === QUbuntuOneNodeListModel.Loading) {
                    return;
                }

                view.selectedIndex = index;

                if (nodeType === QUbuntuOneNode.File) {
                    loader.sourceComponent = fileDetailsDialog;
                    loader.item.open();
                }
                else {
                    nodeModel.cd(index);
                }
            }
            onPressAndHold: {
                view.selectedIndex = index;
                contextMenu.open();
            }
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
        visible: (!nodeModel.count) && (nodeModel.status === QUbuntuOneNodeListModel.Ready)
    }

    ContextMenu {
        id: contextMenu

        MenuLayout {

            Repeater {
                id: repeater

                property variant volumeModel: [ qsTr("Delete") ]
                property variant folderModel: [ qsTr("Rename"), qsTr("Cut"), qsTr("Delete") ]
                property variant fileModel: [ qsTr("Rename"), qsTr("Cut"), qsTr("Delete"), qsTr("Download") ]

                model: nodeModel.data(view.selectedIndex, "nodeType") === QUbuntuOneNode.Volume ? volumeModel
                                                                                          : nodeModel.data(view.selectedIndex, "nodeType") === QUbuntuOneNode.Folder ? folderModel
                                                                                                                                                               : fileModel

                MenuItem {
                    text: modelData
                    onClicked: {
                        switch (nodeModel.data(view.selectedIndex, "nodeType")) {
                        case QUbuntuOneNode.Volume: {
                            loader.sourceComponent = confirmDeleteDialog;
                            loader.item.open();
                            return;
                        }
                        default:
                            switch (index) {
                            case 0: {
                                loader.sourceComponent = renameDialog;
                                loader.item.open();
                            }

                            return;
                            case 1:
                                nodeModel.cutNode(view.selectedIndex);
                                return;
                            case 2: {
                                loader.sourceComponent = confirmDeleteDialog;
                                loader.item.open();
                            }

                            return;
                            case 3: {
                                if (Settings.useDefaultDownloadDirectory) {
                                    TransferManager.addDownloadTransfer(nodeModel.data(view.selectedIndex, "contentPath"), Settings.defaultDownloadDirectory);
                                }
                                else {
                                    loader.sourceComponent = folderDialog;
                                    loader.item.open();
                                }
                            }

                            return;
                            default:
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    Loader {
        id: loader
    }

    Component {
        id: uploadDialog

        UploadDialog {
            onAccepted: TransferManager.addUploadTransfer(fileName, "application/octet-stream", nodeModel.activeNode.contentPath, isPublic)
        }
    }

    Component {
        id: fileDetailsDialog

        FileDetailsDialog {}
    }

    Component {
        id: renameDialog

        RenameDialog {
            name: nodeModel.data(view.selectedIndex, "name")
            onAccepted: nodeModel.renameNode(view.selectedIndex, name)
        }
    }

    Component {
        id: newFolderDialog

        NewFolderDialog {
            onAccepted: nodeModel.makeDirectory(name)
        }
    }

    Component {
        id: confirmDeleteDialog

        QueryDialog {
            titleText: qsTr("Delete?")
            message: qsTr("Do you wish to delete") + " '"  + nodeModel.data(view.selectedIndex, "name") + "' ?"
            acceptButtonText: qsTr("Yes")
            rejectButtonText: qsTr("No")
            onAccepted: nodeModel.deleteNode(view.selectedIndex)
        }
    }

    Component {
        id: folderDialog

        FolderListDialog {
            onAccepted: TransferManager.addDownloadTransfer(nodeModel.data(view.selectedIndex, "contentPath"), folder.toString().slice(7))
        }
    }

    Component {
        id: aboutDialog

        AboutDialog {}
    }

    Component.onCompleted: if (!token.isEmpty) nodeModel.reload();
}
