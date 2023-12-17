package dto;

import modele.Player;

/**
 * Update : Objet notifiant le client qu'un joueur s'est déplacé (envoyé par le
 * serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

public class Update {
    private String player;
    private String dir;

    public String getPlayer() {
        return player;
    }

    public String getDir() {
        return dir;
    }

    public void setPlayer(String player) {
        this.player = player;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }
}
