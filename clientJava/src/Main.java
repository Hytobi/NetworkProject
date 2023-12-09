import java.io.IOException;
import java.net.InetAddress;
import java.util.List;
import java.util.Scanner;
import vueGraphique.*;
import api.TcpClient;
import api.UdpBroadcastClient;

public class Main {
    public static void main(String[] args) {

        Bomber bomber = new Bomber();
        try {
            bomber.start();
        } catch (Exception e) {
            System.err.println("Erreur lors du lancement du jeu " + e.getMessage());
        }

        /*
        for (;;){
            //TODO Gerer quand le serveur s'arrete
            tcp.post(JsonConnection.msgSearch());
            System.out.println("Message reçu : "+tcp.get());
            
        }*/

    }
}
