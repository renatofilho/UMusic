#include "usubsonic.h"
#include "auth.h"
#include "uartist.h"
#include "ualbum.h"
#include "usong.h"

#include <QStringList>
#include <QDebug>
#include <qjson/parser.h>
#include <phonon>

#define UBUNTU_STREAMING_BASE_URL   "https://streaming.one.ubuntu.com/rest/"
#define UBUNTU_SERVICE_BASE_URL     "https://one.ubuntu.com/music/api/1.0/"


USubSonic::USubSonic()
    : m_oauth(new UbuntuOAuth()),
      m_activeCommand(LAST_COMMAND)
{
    connect(m_oauth, SIGNAL(accessGranted()),
            this, SIGNAL(authenticationSucess()));
    connect(m_oauth, SIGNAL(accessFail(QString)),
            this, SIGNAL(authenticationFail(QString)));
    connect(m_oauth, SIGNAL(requestReady(QByteArray)),
            this, SLOT(onRequestReady(QByteArray)));
}

USubSonic::~USubSonic()
{
}

void USubSonic::login()
{
    m_oauth->getAccess();
}

void USubSonic::play(const QString &id)
{
    QMap<QString, QString> args;
    args.insert("id", id);
    QUrl url = m_oauth->buildUrl(UBUNTU_STREAMING_BASE_URL + QString("stream.view"), args);

    // TODO move this to model or view
    Phonon::MediaSource source = Phonon::MediaSource(url);
    Phonon::MediaObject *music = Phonon::createPlayer(Phonon::MusicCategory, source);
    music->play();

    qDebug() << "Url: " << url;
}

void USubSonic::ping()
{
    static QString pingCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (pingCommand.isEmpty()) {
        pingCommand = UBUNTU_SERVICE_BASE_URL + QString("ping.view");
    }

    if (m_activeCommand != LAST_COMMAND) {
        return;
    }
    m_activeCommand = PING;
    m_oauth->doRequest(pingCommand);
}

void USubSonic::getMusicFolders()
{
    static QString getMusicDirectoryCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (getMusicDirectoryCommand.isEmpty()) {
        getMusicDirectoryCommand = UBUNTU_SERVICE_BASE_URL + QString("getMusicFolders.view");
    }

    if (m_activeCommand != LAST_COMMAND) {
        return;
    }
    m_activeCommand = GET_FOLDERS;
    m_oauth->doRequest(getMusicDirectoryCommand);
}

void USubSonic::getAlbumList(const QString &type, int size, int offset)
{
    static QString getAlbumListCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (getAlbumListCommand.isEmpty()) {
        getAlbumListCommand = UBUNTU_SERVICE_BASE_URL + QString("getAlbumList.view");
    }

    if (m_activeCommand != LAST_COMMAND) {
        return;
    }
    m_activeCommand = GET_ALBUM_LIST;
    QMap<QString, QString> extraArgs;
    extraArgs.insert("type", type);
    if (size > 0) {
        extraArgs.insert("size", QString::number(size));
    }
    if (offset > -1) {
        extraArgs.insert("offset", QString::number(offset));
    }
    m_oauth->doRequest(getAlbumListCommand);
}


void USubSonic::getIndexes(const QString &musicFolderId,
                           const QString &ifModifiedSince)
{
    static QString getIndexesCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (getIndexesCommand.isEmpty()) {
        getIndexesCommand = UBUNTU_SERVICE_BASE_URL + QString("getIndexes.view");
    }

    if (m_activeCommand != LAST_COMMAND) {
        return;
    }
    m_activeCommand = GET_INDEXS;
    QMap<QString, QString> extraArgs;
    if (!musicFolderId.isEmpty()) {
        extraArgs.insert("musicFolderId", musicFolderId);
    }
    if (!ifModifiedSince.isEmpty()) {
        extraArgs.insert("ifModifiedSince", ifModifiedSince);
    }
    m_oauth->doRequest(getIndexesCommand, extraArgs);
}

void USubSonic::getArtists()
{
    static QString getArtistsCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (getArtistsCommand.isEmpty()) {
        getArtistsCommand = UBUNTU_SERVICE_BASE_URL + QString("getArtists.view");
    }

    if (m_activeCommand != LAST_COMMAND) {
        return;
    }
    m_activeCommand = GET_ARTISTS;
    m_oauth->doRequest(getArtistsCommand);
}

void USubSonic::getMusicDirectory(const QString &index)
{
    static QString getMusicDirectoryCommand;
    if (m_activeCommand != LAST_COMMAND) {
        return;
    }

    if (getMusicDirectoryCommand.isEmpty()) {
        getMusicDirectoryCommand = UBUNTU_SERVICE_BASE_URL + QString("getMusicDirectory.view");
    }

    m_activeCommand = GET_MUSIC_DIRECTORY;
    QMap<QString, QString> extraArgs;
    extraArgs.insert("id", index);
    m_oauth->doRequest(getMusicDirectoryCommand, extraArgs);
}


