#include "combatroundmodel.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

void CombatRoundModel::fetchData()
{
    qDebug() << "Fetchdata";
    QJsonObject query;
    query["objectType"] = QString::fromUtf8("objects.weapons");
    EnginioReply *reply = _client->query(query);
    connect(reply,SIGNAL(finished(EnginioReply*)),this,SLOT(processReply(EnginioReply*)));
}


void CombatRoundModel::processReply(EnginioReply* reply)
{

    QJsonObject results = reply->data();
    qDebug() << "fetch done:"<< results;
    QJsonArray weapons = results["results"].toArray();
    for (int i = 0 ; i < weapons.count(); i++)
    {
        QJsonObject wJson = weapons[i].toObject();
        Weapon *w = new Weapon(wJson);
        qDebug() << w->toString();
        _weapons[w->toString()] = w;
    }
    reply->deleteLater();
    emit finished();
}
