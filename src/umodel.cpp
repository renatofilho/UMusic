#include "umodel.h"
#include "usubsonic.h"
#include "ualbum.h"
#include "usong.h"

#include <QDebug>

UModel::UModel(USubSonic *server)
    : m_server(server),
      m_loading(false)
{
    connect(m_server, SIGNAL(requestFail(QString)),
            this, SLOT(onRequestFail(QString)));

    connect(m_server, SIGNAL(getAlbumListFinish(QList<UAlbum*>)),
            this, SLOT(onGetAlbumListFinish(QList<UAlbum*>)));

    connect(m_server, SIGNAL(getMusicDirectoryFinish(QList<USong*>)),
            this, SLOT(onGetMusicDirectoryFinish(QList<USong*>)));

    QHash<int, QByteArray> roleNames;
    roleNames.insert(ID, "id");
    roleNames.insert(COVER_ART, "coverArt");
    roleNames.insert(TITLE, "title");
    roleNames.insert(SUB_TITLE, "subTitle");
    roleNames.insert(HAS_CHILDREN, "hasChildren");
    roleNames.insert(DATA, "data");
    setRoleNames(roleNames);
}

void UModel::loadAlbumns()
{
    if (m_loading) {
        return;
    }
    m_loading = true;
    // TODO: implement getAllAlbumList(type, pageSize);
    m_server->getAlbumList("frequent");
}

void UModel::onGetAlbumListFinish(QList<UAlbum *> albums)
{
    clearData();
    beginInsertRows(QModelIndex(), 0, albums.size()-1);
    foreach(UAlbum *album, albums) {
        QMap<int, QVariant> data;

        data.insert(ID, album->id());
        data.insert(COVER_ART, album->coverArt());
        data.insert(TITLE, album->title());
        data.insert(SUB_TITLE, album->artist());
        data.insert(HAS_CHILDREN, true);
        data.insert(DATA, QVariant::fromValue<QObject*>(album));

        m_data << data;
    }
    endInsertRows();
    m_loading = false;
}

void UModel::loadFolder(const QString &index)
{
    if (m_loading) {
        return;
    }
    m_loading = true;
    m_server->getMusicDirectory(index);
}

void UModel::playSong(const QString &songId)
{
    m_server->play(songId);
}

void UModel::onGetMusicDirectoryFinish(QList<USong *> songs)
{
    clearData();
    beginInsertRows(QModelIndex(), 0, songs.size()-1);
    foreach(USong *song, songs) {
        QMap<int, QVariant> data;
        data.insert(ID, song->id());
        data.insert(COVER_ART, "");
        data.insert(TITLE, song->title());
        data.insert(SUB_TITLE, song->artist());
        data.insert(HAS_CHILDREN, false);
        data.insert(DATA, QVariant::fromValue<QObject*>(song));
        m_data << data;
    }
    endInsertRows();
    m_loading = false;
}

void UModel::clearData()
{
    beginRemoveRows(QModelIndex(), 0, m_data.size()-1);
    for(int i=0; i < m_data.size(); i++) {
        QMap<int, QVariant> data = m_data.at(i);
        delete data[DATA].value<QObject*>();
    }
    endRemoveRows();
    m_data.clear();
}

void UModel::onRequestFail(const QString &error)
{
    m_loading = false;
    qWarning() << "Model error:" << error;
}

int	UModel::columnCount(const QModelIndex &parent) const
{
    return LAST_COLUMN;
}

QVariant UModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < m_data.size()) {
        return m_data.at(index.row())[role];
    }
    return QVariant();
}

QModelIndex	UModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int	UModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}
