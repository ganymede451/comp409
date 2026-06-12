#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* ---- keyword table ---- */
static const char *KEYWORDS[] = {
    "int","float","char","double","void",
    "if","else","while","for","return",
    "break","continue","struct","typedef",NULL
};

static int isKeyword(const char *s) {
    for (int i = 0; KEYWORDS[i]; i++)
        if (strcmp(s, KEYWORDS[i]) == 0) return 1;
    return 0;
}

/* ---- two-character operator check ---- */
static int isTwoCharOp(char a, char b) {
    char pair[3] = {a, b, '\0'};
    const char *two[] = {"==","!=","<=",">=","&&","||","++","--",NULL};
    for (int i = 0; two[i]; i++)
        if (strcmp(pair, two[i]) == 0) return 1;
    return 0;
}

int main(void) {
    FILE *fp = fopen("File.txt", "r");
    if (!fp) { fprintf(stderr, "Cannot open File.txt\n"); return 1; }

    printf("\n%-20s %-15s\n", "LEXEME", "TOKEN");
    printf("%.35s\n", "-----------------------------------");

    int ch;
    char buf[256];
    int  idx;

    while ((ch = fgetc(fp)) != EOF) {

        /* whitespace: skip */
        if (isspace(ch)) continue;

        /* alpha-start: keyword or identifier */
        if (isalpha(ch) || ch == '_') {
            idx = 0;
            do { buf[idx++] = (char)ch; ch = fgetc(fp); }
            while (ch != EOF && (isalnum(ch) || ch == '_'));
            buf[idx] = '\0';
            if (ch != EOF) ungetc(ch, fp);

            printf("%-20s %-15s\n", buf,
                   isKeyword(buf) ? "KEYWORD" : "IDENTIFIER");
            continue;
        }

        /* digit-start: integer or real number */
        if (isdigit(ch)) {
            idx = 0;
            int hasDot = 0;
            do {
                buf[idx++] = (char)ch;
                ch = fgetc(fp);
                if (ch == '.' && !hasDot) {
                    hasDot = 1;
                    buf[idx++] = (char)ch;
                    ch = fgetc(fp);
                }
            } while (ch != EOF && isdigit(ch));
            buf[idx] = '\0';
            if (ch != EOF) ungetc(ch, fp);

            printf("%-20s %-15s\n", buf, "NUMBER");
            continue;
        }

        /* operator characters: single or double */
        if (strchr("+-*/%=<>!&|^~", ch)) {
            int nx = fgetc(fp);
            if (nx != EOF && isTwoCharOp((char)ch, (char)nx)) {
                buf[0] = (char)ch; buf[1] = (char)nx; buf[2] = '\0';
            } else {
                buf[0] = (char)ch; buf[1] = '\0';
                if (nx != EOF) ungetc(nx, fp);
            }
            printf("%-20s %-15s\n", buf, "OPERATOR");
            continue;
        }

        /* delimiters */
        if (strchr(";,(){}[]", ch)) {
            buf[0] = (char)ch; buf[1] = '\0';
            printf("%-20s %-15s\n", buf, "DELIMITER");
            continue;
        }

        /* unknown */
        buf[0] = (char)ch; buf[1] = '\0';
        printf("%-20s %-15s\n", buf, "UNKNOWN");
    }

    printf("%.35s\n", "-----------------------------------");
    fclose(fp);
    return 0;
}