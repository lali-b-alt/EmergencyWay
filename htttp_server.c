#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.c"
#pragma comment(lib, "ws2_32.lib")

//משתנה לחסימה -לא תמיד מעודכן
//location Roadblock;
//Roadblock=0;
// פונקציה שמקבלת יעד נסיעה ומדפיסה אותו למסך ומחזירה 1 אם לא ריק, 0 אם ריק
int evasion(char* end) {
    printf("evasion called! destination: %s\n", end);
    //קריא לפונקצית הדיאקסטרה- כל הקטע של החיבור לשרת ושליחת המסלול
            //מציאת יעד הנסיעה- שליחה לפונקציה שמקבלת יעד במילים ומחזירה את הקוארדינטיות שלו
            // location destination;
            // destination = Convert_travel_destination(end);
            // if(destination.Latitude==0 && destination.Longitude==0){
            //     return 0;
            // }
            // printf("%s,%f,%f\n","destination is:" ,destination.Latitude   ,destination.Longitude);
            //חיבור לקלט מחיישן הGPS כדי לקבל את מיקום היציאה
            // location start;
            // start=Convert_gps_data();
            
        //שליחת הנתונים לשרת המפות שיחשב את המסלול וקבלת המסלול בחזרה
        location_net start ,eend,roadblock;
        start.Longitude=790;
        start.Latitude=200;
        eend.Longitude=320;
        eend.Latitude=1300;
        roadblock.Longitude=0;
        roadblock.Latitude=0;
        //שליחה לשרת המפות
        clientt(start,eend,roadblock);
            if(route!=NULL){
           return 1; // TRUE
                }
             else {
            //אם לא מצא יעד נסיעה
            return 0; // FALSE
            }
}

// פונקציה שמפעילה את ההתחמקות (מדפיסה הודעה קבועה ומקבלת ערך)
void Travel_destination_in_words(const char* start) {
    printf("Travel_destination_in_words called! start=%s\n", start);
    if (strcmp(start, "true") == 0) {
        printf("התחלת נסיעה!\n");
    } else {
        printf("לא התחלת נסיעה.\n");
    }
}

int main() {
    //בנית שרת HHTTP
    WSADATA wsa;
    SOCKET server, client;
    struct sockaddr_in server_addr, client_addr;
    int c, recv_size;
    char buffer[2048];

    WSAStartup(MAKEWORD(2,2), &wsa);
    server = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9090); // שנה לפורט שתרצה

    bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server, 3);

    printf("HTTP Server started on port 9090\n");

    c = sizeof(struct sockaddr_in);
    while ((client = accept(server, (struct sockaddr *)&client_addr, &c)) != INVALID_SOCKET) {
        recv_size = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (recv_size > 0) {
            buffer[recv_size] = '\0';

            // בדוק אם זו בקשת POST ל /evasion
            if (strncmp(buffer, "POST /evasion", 13) == 0) {
                char* body = strstr(buffer, "\r\n\r\n");
                int result = 0;
                if (body) {
                    body += 4;
                    result = evasion(body); // קריאה לפונקציה שלך שמחזירה TRUE/FALSE
                }
                char response[256];
                sprintf(response,
                    "HTTP/1.1 200 OK\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Type: text/plain\r\n\r\n%s",
                    result ? "true" : "false");
                send(client, response, strlen(response), 0);
            }
            // בדוק אם זו בקשת POST ל /travel
            else if (strncmp(buffer, "POST /travel", 12) == 0) {
                char* body = strstr(buffer, "\r\n\r\n");
                if (body) {
                    body += 4;
                    Travel_destination_in_words(body); // שלח את הערך שהגיע מהאנגיולר
                }
                char response[] =
                    "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\nTravel called!";
                send(client, response, strlen(response), 0);
            }
            // בקשת OPTIONS (כדי לאפשר CORS)
            else if (strncmp(buffer, "OPTIONS", 7) == 0) {
                char response[] =
                    "HTTP/1.1 204 No Content\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                    "Access-Control-Allow-Headers: Content-Type\r\n\r\n";
                send(client, response, strlen(response), 0);
            }
            else {
                char response[] =
                    "HTTP/1.1 404 Not Found\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\nNot found";
                send(client, response, strlen(response), 0);
            }
        }
        closesocket(client);
    }

    //הפעלת פונקציה שקוראת ערכים מהserial
    //setupSerial_9600();

    closesocket(server);
    WSACleanup();
    return 0;
}