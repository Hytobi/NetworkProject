//
// Created by hana on 01/12/23.
//

#ifndef PROJECT_ERR_H
#define PROJECT_ERR_H

#define ERR -1

#define PERROR(msg,value) perror(msg), exit(value)

#define EXIT_IF_FAIL(x,msg) if (x == -1) { perror(msg); exit(EXIT_FAILURE); }
#define CONTINUE_IF_FAIL(x,msg) if (x == -1) { perror(msg); continue; }
#define BREAK_IF_FAIL(x,msg) if (x == -1) { perror(msg); break; }

#endif //PROJECT_ERR_H
