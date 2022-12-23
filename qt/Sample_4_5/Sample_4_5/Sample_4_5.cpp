#include "Sample_4_5.h"
#include <qDebug>
Sample_4_5::Sample_4_5(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
}

void Sample_4_5::on_btnGetTime_clicked()
{
    QDateTime curDataTime = QDateTime::currentDateTime();
    ui.timeEditCurrent->setDateTime(curDataTime);
    ui.lineEditTime->setText(curDataTime.toString("HH:mm:ss"));
    ui.dateEdit->setDate(curDataTime.date());
    ui.lineEditDate->setText(curDataTime.toString("yyyy-MM-dd"));
    ui.dateTimeEdit->setDateTime(curDataTime);
    ui.lineEditDateTime->setText(curDataTime.toString("yyyy-MM-dd HH:mm:ss"));
    
}

void Sample_4_5::on_btnSetTime_clicked()
{
    //��ȡ�༭����ı�
    QString strTime = ui.lineEditTime->text();
    //���ı�ת��Ϊʱ��
    QTime time = QTime::fromString(strTime, "HH:mm:ss");
    //��ʱ�����õ�ʱ��༭��
    ui.timeEditCurrent->setTime(time);
}

void Sample_4_5::on_btnSetDate_clicked()
{
    //��ȡ�༭����ı�
    QString strTime = ui.lineEditDate->text();
    //���ı�ת��Ϊʱ��
    QTime time = QTime::fromString(strTime, QString::fromLocal8Bit("yyyy-MM-dd"));
    //��ʱ�����õ�ʱ��༭��
    ui.dateEdit->setTime(time);
}

void Sample_4_5::on_btnSetDateTime_clicked()
{
    //��ȡ�༭����ı�
    QString strTime = ui.lineEditDateTime->text();
    //���ı�ת��Ϊʱ��
    QTime time = QTime::fromString(strTime, "yyyy-MM-dd HH:mm:ss");
    //��ʱ�����õ�ʱ��༭��
    ui.dateTimeEdit->setTime(time);
}
