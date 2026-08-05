#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * This program parses a gemini .gmi post and produces an
 * equivalent HTML post.
 */

// basic translations
#define H1 "<h1>"
#define H2 "<h2>"
#define H3 "<h3>"
#define P  "<p>"
#define ANCHOR(HREF, CONTENT) u8"<a href=\"" #HREF "\">🌐 " #CONTENT "</a>"
#define PRE "<pre>"
#define QUOTE "<blockquote>" 

typedef struct PARSING_STATE {
    int has_had_paragraph;
    int inside_pre;
    int ul_started;
    int quote_started;
} Parsing_State;

Parsing_State st = {0};

void print_HTTP_headers() {

}

void print_HTML_header() {
}

int main() {

    print_HTTP_headers();
    print_HTML_header();

    // read query to obtain file name
    char* filename = getenv("QUERY_STRING");
    FILE *gemfile = fopen(filename, "r");
    
    fclose(gemfile);

    return 0;
}
