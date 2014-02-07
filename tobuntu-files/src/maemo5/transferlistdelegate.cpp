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

#include "transferlistdelegate.h"
#include "../shared/transfermodel.h"
#include "../shared/utils.h"
#include <QStyleOptionProgressBar>
#include <QApplication>
#include <QPainter>

TransferListDelegate::TransferListDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void TransferListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if ((option.state) & (QStyle::State_Selected)) {
        painter->drawImage(option.rect, QImage("/etc/hildon/theme/images/TouchListBackgroundPressed.png"));
    }
    else {
        painter->drawImage(option.rect, QImage("/etc/hildon/theme/images/TouchListBackgroundNormal.png"));
    }

    QRect textRect = option.rect;
    textRect.setLeft(textRect.left() + 8);
    textRect.setRight(textRect.right() - 166);
    textRect.setTop(textRect.top() + 8);
    textRect.setBottom(textRect.bottom() - 8);

    painter->drawText(textRect, Qt::TextWrapAnywhere, index.data(TransferRoles::FileNameRole).toString());

    painter->save();

    QFont smallFont;
    smallFont.setPixelSize(18);
    painter->setFont(smallFont);


    int status = index.data(TransferRoles::StatusRole).toInt();

    switch (status) {
    case FileTransfer::Failed:
        painter->setPen(Qt::red);
        painter->drawText(textRect, Qt::AlignBottom | Qt::TextSingleLine, QString("%1: %2").arg(index.data(TransferRoles::StatusStringRole).toString()).arg(index.data(TransferRoles::ErrorStringRole).toString()));
        break;
    default:
        painter->drawText(textRect, Qt::AlignBottom | Qt::TextSingleLine, index.data(TransferRoles::StatusStringRole).toString());
        break;
    }

    painter->restore();

    QRect progressRect = option.rect;
    progressRect.setLeft(progressRect.right() - 158);
    progressRect.setWidth(150);
    progressRect.setTop(progressRect.top() + 15);
    progressRect.setBottom(progressRect.bottom() - 15);

    int progress = index.data(TransferRoles::ProgressRole).toInt();

    QStyleOptionProgressBar progressBar;
    progressBar.rect = progressRect;
    progressBar.minimum = 0;
    progressBar.maximum = 100;
    progressBar.progress = progress;
    progressBar.text = QString::number(progress) + "%";
    progressBar.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBar, painter);

}

QSize TransferListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);

    return QSize(option.rect.width(), 120);
}
