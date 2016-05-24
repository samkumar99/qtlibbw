#ifndef BOSSWAVE_H
#define BOSSWAVE_H

#include <QQuickItem>
#include <QTimer>
#include <QJSValueList>
#include <QSharedPointer>

#include "utils.h"
#include "agentconnection.h"
#include "message.h"

/*! \mainpage BOSSWAVE Wavelet Viewer
 *
 * \section intro_sec Introduction
 *
 * This contains the documentation for the BOSSWAVE Wavelet API.
 *
 * The functions are grouped into two modules, those accessible only from C++,
 * and those accessible from both C++ and QML.
 *
 * To view the documentation, click the "Wavelet API overview" tab above.
 */

/**
 * \defgroup cpp C++ API
 * \brief Functions for interacting with BOSSWAVE from C++ code
 *
 * These functions are not accessible from Wavelets, but for programs that
 * are fully standalone (compiled against libqtbw) they can be used to obtain
 * functionality not found in the QML API. All the QML functions can also be
 * invoked from C++.
 */

/**
 * \defgroup qml QML API
 * \brief Functions accessible to Wavelets
 *
 * These functions can be used in Wavelets by importing the BOSSWAVE module:
 * \code{.qml}
 * import BOSSWAVE 1.0
 *
 * Button {
 *   onClicked:
 *   {
 *     BW.publishMsgPack("my/url", "2.0.7.2", {"key":"value"},
 *       function(status) {
 *         console.log("publish status:", status);
 *       });
 *   }
 * }
 * \endcode
 *
 * They can also be used in C++ via normal function invocations.
 */

class BW : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(BW)

public:
    BW(QObject *parent = 0);
    ~BW();

    // This is used by the QML engine to instantiate the bosswave singleton
    static QObject *qmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    /**
     * @brief This can be used to obtain references to the BW object
     * @return The BW2 singleton instance
     *
     * @ingroup cpp
     * @since 1.0
     */
    static BW *instance();

    /**
     * @brief Get the integer PO Number from dot form
     * @param df The dot form, e.g. 2.0.3.1
     * @return The integer PONum
     *
     * @ingroup cpp
     * @since 1.0
     */
    static int fromDF(QString df);

