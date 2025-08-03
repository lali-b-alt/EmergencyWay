#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "price_table.c"
#define MAX_LINE_LENGTH  1024


char line_nodes[MAX_LINE_LENGTH];
char line_road[MAX_LINE_LENGTH];
char line_price[MAX_LINE_LENGTH];
char line_r_copy[MAX_LINE_LENGTH];
//שלושת הקבצים
 FILE* f_nodes=NULL;
 FILE* f_roads=NULL;
 FILE * f_price=NULL;

long long visited_nodes[MAX_NODES_VISITED] = {0};
long long visited_count = 0;

node_ptr all_nodes[MAX_NODES_VISITED];
int all_nodes_count = 0;
//מציאת השעה הנוכחית
int Current_time();

//פתיחת הקבצים לקריאה
void open_files();
//פונקציה שבונה צומת
node_ptr building_node(char*);
//פונקציה שבונה קטע כביש
road_section_ptr building_road(char*);
//פונקציה שבונה מבנה המכיל נתונים של קטע כביש
Road_segment_data_ptr building_data_road(char*,char*);
//מחשבת מחיר של קטע כביש
double price_road(Road_segment_data_ptr data_struct);
//פונקציה שבונה כביש שלם
road_section_ptr Construction_of_a_complete_road(road_section_ptr first_road, long long num_road);
//פותחת קובץ גרף
void start_dot_file();
//מחברת קשת לצומת
void append_road_to_dot(long long from, long long to, double price);
//פונקציה סופית
void build_all(node_ptr first_node);
//פונקציה שמוצאת את הצומת אליה נכנס הכביש
node_ptr find_end_node(road_section_ptr road_end);
//פונקציה שמחשבת מחיר של כביש שלם
float Complete_road_price(road_section_ptr begin_road);
//חיבור כביש לצומת
double Connecting_an_intersection_to_a_road(road_section_ptr beginning_of_a_road, node_ptr Road_exit_junction);
//בודקת אם ביקרנו בצומת מסוימת
int is_visited(long long node_id);

// פונקציה למציאת צומת קיימת במערך הצמתים
node_ptr get_existing_node(long long node_id) {
    for (int i = 0; i < all_nodes_count; i++) {
        if (all_nodes[i]->id == node_id)
            return all_nodes[i];
    }
    return NULL;
}
//פונקציה שבודקת האם ביקרנו בצומת מסוימת
int is_visited(long long node_id) {
    for (int i = 0; i < visited_count; i++) {
        if (visited_nodes[i] == node_id) return 1;
    }
    return 0;
}

