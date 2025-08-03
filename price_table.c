//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
#include "graph_tabels.c"




//פונקציה לבנית קובץ טבלת מחירים
FILE* Building_a_price_table();
//פונקציה לבנית העמודות בטבלת מחירים
void Building_a_Price_table_columns() ;
//-פונקצית ההכנסה הראשית-פונקציה להכנסת מספר נתיבים
void Inserting_data_into_price();
//פונקציה להכנסת השוליים
double** insert_margin(int ,double**);
//פונקציה להכנסת סיכוי לתאונות
double** insert_Road_congestion(int,int,double**);
// פונקציה להכנסת סיכוי למזג אויר
double** insert_weather(int, int, int,double**);
//פונקציה לאפשרויות של רכבת
double** insert_train(int,int,int,int,double**);
//פונקציה להכנסת מערכים
double** insert_arrais(int, int, int , int, int , double** a);
//פונקציה שמחשבת מה מספר הנתיבים שיכול להיות במדינה בה נמצא הרכב
int Maximum_number_of_routes_in_th_ecountry();
//פונקציה שמביאה את הרוחב הכי רחב של שוליים שיכולים להיות במדינה בה נמצא הרכב
float The_widest_margins_in_the_country();
//הפונקציה הסופית
void building_price_table();
//פונקציה לחישוב מספר השורות בקובץ
long num_lines();
//בנית המערך
double** building_arr_h();
//משתנה לחסם תאונות
float  Road_congestion_block = 100;
//משתנה לחסם מזג אויר
float  Weather_barrier = 100;

//פונקציה שמחשבת מה מספר הנתיבים שיכול להיות במדינה בה נמצא הרכב
int Maximum_number_of_routes_in_th_ecountry(){
    int num=3;
    //הנתונים לא אמיתיים זה לא בתחום הפיתוח של הפרויקט שלי
    return num;
}
//פונקציה שמביאה את הרוחב הכי רחב של שוליים שיכולים להיות במדינה בה נמצא הרכב
float The_widest_margins_in_the_country(){
    float width=36.6;
    //הנתונים לא אמיתיים זה לא בתחום הפיתוח של הפרויקט שלי
    return width;
}

//פונקציה לחישוב מספר השורות בקובץ
long num_lines(){
    long num=0;
    num=pow(Maximum_number_of_routes_in_th_ecountry(),pow(The_widest_margins_in_the_country(),pow(fabs(Road_congestion_block/10),pow(fabs(Weather_barrier),2))));
    return num;
}

//פונקציה לבנית קובץ טבלת מחירים
FILE* Building_a_price_table() {
	f_price=fopen( "price.txt", "wt");
	return(f_price);
}
//פונקציה לבנית העמודות בטבלת מחירים
void Building_a_Price_table_columns() {
	fprintf(f_price, "%s", "Number of lanes, Number of vehicles on the sidelines, Chance of congestion,Chance of weather disruptions,trains?,pointer Harr\n");
}
//פונקציה לבנית המערך
double**  building_arr_h(){
    int m=num_lines();
    double** Houers_arr=(double**)malloc(m*sizeof(double*));
    for(long n=0; n<m ;n++){
        Houers_arr[n]=(double*)malloc(24*sizeof(double));
    }
    return Houers_arr;
}
//פונקציה אחרונה שבונה הכל
//מקבלת : מספר נתיבים רוחב שולייםת סיכוי לעומסת סיכוי למזג אויר, אפשרות לרכבת
double** insert_arrais(int num_paths, int num_of_cars_in_margin, int Chance_of_congestion, int Chance_of_weather_disruptions, int train_f, double** arre){    //הכנסת הנתונים לטבלת מחירים
    
        fprintf(f_price,"%d,%d,%d,%d,%d,%p\n",num_paths,num_of_cars_in_margin,Chance_of_congestion,Chance_of_weather_disruptions,train_f,(void*)arre);
        arre ++;
        return arre;
}

//פונקציה להכנסת האפשרויות לרכבת
//מקבלת: מספר נתיביםת רוחב שולייםת סיכוי לעומסת סיכוי למזג אויר
double** insert_train(int num_paths, int num_of_cars_in_margin, int Chance_of_congestion,int Chance_of_weather_disruptions,double**arrd){
    for(int  r=0; r<2 ;r++){
        //שליחה לפונקציה שבונה מערכים
         arrd=insert_arrais(num_paths,num_of_cars_in_margin,Chance_of_congestion,Chance_of_weather_disruptions,r,arrd);

        }
        return arrd;
}
//פונקציה להכנסת סיכוי למזג אויר קשה
//מקבלת מספר נתיבים,מספר רכבים בשוליים,סיכוי לעומס בכביש
// (מקבלת מספר סופי לחסם האפשרויות לדוגמא:100%)  ע
double**  insert_weather(int num_paths, int num_of_cars_in_margin, int Chance_of_congestion,double** arrd) {
//חסם למזג אויר
//int wether=Weather_barrier;
for(int b=0;b<=100;b+=10){
    //שליחה לפונקציה של רכבת
   arrd= insert_train(num_paths,num_of_cars_in_margin,Chance_of_congestion,b,arrd);
}
return arrd;
}

//פונקציה להכנסת סיכוי לעומס
// מקבלת מספר רכבים בשוליי, מספר נתיבים ומחשבת את כל סוגי אפשרויות לעומס בכביש
// (מקבלת מספר סופי לחסם האפשרויות לדוגמא:100%)  ע
double** insert_Road_congestion(int num_paths, int num_of_cars_in_margin,double** arrc) {
   // int congestion_barrier=Road_congestion_block;
	for (int t = 0; t <= 100; t += 10) {
        //שליחה לפונקציה שתחשב מזג אויר
		arrc=insert_weather(num_paths, num_of_cars_in_margin, t,arrc);
	}
    return arrc;
}

//פונקציה שמקבלת מספר נתיבים
//ומחשבת עבור כל סוג נתיב את כל סוגי השוליים
double** insert_margin(int num_path,double** arrb) {
//פונקציה שמביאה את הרוחב שוליים של המדינה
    int num_cars=num_cars_in_marginn(The_widest_margins_in_the_country());
	for (int j = 0; j < num_cars; j++) {
        //שליחה לפונקציה את השוליים ואת הנתיבים פונקציה שמחשבת סיכוי לתאונות
		arrb=insert_Road_congestion(num_path, j,arrb);
	}
    return arrb;
}

//פונקציה להכנסת נתונים לטבלת  מחירים
//פונקציה להכנסת נתיבים
//מחשבת בלולאה את כל סוגי השוליים שיכולים להיות ושולחת לפונקציה שמכניסה רוחב שוליים
void  Inserting_data_into_price(double** arra) {
    //מציאת מספר נתיבים ע"י שליחה לפונקציה
     int peths=Maximum_number_of_routes_in_th_ecountry();
	for (int i = 1; i < peths; i++) {
        //שליחה לפונקציה שמכניסה סוגי שוליים
		arra=insert_margin(i,arra);
	}
}

//הפונקציה הסופית
void building_price_table(){
   	if (Building_a_price_table() == NULL) {
		printf("%s", "the table not building");
	}
	else {
       double**arr=building_arr_h();
		Building_a_Price_table_columns();
        Inserting_data_into_price(arr);
        fclose(f_price);
	}
}