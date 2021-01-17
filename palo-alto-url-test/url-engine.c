#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <regex.h>

#define PRINT 0

/*
LIMITATIONS:

   >  We cannot have multiple * together, or else extra code (very few lines) must be added to normalize each Pattern
   >  Currently for each URL we are scanning ALL XML nodes - this can be made better by reducing the sample space [ first letter no match is no op ] 
      Hash Table will help here to reduce the sample space
   >  The Output is in a different print format where it is easier to tell which patterns match
   >  Did not get enough time to re-factor the code but I have made it as readable as possible
   >  If you want to print the DP Matrix please make PRINT 1 above. Give a small input as it can flood the screen
   >  Used help from Stackoverflow and a DP problem on youtube

IMPROVEMENTS:

   > Traverse tree once and then make it Linear search as opposed to recursive function calls to avoid Stack Overflow BUT I think max stack frames = 2
     because its not really a Tree structure, its more like LL with a child attribute that is also a LL, so at max you will hae 2 stack frames

*/

//Forward Declarations
bool pattern_match(char* url, char* pattern);
bool posix_regex(char* url, char* pattern);
void run(xmlNode* a_node, char* url, bool algo_self);

int main(int argc, char** argv) {

//=============================================== ERROR CHECKING ===============================================

    //Check if we have 4 arguments - as required
    if(argc != 4) {
        fprintf(stderr, "Error: Expect 4 arguments but got %d\n", argc);
        fprintf(stderr, "======> Usage of application is 'url-engine <algorithm(posix|self)> configuration.xml urls.txt'\n");
        return -1;
        //exit(-1); Not portable?
    }

    //Check if matching algo is either "posix" or "self"
    if(strcmp(argv[1], "posix") && strcmp(argv[1], "self")) {
        fprintf(stderr, "Error: Matching algorithm options are 'posix' or 'self', '%s' is invalid\n", argv[1]);
        return -1;
    }
    
//=============================================== PARSE FILES ===============================================

//Set the algo needed to be called
    bool algo_self = true;
    if(!strcmp(argv[1], "posix")) algo_self = false;

//Parse URLS.txt
    FILE* urls_fp;
    char* url = NULL;
    size_t len = 0; //For portability
    ssize_t read;

    urls_fp = fopen(argv[3], "r"); //URL file path is the 3rd argument. Use 'fopen' because it uses Buffered I/O so less syscalls to open
    if (urls_fp == NULL) {
        fprintf(stderr, "Error: Cannot open URL file - %s\n", argv[3]);
        return -1;
    }

//Parse Config. XML
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    if ((doc = xmlReadFile(argv[2], NULL, 0)) == NULL) {
        fprintf(stderr, "Error: Cannot Parse XML file - %s\n", argv[2]);
        return -1;
    }

    root_element = xmlDocGetRootElement(doc); //The whole file is parsed

//Read URL's Line by Line and Pattern match the whole XML [ brute force can be made better ]
    printf("\n");
    while ((read = getline(&url, &len, urls_fp)) != -1) {
        printf("\n%s", url);
        run(root_element, url, algo_self);
        printf("\n");
    }

//Cleanup
    fclose(urls_fp);
    if (url) free(url);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}

//=============================================== ITERATE URL OVER ALL XML NODES ===============================================

//Due to lack of time, using brute force approach where we iterate over the whole XML for each URL
//Can atleast reduce the sample space by NOT iterating over patterns whose first letter is not a match
//Hash table might help here

void run(xmlNode* a_node, char* url, bool algo_self) {

    xmlNode* cur_node = NULL;
    xmlChar* prop = NULL;
    xmlChar* content = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            if (!strcmp((char*)cur_node->name, "set")) {
                prop = xmlGetProp(cur_node, (xmlChar*)"id");

                printf("|____ ID: %s\n", prop);

                xmlFree(prop);
                prop = NULL; //Make NULL after you free, just good practice to avoid double free bugs
            }

            if (!strcmp((char*)cur_node->name, "pattern")) {
                content = xmlNodeGetContent(cur_node);

                //Can be optimized, do not need to check each time, can be done at init time
                bool match = algo_self ? pattern_match(url, (char*)content) : posix_regex(url, (char*)content);
                if(match) {
                    printf("      |____ %s\n", content);
                }

                xmlFree(content);
                content = NULL;
            }
        } 

        run(cur_node->children, url, algo_self);
    }
}

//=============================================== POSIX ALGORITHM ===============================================

//Util function to create substring at first occurence of 'ch'
static void substring_char(char* string, char* substr1, char* substr2, char ch) {

    int i = 0;
    int j = 0;
    int k = 0;
    bool pre_ch = true;

    while((string[i] != '\0') && (string[i] != '\n')) {

        if((pre_ch) && (string[i] == ch)) {
            pre_ch = false;
            i++;
        }

        if(pre_ch) {
            substr1[j] = string[i];
            j++;
        } else {
            substr2[k] = string[i];
            k++;
        }

        i++;
    }

    substr1[j] = '\0';
    substr2[k] = '\0';
}

//Add escape to all Special characters i.e. '.' and '/' here. For '*' make it '.*'
static void dos_to_regex(char* pattern) {

    if(!strlen(pattern)) return;

    char* temp = (char*)malloc(2*strlen(pattern) + 1); //1 for '\0'

    int i = 0;
    int k = 0;

    //Add all special cases here
    while(pattern[i] != '\0') {
        if((pattern[i] == '.') || (pattern[i] == '/')) {
            temp[k++] = '\\';
            temp[k++] = pattern[i];
        } else if(pattern[i] == '*') {
            temp[k++] = '.';
            temp[k++] = pattern[i];
        } else {
            temp[k++] = pattern[i];
        }

        i++;
    }

    temp[k] = '\0';

    strcpy(pattern, temp);

    free(temp);
}

