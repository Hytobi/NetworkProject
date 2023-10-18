/** @author: PLOUVIN Patrice
 * @date: 02/12/2022
 * @brief: Fichier principale du programme des tests
 * @file: test/main.c
 */

#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "jouer_coup/test_jouer.h"
#include "jouer_coup/test_jouer_fin.h"
#include "jouer_coup/test_jouer_ia.h"

/**
 * @brief Fonction principale du programme de test
 *
 * @return 0
 */
int main() {
    // Initialisation du registre de test
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    // Enregistrement des suites de test
    CU_ErrorCode error =
        CU_register_nsuites(3, getTestJouerSuites(), getTestJouerFinSuites(),
                            getTestJouerIaSuites());

    // Si une erreur est survenue, on quitte
    if (error != CUE_SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // Lancement des tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return EXIT_SUCCESS;
}