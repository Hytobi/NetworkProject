package dto;

import java.util.List;
public class ResMapList extends ResponseApi{
    private int nbMapsList;
    private List<MapInfo> maps;

    public int getNbMapsList() {
        return nbMapsList;
    }

    public List<MapInfo> getMaps() {
        return maps;
    }

    public void setNbMapsList(int nbMapsList) {
        this.nbMapsList = nbMapsList;
    }

    public void setMaps(List<MapInfo> maps) {
        this.maps = maps;
    }
}
