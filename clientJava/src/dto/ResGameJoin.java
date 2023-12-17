package dto;

/**
 * ResGameJoin : Objet reponse de connection a une game en cours (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

import java.util.List;

import modele.Player;

public class ResGameJoin extends ResGameCreate {
    private List<Player> players;

    public List<Player> getPlayers() {
        return players;
    }

    public void setPlayers(List<Player> player) {
        this.players = player;
    }
}
