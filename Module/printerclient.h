#ifndef PRINTERCLIENT_H
#define PRINTERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDateTime>
#include <QDataStream>
#include <QTimer>
class printerClient : public QObject
{
Q_OBJECT
public:
  printerClient(const QString& strHost, int nPort) ;
  ~printerClient();
  //printerClient();
  void connectToHost();
  void disconnectToHost();
  bool isConnect();
  quint16 m_nNextBlockSize;
  void SendToServer(QString str);
private:
  QTcpSocket* m_pTcpSocket;
  QString host;
  int port;
  bool status;
  QTimer* recontmr;
signals:
    void signalConnected();
    void signalDisconnected();
    void signalError(QString str);
    void signalSay(QDateTime dt,QString str);
public slots:
private slots:
    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   ();
    void slotDisconnected();
    void reconnectTimer();
};

#endif // PRINTERCLIENT_H
