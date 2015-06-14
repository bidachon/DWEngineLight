#include "playerlistmodel.h"
#include <QDebug>
#include <QModelIndex>

PlayerListModel::PlayerListModel()
{

}

int PlayerListModel::rowCount(const QModelIndex & parent) const
{
    return _players.size();
}

bool PlayerListModel::exists(const QString &name)
{
    if (_players.count(name)!= 0)
        return true;
    return false;
}

void PlayerListModel::clearDeads()
{
    QMap<QString, Player*>::iterator i = _players.begin();
    while (i != _players.end()) {
        if (i.value()->healthPoints() <= -3)
        {
            _keys.removeAll(i.value()->name());
            i = _players.erase(i);
        }
        else
            ++i;
    }
    QModelIndex topleft = createIndex(0,0);
    QModelIndex bottomright = createIndex(rowCount(),0);
    emit dataChanged(topleft,bottomright);
}

QVariant PlayerListModel::data(const QModelIndex & index, int role) const
{
    int row = index.row();
    if (row >= rowCount())
    {
        qDebug() << "Invalid row in PlayerListModel";
    }
    QString key = _keys[row];
    Player *p = _players.value(key);
    if (role == Qt::DisplayRole)
    {
       return QString("%1 (%2)").arg(p->name()).arg(p->healthPoints());
    }
    return QVariant();
}

Player *PlayerListModel::player(const QString & name)
{
    return _players.value(name);
}

Player *PlayerListModel::player(const QModelIndex &index)
{
    if (!index.isValid())
        return NULL;
    if (index.row() < 0 || index.row() >= _keys.size())
        return NULL;
    return _players.value(_keys[index.row()]);
}

QModelIndex PlayerListModel::find(const QString &name)
{
    if (_keys.contains(name))
    {
        return createIndex(_keys.indexOf(name),0);
    }
    else
        return QModelIndex();
}

void PlayerListModel::addPlayer(Player*p)
{

    _players[p->name()] = p;
    _keys.push_back(p->name());
    QModelIndex topleft = createIndex(0,0);
    QModelIndex bottomright = createIndex(rowCount(),0);

    emit dataChanged(topleft,bottomright);
}


