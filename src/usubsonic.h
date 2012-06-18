#ifndef USUBSONIC_H
#define USUBSONIC_H

#include <QObject>
#include <QMap>
#include <QVariantMap>

class UArtist;
class UAlbum;
class USong;
class UbuntuOAuth;
class USubSonic : public QObject
{
    Q_OBJECT
public:
    USubSonic();
    virtual ~USubSonic();

    void login();
    void play(const QString &id);

    // Music API
    void ping();
    void getMusicFolders();
    void getMusicDirectory(const QString &index);
    void getArtists();
    void getIndexes(const QString &musicFolderId = QString(),
                    const QString &ifModifiedSince = QString());
    void getAlbumList(const QString &type,
                      int size=-1,
                      int offset=-1);

signals:
    void authenticationSucess();
    void authenticationFail(const QString &message);
    void requestFail(const QString &error);

    void pingFinish(const QString &version);
    void getMusicFoldersFinish(const QMap<QString, QString> &list);
    void getMusicDirectoryFinish(const QList<USong*> &list);
    void getIndexesFinish(const QMap<QString, QMap<QString, QString> > &list);
    void getArtistsFinish(const QList<UArtist* > &list);
    void getAlbumListFinish(const QList<UAlbum* > &list);

private slots:
    void onRequestReady(const QByteArray &response);

private:
    enum SubSonicCommands {
        PING,
        GET_FOLDERS,
        GET_MUSIC_DIRECTORY,
        GET_INDEXS,
        GET_ARTISTS,
        GET_ALBUM_LIST,
        LAST_COMMAND
    };

    void getMusicFoldersParse(QVariantMap result);
    void getMusicDirectoryParse(QVariantMap result);
    void getIndexesParse(QVariantMap result);
    void getArtistsParse(QVariantMap result);
    void getAlbumListParse(QVariantMap result);

private:
    UbuntuOAuth *m_oauth;
    SubSonicCommands m_activeCommand;
};

#endif
