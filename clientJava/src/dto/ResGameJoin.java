package dto;

import java.util.List;

import modele.Player;

public class ResGameJoin extends ResGameCreate{
    private List<Player> players;

    public List<Player> getPlayers() {
        return players;
    }

    public void setPlayers(List<Player> player) {
        this.players = player;
    }
}
