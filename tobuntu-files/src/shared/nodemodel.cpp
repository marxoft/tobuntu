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

#include "nodemodel.h"
#include "files.h"
#include "notifications.h"

NodeModel* NodeModel::m_instance = 0;

NodeModel::NodeModel() :
    QAbstractItemModel(),
    m_rootNode(new Node(this)),
    m_loading(false)
{
    if (!m_instance) {
        m_instance = this;
    }

    m_roleNames[NodeRoles::NodeTypeRole] = "nodeType";
    m_roleNames[NodeRoles::PathRole] = "path";
    m_roleNames[NodeRoles::ResourcePathRole] = "resourcePath";
    m_roleNames[NodeRoles::ContentPathRole] = "contenPath";
    m_roleNames[NodeRoles::ParentPathRole] = "parentPath";
    m_roleNames[NodeRoles::VolumePathRole] = "volumePath";
    m_roleNames[NodeRoles::NodePathRole] = "nodePath";
    m_roleNames[NodeRoles::NameRole] = "name";
    m_roleNames[NodeRoles::SuffixRole] = "suffix";
    m_roleNames[NodeRoles::KeyRole] = "key";
    m_roleNames[NodeRoles::WhenCreatedRole] = "whenCreated";
    m_roleNames[NodeRoles::LastModifiedRole] = "lastModified";
    m_roleNames[NodeRoles::GenerationRole] = "generation";
    m_roleNames[NodeRoles::GenerationCreatedRole] = "generationCreated";
    m_roleNames[NodeRoles::HashRole] = "hash";
    m_roleNames[NodeRoles::SizeRole] = "size";
    m_roleNames[NodeRoles::IsPublicRole] = "isPublic";
    m_roleNames[NodeRoles::PublicUrlRole] = "publicUrl";
    m_roleNames[NodeRoles::HasChildrenRole] = "hasChildren";
    m_roleNames[NodeRoles::IsRootRole] = "isRoot";
#if QT_VERSION < 0x050000
    this->setRoleNames(m_roleNames);
#endif
}

NodeModel::~NodeModel() {}

NodeModel* NodeModel::instance() {
    return !m_instance ? new NodeModel : m_instance;
}

#if QT_VERSION >= 0x050000
QHash<int, QByteArray> NodeModel::roleNames() const {
    return m_roleNames;
}
#endif

int NodeModel::rowCount(const QModelIndex &parent) const {
    Node *parentNode;

    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentNode = m_rootNode;
    }
    else {
        parentNode = this->get(parent);
    }

    return parentNode->childNodeCount();
}

int NodeModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);

    return 1;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const {
    if (Node *node = this->get(index)) {
        switch (role) {
        case NodeRoles::NodeTypeRole:
            return node->nodeType();
        case NodeRoles::PathRole:
            return node->path();
        case NodeRoles::ResourcePathRole:
            return node->resourcePath();
        case NodeRoles::ContentPathRole:
            return node->contentPath();
        case NodeRoles::ParentPathRole:
            return node->parentPath();
        case NodeRoles::VolumePathRole:
            return node->volumePath();
        case NodeRoles::NodePathRole:
            return node->nodePath();
        case NodeRoles::NameRole:
            return node->name();
        case NodeRoles::SuffixRole:
            return node->suffix();
        case NodeRoles::KeyRole:
            return node->key();
        case NodeRoles::WhenCreatedRole:
            return node->whenCreated();
        case NodeRoles::LastModifiedRole:
            return node->lastModified();
        case NodeRoles::GenerationRole:
            return node->generation();
        case NodeRoles::GenerationCreatedRole:
            return node->generationCreated();
        case NodeRoles::HashRole:
            return node->hash();
        case NodeRoles::SizeRole:
            return node->size();
        case NodeRoles::IsPublicRole:
            return node->isPublic();
        case NodeRoles::PublicUrlRole:
            return node->publicUrl();
        case NodeRoles::HasChildrenRole:
            return node->hasChildren();
        case NodeRoles::IsRootRole:
            return node->isRoot();
        default:
            break;
        }
    }

    return QVariant();
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    switch (role) {
    case NodeRoles::IsPublicRole:
        this->setFilePublic(index, value.toBool());
        return true;
    default:
        return false;
    }
}

QMap<int, QVariant> NodeModel::itemData(const QModelIndex &index) const {
    QMap<int, QVariant> map;

    for (int role = NodeRoles::NodeTypeRole; role <= NodeRoles::IsRootRole; role++) {
        map[role] = this->data(index, role);
    }

    return map;
}

QModelIndex NodeModel::index(int row, int column, const QModelIndex &parent) const {
    if ((parent.isValid()) && (parent.column() != 0)) {
        return QModelIndex();
    }

    Node *parentNode = this->get(parent);

    if (parentNode) {
        Node *childNode = parentNode->childNode(row);

        if (childNode) {
            return this->createIndex(row, column, childNode);
        }
    }

    return QModelIndex();
}

