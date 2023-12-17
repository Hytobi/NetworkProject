package dto;

/**
 * ObjectNew : Objet reponse nouvelle statistique du joueur (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

import modele.Player;

public class ObjectNew extends ResponseApi {
    private Player player;

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }
}