bool posix_regex(char* url, char* pattern) {

    regex_t regex;
    int ulen = strlen(url);
    int plen = strlen(pattern);
    char buffer[100];

    //Split URL into Host and Path name
    char* host =         (char*)malloc(ulen);
    char* host_pattern = (char*)malloc(2*plen);
    char* path =         (char*)malloc(ulen);
    char* path_pattern = (char*)malloc(2*plen);

    //Substring at '/'
    substring_char(url, host, path, '/');
    substring_char(pattern, host_pattern, path_pattern, '/');

    //Convert pattern to Regex pattern
    dos_to_regex(host_pattern);
    dos_to_regex(path_pattern);

    //Run Regex 
    int reti_host = 1;
    int reti_path = 1;

    reti_host = regcomp(&regex, host_pattern, 0);
    if(reti_host) {
        regerror(reti_host, &regex, buffer, 100);
        fprintf(stderr, "Error: Host Regex init failed - %s\n", buffer);
        goto clean;
    }

    reti_host = regexec(&regex, host, 0, NULL, 0);
    if(reti_host) {
//        regerror(reti_host, &regex, buffer, 100);
//        fprintf(stderr, "Error: Host Regex pattern match failed - %s\n", buffer);
        goto clean;
    }

    regfree(&regex);

    //If path exists, 
    if(strlen(path) || strlen(path_pattern)) {

        if((strlen(path) == 0) || (strlen(path_pattern) == 0)) goto clean;

        reti_path = regcomp(&regex, path_pattern, 0); 
        if(reti_path) {
            regerror(reti_path, &regex, buffer, 100);
            fprintf(stderr, "Error: Path Regex init failed - %s\n", buffer);
            goto clean;
        }

        reti_path = regexec(&regex, path, 0, NULL, 0);
        if(reti_path) {
//           regerror(reti_path, &regex, buffer, 100);
//           fprintf(stderr, "Error: Path Regex pattern match failed - %s\n", buffer);
            goto clean;
        }

    } else {
        reti_path = 0;
    }

//Cleanup
clean:
//    printf("\n-----Pattern: %s, Reti Host: %d, Reti Path: %d\n-----host: %s, path: %s\n", pattern, reti_host, reti_path, host, path);

    free(host);
    free(host_pattern);
    free(path);
    free(path_pattern);
    regfree(&regex);

    return ((!reti_host) && (!reti_path));
}

//=============================================== SELF ALGORITHM ===============================================

//Send normalized pattern i.e. if multiple * together, make them into 1
bool pattern_match(char* url, char* pattern) {

    int ulen = strlen(url) - 1; //Because url getline array so ends in \n\0
    int plen = strlen(pattern);

    //If URL and pattern arent too big it will not Stackoverflow, since they are strings, this is a reasonable assumption!
    bool dp_matrix[ulen + 1][plen + 1];	//Using VLA's. Is that fine? Wont work on old C standard

    //Do the 0th case first
    if((url[0] != pattern[0]) && (pattern[0] != '*')) return false;
    dp_matrix[0][0] = 1;
    dp_matrix[1][1] = 1; //No need as that is the first case in Logic loop

    //Fill first column and first row - Init condition
    for(int i = 1; i <= ulen; i++) {
        dp_matrix[i][0] = 0;
    }

    for(int j = 1; j <= plen; j++) {
        dp_matrix[0][j] = 0;
    } 

    //If first char in pattern in *
    if(pattern[0] == '*') dp_matrix[0][1] = 1;

    bool hostname = true; //true until first '/'

    //DP'ing the Solution - Took idea from https://www.youtube.com/watch?v=3ZDZ-N0EPV0
    for(int i = 1; i <= ulen; i++) {
        for(int j = 1; j <= plen; j++) {

            //Core Algo/Logic - for NON Edge cases
            if(url[i - 1] == pattern[j - 1]) {
                dp_matrix[i][j] = dp_matrix[i - 1][j - 1];

                //The only point where hostname ends is if url[] == pattern[] where url[] = /
                if(hostname && (url[i - 1] == '/')) {
                    hostname = false;
                }

            } else if(pattern[j - 1] == '*') {
                if((hostname) && (url[i - 1] == '/')) { //Special handling - "dont match * to / in the hostname"
                    dp_matrix[i][j] = 0;
                    continue;
                 }
                dp_matrix[i][j] = (dp_matrix[i][j - 1] || dp_matrix[i - 1][j]);
            } else {
                dp_matrix[i][j] = 0;
            }
        }
    }

/*
Without special handling code is very simple =>

    for(int i = 1; i <= ulen; i++) {
        for(int j = 1; j <= plen; j++) {
            //Core Algo/Logic - for NON Edge cases
            if(url[i - 1] == pattern[j - 1]) {
                dp_matrix[i][j] = dp_matrix[i - 1][j - 1];
            } else if(pattern[j - 1] == '*') {
                dp_matrix[i][j] = (dp_matrix[i][j - 1] || dp_matrix[i - 1][j]);
            } else {
                dp_matrix[i][j] = 0;
            }
        }
    }
*/


#if PRINT //MAKE 1 and PRING DP MATRIX
#define SPACE "  "
    printf("\n\n   -" SPACE); //Extra 1 col
    for(int j = 0; j < plen; j++) {
        printf("%c" SPACE, pattern[j]);
    }

    for(int i = 0; i <= ulen; i++) {

        if(i == 0) printf("\n-" SPACE);
        else printf("\n%c" SPACE, url[i - 1]);

        for(int j = 0; j <= plen; j++) {
            printf("%d" SPACE, dp_matrix[i][j]);
        }
    }

    printf("<---\n\n");
#endif

    return dp_matrix[ulen][plen];
}
