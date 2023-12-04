package dto;

public class Game {
    private String name;
    private String mapId;

    public Game(String name) {
        this.name = name;
    }

    public Game(String name, String mapId) {
        this.name = name;
        this.mapId = mapId;
    }

    public String getName() {
        return name;
    }

    public String getMapId() {
        return mapId;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setMapId(String mapId) {
        this.mapId = mapId;
    }

    public String toString(){
        StringBuffer sb = new StringBuffer();
        sb.append("{\"name\":\"");
        sb.append(name);
        if (mapId != null){
            sb.append("\",\"mapId\":\"");
            sb.append(mapId);
        }
        sb.append("\"}");
        return sb.toString();
    }
}
