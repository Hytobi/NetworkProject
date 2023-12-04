package api;

import dto.Game;

public class JsonConnection {

    public String msgSearch(){
        return "looking for bomberstudent servers";
    }

    public String getterMapList(){
        return "GET maps/list";
    }

    public String getterGameList(){
        return "GET game/list";
    }

    public String postGameCreate(Game game){
        return "POST game/create\n" + game.toString();
    }

    public String postGameJoin(Game game){
        return "POST game/join\n" + game.toString();
    }
    
}
