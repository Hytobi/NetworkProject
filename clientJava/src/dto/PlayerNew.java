package dto;

/**
 * PlayerNew : Objet qui fait spawn un joueur (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class PlayerNew {
    private String player;
    private String pos;

    public String getPlayer() {
        return player;
    }

    public String getPos() {
        return pos;
    }

    public void setPlayer(String player) {
        this.player = player;
    }

    public void setPos(String pos) {
        this.pos = pos;
    }
}
