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
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

ValueListItem {
    id: root

    property variant value
    property variant model

    onClicked: {
        loader.sourceComponent = dialog;
        loader.item.open();
    }

    Loader {
        id: loader
    }

    Component {
        id: dialog

        ValueDialog {
            titleText: root.title
            model: root.model
            value: root.value
            onNameChanged: root.subTitle = name
            onValueChanged: root.value = value
        }
    }

    Component.onCompleted: {
        if ((value !== undefined) && (model)) {
            for (var i = 0; i < model.count; i++) {
                if (model.data(i, "value") === value) {
                    subTitle = model.data(i, "name");

                    return;
                }
            }
        }
    }
}
