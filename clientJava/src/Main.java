import java.net.InetAddress;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        UdpBroadcastClient udp = new UdpBroadcastClient();
        List<String> servers = udp.scanConnection();
        if (servers.isEmpty()){
            System.err.println("Aucuns servers trouvé");
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
        tcp.tcpConnect();
        for (;;){
            tcp.post("Bonjour !");
            System.out.println("Message reçu : "+tcp.get());
        }

    }
}
