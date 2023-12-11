package dto;
import modele.Player;

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
