package api;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.NetworkInterface;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.Objects;

public class Api {
    
    /**
     * Api pour du reseau, le serveur ecoute le port 58968, on lui envoi le mdg en param
     * @param ipServeur
     * @param msg
     * @return
     */
    public static void post(String ipServeur, String msg){
        // Creation d’un socket client
        Socket socket = null;
        try {
            socket = new Socket(ipServeur, 42069);
            // Creation d’un flux de sortie pour envoyer des donnees au serveur
            OutputStream os = socket.getOutputStream();
            // Creation d’un flux d’entree pour recevoir des donnees du serveur
            InputStream is = socket.getInputStream();
            // Creation d’un flux de sortie pour envoyer des donnees au serveur
            PrintWriter pw = new PrintWriter(os, true);
            // Creation d’un flux d’entree pour recevoir des donnees du serveur
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            // Envoi d’un message au serveur
            pw.println(msg);
            // Reception d’un message envoye par le serveur
            String message_distant = br.readLine();
            System.out.println("Message recu : " + message_distant);
            // Fermeture des flux et des sockets
            pw.close();
            br.close();
            socket.close();
        } catch (UnknownHostException e) {
            System.err.println("Erreur : l’adresse du serveur est inconnue.");
        } catch (IOException e) {
            System.err.println("Erreur : pas de connexion ? [" + e + "]");
        }
    }

    // methode qui envoie un message a tout les appareils du reseau
    public static void broadcast(String msg){
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface networkInterface = interfaces.nextElement();
                if (networkInterface.isLoopback() || !networkInterface.isUp()) {
                    continue;
                }
                networkInterface.getInterfaceAddresses().stream()
                    .map(a -> a.getBroadcast())
                    .filter(Objects::nonNull)
                    .forEach(broadcast -> {
                        try {
                            Socket socket = new Socket();
                            socket.connect(new InetSocketAddress(broadcast, 42069), 1000);
                            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                            out.println(msg);
                            out.close();
                            socket.close();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    });
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}


