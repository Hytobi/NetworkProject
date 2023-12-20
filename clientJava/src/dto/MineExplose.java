package dto;

/**
 * MineExplose : Objet notifiant que la mine a explosé (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class MineExplose {
    private String pos;

    public String getPos() {
        return pos;
    }

    public void setPos(String pos) {
        this.pos = pos;
    }
}
