package api;

import dto.Game;

public class JsonConnection {

    public static String msgSearch(){
        return "looking for bomberstudent servers\0";
    }

    public static String getterMapList(){
        return "GET maps/list";
    }

    public static String getterGameList(){
        return "GET game/list";
    }

    public static String postGameCreate(Game game){
        return "POST game/create\n" + game.toString();
    }

    public static String postGameJoin(Game game){
        return "POST game/join\n" + game.toString();
    }
    
}
