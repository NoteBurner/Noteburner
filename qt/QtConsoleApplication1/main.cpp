#include "m.h"
int main(int argc, char* argv[]) {
    A ma;    B mb;
    QObject::connect(&ma, &A::s3, &mb, &B::x);  //�����źźͲۣ��������
    ma.g();   //���ö���mb�ĳ�Ա����x���X���ɼ�����ma��mb֮��ʵ����ͨ�š�
    return 0;
}