package vueTexte;

import modele.*;

/**
 * Classe Main qui lance le Sokoban.
 * @author PLOUVIN Patrice
 */
public class SokobanTexte{
    public static void main(String[] args) throws Exception {
        Carte jeu = new Carte(0);
        System.out.println(jeu.toString());
        ModeTexte mode = new ModeTexte(jeu);
        while (!jeu.finDePartie()){
            System.out.println("Entrer une direction : ");
            char c = mode.dialogue();
            System.out.println("\n");
            jeu.robotSeDeplace(c);
            System.out.println(jeu.toString());
        }
        System.out.println("Vous avez gagné !");
    }
}