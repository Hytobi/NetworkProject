
#ifndef API_H
#define API_H

#define PORT 42069

char* post(char* ip, char* path, char* data);
char* get(char* ip, char* path);

#endif