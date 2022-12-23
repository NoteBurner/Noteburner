#include "Sample_4_5.h"
#include <qDebug>
Sample_4_5::Sample_4_5(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    timer.setInterval(1000);
    timer.stop();
    connect(&timer, &QTimer::timeout, this, &Sample_4_5::displayTime);
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
    QDate time = QDate::fromString(strTime, QString::fromLocal8Bit("yyyy-MM-dd"));
    //��ʱ�����õ�ʱ��༭��
    ui.dateEdit->setDate(time);
}

void Sample_4_5::on_btnSetDateTime_clicked()
{
    //��ȡ�༭����ı�
    QString strTime = ui.lineEditDateTime->text();
    //���ı�ת��Ϊʱ��
    QDateTime time = QDateTime::fromString(strTime, "yyyy-MM-dd HH:mm:ss");
    //��ʱ�����õ�ʱ��༭��
    ui.dateTimeEdit->setDateTime(time);
}

void Sample_4_5::on_calendarWidget_selectionChanged()
{
    //��ȡ������ѡ�е�ʱ��
    QDate date = ui.calendarWidget->selectedDate();
    QString dataString = date.toString(QString::fromLocal8Bit("yyyy��MM��dd��"));
    ui.lineEditSelectedDate->setText(dataString);
    
}

void Sample_4_5::on_btnSetInternal_clicked()
{
    // ��spinInternal��ȡʱ����
    int value = ui.spinInternal->value();
    //��QTime����ʱ����
    timer.setInterval(value);
}

void Sample_4_5::on_btnStart_clicked()
{
    ui.btnStart->setEnabled(false);
    ui.btnSetInternal->setEnabled(false);
    ui.btnStop->setEnabled(true);

    //������ʱ��
    timer.start();

    timeCount.start();
}

void Sample_4_5::on_btnStop_clicked()
{
    ui.btnStart->setEnabled(true);
    ui.btnSetInternal->setEnabled(true);
    ui.btnStop->setEnabled(false);
    timer.stop();
    int elapsed = timeCount.elapsed();
    int hour = elapsed / 3600000;
    elapsed -= 3600000 * hour;
    int minute = elapsed / 60000;
    elapsed -= minute * 60000;
    int second = elapsed/1000;

    ui.labelEcilpse->setText(QString::asprintf(QString::fromLocal8Bit("����ʱ��: %dʱ%d��%d��").toUtf8(), hour, minute, second));
}

void Sample_4_5::displayTime()
{
    //��ȡ��ǰʱ���ʱ����
    int hour = QTime::currentTime().hour();
    int minute = QTime::currentTime().minute();
    int second = QTime::currentTime().second();
    // ����ʱ�����LCDNumber
    ui.lcdHour->display(hour);
    ui.lcdMinute->display(minute);
    ui.lcdSecond->display(second);

}

