#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <windows.h> 
 #include "building_the_graph.c"
#define  MAX_NODE 100000

// === הוספה: מבנה לשמירת קשתות שדיאקסטרה עבר ===
typedef struct {
    long long from;
    long long to;
} Edge;
// === הוספה: מערך קשתות דיאקסטרה ומונה ===
Edge dijkstra_edges[MAX_NODE];
int dijkstra_edges_count = 0;
//מבנה של איבר במערך הוירם
typedef struct parent
{
long long id;
double longitude_node;
double latitude_node;
double longitude_road;
double latidude_road;
struct parent* next;
}parent;

typedef struct {
    double Latitude;
    double Longitude;
} location_net;

// מבנה של איבר בערימה: מזהה צומת ומרחק
typedef struct {
    int node;      // אינדקס הצומת במערך all_nodes
    int distance;  // המרחק מההתחלה
} HeapNode;

// מבנה של ערימה בינארית (min heap)
typedef struct {
    HeapNode arr[MAX_NODE];
    int size;
} MinHeap;

//רשימת צמתים שכבר ביקרנו בהם
long long viiisited[MAX_NODE];
//מערך מרחקים 
long long distance[MAX_NODE];
//מערך הורים
parent parents[MAX_NODE];
//תור עדיפויות
MinHeap heap;
//ראש הרשימה
node_ptr staet_id=NULL;


//הצהרות על פונקציות
void heap_init(MinHeap* heap);
void swap(HeapNode* a, HeapNode* b);
void heap_push(MinHeap* heap, long long node, int distance);
HeapNode heap_pop(MinHeap* heap);
int heap_empty(MinHeap* heap);
node_ptr find_node(location_net node_location);


// === הוספה: פונקציה שמייצרת קובץ DOT עם קשתות דיאקסטרה מודגשות ===
void create_dot_with_dijkstra_edges(const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "digraph G {\n");
    // הדפסת כל הצמתים
    for (int i = 0; i < all_nodes_count; i++) {
        fprintf(f, "  %lld [label=\"%lld\"];\n", all_nodes[i]->id, all_nodes[i]->id);
    }
    // הדפסת כל הקשתות (רגיל)
    for (int i = 0; i < all_nodes_count; i++) {
        node_ptr node = all_nodes[i];
        for (int j = 0; j < node->current_size_temp; j++) {
            long long from = node->id;
            long long to = node->arr[j].next_node->id;
            // בדוק אם זו קשת דיאקסטרה
            int is_dijkstra = 0;
            for (int k = 0; k < dijkstra_edges_count; k++) {
                if (dijkstra_edges[k].from == from && dijkstra_edges[k].to == to) {
                    is_dijkstra = 1;
                    break;
                }
            }
            if (is_dijkstra)
                fprintf(f, "  %lld -> %lld [color=red, penwidth=3];\n", from, to); // === שינוי: קשת דיאקסטרה מודגשת
            else
                fprintf(f, "  %lld -> %lld;\n", from, to);
        }
    }
    fprintf(f, "}\n");
    fclose(f);
}




// פונקציה לאתחול הערימה
void heap_init(MinHeap* heap) {
    heap->size = 0;
}
void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}
//פונקציה להכנסת איבר לערימה
void heap_push(MinHeap* heap, long long node, int distance) {
    int i = heap->size++;
    heap->arr[i].node = node;
    heap->arr[i].distance = distance;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->arr[parent].distance <= heap->arr[i].distance)
            break;
        swap(&heap->arr[parent], &heap->arr[i]);
        i = parent;
    }
}
//פונקציה להוצאת האיבר הכי קטן מהתור
HeapNode heap_pop(MinHeap* heap) {
    HeapNode min = heap->arr[0];
    heap->arr[0] = heap->arr[--heap->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heap->size && heap->arr[left].distance < heap->arr[smallest].distance)
            smallest = left;
        if (right < heap->size && heap->arr[right].distance < heap->arr[smallest].distance)
            smallest = right;

        if (smallest == i)
            break;

        swap(&heap->arr[i], &heap->arr[smallest]);
        i = smallest;
    }
    return min;
}
int heap_empty(MinHeap* heap) {
    return heap->size == 0;
}
//פונקציה חמציאת מצביע לצומת מתוך מערך  צמתים
node_ptr find_node(location_net node_location){
    for(int i=0;i < all_nodes_count;i++){
        if(all_nodes[i]!=NULL){
        if(all_nodes[i]->longitude==node_location.Longitude && all_nodes[i]->latitude==node_location.Latitude){
            return all_nodes[i];
        }
    }
    }
    return NULL;
}
    
