
#include "cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

char* getJsonScan() {
    char* jsonString = "{\"message\":\"scan\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
}


