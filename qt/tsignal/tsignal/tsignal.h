#pragma once

#include <QMainWindow>
#include <QObject>
// ����̳�QObject����ʹ���źźͲ�
class TsignalApp :public QMainWindow
{
public:
    TsignalApp();
    void slotFileNew();
    Q_OBJECT
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
   // TsignalApp* mySlot2();
};