#include "combatroundmodel.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QList>

void CombatRoundModel::fetchData()
{
    qDebug() << "Fetchdata";
    QNetworkConfigurationManager mgr;
    //QNetworkConfiguration conf = mgr.defaultConfiguration();
    //qDebug() << conf.name() << ":" << conf.identifier() << ":" << conf.state() << ":" << conf.type();
    /*QList<QNetworkConfiguration> confs = mgr.allConfigurations();
    for (int i = 0 ; i < confs.size(); i++)
    {
        conf = confs[i];
        qDebug() << conf.name() << ":" << conf.identifier() << ":" << conf.state();
    }*/
    if (mgr.isOnline()&&false)
    {
    QJsonObject query;
    query["objectType"] = QString::fromUtf8("objects.weapons");
    EnginioReply *reply = _client->query(query);
    connect(reply,SIGNAL(finished(EnginioReply*)),this,SLOT(processReply(EnginioReply*)));
    }
    else
    {
        qDebug() << "System seems to be offline. Creating local data";
        Weapon *w = new Weapon();
        _weapons[w->toString()] = w;
        w = new Weapon("Bow","D6",4);
        _weapons[w->toString()] = w;
        emit finished();
    }
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
