#include "js_writer.h"

#define BUFSIZE 264

FILE *file;
char *JS_FILE = "canvas.js";
char *HTML_FILE = "html_code.html";
int init = 0;



int html_write(){
  file = fopen(HTML_FILE, "wb");
  char * buffer = malloc (BUFSIZE * sizeof(char));

  fprintf (file, "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Canvas parser</title>\n\t\t<script src=\"%s\"></script>\n\t</head>\n\t<body>\n\t\t<canvas id=\"mon_canvas\" width=\"1000\" height=\"1000\">\n\t\t\tMessage pour les navigateurs ne supportant pas encore canvas.\n\t\t</canvas>\n\t</body>\n</html>\n ", JS_FILE);

  return fclose(file);
}

int js_write_init(FILE *f){

  fprintf (f, "window.onload=function()\n{\n\tvar canvas = document.getElementById('mon_canvas');\n\t\tif(!canvas)\n\t\t{\n\t\t\talert(\"Impossible de récupérer le canvas\");\n\t\t\treturn;\n\t\t}\n\tvar context = canvas.getContext('2d');\n\t\tif(!context)\n\t\t{\n\t\t\talert(\"Impossible de récupérer le context du canvas\");\n\t\t\treturn;\n\t\t}\n\n\n}\n");
  init++;

}

int js_write(char *buffer){
  file = fopen(JS_FILE, "r+");

  if( file == NULL)
    file = fopen(JS_FILE, "w");
  fseek(file, 0, SEEK_END);

  if (ftell(file) == 0 || init == 0 )
    js_write_init(file);
 
  fseek(file, -3 * sizeof(char), SEEK_END);
  fwrite(buffer, strlen(buffer), 1, file);
  fwrite("\n}\n", 3, 1, file);
   
  /*
  fprintf (file, "window.onload=function()\n{\n\tvar canvas = document.getElementById('mon_canvas');\n\t\tif(!canvas)\n\t\t{\n\t\t\talert(\"Impossible de récupérer le canvas\");\n\t\t\treturn;\n\t\t}\n\tvar context = canvas.getContext('2d');\n\t\tif(!context)\n\t\t{\n\t\t\talert(\"Impossible de récupérer le context du canvas\");\n\t\t\treturn;\n\t\t}\n%s\n}\n", buffer)
  */

  return fclose(file);
}
