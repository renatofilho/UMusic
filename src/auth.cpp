#include <QStringList>
#include <QtDebug>

#include <QtKOAuth/QtKOAuth>

#include "auth.h"

#define UBUNTU_ONE_CONSUMER_KEY         "ubuntuone"
#define UBUNTU_ONE_CONSUMER_SECRET_KEY  "hammertime"
#define UBUNTU_SUBSONIC_API_VERSION     "1.2.0"
#define UMUSIC_APPLICATION_NAME         "UMusic"

#define UBUNTU_ONE_TOKEN_REQUEST_URL    "https://one.ubuntu.com/oauth/request/"
#define UBUNTU_ONE_AUTH_URL             "https://one.ubuntu.com/oauth/authorize/"
#define UBUNTU_ONE_ACCESS_TOKEN_URL     "https://one.ubuntu.com/oauth/access/"

UbuntuOAuth::UbuntuOAuth()
{
    m_oauthRequest = new KQOAuthRequest;
    m_oauthManager = new KQOAuthManager(this);
    m_oauthManager->setNetworkManager(new QNetworkAccessManager());
    //m_oauthRequest->setEnableDebugOutput(true);

    connect(m_oauthManager, SIGNAL(temporaryTokenReceived(QString,QString)),
            this, SLOT(onTemporaryTokenReceived(QString, QString)));
    connect(m_oauthManager, SIGNAL(authorizationReceived(QString,QString)),
            this, SLOT(onAuthorizationReceived(QString, QString)));
    connect(m_oauthManager, SIGNAL(accessTokenReceived(QString,QString)),
            this, SLOT(onAccessTokenReceived(QString,QString)));
    connect(m_oauthManager, SIGNAL(requestReady(QByteArray)),
            this, SIGNAL(requestReady(QByteArray)));
    connect(m_oauthManager, SIGNAL(authorizedRequestDone()),
            this, SLOT(onAuthorizedRequestDone()));

    // Catch redirection
    connect(m_oauthManager->networkManager(), SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReplyFinished(QNetworkReply*)));
}

UbuntuOAuth::~UbuntuOAuth()
{
    delete m_oauthRequest;
    delete m_oauthManager;
}

bool UbuntuOAuth::hasKeys()
{
    if (m_oauthSettings.value("oauth_token").toString().isEmpty() ||
        m_oauthSettings.value("oauth_token_secret").toString().isEmpty()) {
        return false;
    }
    return true;
}

void UbuntuOAuth::getAccess()
{
    if (hasKeys()) {
        emit accessGranted();
        return;
    }

    m_oauthRequest->initRequest(KQOAuthRequest::TemporaryCredentials,
                              QUrl(UBUNTU_ONE_TOKEN_REQUEST_URL));
    m_oauthRequest->setConsumerKey(UBUNTU_ONE_CONSUMER_KEY);
    m_oauthRequest->setConsumerSecretKey(UBUNTU_ONE_CONSUMER_SECRET_KEY);
    m_oauthManager->setHandleUserAuthorization(true);
    m_oauthManager->executeRequest(m_oauthRequest);
}

void UbuntuOAuth::onTemporaryTokenReceived(QString token, QString tokenSecret)
{
    qDebug() << ">>>>>>>>>> Temporary token received: " << token << tokenSecret;
    QUrl userAuthURL(UBUNTU_ONE_AUTH_URL);

    if(m_oauthManager->lastError() == KQOAuthManager::NoError) {
        qDebug() << "Asking for user's permission to access protected resources. Opening URL: " << userAuthURL;
        m_oauthManager->getUserAuthorization(userAuthURL);
    }
}

void UbuntuOAuth::onAuthorizationReceived(QString token, QString verifier)
{
    qDebug() << ">>>>>>>>>> User authorization received: " << token << verifier;
    m_oauthManager->getUserAccessTokens(QUrl(UBUNTU_ONE_ACCESS_TOKEN_URL));
    if(m_oauthManager->lastError() != KQOAuthManager::NoError) {
        getUserMobileInformation();
    } else {
        emit accessFail("Fail to authentication");
    }
}

