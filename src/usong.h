#ifndef __USONG_H__
#define __USONG_H__

#include <QObject>

class USong : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(QString albumArtist READ albumArtist)

    Q_PROPERTY(bool hasCoverArt READ hasCoverArt)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString genre READ genre)

    Q_PROPERTY(QString contentType READ contentType)
    Q_PROPERTY(QString suffix READ suffix)
    Q_PROPERTY(int bitRate READ bitRate)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(int track READ track)

    Q_PROPERTY(int discNumber READ discNumber)
    Q_PROPERTY(int year READ year)
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(bool isDir READ isDir)

    public:
        USong(const QString &id,
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
              bool isDir);

        QString id() const;
        QString album() const;
        QString albumArtist() const;
        bool hasCoverArt() const;
        QString title() const;
        QString artist() const;
        QString genre() const;
        QString contentType() const;
        QString suffix() const;
        int bitRate() const;
        int duration() const;
        int track() const;
        int discNumber() const;
        int year() const;
        QString path() const;
        bool isDir() const;

    private:
        QString m_id;
        QString m_album;
        QString m_albumArtist;
        bool m_hasCoverArt;
        QString m_title;
        QString m_artist;
        QString m_genre;
        QString m_contentType;
        QString m_suffix;
        int m_bitRate;
        int m_duration;
        int m_track;
        int m_discNumber;
        int m_year;
        QString m_path;
        bool m_isDir;
};

#endif
