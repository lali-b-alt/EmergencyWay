#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#include "semafor.c"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024



void create_png_from_dot(const char* , const char* );



//פונקציה לפתיחת שיחה עם לקוח
void __cdecl handle_client(void *client_socket);

int main() {

    node_ptr head_graph=(node_ptr)malloc(sizeof(node_ptr));
    //בנית הטבלאות
    Database_construction();

    // הכנסת נתונים ידנית לטבלאות
    //טבלת צמתים
    Inserting_data_into_odes(1,790.0,200.0,true,false);
    Inserting_data_into_odes(2,950,550,false,false);
    Inserting_data_into_odes(5,220,200,false,false);
    Inserting_data_into_odes(4,690,700,false,false);
    Inserting_data_into_odes(3,990,950,false,false);
    Inserting_data_into_odes(9,610,1200,true,false);
    Inserting_data_into_odes(7,350,850,false,false);
    Inserting_data_into_odes(8,320,1300,false,false);
    Inserting_data_into_odes(15,250,1550,false,false);
    Inserting_data_into_odes(10,520,1550,true,false);
    Inserting_data_into_odes(11,650,1900,false,false);
    Inserting_data_into_odes(12,820,1600,true,false);
    Inserting_data_into_odes(13,1000,1650,false,false);
    Inserting_data_into_odes(14,850,1400,false,false);
    Inserting_data_into_odes(6,100,650,false,false);

    //טבלת כבישים
    Entering_data_into_a_road_table(1,1,1,2,890,220,1,2,0,10,1,0,false);
    Entering_data_into_a_road_table(40,2,2,0,950,450,1,3,1,20,5,0,false);
    Entering_data_into_a_road_table(2,2,0,3,960,800,2,2,0,30,0,0,false);
    Entering_data_into_a_road_table(3,3,1,0,700,350,1,2,0,7,7,0,false);
    Entering_data_into_a_road_table(4,3,0,0,690,550,2,2,0,5,2,0,false);
    Entering_data_into_a_road_table(5,3,0,4,690,600,3,2,1,10,0,0,false);
    Entering_data_into_a_road_table(12,4,1,0,690,200,1,3,22,0,0,0,false);
    Entering_data_into_a_road_table(13,4,0,0,520,190,2,3,2,30,12,0,false);
    Entering_data_into_a_road_table(14,4,0,5,350,150,3,3,2,35,10,0,false);
    Entering_data_into_a_road_table(4,5,5,0,250,200,1,3,2,35,10,0,false);
    Entering_data_into_a_road_table(42,5,0,0,500,200,2,3,2,30,12,0,false);
    Entering_data_into_a_road_table(41,5,0,1,700,200,3,3,2,20,0,0,false);
    Entering_data_into_a_road_table(45,6,3,0,1000,900,1,1,0,5,2,0,false);
    Entering_data_into_a_road_table(44,6,0,2,950,720,2,1,0,5,2,0,false);
    Entering_data_into_a_road_table(46,7,6,7,200,800,1,1,1,2,0,0,false);
    Entering_data_into_a_road_table(15,19,7,6,210,850,1,1,1,2,0,0,false);
    Entering_data_into_a_road_table(21,8,4,0,650,820,1,3,2,0,0,0,false);
    Entering_data_into_a_road_table(20,8,0,9,590,1000,2,3,1,0,0,0,false);
    Entering_data_into_a_road_table(22,9,3,0,750,910,1,1,0,20,30,0,false);
    Entering_data_into_a_road_table(23,9,0,0,690,1100,2,1,1,20,30,0,false);
    Entering_data_into_a_road_table(24,9,0,9,620,1150,3,2,0,20,30,0,false);
    Entering_data_into_a_road_table(49,10,9,0,620,1150,1,1,0,20,30,0,false);
    Entering_data_into_a_road_table(48,10,0,0,690,1100,2,0,0,20,30,0,false);
    Entering_data_into_a_road_table(47,10,0,3,890,1050,3,1,0,20,25,0,false);
    Entering_data_into_a_road_table(50,12,9,0,600,1250,1,2,1,4,1,0,false);
    Entering_data_into_a_road_table(51,12,0,8,350,1250,2,2,0,4,1,0,false);
    Entering_data_into_a_road_table(18,11,8,0,350,1250,1,2,1,4,1,0,false);
    Entering_data_into_a_road_table(19,11,0,9,500,1250,2,1,1,4,1,0,false);
    Entering_data_into_a_road_table(17,13,8,0,300,1100,1,2,0,5,0,0,false);
    Entering_data_into_a_road_table(16,13,0,7,300,1000,2,2,1,5,0,0,false);
    Entering_data_into_a_road_table(52,14,7,0,290,1050,1,1,1,15,10,0,false);
    Entering_data_into_a_road_table(53,14,0,8,290,1100,2,1,0,15,10,0,false);
    Entering_data_into_a_road_table(6,16,1,0,650,400,1,3,1,30,10,0,false);
    Entering_data_into_a_road_table(7,16,0,0,550,450,2,3,0,30,10,0,false);
    Entering_data_into_a_road_table(8,16,0,0,400,550,3,3,2,20,10,0,false);
    Entering_data_into_a_road_table(9,16,0,0,350,500,4,4,1,20,5,0,false);
    Entering_data_into_a_road_table(10,16,0,0,290,600,5,4,2,10,7,0,false);
    Entering_data_into_a_road_table(11,16,0,6,150,590,6,4,1,40,5,0,false);
    Entering_data_into_a_road_table(59,15,6,0,200,600,1,3,2,40,10,0,false);
    Entering_data_into_a_road_table(58,15,0,0,250,580,2,3,1,40,10,0,false);
    Entering_data_into_a_road_table(57,15,0,0,350,520,3,3,2,20,3,0,false);
    Entering_data_into_a_road_table(56,15,0,0,420,490,4,2,1,30,6,0,false);
    Entering_data_into_a_road_table(55,15,0,0,550,410,5,2,1,30,7,0,false);
    Entering_data_into_a_road_table(54,15,0,1,610,390,6,2,1,40,10,0,false);
    Entering_data_into_a_road_table(60,19,2,1,900,300,1,4,2,10,4,0,false);
    // מחירים
    building_price_table();
    
    fclose(f_nodes);
    fclose(f_roads);
////////////////////////////בנית הגרף
    //פתיחת הקבצים לקריאה
    open_files(); 
    //פתיחת קובץ שייצג את הגרף
    start_dot_file();  
//מעבר לצומת הראשונה בקובץ
    char line[1024];
    // קריאה ראשונה - דילוג על כותרת
    if (fgets(line, sizeof(line), f_nodes) == NULL) {
        printf("%s\n", "Error reading from roads file\n");
        exit(EXIT_FAILURE);
    }
    // קריאה שנייה - קריאת שורת הנתונים הראשונה
    if (fgets(line, sizeof(line), f_nodes) == NULL) {
        printf("%s\n", "Error reading from roads file\n");
        exit(EXIT_FAILURE);
    }
        //בנית הצומת הראשונה
        node_ptr new = building_node(line);
        head_graph = new;
        //בנית הגרף כולו
        build_all(new);

        // סגירת הקבצים
        fclose(f_roads);
        fclose(f_price); 
        //סיום בנית קובץ גרף
        FILE* dot = fopen("graph.dot", "a");
        fprintf(dot, "}\n");
        fclose(dot);
        printf("Finished reading files, starting server...\n");
        for(int i=0;i<all_nodes_count;i++){
        printf("%s,%lld\n","the node jhghjgh",all_nodes[i]->id);
}
    //שליחה לדיאקסטרה לא דרך הלקוח
    //רק דיבוג
    location_net buffer[3];
    buffer[0].Longitude=790;
    buffer[0].Latitude=200;
    buffer[1].Longitude=320;
    buffer[1].Latitude=1300;
 
 
   if(buffer[0].Latitude!=0 && buffer[0].Longitude!=0&& buffer[1].Longitude!=0 && buffer[1].Longitude!=0){
    create_png_from_dot("graph.dot", "dijkstra_pathh.png");
    system("start \"\" \"dijkstra_pathh.png\"");    //קיריאה לדיאקסטרה
    for(int i=0;i<all_nodes_count;i++){
        printf("%s,%lld\n","all nodes:",all_nodes[i]->id);
    }
    location* daya= dayaqstra(buffer[0],buffer[1]);     
  }
   
//////////////////////////בנית השרת 
    WSADATA wsaData;
    SOCKET serverSocket, newSocket;
    struct sockaddr_in serverAddr, newAddr;
    int addrLen = sizeof(newAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        newSocket = accept(serverSocket, (struct sockaddr *)&newAddr, &addrLen);
        if (newSocket == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            continue;
        }

        SOCKET *client_sock = malloc(sizeof(SOCKET));
        *client_sock = newSocket;
        _beginthread(handle_client, 0, (void*)client_sock);

        
    }

    closesocket(serverSocket);
    WSACleanup();
}

