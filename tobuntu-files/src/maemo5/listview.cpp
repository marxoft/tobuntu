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

#include "listview.h"
#include <QScrollBar>
#include <QAbstractKineticScroller>
#include <QTimer>

ListView::ListView(QWidget *parent) :
    QListView(parent),
    m_kineticScroller(property("kineticScroller").value<QAbstractKineticScroller *>()),
    m_scrollTimer(0),
    m_minimum(0),
    m_maximum(1000000),
    m_monitor(false)
{
    this->setUniformItemSizes(true);
    this->setAutoScroll(false);
    this->setEditTriggers(QListView::NoEditTriggers);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setScrollRange(int,int)));
}

void ListView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Down) {
        if (event->modifiers() == Qt::ShiftModifier) {
            this->positionAtEnd();
        }
        else {
            m_kineticScroller->scrollTo(QPoint(0, this->rectForIndex(this->indexAt(QPoint(0, this->height()))).y()));
        }

        event->accept();
    }
    else if (event->key() == Qt::Key_Up) {
        if (event->modifiers() == Qt::ShiftModifier) {
            this->positionAtBeginning();
        }
        else {
            m_kineticScroller->scrollTo(QPoint(0, this->rectForIndex(this->indexAt(QPoint(0, -this->height()))).y()));
        }

        event->accept();
    }
    else {
        event->ignore();
    }
}

void ListView::mousePressEvent(QMouseEvent *event) {
    this->setCurrentIndex(this->indexAt(event->pos()));
    QListView::mousePressEvent(event);
}

void ListView::mouseReleaseEvent(QMouseEvent *event) {
    QListView::mouseReleaseEvent(event);
    this->setCurrentIndex(QModelIndex());
}

bool ListView::monitorScrolling() const {
    return m_monitor;
}

void ListView::setMonitorScrolling(bool monitor) {
    if (monitor != this->monitorScrolling()) {
        m_monitor = monitor;

        if (monitor) {
            if (!m_scrollTimer) {
                m_scrollTimer = new QTimer(this);
                m_scrollTimer->setInterval(500);
                m_scrollTimer->setSingleShot(true);
                this->connect(m_scrollTimer, SIGNAL(timeout()), this, SIGNAL(scrollingStopped()));
            }

            this->connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollPositionChanged(int)));
        }
        else {
            this->disconnect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollPositionChanged(int)));
        }
    }
}

bool ListView::isScrolling() const {
    switch (m_kineticScroller->state()) {
    case QAbstractKineticScroller::Pushing:
    case QAbstractKineticScroller::AutoScrolling:
        return true;
    default:
        return false;
    }
}

void ListView::setScrollRange(int minimum, int maximum) {
    m_minimum = minimum;
    m_maximum = maximum;
}

void ListView::onScrollPositionChanged(int position) {    
    if (!m_scrollTimer->isActive()) {
        emit scrollingStarted();
    }

    if (position == m_maximum) {
        emit atEnd();
    }

    m_scrollTimer->start();
}

void ListView::setRootIndex(const QModelIndex &index) {
    QListView::setRootIndex(index);
    emit rootIndexChanged(index);
}

void ListView::positionAtBeginning() {
    this->scrollTo(this->model()->index(0, 0), QListView::PositionAtTop);
}

void ListView::positionAtEnd() {
    this->scrollTo(this->model()->index(model()->rowCount() - 1, 0), QListView::PositionAtBottom);
}