int Current_time() {
    time_t now;
    struct tm* local;
    time(&now);
    local = localtime(&now);
    int hour = local->tm_hour;
    return hour;
}
//פתיחת הקבצים במצב קריאה
void open_files() {
    printf("DEBUG: open_files called\n");
    fopen_s(&f_nodes, "nodess.txt", "r");
    fopen_s(&f_roads, "roadss.txt", "r");
    fopen_s(&f_price, "price.txt", "r");
    if (f_nodes == NULL || f_roads == NULL || f_price == NULL) {
        printf_s("%s\n", "eror open files roads price nodes");
    }
    else{
        printf("%s\n","yes");
    }
}
//הוספת קשת לגרף בקובץ
void append_road_to_dot(long long from, long long to, double price) {
    FILE* dot = fopen("graph.dot", "a");
    if (!dot) return;
    fprintf(dot, "  %lld -> %lld [label=\"%.2f\"];\n", from, to, price);
    fclose(dot);
}
//פתיחת קובץ גרף
void start_dot_file() {
    FILE* dot = fopen("graph.dot", "w");
    if (!dot) return;
    fprintf(dot, "digraph G {\n");
    fclose(dot);
}
//פונקציה שבונה צומת מקבלת שורה של טבלת צמתים
node_ptr building_node(char* node_arr) {
    char* token_node;
    char* internal_node;
    char* long_internal_node;
    char* float_internal_node;

    node_ptr new_node = (node_ptr)malloc(sizeof(*new_node));
    if (new_node == NULL) {
    }
    token_node = strtok_s(node_arr, ",", &internal_node);
    new_node->id = strtoll(token_node, &long_internal_node, 10);
    token_node = strtok_s(NULL, ",", &internal_node);
    new_node->longitude= strtof(token_node, &float_internal_node);
    token_node = strtok_s(NULL, ",", &internal_node);
    new_node->latitude= strtof(token_node, &float_internal_node);
    token_node = strtok_s(NULL, ",", &internal_node);
    new_node->traffic_lights = atoi(token_node);
    new_node->arr=NULL;
    new_node->current_size = 0;
    //במידה והצומת לא קיימת מוסיפים אותה למערך צמתים
    if (get_existing_node(new_node->id) == NULL) {
        all_nodes[all_nodes_count++] = new_node;
    }
    return new_node;
}
//פונקציה שבונה מבנה המכיל נתוני קטע כביש
Road_segment_data_ptr building_data_road(char* data_arr,char* internel_data) {
    Road_segment_data_ptr new_data = (Road_segment_data_ptr)malloc(sizeof(*new_data));
    if (new_data == NULL) {
        printf("%s","error in building data road");
    }
    char* float_data=NULL;
    data_arr = strtok_s(NULL, ",", &internel_data);
    new_data->Multiple_lanes = atoi(data_arr);
    data_arr = strtok_s(NULL, ",", &internel_data);
    new_data->margin_width = atoi(data_arr);
    data_arr = strtok_s(NULL, ",", &internel_data);
    new_data->Chance_of_traffic_congestion = strtof(data_arr, &float_data);
    data_arr = strtok_s(NULL, ",", &internel_data);
    new_data->Chance_of_weather_disruptions = strtof(data_arr, &float_data);
    data_arr = strtok_s(NULL, ",", &internel_data);
    new_data->train = atoi(data_arr);
   return new_data;
}
//פונקציה שבונה קטע כביש
road_section_ptr building_road(char* road_arr) {
    char* token_road=NULL;
    char* internal_road=NULL;
    char* long_internal_road=NULL;
    char* float_internal_road=NULL;
    road_section_ptr new_road = (road_section_ptr)malloc(sizeof(*new_road));
    if (new_road == NULL) {
    }
    new_road->points_to_the_next = NULL;   
    token_road = strtok_s(road_arr, ",", &internal_road);
    token_road = strtok_s(NULL, ",", &internal_road);
    token_road = strtok_s(NULL, ",", &internal_road);
    new_road->id_Previous_node = strtoll(token_road, &long_internal_road, 10);
    token_road = strtok_s(NULL, ",", &internal_road);
    new_road->id_next_junction = strtoll(token_road, &long_internal_road, 10);
    token_road = strtok_s(NULL, ",", &internal_road);
    new_road->longitude = strtof(token_road, &float_internal_road);
    token_road = strtok_s(NULL, ",", &internal_road);
    new_road->latitude=strtof(token_road,&float_internal_road);
    token_road = strtok_s(NULL, ",", &internal_road);
    new_road->num_road_section=atoi(token_road);
    new_road->road_segment_data = building_data_road(token_road,internal_road);
    new_road->price = price_road(new_road->road_segment_data);
    return new_road;
}
double price_road(Road_segment_data_ptr data_struct) {
    if (data_struct == NULL) {
        return 0;
    }
    data_struct->Chance_of_traffic_congestion = (data_struct->Chance_of_traffic_congestion+9)/10*10;;
    data_struct->Chance_of_weather_disruptions= ((data_struct->Chance_of_weather_disruptions+9)/10)*10;
    rewind(f_price);

    char* internal_price;
    char* token_price;
    char* f_pricee;
    char* ff_price;
    char* d_price;
    int mul_lence;
    int mar_width;
    float traffic_congestion;
    float weather_disruptions;
    int if_train;
    char * houer;
    while (fgets(line_price, sizeof(line_price), f_price)) {
        token_price = strtok_s(line_price, ",", &internal_price);
        mul_lence = atoi(token_price);
        token_price = strtok_s(NULL, ",", &internal_price);
        mar_width = atoi(token_price);
        token_price = strtok_s(NULL, ",", &internal_price);
        traffic_congestion = strtof(token_price, &f_pricee);
        token_price = strtok_s(NULL, ",", &internal_price);
        weather_disruptions = strtof(token_price, &ff_price);
        token_price = strtok_s(NULL, ",", &internal_price);
        if_train = atoi(token_price);
        if (mul_lence == data_struct->Multiple_lanes && mar_width == data_struct->margin_width && traffic_congestion == data_struct->Chance_of_traffic_congestion
            && weather_disruptions == data_struct->Chance_of_weather_disruptions && if_train == data_struct->train) {
            int houer=Current_time();
            double price=0;
            return  price=mar_width+mul_lence*traffic_congestion+weather_disruptions+100;
        }
    }
    return 100;
}
//פונקציה שבונה כביש שלם  מקבלת מצביע לקטע תחילת הכביש ואת מספר הכביש
road_section_ptr Construction_of_a_complete_road(road_section_ptr first_road, long long num_road) {
    char* token_complete_road;
    char* internal_complete_road;
    char* long_complete_road;
    road_section_ptr temp=first_road;
    road_section_ptr new_section;
    rewind(f_roads);

    while (fgets(line_road, sizeof(line_road), f_roads)) {
        strcpy(line_r_copy,line_road);
        token_complete_road = strtok_s(line_road, ",", &internal_complete_road);
        token_complete_road = strtok_s(NULL, ",", &internal_complete_road);
        long long num_roadd = strtoll(token_complete_road, &long_complete_road, 10);
        if(num_roadd== num_road){
            new_section=building_road(line_r_copy);
            if(new_section->num_road_section<temp->num_road_section){
            while (new_section->num_road_section<temp->num_road_section)
            {
                temp->points_to_the_previous=new_section;
                new_section->points_to_the_next=temp;
                new_section->points_to_the_previous=temp->points_to_the_previous;
                temp->points_to_the_previous->points_to_the_next=new_section;
                temp=temp->points_to_the_previous;
            }
            temp=new_section;
        }
        else{
            temp->points_to_the_next=new_section;
            new_section->points_to_the_previous=temp;
            temp=new_section;
        }
      }       
    }
    return new_section;
}
//חיבור בין תחילת או סוף כביש לצומת
double Connecting_an_intersection_to_a_road(road_section_ptr beginning_of_a_road, node_ptr Road_exit_junction) {
    //איבר חדש למערך
    Pointing_to_the_bow new_section;
    //מחיר כל הכביש
    double price=Complete_road_price(beginning_of_a_road);
    new_section.the_price_of_the_bow = price;
    new_section.top_of_the_list = beginning_of_a_road;
      
    Pointing_to_the_bow* new_arr = realloc(Road_exit_junction->arr, sizeof(Pointing_to_the_bow) * (Road_exit_junction->current_size + 1));
    if (new_arr != NULL) {
        Road_exit_junction->arr = new_arr;
        Road_exit_junction->arr[Road_exit_junction->current_size] = new_section;
        Road_exit_junction->current_size++;
    }
    return price;
}
//חישוב מחיר כביש שלם
float Complete_road_price(road_section_ptr begin_road) {
    float price = 0;
    road_section_ptr temp_road = begin_road;
    while (temp_road != NULL) {
        price += temp_road->price;
        temp_road = temp_road->points_to_the_next;
    }
    return price;
}

