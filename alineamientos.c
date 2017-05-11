#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alineamientos.h"

/*GUI*/
GtkWidget *windowInitial;
GtkWidget * windowResult;
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

/*Bandera de algoritmo 1 para NW 2 para SW*/
int flagType = 1;

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
}

/*FINISH READ FILES*/

int main(int argc, char const *argv[]) {

    GtkBuilder      *builder;

    gtk_init(NULL, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui.glade", NULL);

    windowInitial = GTK_WIDGET(gtk_builder_get_object(builder, "window_initial"));
    gtk_builder_connect_signals(builder, NULL);

    gtk_window_set_default_size (GTK_WINDOW(windowInitial),100,100);

    windowResult = GTK_WIDGET(gtk_builder_get_object(builder, "windowResult"));
    gtk_builder_connect_signals(builder, NULL);

    scrollWindow = GTK_WIDGET(gtk_builder_get_object(builder, "scrolledwindow_table"));

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

void getInformation(){
    if (userInput){
        printf("Archivos\n");
        readFiles(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(g_filechooser_btn)));
    }
    if (!userInput){

        printf("Manual\n");
        vLength = gtk_entry_get_text_length (GTK_ENTRY(g_entry_v));
        wLength = gtk_entry_get_text_length(GTK_ENTRY(g_entry_w));

        v = (char*) malloc(vLength);

        strcpy(v,gtk_entry_get_text(GTK_ENTRY(g_entry_v)));



        w = (char*) malloc(wLength +1 );
        strcpy(w,gtk_entry_get_text(GTK_ENTRY(g_entry_w)));

    }



    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioNW))==TRUE){
        flagType = 2;
    }
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(radioSW))==TRUE){
        flagType = 1;
    }



    setWeight(gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_match)),gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_mismatch)),gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(g_spin_gap)));

}


