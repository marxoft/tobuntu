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

TextField {
    property alias actionLabel: sipatts.actionKeyLabel
    property alias actionIcon: sipatts.actionKeyIcon
    property alias actionEnabled: sipatts.actionKeyEnabled
    property alias actionHighlighted: sipatts.actionKeyHighlighted
    property alias rightMargin: style.paddingRight

    signal accepted

    platformStyle: TextFieldStyle {
        id: style

        backgroundSelected: "image://theme/color14-meegotouch-textedit-background-selected"
        backgroundDisabled: "image://theme/color14-meegotouch-textedit-background-disabled"
    }

    Keys.onReturnPressed: {
        platformCloseSoftwareInputPanel();
        accepted();
    }
    platformSipAttributes: SipAttributes {
        id: sipatts

        actionKeyEnabled: acceptableInput
        actionKeyHighlighted: true
        actionKeyLabel: qsTr("Done")
        actionKeyIcon: ""
    }
}
