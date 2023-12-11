package dto;

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
