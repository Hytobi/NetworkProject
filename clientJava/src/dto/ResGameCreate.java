package dto;

import modele.Player;

public class ResGameCreate extends ResponseApi {
    private int nbPlayers;
    private int  mapId;
    private String startPos;
    private Player player;
    private MapInfo startingMap;

    public int getNbPlayers() {
        return nbPlayers;
    }

    public int getMapId() {
        return mapId;
    }

    public String getStartPos() {
        return startPos;
    }

    public Player getPlayer() {
        return player;
    }

    public MapInfo getStartingMap() {
        return startingMap;
    }

    public void setNbPlayers(int nbPlayers) {
        this.nbPlayers = nbPlayers;
    }

    public void setMapId(int mapId) {
        this.mapId = mapId;
    }

    public void setStartPos(String startPos) {
        this.startPos = startPos;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public void setStartingMap(MapInfo startingMap) {
        this.startingMap = startingMap;
    }

    public String toString(){
        StringBuffer sb = new StringBuffer();
        sb.append("{\"nbPlayer\":\"");
        sb.append(nbPlayers);
        sb.append("\",\"gameId\":\"");
        sb.append(mapId);
        sb.append("\",\"startPos\":\"");
        sb.append(startPos);
        sb.append("\",\"player\":");
        sb.append(player.toString());
        sb.append(",\"startingMap\":\"");
        sb.append(startingMap.toString());
        sb.append("}");
        return sb.toString();
    }
}
