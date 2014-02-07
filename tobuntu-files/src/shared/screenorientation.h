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

#ifndef SCREENORIENTATION_H
#define SCREENORIENTATION_H

#include <QObject>

class ScreenOrientation : public QObject
{
    Q_OBJECT

    Q_ENUMS(Orientation)

public:
    enum Orientation {
#ifdef SAILFISH_OS
        LockPortrait = 1,
        LockLandscape = 2,
        LockPortraitInverted = 4,
        LockLandcapeInverted = 8,
        Automatic = 15
#else
        Automatic = 0,
        LockPortrait,
        LockLandscape
#endif
    };

    inline static QString orientationString(Orientation orientation) {
        switch (orientation) {
            case Automatic:
            return tr("Automatic");
        case LockPortrait:
            return tr("Portrait");
        case LockLandscape:
            return tr("Landscape");
#ifdef SAILFISH_OS
        case LockPortraitInverted:
            return tr("Portrait (inverted)");
        case LockLandscapeInverted:
            return tr("Landscape (inverted)");
#endif
        default:
            return tr("Unknown");
        }
    }

private:
    ScreenOrientation() {}
    ~ScreenOrientation() {}
};

#endif // SCREENORIENTATION_H
