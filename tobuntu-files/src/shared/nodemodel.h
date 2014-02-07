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

#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "nodelist.h"
#include "reply.h"
#include <QAbstractItemModel>

using namespace QtUbuntuOne;

class NodeRoles : public QObject
{
    Q_OBJECT

    Q_ENUMS(Role)

public:
    enum Role {
        NodeTypeRole = Qt::UserRole + 1,
        PathRole,
        ResourcePathRole,
        ContentPathRole,
        ParentPathRole,
        VolumePathRole,
        NodePathRole,
        NameRole,
        SuffixRole,
        KeyRole,
        WhenCreatedRole,
        LastModifiedRole,
        GenerationRole,
        GenerationCreatedRole,
        HashRole,
        SizeRole,
        IsPublicRole,
        PublicUrlRole,
        HasChildrenRole,
        IsRootRole
    };

private:
    NodeRoles() {}
    ~NodeRoles() {}
};

class NodeModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(bool loading
               READ loading
               NOTIFY loadingChanged)

public:
    ~NodeModel();

    static NodeModel* instance();

#if QT_VERSION >= 0x050000
    QHash<int QByteArray> roleNames() const;
#endif
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const;
    
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role);

    Q_INVOKABLE QMap<int, QVariant> itemData(const QModelIndex &index) const;

    Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QModelIndex parent(const QModelIndex &child) const;

    Q_INVOKABLE QModelIndex activeIndex() const;

    Q_INVOKABLE Node* get(const QModelIndex &index) const;

    bool loading() const;

public slots:
    void listDirectory(const QModelIndex &parent = QModelIndex());
    void makeDirectory(const QModelIndex &parent, const QString &name);

    void renameNode(const QModelIndex &index, const QString &name);
    void moveNode(const QModelIndex &index, const QModelIndex &destination);
    void deleteNode(const QModelIndex &index);

    void setFilePublic(const QModelIndex &index, bool isPublic);

    void setActiveIndex(const QModelIndex &index);

    void reload(const QModelIndex &parent = QModelIndex());
    void clear(const QModelIndex &parent = QModelIndex());

private:
    NodeModel();

    void setLoading(bool loading);

    QModelIndex destinationIndex() const;
    void setDestinationIndex(const QModelIndex &index);

    QModelIndex actionIndex() const;
    void setActionIndex(const QModelIndex &index);

    void insertNode(int pos, Node *node, const QModelIndex &parent);
    void insertNodeAlphabetically(Node *node, const QModelIndex &parent);
    void appendNode(Node *node, const QModelIndex &parent);
    void removeNode(int pos, const QModelIndex &parent);
    void removeNodes(int pos, int count, const QModelIndex &parent);

private slots:
    void addNodes(NodeList *nodes);

    void onNodeCreated(Node *node);
    void onNodeDeleted(Reply *reply);
    void onNodeDataChanged(Node *node);

signals:
    void countChanged();
    void loadingChanged(bool loading);

private:
    static NodeModel *m_instance;

    Node *m_rootNode;

    QModelIndex m_activeIndex;
    QModelIndex m_destinationIndex;
    QModelIndex m_actionIndex;

    bool m_loading;

    QHash<int, QByteArray> m_roleNames;
};

#endif // NODEMODEL_H
