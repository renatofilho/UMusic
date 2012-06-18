#ifndef __UMODEL_H__
#define __UMODEL_H__

#include <QObject>
#include <QAbstractListModel>

class USubSonic;
class USong;
class UAlbum;

class UModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UModelColumns {
        ID,
        COVER_ART,
        TITLE,
        SUB_TITLE,
        HAS_CHILDREN,
        DATA,
        LAST_COLUMN
    };

    UModel(USubSonic *server);

    // Actions
    Q_INVOKABLE void loadAlbumns();
    Q_INVOKABLE void loadFolder(const QString &album);
    Q_INVOKABLE void playSong(const QString &id);

    // Model
    virtual int	columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex	parent(const QModelIndex &index) const;
    virtual int	rowCount(const QModelIndex &parent = QModelIndex()) const;

private slots:
    void onGetAlbumListFinish(QList<UAlbum*>);
    void onGetMusicDirectoryFinish(QList<USong *> songs);
    void onRequestFail(const QString &error);

private:
    USubSonic* m_server;
    bool m_loading;
    QList<QMap<int, QVariant> > m_data;

    void clearData();
};

#endif
