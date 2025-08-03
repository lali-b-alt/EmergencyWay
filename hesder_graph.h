

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HESDER_GRAPH_H


//מצביע לטבלת כבישים
extern FILE* f_roads;
//מצביע לטבלת צמתים
extern FILE* f_nodes;
//מצביע לטבלת מחירים
extern FILE* f_price;


#define FLOAT_EQ(a, b) (fabs((a)-(b)) < 0.01)
#define MAX_NODES_VISITED 100000

//מערך ששומר את מזהי הצמתים שכבר נבנו
extern long long visited_nodes[MAX_NODES_VISITED];
extern long long visited_count;

#define BUFFER_SIZE 1024

typedef enum { forward, backwards }direction;
typedef struct location {
    double Longitude;  
    double Latitude;    
    struct location* next;
} location;

//מבנה שמייצג נתונים על קטע כביש
typedef struct {
	//מספר נתיבים
	int  Multiple_lanes;
	//רוחב שוליים (מספר רכבים שיכולים להיות בשוליים יחד)ח
	int margin_width;
	//סיכוי לעומס בכביש
	int Chance_of_traffic_congestion;
	//סיכוי למזגק אויר שישבש את הנסיעה
	int Chance_of_weather_disruptions;
	//פסי רכבת
	int train;
}Road_segment_data, * Road_segment_data_ptr;

//מבנה של קטע כביש-איבר ברשימה מקושרת שמייצגת קשת
typedef struct road_section {
	//מצביע לצומת קודמת 
	long long id_Previous_node;
	// מצביע לצומת הבאה
	long long id_next_junction;
	//מצביע לקודם
	struct road_section* points_to_the_previous;
	//מצביע לבא
	struct road_section* points_to_the_next;
	//מחיר קטע כביש זה
	float price;
	// קו אורך
	double longitude;
	//קו רוחב
	double latitude;
	//מספר חלק כביש
	int num_road_section;
	// מתצביע למבנה שמכל את נתוני הקטע כביש הזה
	Road_segment_data_ptr road_segment_data;
} road_section, * road_section_ptr;

//מבנה של צומת
typedef struct node {
	//שם הצומת-מזהה ייחודי
	long long id;
	//קו אורך
	double longitude;
	//קו רוחב
	double latitude;
	// האם יש רמזור
	int traffic_lights;
	// מצביע למערך דינאמי של מצביעים לקשתות
	struct  Pointing_to_the_bow* arr;
	//משתנה ששומר את גודל המערך הדינאמי בכל שלב
	int current_size ;
	int current_size_temp;
}node, * node_ptr;



//איבר במערך דינאמי בכל צומת שמיצג מצביע לקשת שמייצגת כביש
typedef struct Pointing_to_the_bow {
	//מצביע לאיבר הראשון בקשת
	struct road_section* top_of_the_list;
	//מחיר מהצומת הזו לצומת הבאה
	float the_price_of_the_bow;
	//מצביע לצומת הבאה
	node_ptr next_node;
	//כיוון הקשת     
	direction Direction_of_travel;
}Pointing_to_the_bow, * Pointing_to_the_bow_ptr;

//ראש הגרף
extern node_ptr head_graph;
