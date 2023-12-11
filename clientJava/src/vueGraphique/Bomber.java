package vueGraphique;

import modele.Carte;
import api.*;
import dto.*;
import modele.Player;

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
        String msg = null;
        while ((!jeu.finDePartie())) {
            msg = intro.getTcp().get();
            if (msg != null && !msg.isBlank()){
                // si le message commence par POST, on split le message pour récupérer les infos
                if (msg.startsWith("POST")) {
                    String json = "{" + msg.split("\\{")[1];
                    if (msg.contains("player/position/update")){
                        Update update = MapperRes.fromJson(json, Update.class);
                        if (update != null){
                            String item = jeu.robotSeDeplace(update);
                            if (item != null){
                                intro.getTcp().post(JsonJouer.postObjectNew(item));
                            }
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    } else if (msg.contains("attack/newbomb")){
                        AttackNewBomb anb = MapperRes.fromJson(json, AttackNewBomb.class);
                        if (anb != null){
                            jeu.setABomb(anb);
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    } else if (msg.contains("attack/explose")){
                        AttackExplose ae = MapperRes.fromJson(json, AttackExplose.class);
                        if (ae != null){
                            jeu.explose(ae);
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    } else if (msg.contains("attack/affect")){
                        Player p = MapperRes.fromJson(json, Player.class);
                        if (p != null){
                            jeu.updateMyPlayer(p);
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    }
                } else {
                    if (msg.contains("attack/bomb")){
                        AttackBomb res = MapperRes.fromJson(msg, AttackBomb.class);
                        if (res != null){
                            jeu.updateMyPlayer(res.getPlayer());
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    } else if (msg.contains("object/new")){
                        ObjectNew res = MapperRes.fromJson(msg, ObjectNew.class);
                        if (res != null){
                            jeu.updateMyPlayer(res.getPlayer());
                        } else {
                            System.out.println("Erreur lors de la récupération du move");
                        }
                    }
                }
                // Info du joueur mis a jour
                vs.updateGameInfos();
                //On met à jour la vue
                vs.parcourDesMAJ();
            }
        }
        
        vs.messageFin();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {}

        vs.dispose();
    }
}
