/** @author: PLOUVIN Patrice, PLOUVIN Patrick
 * @date: 02/12/2022
 * @brief: Fait les tests sur les fonctions de src/jouer_coup/jouer.c
 * @file: test/jouer_coup/test_jouer.h
 */

#ifndef TEST_JOUER_H
#define TEST_JOUER_H
#include <CUnit/Basic.h>

/**
 * @brief Récupère les suites de test de src/jouer_coup/jouer.c
 * @return CU_SuiteInfo*
 */
CU_SuiteInfo* getTestJouerSuites();

#endif