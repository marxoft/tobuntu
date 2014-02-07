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
import com.marxoft.models 1.0
import com.marxoft.enums 1.0
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

Page {
    id: root

    orientationLock: Settings.screenOrientation
    tools: ToolBarLayout {

        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: window.pageStack.pop()
        }
    }

    PageHeader {
        id: header

        text: qsTr("Settings")
    }

    Flickable {
        id: flicker

        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: parent.bottom
        }
        contentHeight: column.height + UI.PADDING_DOUBLE * 2

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: UI.PADDING_DOUBLE
            }

            SeparatorLabel {
                width: parent.width
                text: qsTr("Transfers")
            }

            Item {
                width: parent.width
                height: UI.PADDING_DOUBLE
            }

            ValueListItem {
                x: -UI.PADDING_DOUBLE
                width: parent.width + UI.PADDING_DOUBLE * 2
                title: qsTr("Default download folder")
                subTitle: Settings.defaultDownloadDirectory
                onClicked: {
                    loader.sourceComponent = folderDialog;
                    loader.item.open();
                }
            }

            Item {
                width: parent.width
                height: UI.PADDING_DOUBLE
            }

            MyCheckBox {
                width: parent.width
                text: qsTr("Use default download folder")
                checked: Settings.useDefaultDownloadDirectory
                onCheckedChanged: Settings.useDefaultDownloadDirectory = checked
            }

            Item {
                width: parent.width
                height: UI.PADDING_DOUBLE
            }

            SeparatorLabel {
                width: parent.width
                text: qsTr("Other")
            }

            Item {
                width: parent.width
                height: UI.PADDING_DOUBLE
            }

            ValueSelector {
                x: -UI.PADDING_DOUBLE
                width: parent.width + UI.PADDING_DOUBLE * 2
                title: qsTr("Screen orientation")
                model: ScreenOrientationModel {}
                value: Settings.screenOrientation
                onValueChanged: Settings.screenOrientation = value
            }

            ValueSelector {
                x: -UI.PADDING_DOUBLE
                width: parent.width + UI.PADDING_DOUBLE * 2
                title: qsTr("Langugage")
                model: LanguageModel {}
                value: Settings.language
                onValueChanged: Settings.language = value
            }

            Item {
                width: parent.width
                height: UI.PADDING_DOUBLE
            }

            MyCheckBox {
                width: parent.width
                text: qsTr("Use dark theme")
                checked: Settings.useDarkTheme
                onCheckedChanged: Settings.useDarkTheme = checked
            }
        }
    }

    ScrollDecorator {
        flickableItem: flicker
    }

    Loader {
        id: loader
    }

    Component {
        id: folderDialog

        FolderListDialog {
            folder: "file://" + Settings.defaultDownloadDirectory
            onAccepted: Settings.defaultDownloadDirectory = folder.toString().slice(7)
        }
    }
}
