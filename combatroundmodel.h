#ifndef COMBATROUNDMODEL
#define COMBATROUNDMODEL

#include <enginioclient.h>
#include <QObject>
#include <player.h>
#include <enginioreply.h>

class CombatRoundModel: public QObject
{
    Q_OBJECT
public:
    CombatRoundModel(QObject *parent):QObject(parent)
    {
        _client = new EnginioClient(parent);
        _client->setBackendId(QByteArrayLiteral("53d01c99698b3c64bd028c89"));
    }

    ~CombatRoundModel()  { }

    void fetchData();

    QMap<QString, Weapon*> & weapons() {return _weapons;}

signals:
    void finished();

private slots:
    void processReply(EnginioReply *);

private:

    EnginioClient *_client;
    QMap<QString, Weapon*> _weapons;

};

#endif // COMBATROUNDMODEL

