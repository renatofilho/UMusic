#include "uartist.h"

UArtist::UArtist(const QString &id,
                 const QString &name,
                 const QString &coverArt,
                 int albumCount)
    : m_id(id),
      m_name(name),
      m_coverArt(coverArt),
      m_albumCount(albumCount)
{
}

QString UArtist::id()
{
    return m_id;
}

QString UArtist::name()
{
    return m_name;
}

QString UArtist::coverArt()
{
    return m_coverArt;
}

int UArtist::albumCount()
{
    return m_albumCount;
}

QDataStream& operator<<(QDataStream & stream, UArtist* artist)
{
    stream << artist->id() << artist->name() << artist->coverArt() << artist->albumCount();
    return stream;
}
