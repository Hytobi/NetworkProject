package vueTexte;

import modele.*;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class ModeTexte{
    private char direc;

    /**Méthode qui lit l'entrée du clavier tant que l'entré n'est pas valide
     * @return la direction z,q,s ou d
     */
    public char dialogue(){
        do{
            direc = Outil.lireCaractere();
        } while ((direc !='z') && (direc !='s') && (direc != 'q') && (direc !='d'));
        return direc;
    }
}