#include "tsignal.h"
#include <QMessageBox>
TsignalApp::TsignalApp()
{
    // ���ź� mySignal() ��� mySlot() �����
    connect(this, SIGNAL(mySignal()), SLOT(mySlot()));
    // ���ź� mySignal(int) ��� mySlot(int) �����
    connect(this, SIGNAL(mySignal(int)), SLOT(mySlot(int)));
    // ���ź� mySignalParam(int,int) ��� mySlotParam(int,int) �����
    connect(this, SIGNAL(mySignalParam(int, int)), SLOT(mySlotParam(int, int)));
    
}
// ����ۺ��� mySlot()
void TsignalApp::mySlot()
{
    QMessageBox::about(this, "Tsignal", "This is a signal/slot sample withoutparameter.");
}
// ����ۺ��� mySlot(int)
void TsignalApp::mySlot(int x)
{
    QMessageBox::about(this, "Tsignal", "This is a signal/slot sample with oneparameter.");
}
// ����ۺ��� mySlotParam(int,int)
void TsignalApp::mySlotParam(int x, int y)
{
    char s[256];
    sprintf(s, "x:%d y:%d", x, y);
    QMessageBox::about(this, "Tsignal", s);
}
void TsignalApp::slotFileNew()
{
    // �����ź� mySignal()
    emit mySignal();
    // �����ź� mySignal(int)
    emit mySignal(5);
    // �����ź� mySignalParam(5��100)
    emit mySignalParam(5, 100);
}