QModelIndex NodeModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    Node *childNode = this->get(child);
    Node *parentNode = childNode->parentNode();

    if (parentNode == m_rootNode) {
        return QModelIndex();
    }

    return this->createIndex(parentNode->index(), 0, parentNode);
}

Node* NodeModel::get(const QModelIndex &index) const {
    return index.isValid() ? static_cast<Node*>(index.internalPointer()) : m_rootNode;
}

bool NodeModel::loading() const {
    return m_loading;
}

void NodeModel::setLoading(bool loading) {
    if (loading != this->loading()) {
        m_loading = loading;
        emit loadingChanged(loading);
    }
}

QModelIndex NodeModel::activeIndex() const {
    return m_activeIndex;
}

void NodeModel::setActiveIndex(const QModelIndex &index) {
    m_activeIndex = index;
}

QModelIndex NodeModel::destinationIndex() const {
    return m_destinationIndex;
}

void NodeModel::setDestinationIndex(const QModelIndex &index) {
    m_destinationIndex = index;
}

QModelIndex NodeModel::actionIndex() const {
    return m_actionIndex;
}

void NodeModel::setActionIndex(const QModelIndex &index) {
    m_actionIndex = index;
}

void NodeModel::insertNode(int pos, Node *node, const QModelIndex &parent) {
    if ((pos >= 0) && (pos < this->rowCount(parent))) {
        if (Node *parentNode = this->get(parent)) {
            this->beginInsertRows(parent, pos, pos);
            parentNode->insertChildNode(pos, node);
            node->setParent(parentNode);
            this->endInsertRows();

            emit countChanged();
        }
    }
    else {
        this->appendNode(node, parent);
    }
}

void NodeModel::insertNodeAlphabetically(Node *node, const QModelIndex &parent) {
    if (Node *parentNode = this->get(parent)) {
        int i = 0;

        switch (node->nodeType()) {
        case Node::File:
            foreach (Node *childNode, parentNode->childNodes()) {
                if ((childNode->nodeType() == Node::File) && (childNode->path().compare(node->path(), Qt::CaseInsensitive) > 0)) {
                    break;
                }

                i++;
            }

            break;
        default:
            foreach (Node *childNode, parentNode->childNodes()) {
                if ((childNode->nodeType() == Node::File) || (childNode->path().compare(node->path(), Qt::CaseInsensitive) > 0)) {
                    break;
                }

                i++;
            }

            break;
        }

        this->insertNode(i, node, parent);
    }
}

void NodeModel::appendNode(Node *node, const QModelIndex &parent) {
    if (Node *parentNode = this->get(parent)) {
        int count = this->rowCount(parent);
        this->beginInsertRows(parent, count, count);
        parentNode->addChildNode(node);
        node->setParent(parentNode);
        this->endInsertRows();

        emit countChanged();
    }
}

void NodeModel::removeNode(int pos, const QModelIndex &parent) {
    if ((pos >= 0) && (pos < this->rowCount(parent))) {
        if (Node *parentNode = this->get(parent)) {
            this->beginRemoveRows(parent, pos, pos);

            if (Node *childNode = parentNode->removeChildNode(pos)) {
                childNode->deleteLater();
            }

            this->endRemoveRows();

            emit countChanged();
        }
    }
}

void NodeModel::removeNodes(int pos, int count, const QModelIndex &parent) {
    if ((pos >= 0) && ((pos + count) <= this->rowCount(parent))) {
        if (Node *parentNode = this->get(parent)) {
            this->beginRemoveRows(parent, pos, pos + count - 1);

            for (int i = pos; i < (pos + count); i++) {
                if (Node *childNode = parentNode->removeChildNode(pos)) {
                    childNode->deleteLater();
                }
            }

            this->endRemoveRows();

            emit countChanged();
        }
    }
}

void NodeModel::listDirectory(const QModelIndex &parent) {
    this->setLoading(true);
    this->setActiveIndex(parent);

    if (!parent.isValid()) {
        this->connect(Files::getVolumes(), SIGNAL(ready(NodeList*)), this, SLOT(addNodes(NodeList*)));
    }
    else {
        switch (parent.data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::Volume:
            this->connect(Files::listDirectory(parent.data(NodeRoles::NodePathRole).toString()), SIGNAL(ready(NodeList*)), this, SLOT(addNodes(NodeList*)));
            return;
        default:
            this->connect(Files::listDirectory(parent.data(NodeRoles::ResourcePathRole).toString()), SIGNAL(ready(NodeList*)), this, SLOT(addNodes(NodeList*)));
            return;
        }
    }
}

