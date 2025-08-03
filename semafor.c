#include <windows.h>
#include <stdio.h>
#include "dayaqstra.c"
//מיוטקס למשאב המשותף
HANDLE hMutex; 
//ראש הגרף- המשאב המשותף   
//extern  head_graph;

//פונקציה של סמפור גישה למשאב המשותףs
void accessResource(location);
//פונקציה לעדכון חסימה במיקום מסוים בכביש
void Roadblock_update(location);

void accessResource(location Roadblock) {
    // בקשת גישה למוטקס
    // ממתינה עד שהמיוטקס יהיה זמין לרכישה
    DWORD waitResult = WaitForSingleObject(hMutex, INFINITE);
    if (waitResult == WAIT_FAILED) {
        printf("Failed to acquire mutex: %d\n", GetLastError());
        return;
    }
    //פונקציה לעדכון חסימה במיקום מסוים בכביש
    Roadblock_update(Roadblock);
    // שחרור המוטקס
   if (!ReleaseMutex(hMutex)) {
        printf("Failed to release mutex: %d\n", GetLastError());
    }
}

void Roadblock_update(location Road){
    //צומת קודמת לכביש
    //node_ptr temp=head_graph;
    //צומת שאליה הכביש יוצא
    //מצביע לחלק מהכביש
    
    
    // while ( temp->longitude!=Road.Longitude && temp->latitude!=Road.Latitude  )
    // {
    //     // בטבלאותמעבר על כל הצמתים והכבישים בגרף


    // }
    //     //מחיקת הכביש מהטבלה
    //     //בנית הגרף מחדש

}

