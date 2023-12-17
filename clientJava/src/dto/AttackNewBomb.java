package dto;

/**
 * AttackNewBomb : Objet notifiant le placement d'une bombe (envoyé par le
 * client)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

public class AttackNewBomb {
    private String pos;
    private String type;

    public String getPos() {
        return pos;
    }

    public void setPos(String pos) {
        this.pos = pos;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
}
