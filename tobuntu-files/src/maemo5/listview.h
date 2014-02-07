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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>
#include <QMouseEvent>

class QAbstractKineticScroller;

class ListView : public QListView
{
    Q_OBJECT

public:
    explicit ListView(QWidget *parent = 0);

    bool monitorScrolling() const;
    bool isScrolling() const;

public slots:
    void setRootIndex(const QModelIndex &index);
    void positionAtBeginning();
    void positionAtEnd();
    void setMonitorScrolling(bool monitor);

private:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void setScrollRange(int minimum, int maximum);
    void onScrollPositionChanged(int position);

signals:
    void scrollingStarted();
    void scrollingStopped();
    void rootIndexChanged(const QModelIndex &index);
    void atEnd();

private:
    QAbstractKineticScroller *m_kineticScroller;
    QTimer *m_scrollTimer;

    int m_minimum;
    int m_maximum;

    bool m_monitor;
};

#endif // LISTVIEW_H
