#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alineamientos.h"

/*GUI*/
GtkWidget *windowInitial;
GtkWidget * window;
GtkWidget *g_frame_manualEntry;
GtkWidget *g_frame_fileEntry;
GtkWidget *g_filechooser_btn;
GtkWidget *g_start_btn;
GtkWidget *g_entry_v;
GtkWidget *g_entry_w;
GtkWidget *scrollWindow;
GtkWidget ***entrada;
GtkWidget *g_table;
GtkWidget *g_spin_match;
GtkWidget *g_spin_gap;
GtkWidget *g_spin_mismatch;
GtkWidget *radioNW;
GtkWidget *radioSW;
GtkWidget *lblAligment2;
GtkWidget *lblAligment1;
GtkWidget * lblScoring ;

/*Drawing*/
GtkWidget *darea;
GtkWidget *frame;
GtkWidget *viewport;
GtkWidget * swindow ;

/*Bandera de algoritmo 1 para NW 2 para SW*/
bool flagType = TRUE;

/*Variables
  0 para manual
  1 para archivos
*/

bool userInput =false;
char * v;
char * w;
int vLength;
int wLength;

//READ FILES VARIABLES
FILE * archivo;
int newLine=0;
int textposition = 0;
int startString =0;
int contador = 0;
char buffer[1000];

/*READ FILES CODE*/
void fillBuffer(int _val) {
    if (strlen(buffer) == 0) {
        char dato= (char)_val;
        char auxiliar[] = {dato,'\0'};
        strcat(buffer,auxiliar);
    } else{
        char dato = (char)_val;
        char auxiliar[]={dato,'\0'};
        strcat(buffer,auxiliar);
    }
}

void cleanBuffer(){
    memset(buffer,'\0',strlen(buffer));
}


void readFiles (char * nombre){
    archivo = fopen(nombre,"r");
    int ch;
    while (feof(archivo)==0) {
        ch = fgetc(archivo);

        if (ch=='\n'){
            textposition ++;
            startString = 0;
        }

        if (startString == 1){
            char dato= (char) ch;
            char auxiliar[] = {dato,'\0'};
            if (textposition==0){
                strcat(v,auxiliar);
            }
            if (textposition==1){
                strcat(w,auxiliar);
            }

        }

        if (ch== '-'){
            if (textposition == 0){
                vLength = atoi(buffer);
                v = (char*) malloc(vLength+2);
                strcpy(v,"");
            }
            if (textposition == 0){
                wLength = atoi(buffer);
                w = (char*) malloc(wLength+2);
                strcpy(w,"");
            }
            startString = 1;

        }
        if (startString ==0 && ch !='-'){
            fillBuffer(ch);
        }
    }
    fclose(archivo);
}

/*FINISH READ FILES*/

int main(int argc, char const *argv[]) {

    GtkBuilder      *builder;


    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    windowInitial = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial"));
    gtk_builder_connect_signals(builder, NULL);


    g_frame_fileEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_fileEntry"));
    gtk_widget_hide(g_frame_fileEntry);
    g_frame_manualEntry = GTK_WIDGET(gtk_builder_get_object(builder, "frame_manualEntry"));
    gtk_widget_hide(g_frame_manualEntry);

    radioSW = GTK_WIDGET(gtk_builder_get_object(builder, "SW"));
    radioNW = GTK_WIDGET(gtk_builder_get_object(builder, "NW"));

    g_filechooser_btn = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser_btn"));
    //g_start_btn =  GTK_WIDGET(gtk_builder_get_object(builder, "btnstart"));
    g_entry_v =  GTK_WIDGET(gtk_builder_get_object(builder, "lbl"));
    g_entry_w =  GTK_WIDGET(gtk_builder_get_object(builder, "lblw"));

    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.txt");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(g_filechooser_btn),filter);

    g_spin_match = GTK_WIDGET(gtk_builder_get_object(builder, "spinMatch"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(  g_spin_match),-10000,10000);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(  g_spin_match),1,3);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(  g_spin_match),1);

    g_spin_mismatch = GTK_WIDGET(gtk_builder_get_object(builder, "spinmin"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_mismatch),-10000,10000);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_mismatch),1,3);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(  g_spin_mismatch),-1);

    g_spin_gap = GTK_WIDGET(gtk_builder_get_object(builder, "spinPenalty"));
    gtk_spin_button_set_range (GTK_SPIN_BUTTON(g_spin_gap),-10000,10000);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(g_spin_gap),1,3);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(  g_spin_gap),-2);


    gtk_widget_show(windowInitial);
    gtk_main();

    return 0;
}


void on_window_initial_destroy() {
    gtk_main_quit();
}
void on_window_result_destroy(){
    gtk_main_quit();
}



void on_btn_manualEntry_clicked() {
    gtk_widget_hide(g_frame_fileEntry);
    gtk_widget_show(g_frame_manualEntry);
    userInput = false;
}

void on_btn_fileEntry_clicked() {
    gtk_widget_hide(g_frame_manualEntry);
    gtk_widget_show(g_frame_fileEntry);
    userInput = true;
}



/*Draw Table*/

