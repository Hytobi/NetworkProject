package dto;

public class PlayerNew {
    private String name;
    private String pos;

    public PlayerNew() {
    }

    public PlayerNew(String name, String pos) {
        this.name = name;
        this.pos = pos;
    }

    public String getName() {
        return name;
    }

    public String getPos() {
        return pos;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setPos(String pos) {
        this.pos = pos;
    }
}
