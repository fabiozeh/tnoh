#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * This program parses a gemini .gmi post and produces an
 * equivalent HTML post.
 */

// basic translations
#define H1_TAG "<h1>"
#define H2_TAG "<h2>"
#define H3_TAG "<h3>"
#define FIRST_P_TAG "<p class=\"first\">"
#define P_TAG  "<p>"
#define ANCHOR_TAG u8"<br/><a href=\"%s\">🌐 %s</a>\n"
#define PRE_TAG "<pre class=\"\">"
#define BLOCKQUOTE_TAG "<blockquote>\n" 
#define UL_TAG "<ul>\n"
#define LI_TAG "<li>%s</li>\n"

typedef struct PARSING_STATE {
    int has_had_paragraph;
    int pre_opened;
    int p_opened;
    int ul_opened;
    int blockquote_opened;
} Parsing_State;

Parsing_State st = {0};

typedef enum LT {
    TEXT = 0,
    H1,
    H2,
    H3,
    LINK,
    PRE,
    QUOTE,
    LI,
    BLANK
} line_type;

void print_HTTP_headers() {
    printf(u8"Content-type: text/html\n\n");
}

void cat(char *f) {
    FILE *stream = fopen(f, "r");
    unsigned char buffer[512];
    size_t sz_read = fread(buffer, sizeof buffer[0], 512, stream);
    while(sz_read) {
        fwrite(buffer, sizeof buffer[0], sz_read, stdout);
        sz_read = fread(buffer, sizeof buffer[0], 512, stream);
    }
    fclose(stream);
}

void print_HTML_header() {
    cat("topbit.html");
}

void print_HTML_footer() {
    cat("bottombit.html");
}

int is_ws(char c) {
    return c == ' ' |
        c == '\t' |
        c == '\n';
}

line_type parse_line_type(char *line) {
    if (memcmp(line, "```", 3) == 0)
        return PRE;
    if (memcmp(line, "=>", 2) == 0)
        return LINK;
    if (memcmp(line, "* ", 2) == 0)
        return LI;
    if (*line == '>')
        return QUOTE;
    if (memcmp(line, "###", 3) == 0)
        return H3;
    if (memcmp(line, "##", 2) == 0)
        return H2;
    if (*line == '#')
        return H1;
    while(*line == ' ' | *line == '\t' | *line == '\n')
        line++;
    if (*line == '\0')
        return BLANK;
    return TEXT;
}

void close_ul() {
    puts("</ul>");
    st.ul_opened = 0;
}

void close_pre() {
    puts("</pre>");
    st.pre_opened = 0;
}

void close_quote() {
    puts("</blockquote>");
    st.blockquote_opened = 0;
}

void close_p() {
    puts("</p>");
    st.p_opened = 0;
}

void found_text() {
    if (st.ul_opened) {
        close_ul();
    }
    if (st.blockquote_opened) {
        close_quote();
    }
    
    if (!st.p_opened) {
        st.p_opened = 1;
        if (!st.has_had_paragraph) {
            st.has_had_paragraph = 1;
            puts(FIRST_P_TAG);
        }
        else {
            puts(P_TAG);
        }
    }
}

void found_heading(line_type lt) {
    if (st.ul_opened) {
        close_ul();
    }
    if (st.blockquote_opened) {
        close_quote();
    }
    if (st.p_opened) {
        close_p();
    }
}

void print_heading(line_type lt, char *line) {
    switch(lt) {
        case H1:
            printf(H1_TAG "%s</h1><br/>\n", line + 1);
            return;
        case H2:
            printf(H2_TAG "%s</h2><br/>\n", line + 2);
            return;
        case H3:
            printf(H3_TAG "%s</h3><br/>\n", line + 3);
            return;
        default:
            fprintf(stderr, "Error parsing heading. Line type: %d\n", lt);
            exit(1);
    }
}

void found_link() {
    /* links always appear in a sort of list, so if there's
     * an ul opened, just leave it. */
    if (st.blockquote_opened) {
        close_quote();
    }
}

void parse_link(char *line, char *href, char *content) {
    for (line +=2; is_ws(*line); line++);
    for(; !is_ws(*line) && *line != '\0'; line++) {
        *href = *line;
        href++;
    }
    *href = '\0';
    if (*line == '\0') {
        *content = '\0';
        return;
    }
    for (line++; is_ws(*line); line++);
    for(; *line != '\n' && *line != '\0'; line++) {
        *content = *line;
        content++;
    }
    *content = '\0';
    return;
}

void found_li() {
    if (st.blockquote_opened) {
        close_quote();
    }
    if (!st.ul_opened) {
        puts(UL_TAG);
        st.ul_opened = 1;
    }
}

void found_quote() {
    if (st.ul_opened) {
        close_ul();
    }
    if (st.p_opened) {
        close_p();
    }
    if (!st.blockquote_opened) {
        puts(BLOCKQUOTE_TAG);
        st.blockquote_opened = 1;
    }
}

void found_pre() {
    if (st.ul_opened) {
        close_ul();
    }
    if (st.blockquote_opened) {
        close_quote();
    }
    if (st.p_opened) {
        close_p();
    }
    puts(PRE_TAG);
    st.pre_opened = 1;
}

void found_blank() {
    if (st.ul_opened) {
        close_ul();
    }
    if (st.blockquote_opened) {
        close_quote();
    }
    if (st.p_opened) {
        close_p();
    }
}

void parse(FILE *gemfile) {
    char buf[512];
    while (fgets(buf, sizeof buf, gemfile) != NULL) {
        if (st.pre_opened) {
            if (memcmp(buf, "```", 3) == 0) {
                st.pre_opened = 0;
                printf("</pre>\n");
                continue;
            }
            else {
                fputs(buf, stdout); // fputs doesn't add newline
                continue;
            }
        }
        line_type l = parse_line_type(buf);
        char href[512], cont[512];
        switch(l) {
            case TEXT:
                found_text();
                fputs(buf, stdout);
                break;
            case LINK:
                found_link();
                parse_link(buf, href, cont);
                if (strlen(cont) == 0)
                    printf(ANCHOR_TAG, href, href);
                else
                    printf(ANCHOR_TAG, href, cont);
                break;
            case H1:
            case H2:
            case H3:
                found_heading(l);
                print_heading(l, buf);
                break;
            case QUOTE:
                found_quote();
                fputs(buf, stdout);
                break;
            case PRE:
                found_pre();
                break;
            case LI:
                found_li();
                printf(LI_TAG, buf);
                break;
            case BLANK:
                found_blank();
                break;
        }
    }
}

int main() {

    print_HTTP_headers();

    // read query to obtain file name
    char* filename = getenv("QUERY_STRING");
    FILE *gemfile = fopen(filename, "r");

    if (gemfile == NULL) {
        printf("<html><body>URL not found: %s</body></html>\n", filename);
        return 0;
    }

    print_HTML_header();

    parse(gemfile);
    fclose(gemfile);

    print_HTML_footer();
    return 0;
}
