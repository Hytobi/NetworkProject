package api;

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
    private Socket socket;
    private final int bufferSize = 4096;

    public TcpClient(String server) {
        this.server = server;
    }

    public void closeSocket() throws IOException {
        reader.close();
        writer.close();
        socket.close();
    }

    public void tcpConnect() throws IOException {

        // Boucle pour se connecter à chaque serveur
        System.out.println("Tentative de connexion au serveur : "+server);
        socket = new Socket(InetAddress.getByName(server), port);
            // Flux de lecture et écriture pour la communication avec le serveur
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            writer = new PrintWriter(socket.getOutputStream(), true);
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
        char[] response = new char[bufferSize];
        try {
            reader.read(response,0,bufferSize);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        String res = null;
        try {
            res = new String(response).split("\0")[0];
        } catch (Exception e) {
            System.out.println("Erreur lors de la lecture du message");
        }
        return res;
    }

    public String getServer() {
        return server;
    }
}
