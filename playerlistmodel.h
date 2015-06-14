#ifndef PLAYERLISTMODEL_H
#define PLAYERLISTMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QStringList>
#include "player.h"

class PlayerListModel: public QAbstractListModel
{
public:
    PlayerListModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void addPlayer(Player *);
    Player *player(const QString & name);
    Player *player(const QModelIndex &index);
    QModelIndex find(const QString &name);
    bool exists(const QString &name);
    void clearDeads();
    //bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    //Qt::ItemFlags flags(const QModelIndex & index) const;

private:
    //QVector<Player *> _players;
    QStringList _keys;
    QMap<QString,Player*> _players;
};

#endif // PLAYERLISTMODEL_H
