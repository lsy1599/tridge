#include "printerclient.h"

printerClient::printerClient(
    const QString &strHost,
    int nPort
    ) : m_nNextBlockSize(0)
{
  m_pTcpSocket = new QTcpSocket(this);
  host=strHost;
  port=nPort;
  status = false;
  recontmr=new QTimer(this);
  connect(recontmr, SIGNAL(timeout()), this, SLOT(reconnectTimer()));
  //connect();
  //m_pTcpSocket->connectToHost(host, port);
  connect(m_pTcpSocket, SIGNAL(connected()),this, SLOT(slotConnected()));
  connect(m_pTcpSocket, SIGNAL(readyRead()),this, SLOT(slotReadyRead()));
  connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
          this,SLOT(slotError(QAbstractSocket::SocketError)));
  connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

}

printerClient::~printerClient()
{

}

void printerClient::connectToHost()
{
  if(!status)
    m_pTcpSocket->connectToHost(host, port);
}

void printerClient::disconnectToHost()
{
  recontmr->stop();
  m_pTcpSocket->disconnectFromHost();
}

bool printerClient::isConnect()
{
  return status;
}

void printerClient::SendToServer(QString str)
{
  if(status){
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QDateTime::currentDateTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
  }
}

void printerClient::slotReadyRead()
{
  QDataStream in(m_pTcpSocket);
  in.setVersion(QDataStream::Qt_4_2);
  for (;;) {
      if (!m_nNextBlockSize) {
          if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
              break;
          }
          in >> m_nNextBlockSize;
      }

      if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
          break;
      }
      QDateTime   dt;
      QString str;
      in >> dt >> str;
      //qDebug()<<str;
      emit signalSay(dt,str);
      m_nNextBlockSize = 0;
  }
}

void printerClient::slotError(QAbstractSocket::SocketError err)
{
  QString strError =
      "Ошибка:" + (err == QAbstractSocket::HostNotFoundError ?
                   "Хост не может быть найден." :
                   err == QAbstractSocket::RemoteHostClosedError ?
                   "Хост закрыт." :
                   err == QAbstractSocket::ConnectionRefusedError ?
                   "Хост отказал в подключении." :
                   QString(m_pTcpSocket->errorString())
                  );
  status=false;
  emit signalError(strError);
  recontmr->start(5000);
}

void printerClient::slotConnected()
{
  status=true;
  emit signalConnected();
}

void printerClient::slotDisconnected()
{
  status=false;
  emit signalDisconnected();
}

void printerClient::reconnectTimer()
{
  recontmr->stop();
  connectToHost();
}
