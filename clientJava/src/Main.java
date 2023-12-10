import java.io.IOException;
import java.net.InetAddress;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        UdpBroadcastClient udp = new UdpBroadcastClient();
        List<String> servers = udp.scanConnection();
        if (servers.isEmpty()){
            System.out.println("Aucuns servers trouvé");
            return;
        }

        System.out.println("Choisissez un serveur : ");
        int i = 0;
        for (String server:servers){
            System.out.println(i + " : " + server);
            i++;
        }
        Scanner sc = new Scanner(System.in);
        int choix = sc.nextInt();

        TcpClient tcp = new TcpClient(servers.get(choix));
        try {
            tcp.tcpConnect();
        } catch (IOException e) {
            System.err.println("Erreur lors de la connexion au serveur TCP " + e.getMessage());
        }
        System.out.println("Connexion Réussie !");
        for (;;){
            //TODO Gerer quand le serveur s'arrete
            tcp.post("Bonjour !");
            System.out.println("Message reçu : "+tcp.get());
        }

    }
}