//פונקציה שבונה את הרשימה שדיאקסטרה מחזיר
location* build_list(location_net start,location_net end){
    
     static location path[MAX_NODES_VISITED];
    //מספר הצמתים ברשימה
    int len = 0;
    //מציאת מזהה הצומת הסופית
    long long curr = -1;
    for (int i = 0; i < all_nodes_count; i++) {
        if (all_nodes[i]->longitude == end.Longitude && all_nodes[i]->latitude == end.Latitude) {
            curr = all_nodes[i]->id;
        }
    }
    if (curr == -1) return NULL;

    long long curr_index = curr;
    //בנית הרשימה
    while (curr != -1 && curr_index != -1 && curr_index < all_nodes_count && all_nodes[curr_index]) {
        path[len].Longitude = all_nodes[curr_index]->longitude;
        path[len].Latitude = all_nodes[curr_index]->latitude;
        len++;
        curr = parents[curr_index].id;
        curr_index =curr;
    }
    //הופכים את הרשימה
    for (int i = 0; i < len / 2; i++) {
        location tmp = path[i];
        path[i] = path[len - 1 - i];
        path[len - 1 - i] = tmp;
    }
    if (len < MAX_NODES_VISITED)
        path[len].Latitude = -1;
    //החזרת הרשימה
    return path;  
}
//פונקצית דיאקסטרה מקבלת נקודת התחלה וסיום
location* dayaqstra(location_net start ,location_net end){
    dijkstra_edges_count = 0;
    printf("%s\n","insert in dayaqstra");

    // אתחול כל המערכים בלולאה
    for (int i = 0; i < MAX_NODES_VISITED; i++) {
        distance[i] = LLONG_MAX;
        parents[i].id = -1;
        viiisited[i] = 0;
    }
    //אתחול הערימה
    heap_init(&heap); 
    //אתחול צומת ראשונה
    node_ptr first = find_node(start);
    distance[first->id] = 0;
    //ראש הרשימה 
    staet_id=first;
    //הכנסת הצומת הראשונה לתור
    heap_push(&heap, first->id, 0);
    //משתנה עזר לאיבר מהתור
    HeapNode temp;

    //מתחילים לעבור על הגרף
    while (!heap_empty(&heap)) 
    {
        //מוצאייאם את הצומת הראשונה מהתור
        HeapNode temp = heap_pop(&heap);
        //המזהה של הצומת
        long long curr_index = temp.node; 
        //מציאת המצביע לצומת הראשונה
        node_ptr curr =get_existing_node(curr_index);
        //אם זה היעד 
        if(curr->longitude == end.Longitude && curr->latitude == end.Latitude){
            printf("%s,%lld\n","the rnd is",curr->id);
            long long curr_id = curr->id;
            printf("Shortest path (backwards):\n");
            while (curr_id != -1) {
            printf("Node: %lld\n", curr_id);
            curr_id = parents[curr_id].id;
            }
            // הכנסת היעד למערך הורים
          return build_list(start, end);
        }
        //מציאת צומת שלא ביקרנו בה
        if(viiisited[curr_index]){
        }
        else{
        double price = distance[curr_index]; 
        viiisited[curr_index] = 1;
        //מעבר על כל השכנים של הצומת
        printf("%s,%lld\n","the aba is:",curr->id);
        for(int j=0;j<curr->current_size_temp; j++) { 
            printf("%s.%lld\n","the kittke is",curr->arr[j].next_node->id);
            node_ptr neighbor = curr->arr[j].next_node;
                //המרחק מהאבא לבן
                double weight = curr->arr[j].the_price_of_the_bow;
                //המרחק של הבן הנוכחי מההתחלה
                long long new_dist = distance[curr_index] + weight;
                //עם במערך צמתים המרחק יותר גדול 
                if(distance[neighbor->id] > new_dist  ){
                    parent temp2;
                    temp2.id = curr->id;
                    //הכנסה למערך הורים את ההורה החדש
                    temp2.longitude_node = curr->longitude;
                    temp2.latitude_node = curr->latitude;
                    temp2.longitude_road = curr->arr[j].top_of_the_list->longitude;
                    temp2.latidude_road = curr->arr[j].top_of_the_list->latitude;
                    //הכנסת המרחק החדש למערך מרחקים
                    distance[curr->arr[j].next_node->id] = new_dist;
                    //הכנסה בפועל למערך הורים
                    parents[curr->arr[j].next_node->id] = temp2;
                    heap_push(&heap, curr->arr[j].next_node->id, new_dist);

                    //שמירת הקשת החדשה שעברנו עליה והדמיה שלה עובדת
                    dijkstra_edges[dijkstra_edges_count].from = curr->id;
                    dijkstra_edges[dijkstra_edges_count].to = neighbor->id;
                    dijkstra_edges_count++;
                    create_dot_with_dijkstra_edges("dijkstra_step.dot");
                    system("dot -Tpng dijkstra_step.dot -o dijkstra_step.png");
                    system("start \"\" \"dijkstra_step.png\"");
                    Sleep(500); 
                }
            }
        }
    }     
    return NULL;
}