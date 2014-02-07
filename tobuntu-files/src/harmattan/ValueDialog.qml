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

SelectionDialog {
    id: root

    property string name
    property variant value

    onAccepted: {
        name = model.data(selectedIndex, "name");
        value = model.data(selectedIndex, "value");
    }

    onStatusChanged:  {
        if (status === DialogStatus.Opening) {
            for (var i = 0; i < model.count; i++) {
                if (model.data(i, "value") === value) {
                    name = model.data(i, "name");
                    selectedIndex = i;

                    return;
                }
            }
        }
    }
}
