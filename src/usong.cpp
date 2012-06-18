#include "usong.h"

USong::USong(const QString &id,
             const QString &album,
             const QString &albumArtist,
             bool hasCoverArt,
             const QString &title,
             const QString &artist,
             const QString &genre,
             const QString &contentType,
             const QString &suffix,
             int bitRate,
             int duration,
             int track,
             int discNumber,
             int year,
             const QString &path,
             bool isDir)
    : m_id(id),
      m_album(album),
      m_albumArtist(albumArtist),
      m_hasCoverArt(hasCoverArt),
      m_title(title),
      m_genre(genre),
      m_contentType(contentType),
      m_suffix(suffix),
      m_bitRate(bitRate),
      m_duration(duration),
      m_track(track),
      m_discNumber(discNumber),
      m_year(year),
      m_path(path),
      m_isDir(isDir)
{
}

QString USong::id() const
{
    return m_id;
}

QString USong::album() const
{
    return m_album;
}

QString USong::albumArtist() const
{
    return m_albumArtist;
}

bool USong::hasCoverArt() const
{
    return m_hasCoverArt;
}

QString USong::title() const
{
    return m_title;
}

QString USong::artist() const
{
    return m_artist;
}

QString USong::genre() const
{
    return m_genre;
}

QString USong::contentType() const
{
    return m_contentType;
}

QString USong::suffix() const
{
    return m_suffix;
}

int USong::bitRate() const
{
    return m_bitRate;
}

int USong::duration() const
{
    return m_duration;
}

int USong::track() const
{
    return m_track;
}

int USong::discNumber() const
{
    return m_discNumber;
}

int USong::year() const
{
    return m_year;
}

QString USong::path() const
{
    return m_path;
}

bool USong::isDir() const
{
    return m_isDir;
}
