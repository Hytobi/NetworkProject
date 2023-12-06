import api.JsonConnection;

import java.net.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import dto.*;
import api.*;

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
            byte[] sendData = JsonConnection.msgSearch().getBytes();

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
                    System.out.println("Réponse du serveur : " + serverResponse.split("\0")[0]);

                    // Ajout de l'adresse IP à la liste
                    respondingServers.add(receivePacket.getAddress().getHostAddress());
                }
            } catch (SocketTimeoutException e) {
                System.out.println("Fin de la recherche de serveurs.");
            }

            // Affichage des adresses IP des serveurs qui ont répondu
            System.out.println("Serveurs qui ont répondu : ");
            for (String server:respondingServers){
                System.out.println(server);
            }

            // demande a l'utilisateur de choisir un serveur
            System.out.println("Choisissez un serveur : ");
            int i = 0;
            for (String server:respondingServers){
                System.out.println(i + " : " + server);
                i++;
            }
            Scanner sc = new Scanner(System.in);
            int choix = sc.nextInt();
            InetAddress serverAddress = InetAddress.getByName(respondingServers.get(choix));

            // Envoi de la requête au serveur
            sendData = JsonConnection.msgConnect().getBytes();
            sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, serverPort);
            socket.send(sendPacket);

            // Attente de la réponse du serveur
            socket.setSoTimeout(5000);
            receiveData = new byte[1024];
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            socket.receive(receivePacket);

            // Traitement de la réponse
            String serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
            if ("ok".equals(serverResponse)) {
                System.out.println("Connexion au serveur réussie !");
            } else {
                System.out.println("Connexion au serveur échouée : " + serverResponse);
            }

            // On demande a l'user de choisir entre afficher les maps ou les parties
            while (true) {
                System.out.println("Que voulez vous faire ?\n1 : Afficher les maps\n2 : Afficher les parties");
                choix = sc.nextInt();
                if (choix == 1) {
                    sendData = JsonConnection.getMapList().getBytes();
                    sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, serverPort);
                    socket.send(sendPacket);

                    // Attente de la réponse du serveur
                    socket.setSoTimeout(5000);
                    receiveData = new byte[1024];
                    receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    socket.receive(receivePacket);

                    // Traitement de la réponse
                    serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    ResMapList rml = MapperRes.fromJson(serverResponse, ResMapList.class);
                    System.out.println("Réponse du serveur : " + serverResponse.split("\0")[0]);

                } else if (choix == 2) {
                    sendData = JsonConnection.getGameList().getBytes();
                    sendPacket = new DatagramPacket(sendData, sendData.length, serverAddress, serverPort);
                    socket.send(sendPacket);

                    // Attente de la réponse du serveur
                    socket.setSoTimeout(5000);
                    receiveData = new byte[1024];
                    receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    socket.receive(receivePacket);

                    // Traitement de la réponse
                    serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    ResGameList rgl = MapperRes.fromJson(serverResponse, ResGameList.class);
                    System.out.println("Réponse du serveur : " + serverResponse.split("\0")[0]);

                } else {
                    System.out.println("Choix invalide");
                    break;
                }
            }

            // Fermeture du socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
