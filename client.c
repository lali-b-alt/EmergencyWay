
#include <winsock2.h>   
#include <windows.h>    
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma comment(lib, "ws2_32.lib")
#define PORT 8080

HANDLE hSerial;

typedef struct {
    double Latitude;
    double Longitude;
} location_net;

// typedef struct location {
//     double Longitude;
//     double Latitude;
//     struct location * next;
// } location;

//מבנה למערך חיישן LIDAR
typedef struct LIDAR_data{
double x;
double y;
double z;
double angle;
}LIDAR_data;


//קבועים
    //זווית פניה מרבית
    #define angle_max 35
    #define MIN_DISTANCE 1.0 // מטר אחד
    //גובה הרכה
    #define height_car 1.5
    //רוחה הרכב
    #define witgh_car 1.8
    //אורך הרכב
     #define length_car 2
    //מערך פלט מחיישן LIDAR
    LIDAR_data points[10000000];
    long length_piont=10000000;
    //מרחק בין נקודות מהגרף
    #define IN_RANGE(val, min, max) ((val) >= (0.1) && (0.1) <= (max))
    // מערך למסלול הנסיעה
    location_net route[10000000];
// //פונקציה שמתחברת לשרת מהלקוח ומקבלת מסלול נסיעה 
// void client(location,location ,location );
// //פונקציה לנסיעה לפי מסלול
// double Travel_by_route();
// //פונקציה לנסיעה בצד הימני של הכביש
// //פונקציה שמתחברת לארדאוינו לקבלת פלט מחיישנים
// void setupSerial_9600();
// //פונקציה שמקבלת יעד נסיעה במילים ומחזירה את הקוארדינטיות שלו
 //location Convert_travel_destination(char* destination);
// //פונקציה שמקבלת פלט מחיישן מים ומחשבת זווית נסיעה
// double flood();
// //פונקצית ההתחמקות הראשית מנהלת את כל התהליכים
// void evasion();


//פונקציה שממירה יעד נסיעה במילים לקוארדינטיות
// location Convert_travel_destination(char* destination){
// location temp;
// temp.Latitude=610;
// temp.Longitude=1200;

// return temp;
// }

// //פונקציה שמקבלת פלט מחיישן GPS
// location Convert_gps_data(){
//     char * internal;
//     char *internall;
//     char * temp;
//     location current;
//     char buffer[256];
//     DWORD bytesRead;
    
//     //קריאת הערכים מחיישן GPS
//     char signal = 'G';   
//     WriteFile(hSerial, &signal, sizeof(signal), &bytesRead, NULL);
//     Sleep(100); // המתנה לקבלת תשובה
//     // קריאת נתונים מהארדוינו
//     ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
//     buffer[bytesRead] = '\0'; // סיום מחרוזת
    
//     temp=strtok_s(buffer,"\n", &internal);
//     current.Longitude=strtod(internall, &temp);
//     temp=strtok_s(NULL,"\n", &internal);
//     current.Latitude=strtod(internall,temp);
//     return current;
// }

//פונקציה לסריאל 9600
// void setupSerial_9600() {
//     hSerial = CreateFile("COM5", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
//     if (hSerial == INVALID_HANDLE_VALUE) {
//         printf("Error in opening serial port\n");
//         return;
//     }
    
//     DCB dcbSerialParams = {0};
//     dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
//     dcbSerialParams.BaudRate = CBR_9600;
//     dcbSerialParams.ByteSize = 8;
//     dcbSerialParams.StopBits = ONESTOPBIT;
//     dcbSerialParams.Parity = NOPARITY;
    
//     SetCommState(hSerial, &dcbSerialParams);
// }