void drawValList(struct node *head,int start,int finish,int finishArrow,cairo_t * cr) {
    struct node *ptr = head;
    while(ptr != NULL) {
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr,start ,finish);
        char data[2];
        sprintf(data,"%d",ptr->nValue);
          cairo_show_text(cr, data);


        if(ptr->arrow[0] == true)
        {   cairo_move_to(cr, start, finishArrow);
            cairo_line_to(cr, start - 25, finishArrow - 20);
            cairo_stroke(cr);
        }
        if (ptr->arrow[1] == true) {
            cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            cairo_move_to(cr, start+15, finishArrow-20);
            cairo_line_to(cr, start+15, finishArrow+5);
            cairo_stroke(cr);
        }
        if (ptr->arrow[2] == true)
        {
            cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            cairo_move_to(cr, start-20, finishArrow+15);
            cairo_line_to(cr, start+2, finishArrow+15);
            cairo_stroke(cr);
        }

        start = start + 50;

        ptr = ptr->next;
    }

}

void drawMatrixValues(struct matrix *head,cairo_t * cr) {
    int start = 60;
    int finish = 85;
    int finishArrow = 65;
    struct matrix *ptr = head;
    while (ptr != NULL) {
        drawValList(ptr->value,start,finish,finishArrow,cr);
        ptr = ptr->next;
        finish = finish + 45;
        finishArrow = finishArrow + 45;
        start = 60;

    }

}



static void do_drawing(cairo_t *, GtkWidget *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer user_data)
{
    do_drawing(cr, widget);

    return FALSE;
}




static void do_drawing(cairo_t *cr, GtkWidget *widget)
{

    GtkWidget *win = gtk_widget_get_toplevel(widget);

    int width, height;
    gtk_window_get_size(GTK_WINDOW(win), &width, &height);


    int start = 60;
    int finish = 65;
    for (int i =0;i<wLength+1;i++){
        for (int j=0;j<vLength+1;j++){
            cairo_set_source_rgb(cr, 1, 1, 1);
            cairo_set_line_width(cr, 1);
            cairo_rectangle(cr, start, finish, 30, 30);

            cairo_stroke_preserve(cr);
            cairo_fill(cr);

            start = start +50;
        }
        start = 60;
        finish = finish + 45;
    }

    start = 120;
    for (int i =0;i<vLength+1;i++){
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr,start ,60);
        char data[1];
        strncpy(data,&v[i],1);
        cairo_show_text(cr, data);
        start = start +50;
    }
    start = 130;
    for (int i =0;i<wLength+1;i++){
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
        cairo_move_to(cr,40 ,start);
        char data[1];
        strncpy(data,&w[i],1);
        cairo_show_text(cr, data);
        start = start + 45;
    }





    drawMatrixValues(table,cr);


    gtk_widget_set_size_request( darea,60*vLength,60*wLength );


}




void drawTable(){


    GtkBuilder      *builder;

   // gtk_init(NULL, NULL);
    viewport = gtk_viewport_new (NULL,NULL);

    builder = gtk_builder_new();
    swindow = gtk_scrolled_window_new (NULL,NULL);
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "windowResult"));
    gtk_builder_connect_signals(builder, NULL);

    frame = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));
    gtk_container_add(GTK_CONTAINER(frame), swindow);

    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(viewport), darea);

    gtk_container_add(GTK_CONTAINER(swindow), viewport);



    lblAligment1 = GTK_WIDGET(gtk_builder_get_object(builder, "lblAligment1"));
    lblAligment2 = GTK_WIDGET(gtk_builder_get_object(builder, "lblAligment2"));;


    lblScoring =GTK_WIDGET(gtk_builder_get_object(builder, "lblMaxScoring"));

    if (flagType){
        char  data[100];
        sprintf(data,"%d",alignScoring);
        gtk_label_set_text(GTK_LABEL(lblScoring),data);
    }
    else{
        char  data[100];
        sprintf(data,"%d",maxScore);
        gtk_label_set_text(GTK_LABEL(lblScoring),data);
    }

    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL);


    gtk_label_set_text(GTK_LABEL(lblAligment1),"HOLA");
    gtk_label_set_text(GTK_LABEL(lblAligment2),"HOLA");


    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_title(GTK_WINDOW(window), "Resultados del Alineamiento");


    gtk_widget_show_all(window);

}



void getInformation(){

    if (userInput){
        printf("Archivos\n");
        readFiles(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
    }
    if (!userInput){

        printf("Manual\n");
        vLength = gtk_entry_get_text_length (GTK_ENTRY(g_entry_v));
        wLength = gtk_entry_get_text_length(GTK_ENTRY(g_entry_w));

        v = (char*) malloc(vLength+1);

        strcpy(v,gtk_entry_get_text(GTK_ENTRY(g_entry_v)));



        w = (char*) malloc(wLength +1 );
        strcpy(w,gtk_entry_get_text(GTK_ENTRY(g_entry_w)));

    }



    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioNW))==TRUE){
        flagType = true;
    }
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioSW))==TRUE){
        flagType = false;
    }



    setWeight(gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_match)),gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_mismatch)),gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_gap)));

    for (int i=0;i<vLength;i++){
        insertStr2(v[i]);
    }


    for (int i=0;i<wLength;i++){
        insertStr1(w[i]);
    }
    alignStrings(flagType);



    drawTable();

}


