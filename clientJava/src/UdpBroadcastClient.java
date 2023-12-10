import api.JsonConnection;

import java.net.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class UdpBroadcastClient {
    private final int port = 42069;
    private final int bufferSize = 4096;

    public UdpBroadcastClient() {
    }

    public List<String> scanConnection() {
        // Port sur lequel le serveur écoute
        int serverPort = 42069;

        try {
            System.out.println("Initialisation...");
            // Création du socket UDP
            DatagramSocket socket = new DatagramSocket();

            // Activation de l'option de diffusion
            socket.setBroadcast(true);

            // Message à envoyer (vous pouvez personnaliser cela selon vos besoins)
            byte[] sendData = JsonConnection.msgSearch().getBytes();

            InetAddress localBroadcastAddress = InetAddress.getByName("255.255.255.255");

            // Création du paquet à envoyer
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, localBroadcastAddress, serverPort);

            // Envoi du paquet
            socket.send(sendPacket);

            // Attente de la réponse du serveur pendant 5 secondes
            socket.setSoTimeout(1000);

            // Liste pour stocker les adresses IP des serveurs qui ont répondu
            List<String> respondingServers = new ArrayList<>();
            byte[] receiveData = new byte[bufferSize];
            System.out.println("Recherche de serveurs...");

            try {
                while (true) {
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, bufferSize);
                    socket.receive(receivePacket);

                    // Traitement de la réponse
                    String serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    System.out.println("Réponse du serveur : " + serverResponse.split("\0")[0]+".");

                    /* TODO FIX
                    // Ajout de l'adresse IP à la liste
                    System.out.println(JsonConnection.RES_ATTENDU+".");
                    if (serverResponse.equals(JsonConnection.RES_ATTENDU)) {
                        System.out.println("Réponse acceptée");
                        respondingServers.add(receivePacket.getAddress().getHostAddress());
                    } else {
                        System.out.println("Réponse refusée");
                    }
                     */
                    respondingServers.add(receivePacket.getAddress().getHostAddress());

                }
            } catch (SocketTimeoutException e) {
                System.out.println("Fin de la recherche de serveurs.");
            }

            // Fermeture du socket
            socket.close();

            return respondingServers;

        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }
}
