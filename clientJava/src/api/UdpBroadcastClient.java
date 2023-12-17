package api;

/**
 * UdpBroadcastClient : Gestion de la recherche de serveurs
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

import java.net.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class UdpBroadcastClient {
    private final int port = 42069;
    private static final int bufferSize = 4096;

    public UdpBroadcastClient() {
    }

    /**
     * Envoie un message en broadcast sur le réseau local pour détecter les serveurs
     * 
     * @return la liste des adresses IP des serveurs qui ont répondu
     */
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
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, localBroadcastAddress,
                    serverPort);

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
                    if (serverResponse.split("\0")[0].equals(JsonConnection.RES_ATTENDU)) {
                        System.out.println("Serveur trouvé");
                        respondingServers.add(receivePacket.getAddress().getHostAddress());
                    }

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

    /**
     * Envoie un message en UDP pour se reconnecter au serveur si une déconnection
     * est survenue
     * 
     * @return vrai si le serveur a répondu, faux sinon
     */
    public static boolean retryconnection(String serveur) {
        // Port sur lequel le serveur écoute
        int serverPort = 42069;

        try {
            // Création du socket UDP
            DatagramSocket socket = new DatagramSocket();

            // Activation de l'option de diffusion
            socket.setBroadcast(true);

            // Message à envoyer (vous pouvez personnaliser cela selon vos besoins)
            byte[] sendData = JsonConnection.msgConnect().getBytes();

            InetAddress serverAddress = InetAddress.getByName(serveur);

            // Création du paquet à envoyer
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, serverPort);

            // Envoi du paquet
            socket.send(sendPacket);

            // Attente de la réponse du serveur pendant 5 secondes
            socket.setSoTimeout(1000);

            // Liste pour stocker les adresses IP des serveurs qui ont répondu
            List<String> respondingServers = new ArrayList<>();
            byte[] receiveData = new byte[bufferSize];

            try {
                DatagramPacket receivePacket = new DatagramPacket(receiveData, bufferSize);
                socket.receive(receivePacket);

                // Traitement de la réponse
                String serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                if (serverResponse.split("\0")[0].equals(JsonConnection.RES_ATTENDU)) {
                    System.out.println("Serveur trouvé");
                    respondingServers.add(receivePacket.getAddress().getHostAddress());
                    return true;
                }
            } catch (SocketTimeoutException e) {
            }

            // Fermeture du socket
            socket.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}