//    //void setAgent(QString &host, quint16 port);
//    void trigLater(function<void(QString s)> cb)
//    {
//        qDebug() << "norm one";
//        auto leak = new QTimer(this);
//        leak->setSingleShot(true);
//        connect(leak, &QTimer::timeout, this, [=]{
//            cb(QString("hello world"));
//        });
//        leak->start(2000);
//    }
//    Q_INVOKABLE void trigLater(QJSValue v)
//    {
//        qDebug() << "jsv one";
//        trigLater(mkcb<QString>(v));
//        /*
//        Handler<QString> f(v);
//        trigLater(f);
//        if(v.isCallable())
//        {
//            trigLater([=](QString s) mutable
//            {
//                QJSValueList l;
//                l.append(QJSValue(s));
//                v.call(l);
//            });
//        }*/
//    }

    // Set the agent to the given host and port. Any existing connection will
    // be torn down. Any existing subscriptions / views will not be recreated
    // and the entity must be set again. agentConnected() will be signalled
    // when this process is complete
    /**
     * @brief Connect to a BOSSWAVE agent
     * @param host The IP address/hostname to connect to
     * @param port The port (generally 28589)
     *
     * Set the agent to the given host and port. Any existing connection will
     * be torn down. Any existing subscriptions / views will not be recreated
     * and the entity must be set again. agentConnected() will be signalled
     * when this process is complete
     *
     * @ingroup cpp
     * @since 1.0
     */
    void connectAgent(QString host, quint16 port);

    /**
     * @brief Set the entity
     * @param filename a BW entity file to use
     *
     * @ingroup cpp
     * @since 1.0
     */
    void setEntityFile(QString filename, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Set the entity
     * @param contents the binary contents of an entity descriptor
     *
     * Note that if read from an entity file, the first byte of the file
     * must be stripped as it is an RO type indicator.
     *
     * @see setEntityFile
     * @ingroup cpp
     * @since 1.0
     */
    void setEntity(QByteArray &contents, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Set the entity by reading the file denoted by $BW2_DEFAULT_ENTITY
     *
     * @see setEntityFile
     * @ingroup cpp
     * @since 1.0
     */
    void setEntityFromEnviron(Res<QString> on_done = _nop_res_status);

    /**
     * @brief publish a bosswave message
     * @param uri the URI to publish to
     * @param poz the payload objects to publish
     * @param on_done a function to call when the operation is complete.
     *
     * @ingroup cpp
     * @since 1.0
     */
    void publish(QString uri, QList<PayloadObject*> poz, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Publish a MsgPack object to the given URI
     *
     * @ingroup qml
     * @since 1.0
     */
    Q_INVOKABLE void publishMsgPack(QString uri, QString PODF, QVariantMap msg, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Publish a MsgPack object to the given URI
     *
     * @ingroup qml
     * @since 1.0
     */
    Q_INVOKABLE void publishMsgPack(QString uri, int PONum, QVariantMap msg, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Publish a MsgPack object to the given URI, with a javascript callback
     *
     * @ingroup qml
     * @since 1.0
     */
    Q_INVOKABLE void publishMsgPack(QString uri, QString PODF, QVariantMap msg, QJSValue on_done);

    /**
     * @brief Publish a MsgPack object to the given URI, with a javascript callback
     *
     * @ingroup qml
     * @since 1.0
     */
    Q_INVOKABLE void publishMsgPack(QString uri, int PONum, QVariantMap msg, QJSValue on_done);

    /**
     * @brief Query the given URI pattern and return all messages that match
     * @param uri the URI to query
     * @param on_done A callback receiving the status list of messages
     *
     * @ingroup cpp
     * @since 1.0
     */
    void query(QString uri, Res<QString, QList<PMessage>> on_done);

    /**
     * @brief Subscribe to the given URI
     * @param uri The URI to subscribe to
     * @param on_msg A callback for each received message
     * @param on_done The callback to be executed with the error message. Empty implies success
     *
     * @ingroup cpp
     * @since 1.1
     */
    void subscribe(QString uri, Res<PMessage> on_msg, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Subscribe to a MsgPack resource
     * @param uri The resource to subscribe to
     * @param on_msg The callback to be called for each message
     * @param on_done The callback to be executed with the error message. Empty implies success.
     *
     * This will unpack msgpack PO's and pass them to the on_msg callback
     *
     * @ingroup qml
     * @since 1.1
     */
    Q_INVOKABLE void subscribeMsgPack(QString uri, Res<QVariantMap> on_msg, Res<QString> on_done = _nop_res_status);

    /**
     * @brief Subscribe to a MsgPack resource
     * @param uri The resource to subscribe to
     * @param on_msg The callback to be called for each message (javascript function)
     *
     * @ingroup qml
     * @since 1.1
     */
    Q_INVOKABLE void subscribeMsgPack(QString uri, QJSValue on_msg);

    /**
     * @brief Subscribe to a MsgPack resource
     * @param uri The resource to subscribe to
     * @param on_msg The callback to be called for each message (javascript function)
     * @param on_done The callback to be executed with the error message. "" implies success.
     *
     * @ingroup qml
     * @since 1.1
     */
    Q_INVOKABLE void subscribeMsgPack(QString uri, QJSValue on_msg, QJSValue on_done);

signals:
    /**
     * @brief Fired when the BOSSWAVE agent connection changes (connect or disconnect)
     * @param success True if ok, false otherwise
     * @param msg Empty if ok, error message otherwise
     */
    void agentChanged(bool success, QString msg);

private:
    QQmlEngine *engine;
    QJSEngine *jsengine;
    AgentConnection *agent();
    AgentConnection *m_agent;

    template <typename ...Tz> Res<Tz...> ERes(QJSValue callback)
    {
        return Res<Tz...>(engine, callback);
    }

    static Res<QString> _nop_res_status;
};

#endif // BOSSWAVE_H