void NodeModel::makeDirectory(const QModelIndex &parent, const QString &name) {
    this->setLoading(true);
    this->setDestinationIndex(parent);

    if (!parent.isValid()) {
        this->connect(Files::createVolume(name), SIGNAL(ready(Node*)), this, SLOT(onNodeCreated(Node*)));
    }
    else {
        switch (parent.data(NodeRoles::NodeTypeRole).toInt()) {
        case Node::Volume:
            this->connect(Files::makeDirectory(parent.data(NodeRoles::NodePathRole).toString() + '/' + name), SIGNAL(ready(Node*)), this, SLOT(onNodeCreated(Node*)));
            return;
        default:
            this->connect(Files::makeDirectory(parent.data(NodeRoles::ResourcePathRole).toString() + '/' + name), SIGNAL(ready(Node*)), this, SLOT(onNodeCreated(Node*)));
            return;
        }
    }
}

void NodeModel::renameNode(const QModelIndex &index, const QString &name) {
    this->setLoading(true);
    this->setActionIndex(index);
    this->setDestinationIndex(index.parent());

    Node *node = Files::moveNode(index.data(NodeRoles::ResourcePathRole).toString(),
                                 index.data(NodeRoles::PathRole).toString().section('/', 0, -2) + '/' + name);

    this->connect(node, SIGNAL(ready(Node*)), this, SLOT(onNodeDataChanged(Node*)));
}

void NodeModel::moveNode(const QModelIndex &index, const QModelIndex &destination) {
    this->setLoading(true);
    this->setActionIndex(index);
    this->setDestinationIndex(destination);

    switch (destination.data(NodeRoles::NodeTypeRole).toInt()) {
    case Node::Volume:
        this->connect(Files::moveNode(index.data(NodeRoles::ResourcePathRole).toString(),
                                      '/' + index.data(NodeRoles::NameRole).toString()),
                      SIGNAL(ready(Node*)), this, SLOT(onNodeDataChanged(Node*)));

        return;
    default:
        this->connect(Files::moveNode(index.data(NodeRoles::ResourcePathRole).toString(),
                                      destination.data(NodeRoles::PathRole).toString()
                                      + '/' + index.data(NodeRoles::NameRole).toString()),
                      SIGNAL(ready(Node*)), this, SLOT(onNodeDataChanged(Node*)));

        return;
    }


}

void NodeModel::deleteNode(const QModelIndex &index) {
    this->setLoading(true);
    this->setActionIndex(index);

    switch (index.data(NodeRoles::NodeTypeRole).toInt()) {
    case Node::Volume:
        this->connect(Files::deleteVolume(index.data(NodeRoles::ResourcePathRole).toString()), SIGNAL(finished(Reply*)), this, SLOT(onNodeDeleted(Reply*)));
        return;
    default:
        this->connect(Files::deleteNode(index.data(NodeRoles::ResourcePathRole).toString()), SIGNAL(finished(Reply*)), this, SLOT(onNodeDeleted(Reply*)));
        return;
    }
}

void NodeModel::setFilePublic(const QModelIndex &index, bool isPublic) {
    this->setLoading(true);
    this->setActionIndex(index);
    this->setDestinationIndex(index.parent());

    Node *node = Files::setFilePublic(index.data(NodeRoles::ResourcePathRole).toString(), isPublic);
    this->connect(node, SIGNAL(ready(Node*)), this, SLOT(onNodeDataChanged(Node*)));
}

void NodeModel::reload(const QModelIndex &parent) {
    this->clear(parent);
    this->listDirectory(parent);
}

void NodeModel::clear(const QModelIndex &parent) {
    this->removeNodes(0, this->rowCount(parent), parent);
}

void NodeModel::addNodes(NodeList *nodes) {
    this->setLoading(false);

    switch (nodes->error()) {
    case NodeList::NoError:
        foreach (Node *node, nodes->nodes()) {
            this->insertNodeAlphabetically(node, this->activeIndex());
        }

        break;
    default:
        Notifications::showError(nodes->errorString());
        break;
    }

    nodes->deleteLater();
}

void NodeModel::onNodeCreated(Node *node) {
    this->setLoading(false);

    switch (node->error()) {
    case Node::NoError:
        this->insertNodeAlphabetically(node, this->destinationIndex());
        return;
    default:
        Notifications::showError(node->errorString());
        break;
    }

    node->deleteLater();
}

void NodeModel::onNodeDeleted(Reply *reply) {
    this->setLoading(false);

    switch (reply->error()) {
    case Reply::NoError:
        this->removeNode(this->actionIndex().row(), this->actionIndex().parent());
        break;
    default:
        Notifications::showError(reply->errorString());
        break;
    }

    reply->deleteLater();
}

void NodeModel::onNodeDataChanged(Node *node) {
    this->setLoading(false);

    switch (node->error()) {
    case Node::NoError:
        this->removeNode(this->actionIndex().row(), this->actionIndex().parent());
        this->insertNodeAlphabetically(node, this->destinationIndex());
        return;
    default:
        Notifications::showError(node->errorString());
        break;
    }

    node->deleteLater();
}
