#include "ualbum.h"

UAlbum:: UAlbum(const QString &id,
                const QString &parent,
                const QString &title,
                const QString &artist,
                bool isDir,
                const QString &coverArt,
                qreal userRating,
                qreal averageRating)
    : m_id(id),
      m_parent(parent),
      m_title(title),
      m_artist(artist),
      m_isDir(isDir),
      m_coverArt(coverArt),
      m_userRating(userRating),
      m_averageRating(averageRating)
{
}

QString UAlbum::id() const
{
    return m_id;
}

QString UAlbum::parent() const
{
    return m_parent;
}

QString UAlbum::title() const
{
    return m_title;
}

QString UAlbum::artist() const
{
    return m_artist;
}

bool UAlbum::isDir() const
{
    return m_isDir;
}

QString UAlbum::coverArt() const
{
    return m_coverArt;
}

qreal UAlbum::userRating() const
{
    return m_userRating;
}

qreal UAlbum::averageRating() const
{
    return m_averageRating;
}

QDataStream& operator<<(QDataStream & stream, const UAlbum &album)
{
    stream << album.id()
           << album.parent()
           << album.title()
           << album.artist()
           << album.isDir()
           << album.coverArt()
           << QString::number(album.userRating())
           << QString::number(album.averageRating());
    return stream;
}
