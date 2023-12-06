import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class TcpClient {
    private BufferedReader reader;
    private PrintWriter writer;
    private final int port = 42069;
    private String server;

    public TcpClient(String server) {
        this.server = server;
    }

    public void tcpConnect() {

        // Boucle pour se connecter à chaque serveur
        System.out.println("Tentative de connexion au serveur : "+server);
        try (Socket socket = new Socket(InetAddress.getByName(server), port)) {
            // Flux de lecture et écriture pour la communication avec le serveur
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new PrintWriter(socket.getOutputStream(), true);

            System.out.println("Connexion Réussie !");


        } catch (IOException e) {
            System.err.println("Erreur lors de la connexion au serveur TCP (" + server + "): " + e.getMessage());
        }
    }

    /**
     * Envoi un message sur le socket tcp
     * @param str
     */
    public void post(String str){
        // Envoi d'une requête au serveur
        writer.println(str);
    }

    /**
     * lis un message sur le socket tcpConnect
     */
    public String get(){
        // Lecture de la réponse du serveur
        String response = null;
        try {
            response = reader.readLine();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return response;
    }
}
