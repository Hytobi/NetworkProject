
#include "cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>

char* getJsonScan() {
    char* jsonString = "{\"path\":\"scan\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* getJsonConnect() {
    char* jsonString = "{\"path\":\"connect\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* getMap() {
    char* jsonString = "{\"path\":\"Maps/list\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* getJsonPathMap(int mapId){
    char* jsonString = "{\"path\":\"map\", \"mapId\":\"%d\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* getJsonEnterGame(int mapId, int gameId){
    char* jsonString = "{\"path\":\"enter\", \"mapId\":\"%d\", \"gameId\":\"%d\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* getJsonCreateGame(int mapId){
    char* jsonString = "{\"path\":\"create\", \"mapId\":\"%d\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}

char* move(int x, int y){
    char* jsonString = "{\"path\":\"move\", \"x\":\"%d\", \"y\":\"%d\"}";
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(EXIT_FAILURE);
    }
    char* jsonFormatted = cJSON_Print(json);
    return jsonFormatted;
}