void USubSonic::getAlbumListParse(QVariantMap result)
{
    QList<UAlbum *> albums;
    if (result.contains("album")) {
        foreach (QVariant value, result["album"].toList()) {
            QVariantMap map = value.toMap();
            UAlbum *album = new UAlbum(map["id"].toString(),
                                       map["parent"].toString(),
                                       map["title"].toString(),
                                       map["artist"].toString(),
                                       map["isDir"].toBool(),
                                       map["coverArt"].toString(),
                                       map["userRating"].toDouble(),
                                       map["averageRating"].toDouble());
            albums.append(album);
        }
    }

    qDebug() << "Albumns" << albums;
    emit getAlbumListFinish(albums);
}

void USubSonic::getMusicFoldersParse(QVariantMap result)
{
    QMap<QString, QString> folders;
    if (result.contains("musicFolder")) {
        foreach (QVariant value, result["musicFolder"].toList()) {
            QVariantMap map = value.toMap();
            folders.insert(map["id"].toString(), map["name"].toString());
        }
    }

    qDebug() << "Folders" << folders;
    emit getMusicFoldersFinish(folders);
}

void USubSonic::getIndexesParse(QVariantMap result)
{
    QMap<QString, QMap<QString, QString> > folders;

    if (result.contains("index")) {
        foreach (QVariant value, result["index"].toList()) {
            QVariantMap map = value.toMap();
            QMap<QString, QString> artistMap;
            foreach (QVariant artistItem, map["artist"].toList()) {
                QVariantMap artist = artistItem.toMap();
                artistMap.insert("has_cover_art", artist["has_cover_art"].toString());
                artistMap.insert("id", artist["id"].toString());
                artistMap.insert("name", artist["name"].toString());
            }

            folders.insert(map["name"].toString(), artistMap);
        }
    }

    qDebug() << "Folders" << folders;
    emit getIndexesFinish(folders);
}

void USubSonic::getArtistsParse(QVariantMap result)
{
    QList<UArtist* > artists;

    if (result.contains("artist")) {
        foreach (QVariant value, result["artist"].toList()) {
            QVariantMap artist = value.toMap();
            UArtist *a = new UArtist(artist["id"].toString(),
                                     artist["name"].toString(),
                                     artist["covertArt"].toString(),
                                     artist["albumCount"].toInt());
            artists.append(a);
        }
    }
    qDebug() << artists;
    emit getArtistsFinish(artists);
}

void USubSonic::getMusicDirectoryParse(QVariantMap result)
{
    QList<USong*> songs;
    if (result.contains("child")) {
        foreach (QVariant value, result["child"].toList()) {
            QVariantMap map = value.toMap();
            USong *song = new USong(map["id"].toString(),
                                    map["album"].toString(),
                                    map["albumArtist"].toString(),
                                    map["hasCoverArt"].toBool(),
                                    map["title"].toString(),
                                    map["artist"].toString(),
                                    map["genre"].toString(),
                                    map["contentType"].toString(),
                                    map["suffix"].toString(),
                                    map["bitRate"].toInt(),
                                    map["duration"].toInt(),
                                    map["track"].toInt(),
                                    map["discNumber"].toInt(),
                                    map["year"].toInt(),
                                    map["path"].toString(),
                                    map["isDir"].toBool());
            songs << song;
        }
    }

    emit getMusicDirectoryFinish(songs);
}


void USubSonic::onRequestReady(const QByteArray &response)
{
    QJson::Parser parser;
    bool ok;
    SubSonicCommands command = m_activeCommand;
    m_activeCommand = LAST_COMMAND;

    qDebug() << "Parse:" << response;
    QVariantMap result = parser.parse(response, &ok).toMap();
    if (!ok) {
        qWarning() << "An error occurred during parsing: " << parser.errorString();
        emit requestFail("Fail to parse reply");
        return;
    }

    QVariantMap subsonicResponse = result["subsonic-response"].toMap();
    if (subsonicResponse["status"].toString() != "ok") {
        emit requestFail("Server status: " + subsonicResponse["status"].toString());
        return;
    }

    switch (command) {
        case PING:
        {
            pingFinish(subsonicResponse["version"].toString());
            break;
        }
        case GET_FOLDERS:
        {
            getMusicFoldersParse(subsonicResponse["musicFolders"].toMap());
            break;
        }
        case GET_MUSIC_DIRECTORY:
        {
            getMusicDirectoryParse(subsonicResponse["directory"].toMap());
            break;
        }
        case GET_INDEXS:
        {
            getIndexesParse(subsonicResponse["indexes"].toMap());
            break;
        }
        case GET_ARTISTS:
        {
            getArtistsParse(subsonicResponse["artists"].toMap());
            break;
        }
        case GET_ALBUM_LIST:
        {
            getAlbumListParse(subsonicResponse["albumList"].toMap());
            break;
        }
        default:
        {
            qWarning() << "Invalid command" << m_activeCommand;
            break;
        }
    }
}
