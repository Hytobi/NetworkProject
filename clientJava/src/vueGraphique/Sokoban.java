package vueGraphique;

import modele.Carte;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class Sokoban {
    public static void main(String[] args) throws Exception {

        //j'ai 5 map de niveau
        int nbNiveau = 5;
        int i=-1;
        int nbMouvTotal=0;

        //tant qu'on a pas fait tous les niveaux
        while (i!=nbNiveau){
            if (i==-1){
                Intro intro = new Intro();
                while (!(intro.getCommencer())) {}
                intro.dispose();
            } else {
                Carte jeu = new Carte(i);
                VueSokoban vs = new VueSokoban(jeu);
                //Bug du restart, du coup j'ai mis un nombre maximum de restart autorisé
                while ((!jeu.finDePartie()) && (!(jeu.getNbRestart() == 1500))) {
                }
                //Si on a fait 1500 restart on recommence a 0
                if (jeu.getNbRestart() == 1500) i = -1;
                nbMouvTotal += jeu.getNbMouvement();
                //Si on est pas au dernier niveau on ferme juste la fenetre
                if (i < nbNiveau - 1) {
                    vs.dispose();
                } else {
                    //Sinon on affiche le message de fin
                    vs.messageFin(i + 1, nbMouvTotal);
                    //Et on désactive les boutons
                    vs.desactiveAction();
                }
            }
            i++;
        }
    }
}
