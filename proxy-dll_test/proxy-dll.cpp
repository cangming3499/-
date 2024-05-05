#include <windows.h>
#include <stdio.h>

typedef NTSTATUS(NTAPI* TPALLOCWORK)(PTP_WORK* ptpWrk, PTP_WORK_CALLBACK pfnwkCallback, PVOID OptionalArg, PTP_CALLBACK_ENVIRON CallbackEnvironment);
typedef VOID(NTAPI* TPPOSTWORK)(PTP_WORK);
typedef VOID(NTAPI* TPRELEASEWORK)(PTP_WORK);

FARPROC pLoadLibraryA;

EXTERN_C UINT_PTR getLoadLibraryA() {
    return (UINT_PTR)pLoadLibraryA;
}

EXTERN_C VOID CALLBACK WorkCallbackA(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work);

//VOID CALLBACK WorkCallback(
//    _Inout_     PTP_CALLBACK_INSTANCE Instance,
//    _Inout_opt_ PVOID                 Context,
//    _Inout_     PTP_WORK              Work
//) {
//    LoadLibraryA((LPCSTR)Context);
//}


int main() {
    pLoadLibraryA = GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA");

    //�̳߳ع������

    FARPROC pTpAllocWork = GetProcAddress(GetModuleHandleA("ntdll"), "TpAllocWork");      //    TpAllocWork : ����һ���̳߳ع����
    FARPROC pTpPostWork = GetProcAddress(GetModuleHandleA("ntdll"), "TpPostWork");       //    TpPostWork : �ύ�̳߳ع����ʹ���Ŷӵȴ�ִ�С�
    FARPROC pTpReleaseWork = GetProcAddress(GetModuleHandleA("ntdll"), "TpReleaseWork"); //    TpReleaseWork : �ͷ��̳߳ع��������Դ��

    CHAR* libName = (char*)"wininet.dll";
    PTP_WORK WorkReturn = NULL;


    // ����һ���̳߳�����ִ�лص�����   WorkCallback���ص������Ĳ��� ��libName��������������  WorkReturn
    ((TPALLOCWORK)pTpAllocWork)(&WorkReturn, (PTP_WORK_CALLBACK)WorkCallbackA, libName, NULL);
    ((TPPOSTWORK)pTpPostWork)(WorkReturn);  // �ύ����
    ((TPRELEASEWORK)pTpReleaseWork)(WorkReturn);//�ͷ� ����

    WaitForSingleObject((HANDLE)-1, 0x1000);
    printf("hWininet: %p\n", GetModuleHandleA(libName));   // ���dll�Ƿ���������

    system("pause");
    return 0;
}