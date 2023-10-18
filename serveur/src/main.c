/** @author: PLOUVIN Patrice, DELCOURT Hana
 * @date: 18/10/2023
 * @brief: Fonction principale du programme
 * @file: src/main.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "cJSON/cJSON.h"

int main(int arvc, char** argv) {
    printf("\033[H\033[2J");
    char* jsonString = "{\"name\":\"John\", \"age\":30, \"car\":null}";
    cJSON *json = cJSON_Parse( jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char *out = cJSON_Print(json);
    printf("%s\n", out);
    free(out);
    cJSON_Delete(json);
    return EXIT_SUCCESS;
}