#include <QMainWindow>
#include <QObject>
// ����̳�QObject����ʹ���źźͲ�
class TsignalApp :public QMainWindow
{
    Q_OBJECT
public:
    
    TsignalApp();
    void slotFileNew();
    
// �ź�������
 signals:
    // �����ź� mySignal()
    void mySignal();
    // �����ź� mySignal(int)
    void mySignal(int x);
    // �����ź� mySignalParam(int,int)
    void mySignalParam(int x, int y);
    // ��������
public slots:
    // �����ۺ��� mySlot()
    void mySlot();
    // �����ۺ��� mySlot(int)
    void mySlot(int x);
    // �����ۺ��� mySignalParam (int��int)
    void mySlotParam(int x, int y);
};
