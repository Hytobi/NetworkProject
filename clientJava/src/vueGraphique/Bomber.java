package vueGraphique;

import modele.Carte;
import api.*;
import dto.*;
import modele.Player;

/**
 * Bomber : Class principale du jeu
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class Bomber {

    public static void main(String[] args) throws Exception {

        // j'ai 5 map de niveau
        int nbNiveau = 5;
        int i = -1;
        int nbMouvTotal = 0;

        Intro intro = new Intro();
        while (!(intro.getCommencer())) {
            Thread.sleep(100);
        }
        intro.dispose();

        Carte jeu = new Carte(intro.getMyName(), intro.getResGameJoin(), intro.getConfigTouch());
        VueBomber vs = new VueBomber(jeu, intro.getTcp());
        String recu = null;
        String namePlayerForMoveNotMe;
        while ((!jeu.finDePartie())) {
            recu = intro.getTcp().get();
            // System.out.println("Message reçu : " + recu);
            if (recu != null && !recu.isBlank()) {
                String[] msgs = recu.split("EOJ");
                namePlayerForMoveNotMe = null;
                for (String msg : msgs) {
                    System.out.println("Message : " + msg);
                    // si le message commence par POST, on split le message pour récupérer les infos
                    if (msg.startsWith("POST")) {
                        String json = "{" + msg.split("\\{")[1];
                        if (msg.contains("player/position/update")) {
                            try {
                                Update update = MapperRes.fromJson(json, Update.class);
                                if (update != null) {
                                    String item = jeu.robotSeDeplace(update);
                                    if (item != null) {
                                        System.out.println("Notifie le serveur que le joueur a ramassé un item");
                                        intro.getTcp().post(JsonJouer.postObjectNew(item));
                                    }
                                    namePlayerForMoveNotMe = update.getPlayer();
                                } else {
                                    System.out.println("Erreur lors de la récupération du move");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de l'update" + e);
                            }
                        } else if (msg.contains("attack/newbomb")) {
                            try {
                                AttackNewBomb anb = MapperRes.fromJson(json, AttackNewBomb.class);
                                if (anb != null) {
                                    jeu.setABomb(anb);
                                } else {
                                    System.out.println("Erreur lors de la récupération de AttackNewBomb");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de AttackNewBomb");
                            }
                        } else if (msg.contains("attack/explose")) {
                            try {
                                AttackExplose ae = MapperRes.fromJson(json, AttackExplose.class);
                                if (ae != null) {
                                    jeu.explose(ae);
                                } else {
                                    System.out.println("Erreur lors de la récupération de AttackExplose");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de AttackExplose");
                            }
                        } else if (msg.contains("attack/affect")) {
                            try {
                                Player p = MapperRes.fromJson(json, Player.class);
                                if (p != null) {
                                    jeu.updateMyPlayer(p);
                                } else {
                                    System.out.println("Erreur lors de la récupération de Player");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de Player");
                            }
                        } else if (msg.contains("attack/mineExplose")) {
                            try {
                                MineExplose me = MapperRes.fromJson(json, MineExplose.class);
                                if (me != null) {
                                    jeu.exploseMine(me);
                                } else {
                                    System.out.println("Erreur lors de la récupération de MineExplose");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de MineExplose");
                            }
                        } else if (msg.contains("player/new")) {
                            try {
                                PlayerNew playerNew = MapperRes.fromJson(json, PlayerNew.class);
                                if (playerNew != null) {
                                    jeu.setNewPlayer(playerNew);
                                } else {
                                    System.out.println("Erreur lors de la récupération du nouveau player");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de PlayerNew");
                            }
                        }
                    } else {
                        if (msg.contains("attack/bomb")) {
                            try {
                                AttackBomb res = MapperRes.fromJson(msg, AttackBomb.class);
                                if (res != null) {
                                    jeu.updateMyPlayer(res.getPlayer());
                                } else {
                                    System.out.println("Erreur lors de la récupération de AttackBomb");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de AttackBomb");
                            }
                        } else if (msg.contains("object/new")) {
                            try {
                                ObjectNew res = MapperRes.fromJson(msg, ObjectNew.class);
                                if (res != null) {
                                    jeu.updateMyPlayer(res.getPlayer());
                                } else {
                                    System.out.println("Erreur lors de la récupération de ObjectNew");
                                }
                            } catch (Exception e) {
                                System.out.println("Erreur lors du mapper de ObjectNew");
                            }
                        }
                    }
                }

                // Info du joueur mis a jour
                vs.updateGameInfos();
                // On met à jour la vue
                vs.parcourDesMAJ(namePlayerForMoveNotMe);
            } else {
                System.out.println("Connection au serveur perdue");
                vs.serveurRageQuit();
            }
        }

        vs.messageFin();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
        }

        vs.dispose();
    }
}
