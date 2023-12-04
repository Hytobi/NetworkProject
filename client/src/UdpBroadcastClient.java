import java.net.*;

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
            String message = "looking for bomberstudent servers";
            byte[] sendData = message.getBytes();

            // Adresse de diffusion
            InetAddress broadcastAddress = InetAddress.getByName("255.255.255.255");

            // Création du paquet à envoyer
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, broadcastAddress, serverPort);

            // Envoi du paquet
            socket.send(sendPacket);

            // Attente de la réponse
            byte[] receiveData = new byte[1024];
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

            // Attente de la réponse du serveur pendant 5 secondes
            socket.setSoTimeout(5000);

            try {
                socket.receive(receivePacket);

                // Traitement de la réponse
                String serverResponse = new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println("Réponse du serveur : " + serverResponse);
            } catch (SocketTimeoutException e) {
                System.out.println("Aucune réponse du serveur dans le délai spécifié.");
            }

            // Fermeture du socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}