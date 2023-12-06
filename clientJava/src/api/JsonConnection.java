package api;

import dto.Game;

public class JsonConnection {
    public static final String RES_ATTENDU = "hello i'm bomberstudent server";

    public static String msgSearch(){
        return "looking for bomberstudent servers";
    }

    public static String msgConnect(){
        return "I want to connect to you";
    }

    public static String getMapList(){
        return "GET maps/list";
    }

    public static  String getGameList(){
        return "GET game/list";
    }

    public static String postGameCreate(Game game){
        return "POST game/create\n" + game.toString();
    }

    public static String postGameJoin(Game game){
        return "POST game/join\n" + game.toString();
    }
    
}