// פונקציה מתוקנת: מחזירה תמיד את אותו מצביע לצומת אם כבר קיימת
node_ptr find_end_node(road_section_ptr road_end) {

    rewind(f_nodes); 
    char* token_end_road;
    char* internal_end_road;
    long long id = 0;
    node_ptr new_node;
    while (id != road_end->id_next_junction) {
        if (fgets(line_nodes, sizeof(line_nodes), f_nodes) == NULL) {
            return NULL; 
        }
        char line_nodes_copy[MAX_LINE_LENGTH];
        strcpy(line_nodes_copy, line_nodes);

        token_end_road = strtok_s(line_nodes, ",", &internal_end_road);
        if (atoi(token_end_road) == road_end->id_next_junction) {
            id = road_end->id_next_junction;
            new_node = building_node(line_nodes_copy); 
            return new_node;
        }
    }
    return NULL;
}
//הדפסת כל הגרף
void print_graph() {
    printf("----- GRAPH -----\n");
    for (int i = 0; i < all_nodes_count; i++) {
        node_ptr node = all_nodes[i];
        printf("Node %lld (coords: %f, %f, traffic lights: %d):\n", node->id, node->longitude, node->latitude, node->traffic_lights);
        for (int j = 0; j < node->current_size; j++) {
            Pointing_to_the_bow bow = node->arr[j];
            if (bow.top_of_the_list && bow.top_of_the_list->id_next_junction) {
                printf("  -> To node %lld (price: %.2f)\n", bow.top_of_the_list->id_next_junction, bow.the_price_of_the_bow);
            }
        }
    }
    printf("-----------------\n");
}
//בנית כל הגרף כולו וקבלת הצומת הראשונה
void build_all(node_ptr first_node) {
    long long num_road =0;
    if (first_node == NULL) return;
    if (is_visited(first_node->id)) return;

    visited_nodes[visited_count++] = first_node->id;
long long roads_numbers[MAX_LINE_LENGTH];

    road_section_ptr roads_list[MAX_LINE_LENGTH];
    int roads_count = 0;

    rewind(f_roads);
    while (fgets(line_road, sizeof(line_road), f_roads)) {
        char line_road_temp[MAX_LINE_LENGTH];
        strcpy(line_road_temp, line_road);

        char* token_all;
        char* internal_all;
        char* internal_long;

        // חילוץ שדות: id, road number, incoming node, outgoing nod
        token_all = strtok_s(line_road, ",", &internal_all); 
        token_all = strtok_s(NULL, ",", &internal_all); // מספר כביש
        long long num_road = strtoll(token_all, &internal_long, 10);
        token_all = strtok_s(NULL, ",", &internal_all); // incoming node
        long long incoming_node = strtoll(token_all, &internal_long, 10);
        token_all = strtok_s(NULL, ",", &internal_all); // outgoing node
        long long outgoing_node = strtoll(token_all, &internal_long, 10);

        if (incoming_node == first_node->id ) {
            roads_list[roads_count] = building_road(line_road_temp);
            roads_numbers[roads_count] = num_road; // שמור את מספר הכביש במערך נוסף
            roads_count++;
        }
    }

    for (int i = 0; i < roads_count; i++) {
         first_node->current_size_temp=roads_count;
        road_section_ptr temp_road = roads_list[i];
        long long num_r=roads_numbers[i];
        //חיבור צומת היציאה לכביש
        double price = Connecting_an_intersection_to_a_road(temp_road, first_node);
        
        //בנית הכביש כולו
        road_section_ptr end_road=Construction_of_a_complete_road(temp_road,num_r);
        //מציאת צומת היציאה
        
        node_ptr temp_end_node=find_end_node(end_road);
        //חיבור צומת היציאה עם הכביש
        double all_price=Connecting_an_intersection_to_a_road(end_road,temp_end_node);
        first_node->arr[i].next_node=temp_end_node;

        append_road_to_dot(first_node->id, temp_end_node->id, all_price);
        build_all(temp_end_node);

    }
   print_graph();
}