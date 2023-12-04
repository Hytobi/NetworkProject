import java.net.*;
import java.util.ArrayList;
import java.util.List;

public class UdpBroadcastClient {
    public static void main(String[] args) {
        // Port sur lequel le serveur écoute
        int serverPort = 42069;

        try {
            // Création du socket UDP
            DatagramSocket socket = new DatagramSocket();

            // Activation de l'option de diffusion
            socket.setBroadcast(true);

            // Message à envoyer (vous pouvez personnaliser cela selon vos besoins)
            String message = "Hello, any servers out there?";
            byte[] sendData = message.getBytes();

            // Adresse de diffusion spécifique à votre réseau local (ex : 192.168.1.255)
            InetAddress localBroadcastAddress = InetAddress.getByName("255.255.255.255");

            // Création du paquet à envoyer
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, localBroadcastAddress, serverPort);

            // Envoi du paquet
            socket.send(sendPacket);

            // Attente de la réponse du serveur pendant 5 secondes
            socket.setSoTimeout(5000);

            // Liste pour stocker les adresses IP des serveurs qui ont répondu
            List<String> respondingServers = new ArrayList<>();
            byte[] receiveData = new byte[1024];

            try {
                while (true) {
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    socket.receive(receivePacket);

                    // Traitement de la réponse
                    String serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    System.out.println("Réponse du serveur : " + serverResponse);

                    // Ajout de l'adresse IP à la liste
                    respondingServers.add(receivePacket.getAddress().getHostAddress());
                }
            } catch (SocketTimeoutException e) {
                System.out.println("Fin de la recherche de serveurs.");
            }

            // Affichage des adresses IP des serveurs qui ont répondu
            System.out.println("Serveurs qui ont répondu : " + respondingServers);


            // Fermeture du socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
