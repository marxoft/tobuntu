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

#include "nodelistdelegate.h"
#include "../shared/nodemodel.h"
#include "../shared/utils.h"
#include <QPainter>
#include <QStaticText>
#include <QMaemo5Style>

NodeListDelegate::NodeListDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void NodeListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    if ((option.state) & (QStyle::State_Selected)) {
        painter->drawImage(option.rect, QImage("/etc/hildon/theme/images/TouchListBackgroundPressed.png"));
    }
    else {
        painter->drawImage(option.rect, QImage("/etc/hildon/theme/images/TouchListBackgroundNormal.png"));
    }

    QRect iconRect = option.rect;
    iconRect.setSize(QSize(48, 48));
    iconRect.moveTopLeft(QPoint(iconRect.left() + 8, iconRect.top() + 16));

    int nodeType = index.data(NodeRoles::NodeTypeRole).toInt();

    QRect textRect = QRect(iconRect.right() + 8, option.rect.top() + 8, option.rect.width() - 72, option.rect.height() - 16);

    switch (nodeType) {
    case Node::Volume:
        painter->drawImage(iconRect, QImage("/usr/share/icons/hicolor/48x48/hildon/filemanager_media_server.png"));
        painter->setClipRect(textRect);
        painter->drawText(textRect, Qt::AlignVCenter, index.data(NodeRoles::NameRole).toString());
        painter->restore();
        return;
    case Node::File:
        painter->drawImage(iconRect, QImage(Utils::iconPathFromFileSuffix(index.data(NodeRoles::SuffixRole).toString())));
        painter->setClipRect(textRect);
        painter->drawText(textRect, Qt::AlignTop, index.data(NodeRoles::NameRole).toString());
        break;
    default:
        painter->drawImage(iconRect, QImage("/usr/share/icons/hicolor/48x48/hildon/general_folder.png"));
        painter->setClipRect(textRect);
        painter->drawText(textRect, Qt::AlignTop, index.data(NodeRoles::NameRole).toString());
        break;
    }

    painter->setClipping(false);

    iconRect.setSize(QSize(32, 32));
    iconRect.moveBottomLeft(QPoint(textRect.left(), option.rect.bottom() - 8));

    switch (nodeType) {
    case Node::File:
        if (index.data(NodeRoles::IsPublicRole).toBool()) {
            painter->drawImage(iconRect, QImage("/usr/share/icons/hicolor/48x48/hildon/statusarea_sharing.png").scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else {
            painter->drawImage(iconRect, QImage("/usr/share/icons/hicolor/48x48/hildon/statusarea_sharing_blocked.png").scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }

        textRect.moveBottomLeft(QPoint(iconRect.right() + 8, option.rect.bottom() - 8));
        break;
    default:
        textRect.moveBottom(option.rect.bottom() - 8);
        break;
    }

    QFont font;
    font.setPixelSize(20);
    painter->setFont(font);
    painter->setPen(QMaemo5Style::standardColor("SecondaryTextColor"));
    painter->setClipRect(textRect);

    switch (nodeType) {
    case Node::File:
        painter->drawText(textRect, Qt::AlignBottom, QString("%1 - %2").arg(index.data(NodeRoles::LastModifiedRole).toDateTime().toString()).arg(Utils::fileSizeFromBytes(index.data(NodeRoles::SizeRole).toLongLong())));
        break;
    default:
        painter->drawText(textRect, Qt::AlignBottom, index.data(NodeRoles::LastModifiedRole).toDateTime().toString());
        break;
    }

    painter->restore();
}

QSize NodeListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);

    return QSize(option.rect.width(), 80);
}
