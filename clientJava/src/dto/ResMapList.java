package dto;

import java.util.List;
public class ResMapList extends ResponseApi{
    private Integer nbMapsList;
    private List<MapInfo> maps;

    public Integer getNbMapsList() {
        return nbMapsList;
    }

    public List<MapInfo> getMaps() {
        return maps;
    }

    public void setNbMapsList(Integer nbMapsList) {
        this.nbMapsList = nbMapsList;
    }

    public void setMaps(List<MapInfo> maps) {
        this.maps = maps;
    }

    public String toString(){
        StringBuffer sb = new StringBuffer();
        sb.append("{\"action\":\"");
        sb.append(getAction());
        sb.append("\",\"statut\":\"");
        sb.append(getStatut());
        sb.append("\",\"message\":\"");
        sb.append(getMessage());
        sb.append("\",\"nbMapsList\":\"");
        sb.append(nbMapsList);
        sb.append("\",\"maps\":[");
        for (MapInfo map : maps) {
            sb.append(map.toString());
            sb.append(",");
        }
        sb.deleteCharAt(sb.length()-1);
        sb.append("]}");
        return sb.toString();
    }
}