void __cdecl handle_client(void *client_socket) {
    SOCKET sock = *(SOCKET*)client_socket;
    free(client_socket); // משחררים מיד, לא בסוף, כדי למנוע דליפת זיכרון
    location_net buffer[3];
    int recvSize = 0;

    // קבלת הנתונים מהלקוח
    recvSize = recv(sock, (char*)buffer, sizeof(buffer), 0);

    //  הדפסת כל מה שהתקבל מהלקוח
    printf("DEBUG: Server received %d bytes from client\n", recvSize);
    for (int i = 0; i < 3; i++) {
        printf("  buffer[%d]: Latitude = %f, Longitude = %f\n", i, buffer[i].Latitude, buffer[i].Longitude);
    }

    if (recvSize != sizeof(buffer)) {
        printf("Client disconnected or error occurred (recvSize=%d).\n", recvSize);
        closesocket(sock);
        _endthread();
        return;
    }
   
   if(buffer[0].Longitude!=0 && buffer[0].Latitude!=0&& buffer[1].Longitude!=0 && buffer[1].Longitude!=0){
    //קיריאה לדיאקסטרה
   location* daya= dayaqstra(buffer[0],buffer[1]); // תיקון: buffer[1] במקום buffer[0]
    create_png_from_dot("graph.dot", "dijkstra_pathh.png");
    
    for(int i=0;i<all_nodes_count;i++){
        printf("%s,%lld\n","all nodes:",all_nodes[i]->id);
    }
   
}
    // // טיפול בשגיאות קלט
//     // else if(buffer[0].Latitude ==0 || buffer[0].Longitude ==0 || buffer[1].Latitude ==0 || buffer[1].Longitude ==0 || buffer[2].Latitude ==0 || buffer[2].Longitude ==0){
//     //     int error_code = 0;
//     //     char buff[]="this is a eror in the address";
//     //     send(sock, (char*)&error_code, sizeof(int), 0);
//     //     send(sock, buff, strlen(buff) + 1, 0);
//     // }
//     // // טיפול בחסימה (לא ממומש)
//     // else if( buffer[2].Latitude !=0 && buffer[2].Latitude !=0 ){
//     //     //שליחה לפונקציה שמעדכנת חסימה
//     //     // send(sock, (char*)accessResource(buffer[2]),sizeof(buffer[2]), 0);
//     // }
//    else{
//     printf("%s\n","eror in data to dayaqstra");
//    }
   closesocket(sock);
       _endthread();
  //}
}

//פונקציה שהופכת קובץ גרף לתמונה
void create_png_from_dot(const char* dot_filename, const char* png_filename) {
    char command[512];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", dot_filename, png_filename);
    system(command);
}