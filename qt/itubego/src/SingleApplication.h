#include <QApplication>
class SingleApplicationPrivate;
class SingleApplication:public QApplication {
	Q_OBJECT
public :
	SingleApplication(int argc,char ** argv,int arg_8=1,int arg_c=4,int arg_10=1000);
	
signals:
	void instanceStarted();
	void receivedMessage(unsigned int instanceId, QByteArray message);
private:
	SingleApplicationPrivate *m_d; //8
};