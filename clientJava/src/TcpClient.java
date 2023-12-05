import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class TcpClient {
    private final int port = 42069;

    public TcpClient() {
    }

    public void tcpConnect(String server) {

        // Boucle pour se connecter à chaque serveur
        try (Socket socket = new Socket(InetAddress.getByName(server), port)) {
            // Flux de lecture et écriture pour la communication avec le serveur
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter writer = new PrintWriter(socket.getOutputStream(), true);

            // Envoi d'une requête au serveur
            writer.println("Hello from TCP client!");

            // Lecture de la réponse du serveur
            String response = reader.readLine();
            System.out.println("Réponse du serveur TCP (" + server + "): " + response);
        } catch (IOException e) {
            System.err.println("Erreur lors de la connexion au serveur TCP (" + server + "): " + e.getMessage());
        }
    }
}
