#ifndef UBUNTU_OAUTH_H
#define UBUNTU_OAUTH_H

#include <QObject>
#include <QMultiMap>
#include <QSettings>
#include <QUrl>

class QNetworkReply;
class KQOAuthManager;
class KQOAuthRequest;

class UbuntuOAuth : public QObject {
    Q_OBJECT
public:
    UbuntuOAuth();
    ~UbuntuOAuth();

    void getAccess();
    bool hasKeys();
    QUrl buildUrl(const QString &baseUrl,
                  QMap<QString, QString> extraArgs = QMap<QString, QString>());

    void doRequest(const QString &url,
                   QMap<QString, QString> extraArgs = QMap<QString, QString>());

private:
    void getUserMobileInformation();

private slots:
    void onTemporaryTokenReceived(QString temporaryToken, QString temporaryTokenSecret);
    void onAuthorizationReceived(QString token, QString verifier);
    void onAccessTokenReceived(QString token, QString tokenSecret);
    void onReplyFinished(QNetworkReply*);
    void onAuthorizedRequestDone();

signals:
    void accessGranted();
    void accessFail(const QString &error);
    void requestReady(QByteArray);

private:
    KQOAuthManager *m_oauthManager;
    KQOAuthRequest *m_oauthRequest;
    QSettings m_oauthSettings;
    QString m_mobileUser;
    QString m_mobilePassword;
};


#endif // TWITTERAUTH_H
