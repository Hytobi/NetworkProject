package vueGraphique;

import modele.Carte;
import api.TcpClient;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class Bomber {

    public Bomber() {}

    public void start() throws Exception {

        //j'ai 5 map de niveau
        int nbNiveau = 5;
        int i=-1;
        int nbMouvTotal=0;

        Intro intro = new Intro();
        while (!(intro.getCommencer())) {}
        intro.dispose();

        /*
        //tant qu'on a pas fait tous les niveaux
        while (i!=nbNiveau){
            if (i==-1){
                break;
            } else {
                Carte jeu = new Carte(i);
                VueBomber vs = new VueBomber(jeu);
                //Bug du restart, du coup j'ai mis un nombre maximum de restart autorisé
                while ((!jeu.finDePartie())) {
                }
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
        }*/
    }
}
