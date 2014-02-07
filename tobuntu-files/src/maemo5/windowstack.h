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

#ifndef WINDOWSTACK_H
#define WINDOWSTACK_H

#include <QObject>

class StackedWindow;

class WindowStack : public QObject
{
    Q_OBJECT

public:
    ~WindowStack();

    static WindowStack* instance();

    StackedWindow* currentWindow() const;
    StackedWindow* rootWindow() const;
        
public slots:
    void addWindowToStack(StackedWindow *window);
    void removeWindowFromStack(StackedWindow *window);
    void unwindWindowStack();

signals:
    void currentWindowChanged(StackedWindow *window);
    
private:
    WindowStack();

    static WindowStack* m_instance;

    QList<StackedWindow*> m_windowStack;
};

#endif // WINDOWSTACK_H