void UbuntuOAuth::onAccessTokenReceived(QString token, QString tokenSecret)
{
    qDebug() << ">>>>>>>>>> Access token received: " << token << tokenSecret;
    m_oauthSettings.setValue("oauth_token", token);
    m_oauthSettings.setValue("oauth_token_secret", tokenSecret);

    // Get Mobile user information
    getUserMobileInformation();
    qDebug() << "Access tokens now stored. You are ready to send Tweets from user's account!";
}

void UbuntuOAuth::getUserMobileInformation()
{
    m_oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest,
                              QUrl("https://one.ubuntu.com/phones/creds/Meego"));
    m_oauthRequest->setConsumerKey(UBUNTU_ONE_CONSUMER_KEY);
    m_oauthRequest->setConsumerSecretKey(UBUNTU_ONE_CONSUMER_SECRET_KEY);
    m_oauthRequest->setToken(m_oauthSettings.value("oauth_token").toString());
    m_oauthRequest->setTokenSecret(m_oauthSettings.value("oauth_token_secret").toString());
    m_oauthRequest->setHttpMethod(KQOAuthRequest::GET);

    KQOAuthParameters params;
    params.insert("scheme", "x-ubuntuone");
    params.insert("manufacturer", "Nokia");
    params.insert("model", "N9");
    m_oauthRequest->setAdditionalParameters(params);
    m_oauthManager->executeRequest(m_oauthRequest);
}

void UbuntuOAuth::onReplyFinished (QNetworkReply *reply) {
    QVariant redirectUrl;

    redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (redirectUrl.isValid()) {
        QUrl url = redirectUrl.toUrl();
        QList <QPair <QString, QString> > queryItems = url.queryItems();
        for(int i=0; i < queryItems.size(); i++) {
            if (queryItems[i].first == "p") {
                m_oauthSettings.setValue("mobile_password", queryItems[i].second);
            } else if (queryItems[i].first == "u") {
                m_oauthSettings.setValue("mobile_username", queryItems[i].second);
            }
        }

        qDebug() << "User" << m_oauthSettings.value("mobile_username");
        qDebug() << "Password" << m_oauthSettings.value("mobile_password");

        emit accessGranted();
    } else {
        emit accessFail("Fail to get mobile user information");
    }
}

void UbuntuOAuth::doRequest(const QString &url,
                               QMap<QString, QString> extraArgs)
{
    if (!hasKeys()) {
        return;
    }

    m_oauthRequest->initRequest(KQOAuthRequest::AuthorizedRequest, QUrl(url));
    m_oauthRequest->setConsumerKey(UBUNTU_ONE_CONSUMER_KEY);
    m_oauthRequest->setConsumerSecretKey(UBUNTU_ONE_CONSUMER_SECRET_KEY);
    m_oauthRequest->setToken(m_oauthSettings.value("oauth_token").toString());
    m_oauthRequest->setTokenSecret(m_oauthSettings.value("oauth_token_secret").toString());
    m_oauthRequest->setHttpMethod(KQOAuthRequest::GET);

    // subsonic params
    KQOAuthParameters params;
    params.insert("c", UMUSIC_APPLICATION_NAME);
    params.insert("v", UBUNTU_SUBSONIC_API_VERSION);
    params.insert("f", "json");
    params.insert("u", m_oauthSettings.value("mobile_username").toString());
    params.insert("p", m_oauthSettings.value("mobile_password").toString());

    foreach(QString key, extraArgs.keys()) {
        qDebug() << "parans" << key << extraArgs[key];
        params.insert(key, extraArgs[key]);
    }

    m_oauthRequest->setAdditionalParameters(params);
    m_oauthManager->executeRequest(m_oauthRequest);
}

QUrl UbuntuOAuth::buildUrl(const QString &baseUrl,
                           QMap<QString, QString> extraArgs)
{
    QUrl url(baseUrl);

    url.addQueryItem("c", UMUSIC_APPLICATION_NAME);
    url.addQueryItem("v", UBUNTU_SUBSONIC_API_VERSION);
    url.addQueryItem("f", "json");
    url.addQueryItem("u", m_oauthSettings.value("mobile_username").toString());
    url.addQueryItem("p", m_oauthSettings.value("mobile_password").toString());

    foreach(QString key, extraArgs.keys()) {
        url.addQueryItem(key, extraArgs[key]);
    }

    return url;
}

void UbuntuOAuth::onAuthorizedRequestDone() {
    qDebug() << ">>>>>>>>>> Request sent to Twitter!";
}
