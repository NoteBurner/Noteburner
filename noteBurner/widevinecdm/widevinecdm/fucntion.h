#pragma once
#include <Windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

void Log(const char* fmt, ...);
void writeToFile(const wchar_t* buf);

int Write( LPTSTR lpPath, LPSTR lpText);
std::wstring DecodeUtf8(string in);
template<typename T>
T hookCodePatch(T originFun, T hookFuntion)
{
    const int patchSize = 10; // ������Ҫ patch ���ֽ���
    const int pageSize = 5;
    unsigned char* patchedMemory = new unsigned char[patchSize];
    unsigned char* originFunPtr = reinterpret_cast<unsigned char*>(originFun);

    // ���б�Ҫ�� ��ԭ����ת��ʵ�ʵĺ�����ַ
    DWORD oldProtect;
    VirtualProtect(patchedMemory, patchSize, PAGE_EXECUTE_READWRITE, &oldProtect);

    if (originFunPtr[0] == 0xFF && originFunPtr[1] == 0x25) {
        originFunPtr = reinterpret_cast<unsigned char*>(**(int**)(originFunPtr + 2)) ;
    }
    if (originFunPtr[0] == 0xEB) {
        originFunPtr = originFunPtr + originFunPtr[1] + 2;
        if (originFunPtr[0] == 0xFF && originFunPtr[1] == 0x25) {
            originFunPtr = reinterpret_cast<unsigned char*>(*(int*)(originFunPtr + 2));
        }
        if (originFunPtr[0] == 0xE9) {
            originFunPtr = originFunPtr + *(int*)(originFunPtr + 1) + 5;
        }
    }
    if (originFunPtr[0] == 0xE9) {
        originFunPtr = originFunPtr + *(int*)(originFunPtr + 1) + 5;
    }
    // ���� originFun ��ǰ����ֽڵ� patchedMemory
    memcpy(patchedMemory, originFunPtr, 5);

    intptr_t hookOffset = (reinterpret_cast<intptr_t>(originFunPtr) + 5) - reinterpret_cast<intptr_t>(patchedMemory + 0xA);

    // �޸� patchedMemory �ĺ�����ֽ�Ϊ��ת�� originFun + 5 ��ָ��
    patchedMemory[5] = 0xE9; // x86 ��תָ��Ĳ�����
    memcpy(patchedMemory + 6, &hookOffset, sizeof(hookOffset));
    // ������תƫ����
    hookOffset = reinterpret_cast<intptr_t>(hookFuntion) - (reinterpret_cast<intptr_t>(originFunPtr) + 5);


    // �޸� originFun ��ǰ����ֽ�Ϊ��ת�� hookFunction ��ָ��

    VirtualProtect(originFunPtr, pageSize, PAGE_EXECUTE_READWRITE, &oldProtect);
    originFunPtr[0] = 0xE9; // x86 ��תָ��Ĳ�����
    memcpy(originFunPtr + 1, &hookOffset, sizeof(hookOffset));
    VirtualProtect(originFunPtr, pageSize, oldProtect, &oldProtect);
    return (T)patchedMemory;
}