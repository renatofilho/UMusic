#ifndef __UARTIST_H__
#define __UARTIST_H__

#include <QObject>

class UArtist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString coverArt READ coverArt)
    Q_PROPERTY(int albumCount READ albumCount)

    public:
        UArtist(const QString &id,
                const QString &name,
                const QString &covertArt,
                int albumCount);
        QString id();
        QString name();
        QString coverArt();
        int albumCount();

    private:
        QString m_id;
        QString m_name;
        QString m_coverArt;
        int m_albumCount;
};

QDataStream& operator<<(QDataStream & stream, UArtist* artist);

#endif
