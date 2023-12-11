package dto;
import modele.Player;

public class Update {
    private String player;
    private String dir;
    private Player playermodif;

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

    public Player getPlayermodif() {
        return playermodif;
    }

    public void setPlayermodif(Player playermodif) {
        this.playermodif = playermodif;
    }
}
