package dto;

/**
 * AttackBomb : Objet notifiant que la bombe a été posée (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

import modele.Player;

public class AttackBomb extends ResponseApi {
    private Player player;

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }
}
