/*
 * Lab 2: Regular Expression Validator using DFA
 * Course: COMP 409 - Compiler Design
 * RE: (a(b+a)*b) + (b(a+b)*a)
 *
 * Approach: Simulate a DFA with 5 states.
 *
 *  State 0 (START)  : initial state
 *  State 1          : read 'a' as first char → expecting middle or final 'b'
 *  State 2          : read 'b' as first char → expecting middle or final 'a'
 *  State 3 (ACCEPT) : last char satisfies the RE
 *  State 4 (DEAD)   : invalid character encountered
 *
 * Transitions:
 *  S0 --a--> S1
 *  S0 --b--> S2
 *  S1 --a--> S1   (middle 'a', still in branch 1)
 *  S1 --b--> S3   (could be end or middle of branch 1)
 *  S3 --b--> S3   (another 'b' mid-string, still possible end)
 *  S3 --a--> S4   (branch 1 broken: 'a' after possible end 'b')
 *  S2 --b--> S2   (middle 'b', still in branch 2)
 *  S2 --a--> S3   (could be end or middle of branch 2)
 *  S3 --a--> S3   (another 'a' mid-string for branch 2)
 *  S3 --b--> S4   (branch 2 broken)
 *  Any state --other--> S4
 *
 * NOTE: The DFA above is a simplification; the actual accept condition
 * is checked by tracking which branch we entered and what the last char was.
 * See implementation below.
 */

#include <stdio.h>

/* Custom string length - no <string.h> */
int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

/* Custom input reader - no scanf format tricks, reads char by char */
void read_input(char *buf, int maxlen) {
    int i = 0;
    char c;
    while (i < maxlen - 1) {
        c = getchar();
        if (c == '\n' || c == '\r' || c == ' ') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
}

/*
 * DFA simulation for RE: (a(b+a)*b) + (b(a+b)*a)
 *
 * States:
 *   0 = start
 *   1 = started with 'a' (branch 1 active)
 *   2 = started with 'b' (branch 2 active)
 *   3 = dead / invalid character
 *
 * Accept condition after full scan:
 *   branch 1: started 'a', ended 'b'
 *   branch 2: started 'b', ended 'a'
 */
int validate(char *s) {
    int len = my_strlen(s);

    /* Reject epsilon */
    if (len == 1 && s[0] == 'e') return 0;

    /* Minimum length 2 */
    if (len < 2) return 0;

    /* DFA: determine start branch and scan for invalid chars */
    int branch; /* 1 = started 'a', 2 = started 'b' */

    if (s[0] == 'a')      branch = 1;
    else if (s[0] == 'b') branch = 2;
    else                   return 0; /* invalid first char */

    /* Scan middle characters (indices 1 .. len-2) */
    int i;
    for (i = 1; i < len - 1; i++) {
        if (s[i] != 'a' && s[i] != 'b') return 0;
    }

    /* Check last character based on branch */
    char last = s[len - 1];
    if (last != 'a' && last != 'b') return 0;

    if (branch == 1 && last == 'b') return 1; /* a ... b */
    if (branch == 2 && last == 'a') return 1; /* b ... a */

    return 0;
}

int main() {
    char input[200];

    printf("Regular Expression: (a(b+a)*b) + (b(a+b)*a)\n");
    printf("----------------------------------------------\n");
    printf("Enter string (a/b only, 'e' for epsilon): ");

    read_input(input, 200);

    if (validate(input))
        printf("VALID\n");
    else
        printf("INVALID\n");

    return 0;
}