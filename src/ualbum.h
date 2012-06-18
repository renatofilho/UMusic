#ifndef __UALBUM_H__
#define __UALBUM_H__

#include <QObject>

class UAlbum : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString parent READ parent)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(bool isDir READ isDir)
    Q_PROPERTY(QString coverArt READ coverArt)
    Q_PROPERTY(qreal userRating READ userRating)
    Q_PROPERTY(qreal averageRating READ averageRating)

    public:
        UAlbum(const QString &id,
               const QString &parent,
               const QString &title,
               const QString &artist,
               bool isDir,
               const QString &coverArt,
               qreal userRating,
               qreal averageRating);
        QString id() const;
        QString parent() const;
        QString title() const;
        QString artist() const;
        bool isDir() const;
        QString coverArt() const;
        qreal userRating() const;
        qreal averageRating() const;

    private:
        QString m_id;
        QString m_parent;
        QString m_title;
        QString m_artist;
        bool m_isDir;
        QString m_coverArt;
        qreal m_userRating;
        qreal m_averageRating;
};

QDataStream& operator<<(QDataStream & stream, const UAlbum& artist);

#endif
