/** @author: PLOUVIN Patrice
 * @date: 24/11/2022
 * @brief: Ferme correctement le programme s'il y a une erreur dans la SDL
 * @file: src/rage.h
 */

#ifndef RAGE_H
#define RAGE_H


/**
 * @brief Ferme correctement le programme s'il y a une erreur dans la SDL
 */
#define RAGE_QUIT(ui, msg)                                         \
    {                                                              \
        fprintf(stderr, "Erreur %s : %s\n", msg, SDL_GetError());  \
        if (NULL != ui.renderer) SDL_DestroyRenderer(ui.renderer); \
        if (NULL != ui.window) SDL_DestroyWindow(ui.window);       \
        if (NULL != ui.buttons) free(ui.buttons);                  \
        if (!strcmp(ui.path, "")) free(ui.path);                   \
        SDL_Quit();                                                \
        exit(EXIT_FAILURE);                                        \
    }

#endif