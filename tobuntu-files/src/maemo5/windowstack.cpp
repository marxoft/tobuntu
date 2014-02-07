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

#include "windowstack.h"
#include "stackedwindow.h"

WindowStack* WindowStack::m_instance = 0;

WindowStack::WindowStack() :
    QObject()
{
    if (!m_instance) {
        m_instance = this;
    }
}

WindowStack::~WindowStack() {}

WindowStack*  WindowStack::instance() {
    return !m_instance ? new WindowStack : m_instance;
}

StackedWindow* WindowStack::currentWindow() const {
    return m_windowStack.isEmpty() ? 0 : m_windowStack.last();
}

StackedWindow* WindowStack::rootWindow() const {
    return m_windowStack.isEmpty() ? 0 : m_windowStack.first();
}

void WindowStack::addWindowToStack(StackedWindow *window) {
    m_windowStack.append(window);
    emit currentWindowChanged(this->currentWindow());
}

void WindowStack::removeWindowFromStack(StackedWindow *window) {
    m_windowStack.removeOne(window);
    emit currentWindowChanged(this->currentWindow());
}

void WindowStack::unwindWindowStack() {
    for (int i = m_windowStack.size() - 1; i > 0; i--) {
        m_windowStack.at(i)->close();
    }

    emit currentWindowChanged(this->currentWindow());
}
