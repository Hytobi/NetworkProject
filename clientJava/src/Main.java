import java.util.List;

public class Main {
    public static void main(String[] args) {
        UdpBroadcastClient udp = new UdpBroadcastClient();
        List<String> servers = udp.scanConnection();
        if (servers==null){
            System.err.println("Aucuns servers trouvé");
        }
        TcpClient tcp = new TcpClient();
        tcp.tcpConnect(servers.get(0));

    }
}
