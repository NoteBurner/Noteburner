
#ifndef M_H   
#define M_H
#include<QObject>
#include <iostream>
using namespace std;

class A :public QObject {   //�źźͲ۱���̳���QObject��
    Q_OBJECT             //������Ӹú�
    //public signals:void s1(int);  //����signalsǰ�����з��ʿ��Ʒ���
signals:void s();          //ʹ��signals�ؼ��������źţ��źŵ��﷨������������ͬ��
signals:void s(int, int);   //��ȷ���źſ����в�����Ҳ�������ء�
//void s2(){}          //�����ź�ֻ�����������ܶ��塣
       void s3();           //ע�⣺������������һ���ź�
public:                //�ź���������������ʹ�÷��ʿ��Ʒ�����ʾ�����������ǳ�Ա������
    void g() {
        emit s3();   /*�����źţ����﷨�������ͨ������ͬ�����ź���۹���֮ǰ��������źŲ��������Ӧ�Ĳۺ�����*/
        // emit: s3();  //����emit������ð�š�
    }
};

class B :public QObject {
    Q_OBJECT
public slots:                 //ʹ��slots�ؼ���������
    void x() { cout << "X" << endl; }  /*��ȷ���۾���һ����ͨ������ֻ����Ҫʹ��slots�ؼ��֣����ܺ��ź��������*/
            //slots: void x(){}   //����������ʱ��Ҫָ�����ʿ��Ʒ���
public:
    void g() { // emit s3();  //��������B�ж��ڱ�ʶ��s3�ǲ��ɼ���
    }
};

#endif // M_H