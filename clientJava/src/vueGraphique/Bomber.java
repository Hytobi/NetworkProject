package vueGraphique;

import modele.Carte;
import api.TcpClient;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class Bomber {

    public static void main(String[] args) throws Exception {

        //j'ai 5 map de niveau
        int nbNiveau = 5;
        int i=-1;
        int nbMouvTotal=0;

        Intro intro = new Intro();
        while (!(intro.getCommencer())) {
            Thread.sleep(100);
        }
        intro.dispose();


        Carte jeu = new Carte(intro.getMyName(), intro.getResGameJoin());
        VueBomber vs = new VueBomber(jeu, intro.getTcp());
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
}