//פונקציה שמתחברת לשרת מהלקוח ומקבלת מסלול נסיעה 
void clientt(location_net start,location_net end,location_net Roadblock) {
    printf("entering to connected to server 8080\n");
    //בנית סוקט
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Connection to server failed. Error Code: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    printf("Connected to server at %s:%d\n", inet_ntoa(serverAddr.sin_addr), PORT);

    // שליחת הנתונים לשרת
    location_net sendd[3];
    //מיקום ההתחלה
    sendd[0]=start;
    //מיקום יעד
    sendd[1]=end;
    //במידה ויש חסימה מיקום החסימה
    sendd[2]=Roadblock;

    //שליחת הנתונים
    if (send(clientSocket, (char*)sendd, sizeof(sendd), 0) == SOCKET_ERROR) {
        printf("Send failed. Error Code: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
   
    // // קבלת מספר התחנות במסלול
    // int count_location = 0;
    // int bytesReceived = recv(clientSocket, (char*)&count_location, sizeof(int), 0);

    // if (bytesReceived == sizeof(int) && count_location > 0) {
    //     // ==== קבלת מערך struct פשוט ====
    //     location_net* route = malloc(sizeof(location_net) * count_location);
    //     int total = 0;
    //     while (total < sizeof(location_net) * count_location) {
    //         int n = recv(clientSocket, (char*)route + total, sizeof(location_net) * count_location - total, 0);
    //         if (n <= 0) break;
    //         total += n;
    //     }
    //     if (total == sizeof(location_net) * count_location) {
    //         printf("מסלול שהתקבל:\n");
    //         for (int i = 0; i < count_location; i++) {
    //             printf("Stop %d: Latitude = %f, Longitude = %f\n", i, route[i].Latitude, route[i].Longitude);
    //         }
    //     } else {
    //         printf("שגיאה בקבלת המסלול מהשרת\n");
    //     }
    //     free(route);
    //     // כאן אפשר להפעיל את Travel_by_route(route, count_location);
    // } else {
    //     // ==== קבלת מחרוזת שגיאה עד '\0' ====
    //     char buff[256] = {0};
    //     int total = 0, n;
    //     do {
    //         n = recv(clientSocket, buff + total, sizeof(buff) - 1 - total, 0);
    //         if (n <= 0){

    //         closesocket(clientSocket);
    //         WSACleanup();
    //         }
    //         total += n;
    //     } while (buff[total - 1] != '\0' && total < sizeof(buff) - 1);
    //     printf("הודעת שגיאה מהשרת: %s\n", buff);
    //     // כאן אפשר להפעיל טיפול בשגיאה
    // }

    closesocket(clientSocket);
   WSACleanup();
}
// //פונקציה שמביאה פלט מחיישני מרחק
// void duatance_arr(){

//}

// פונקציית השוואה למיון לפי X, ואז Y, ואז Z (אם צריך)
// int compare_lidar_points(const void* a, const void* b) {
//     const LIDAR_data* p1 = (const LIDAR_data*)a;
//     const LIDAR_data* p2 = (const LIDAR_data*)b;

//     if (p1->x < p2->x) return -1;
//     if (p1->x > p2->x) return 1;
//     // אם X שווה, משווים לפי Y
//     if (p1->y < p2->y) return -1;
//     if (p1->y > p2->y) return 1;
//     // אם גם Y שווה, משווים לפי Z
//     if (p1->z < p2->z) return -1;
//     if (p1->z > p2->z) return 1;
//     return 0;
// }
// //פונקציה שמביאה את רוחב הכביש מצד ימין של הרכב
// double witgh_right(){
//     double x=0,y=0,z=0,distance=0;
//     double max=0;
// for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//             if(x>0  && y<=length_car/2 && z>=height_car   || x>0   && abs(y)<=length_car/2 ){
//                 if( z<= height_car+0.2 && z<=height_car&& z>=height_car-0.2){
//                     if(distance>max){
//                         max=distance;
//                     }
//                 }
//             }
//     }
//     return max;
// }
// //פונקציה שמביאה את רוהב הכביש מצד שמאל של הרכב
// double witgh_laft(){

//   double x=0,y=0,z=0,distance=0;
//     double max_left=0;
// for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//             if(x<0  && y<=length_car/2 && z>=height_car   || x<0   && abs(y)<=length_car/2 ){
//                 if( z<= height_car+0.2 && z<=height_car&& z>=height_car-0.2){
//                     if(distance>max_left){
//                         max_left=distance;
//                     }
//                 }
//             }
//     }
//     return max_left;
// }
// //פונקציה שמביאה את זווית הפניה
// double compute_avoidance_angle(double distance, const char* side) {
//     // הגנה על ערכים קיצוניים
//     if (distance < 0.1) distance = 0.1;

//     // חישוב זווית יחסית: ככל שהמרחק קטן, הפנייה חדה יותר
//     double angle = angle_max * (MIN_DISTANCE / distance);
//     if (angle > angle_max) angle = angle_max;

//     // אם המכשול משמאל – פנה ימינה (זווית שלילית), אם מימין – פנה שמאלה (חיובית)
//     if (strcmp(side, "left") == 0) {
//         angle = -angle;
//     } else if (strcmp(side, "right") == 0) {
//         // angle חיובי כבר
//     } else {
//         // ערך לא חוקי, לא פונה
//         angle = 0;
//     }

//     return angle;
// }
//פונקציה לנסיעה לפי מסלול
//מקבלת פלט מחיישן GPS
// double Travel_by_route(){
//      location* start;
//משתנה לנקודה הבאה
//     location* temp;
//     double a;
//     double b;
//     double angle;
//     char * internal;
//     char *internall;
//     char * temp2;
//     location current;
//     char buffer[256];
//     DWORD bytesRead;
    
//     //קריאת הערכים מחיישן GPS
//     char signal = 'G';   
//     WriteFile(hSerial, &signal, sizeof(signal), &bytesRead, NULL);
//     Sleep(100); // המתנה לקבלת תשובה
//     // קריאת נתונים מהארדוינו
//     ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
//     buffer[bytesRead] = '\0'; // סיום מחרוזת
    
//     temp2=strtok_s(buffer,"\n", &internal);
//     current.Longitude=strtod(internall, &temp2);
//     temp2=strtok_s(NULL,"\n", &internal);
//     current.Latitude=strtod(internall,temp2);
//     //אם הגעת לצומת הבאה
//     if(current.Latitude==start->Latitude  && current.Longitude==start->Longitude){
//         //מכניס את נתוני התחילת כביש הבא
//             temp=start->next;
//         //כל עוד הוא לא הגיע לכביש שולח את הזווית
//         while(current.Latitude!=temp->Latitude  && current.Latitude!=temp->Latitude){

//         //חישוב צלעA 
//         a=temp->Latitude-start->Latitude;
//         //חישוב צלע B
//          b=temp->Longitude-start->Longitude;
//         //חישוב הזווית
//         angle=tan(a/b);
//         //שולחת את זווית הנסיעה         
//         return angle;
        
//         }
//             start->next;
//     }
    
// }

// //פונקציה המביאה פלט מחיישן מים
// double flood(){

// }
// //פונקציה ששומרת מליפול לצידי הכביש ימין     
// double Deviation_to_the_right_angle(){
//     double x=0,y=0,z=0,distance=0;

//      //עבור כל נקודה
//     for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//             //זווית של צד ימני 
//             if(x>0 && distance<=2  && y<=length_car/2 || x>0 && distance<=2  && abs(y)<=length_car/2 ){
//            if( z>height_car+0.2 || z<height_car-0.2){
//                     //פניה שמאלה בזווית מסוימת
//                     return(compute_avoidance_angle(distance,"left"));
//                 }
//             }              
//         }
// }
// //פונקציה ששומרת על נסיעה בצד ימין
// double travel_right_angle(){
//     double x=0,y=0,z=0,distance=0;
//     for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//         if(x>0 && distance>2){
//             if(z<=height_car+0.2 && z>=height_car-0.2){
//                 return(compute_avoidance_angle(distance,"right"));

//             }
//         }
//     }

// }
// //פונקציה שבודקת מכשול על הכביש
// double Road_obstacle_angle(){
// //מערך ששומר את כל הנקודות ללא מכשול שממול הרכב
// LIDAR_data no_obstacle_points[100000000];
//  double x=0,y=0,z=0,distance=0,x2=0,y2=0,z2=0,distance2=0;
//  int counter=0;
//  boolean obstacle=FALSE;
//     //עבור כל הנקודות
//     for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//         //אם יש מכשול במסלול הנסיעה של הרכב
//         if(y>=0 && x<=witgh_car/2 && distance>height_car+0.2 && distance<height_car-0.2  ||
//         y>=0 && x>=witgh_car/2 && distance>height_car+0.2 && distance<height_car-0.2)
//         {
//              obstacle=TRUE;
//         }

//     }
//     if(obstacle==TRUE){
//     //מציאת נקודות ממולו ללא מכשול
//             for(int j=0;j<length_piont;j++){
//                 x2=points[j].x;
//                 y2=points[j].y;
//                 z2=points[j].z;
//                 distance2=sqrt(x2*x2+y2*y2+z2*z2);
//                 //אם זוהי נקודה  לא ממול לרכב אבל לפניו
//                 if(x2>0  && y2>=0 && x2<=witgh_right() && x2>=witgh_car/2  && distance2<=5  && y2>=7 || x2<0 && y2>=0 && abc(x2)<= witgh_laft()  && abc(x2)>=witgh_car/2 && y2<=7)
//                 {
//                     //הכנסת נתונים למערך בו יש נקודות אפשריות שאפשר לעבור אליהם
//                     no_obstacle_points[counter].x=x2;
//                     no_obstacle_points[counter].y=y2;
//                     no_obstacle_points[counter].z=z2;
//                     //no_obstacle_points[counter].angle=points[i].angle;
//                 }
//             }     
//             //מיון המערך
//             qsort(no_obstacle_points,length_piont,sizeof(LIDAR_data),compare_lidar_points);
//             //זיהוי שטח ללא מכשול
//             for(int k=0;k<length_piont;k++){
//                 double x3,y3,z3;
//                 x3=no_obstacle_points[k].x;
//                 y3=no_obstacle_points[k].y;
//                 z3=no_obstacle_points[k].z;
//                 //מציאת סדרה הנדסית ברוחב הרכב + 4 מטר ובאורך הרכב + 70 מטר
//                 if( ){
//                     double temp=x
//                 }
//             }
//     }
// }

// //פונקציה שמביאה את המהירות נסיעה שהרכב צריך לנסוע בה
// double speed(){
//      double x=0,y=0,z=0,distance=0,x2=0,y2=0,z2=0,distance2=0;
//  int counter=0;
//  int min_distance=sqrt(points[0].x*points[0].x+points[0].y*points[0].y+points[0].z*points[0].z);
//     //עבור כל הנקודות
//     //מציאת המרחק הכי קטן
//     for(int i;i<length_piont;i++){
//         //X
//         x=points[i].x;
//         //Y
//         y=points[i].y;
//         //Z
//         z=points[i].z;
//         //מרחק
//         distance=sqrt(x*x+y*y+z*z);
//         //רק הנקודות ממול הרכב
//         if(y>=0 && x<=witgh_car/2+2 && abc(x)<=witgh_car/2+2  && z>=height_car/2){
//             if(distance<min_distance);
//             {
//                 min_distance=distance;
//             }
//         }
//     }  
//     //חישוב מהירות התגובה למרחק זהז 
//     double reaction_time = 0.5; // זמן תגובה בשניות
//     double deceleration = 7.0;  // תאוטה במטר לשנייה בריבוע
//     double v=-deceleration*reaction_time+sqrt(pow(deceleration*reaction_time,2)+2*min_distance);
//     v=fmax(v,0);
//     return v;

// }

// //התהליך הראשי שרץ כל הזמן ומופעל כשמתחילים בנסיעה
// void head(){
// while (1){

//     HANDLE thread1, thread2; // מגדירים משתנים להחזקת הידיות של התהליכים
//     DWORD exitCode1, exitCode2; // מגדירים משתנים להחזקת קודי הסיום של התהליכים
//     int result1;
//     double result2;

//     //פונקצי הלמהירות
//     thread1=CreateThread(NULL,0,speedd(),NULL,0,NULL);
//     //פונקציה לזווית
//     thread2=CreateThread(NULL,0,angle(),  NULL,  0, NULL);

//     // מחכים שהתהליכים יסתיימו
//     WaitForSingleObject(thread1, INFINITE); // מחכים לסיום התהליך הראשון
//     WaitForSingleObject(thread2, INFINITE); // מחכים לסיום התהליך השני

//     // מקבלים את קודי הסיום של התהליכים
//     GetExitCodeThread(thread1, &exitCode1); // מקבלים את קוד הסיום של התהליך הראשון
//     GetExitCodeThread(thread2, &exitCode2); // מקבלים את קוד הסיום של התהליך השני
//     // המרת קודי הסיום לערכים מסוג double
//     result1 = (int)exitCode1; // ממירים את קוד הסיום הראשון ל-double
//     result2 = (double)exitCode2; // ממירים את קוד הסיום השני ל-double

//     // סוגרים את הידיות של התהליכים
//     CloseHandle(thread1); // סוגרים את הידית של התהליך הראשון
//     CloseHandle(thread2); // סוגרים את הידית של התהליך השני

//     //שליחת מהירות וזווית לפונקצית מנועים
// }
// }



// //פונקצית ההתחמקות הראשית מקבלת זוויות מכל התהליכים ומוצאת עדיפות
// void evansion(){

// //זווית  GPS
// double gps_angle=0;
// //זווית מים 
// double watet_angle=0;
// //זווית ימין 
// double right_angle=0;
// //זווית שמאל
// double left_angle=0;
// //זווית קדימה 
// double front_angle=0;
// //זווית כשול
// double side_angle=0;
// //זווית אחורה
// double back_angle=0;

//     while(1){
//     //חישוב מהירות הנסיעה
//     //מהירות מקדימה




//     //חישוב הזווית
//     //זווית מחיישן GPS
//     //זווית מחיישן מים
//     //זווית של מכשול


        

//     //זווית של מכשול בצד
//     //זווית לנסיעה למקום הכי ריק
//     //זווית מאחורה
// //אם משהוא שלח זווית  0 צריך לחשב מסלול מחדש
//     }

// }



   