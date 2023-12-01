/** @author: PLOUVIN Patrice
 * @date: 24/11/2022
 * @brief: Ferme correctement le programme s'il y a une erreur dans la SDL
 * @file: src/rage.h
 */

#ifndef RAGE_H
#define RAGE_H

#define EXIT_IF_FAIL(x,msg) if (x == -1) { perror(msg); exit(EXIT_FAILURE); }

#endif