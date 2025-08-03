
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hesder_graph.h"



//פונקציה לבנית טבלת צמתים
FILE* Building_a_node_table();
//פונקציה לבנית קובץ טבלת כבישים
FILE* Building_a_road_table();
//פונקציה לבנית העמודות בטבלאות
void Building_a_road_and_node_table();
//פונקציה להכנסת נתונים לטבלת צמתים
void Inserting_data_into_odes(long long , double,double, bool ,bool );
//פונקציה שמקבלת רוחב שוליים ומחשבת את מספר הרכבים שיכולים להיות בו  
int num_cars_in_marginn(float);
//פונקציה לבניה בסיסית של 2 הטבלאות
void Database_construction() ;

//פונקציה להכנסת נתונים לטבלת כבישים
void Entering_data_into_a_road_table(long long,long long,long long, long long, double,double,int, int, float, float, float, int,bool);



//פונקציה לבנית קובץ טבלת צמתים
FILE* Building_a_node_table() {
	f_nodes=fopen( "nodess.txt", "wt");
	return (f_nodes);
}
//פונקציה לבנית קובץ טבלת כבישים
FILE* Building_a_road_table() {
	f_roads=fopen( "roadss.txt", "wt");
	return(f_roads);
}

//פונקציה לבנית העמודות בטבלאות
void Building_a_road_and_node_table()
{
	//בנית העמודות לטבלת כבישים
	//מזהה ייחודי//עמודת מספר כביש//צומת נכנס//צומת יוצא//עמודת מיקום//מספר הקטע כביש מתוך כל הכביש//עמודת מספר נתיבים//רוחב שוליים//סיכוי לעומס//סיכוי לתאונות//פסי רכבת
	fprintf(f_roads, "%s", "id, road number, incoming node, outgoing node, Longitude,Latitude,num_road_section, Multiple lanes, margin width, Chance of congestion, Chance of flooding, Entrance to railway tracks,flag\n");
	//בנית העמודות לטבלת צמתים
	//מזהה ייחודי//מיקום//רמזור	
	fprintf(f_nodes, "%s", "id, Longitude,Latitude, traffic lights,flag\n");
}
//פונקציה להכנסת נתונים לטבלת צמתיםSFככ
void Inserting_data_into_odes(long long id, double Longitude,double Latitude, bool traffic_lights,bool flag) {
	fprintf(f_nodes, "%lld, %f,%f, %d,%d \n", id, Longitude,Latitude, traffic_lights,flag);
}

//פונקציה להכנסת נתונים לטבלת כבישים
void Entering_data_into_a_road_table(long long id, long long road_number, long long incoming_node, long long outgoing_node,double Longitude, double Latitude,int num_road_section, int multiple_lenes, float margin_width, float Chance_of_congestion, float Chance_of_flooding, int Entrance_o_railway_tracks,bool flag) {
	//פונקציה שתקבל רוחב שוליים ותחשב את מספר הרכבים שיכולים לסוע בו
	int new_margin_width = (int)margin_width;
	new_margin_width = num_cars_in_marginn(new_margin_width);
	fprintf(f_roads, "%lld, %lld, %lld,%lld,%f,%f,%d,%d,%f,%f,%f,%d,%d \n", id, road_number, incoming_node, outgoing_node, Longitude,Latitude,num_road_section, multiple_lenes, margin_width, Chance_of_congestion, Chance_of_flooding, Entrance_o_railway_tracks,flag);
}

//פונקציה שמקבלת רוחב שוליים ומחשבת את מספר הרכבים שיכולים להיות בו  
int num_cars_in_marginn(float margin_width){
int num_cars=4;
//התוכן לא אמיתי הוא לא בתחום הפיתוח של הפרויקט שלי
return num_cars;
}
//פונקציה לבניה בסיסית של 2 הטבלאות
void Database_construction() {
	//בנית קובצי הטבלאות 
	if (Building_a_node_table() != NULL && Building_a_road_table() != NULL)
	{
		//בנית העמודות 
		Building_a_road_and_node_table();
	}
}


