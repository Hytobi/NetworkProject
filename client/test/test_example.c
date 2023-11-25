/** @author: PLOUVIN Patrice
 * @date: 02/12/2022
 * @brief: Fait les tests sur les fonctions de src/example.c
 * @file: test/example.c
 */

#include "test_jouer.h"

#include "../src/example.h"

/**
 * @brief Teste la fonction chercheLigne
 */
void test_chercheLigne(void) {
    // On teste avec un tableau vide
    Player p[NB_LIGNES][NB_COLONNES] = VIDE;

    // On commence par remplir la ligne du bas
    // On teste jusqu'a ce que les colonnes soient pleines et au dela
    for (int l = NB_LIGNES - 1; l > -1; l--) {
        // Pour chaque colonne
        for (int c = 0; c < NB_COLONNES; c++) {
            // on teste si la ligne est bien l
            CU_ASSERT_EQUAL(chercheLigne(p, c), l);
            // On met un pion dans la case pour la prochaine itération
            p[l][c] = CROIX;
        }
    }
}

// Tableau des tests
static CU_TestInfo test_array[] = {{" test_chercheLigne ", test_chercheLigne},
                                   CU_TEST_INFO_NULL};

// Tableau des suites
static CU_SuiteInfo suites[2] = {
    {"suite_jouer", NULL, NULL, NULL, NULL, test_array}, CU_SUITE_INFO_NULL};

/**
 * @brief Récupère les suites de test de src/jouer_coup/jouer.c
 * @return CU_SuiteInfo*
 */
CU_SuiteInfo* getTestJouerSuites() { return suites; }