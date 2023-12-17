package dto;

/**
 * MapInfo : Objet contenant les information d'une game (envoyé par le
 * serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

public class MapInfo {
    private int id;
    private int width;
    private int height;
    private String content;

    public int getId() {
        return id;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public String getContent() {
        return content;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public void setContent(String content) {
        this.content = content;
    }

    @Override
    public String toString() {
        return "{" +
                " id='" + getId() + "'" +
                ", width='" + getWidth() + "'" +
                ", height='" + getHeight() + "'" +
                ", content='" + getContent() + "'" +
                "}";
    }

}
