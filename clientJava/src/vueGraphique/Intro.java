package vueGraphique;

import modele.Carte;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import api.*;
import dto.*;
import java.io.IOException;
import java.util.List;

public class Intro extends JFrame implements ActionListener {

    public static final ImageIcon BOMBER = new ImageIcon("img/bomber.png");
    private static final Integer MAX_RETRY = 3;

    private boolean commencer = false;
    private String myName;
    private ResGameJoin resGameJoin;

    private JPanel infoPanel;
    private JLabel bomber;
    private JLabel infoJeu;
    private JButton quitterBtn;
    private Action quitterAction;
    private JButton commencerBtn;
    private Action commencerAction;
    private TcpClient tcp;
    private GridBagConstraints c = new GridBagConstraints();

    /**Constructeur de la classe vue*/
    public Intro(){
        //Donne le titre via JFrame
        super("Bomberstudent");

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        //Les inits
        initActions();
        initArrierePlan();

        //Les indispensables
        pack();
        Dimension dimEcran = getToolkit().getScreenSize();
        setLocation((dimEcran.width - getWidth())/2, (dimEcran.height- getHeight())/2);
        setResizable(false);
        setVisible(true);
    }


    /**Méthode qui initialise les actions des boutons */
    private void initActions() {
        quitterAction = new AbstractAction("Quitter") {
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        };
        commencerAction = new AbstractAction("Commencer") {
            public void actionPerformed(ActionEvent e){
                scan();
            }
        };
    }

    private void initArrierePlan(){
        infoPanel = new JPanel();
        infoPanel.setLayout(new GridBagLayout());
        infoPanel.setBackground(Color.BLACK);

        bomber = new JLabel(BOMBER, JLabel.CENTER);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 5;
        c.gridx = 0;
        c.gridy = 1;
        infoPanel.add(bomber, c);

        publishMessage("Client Java par Hana Delcourt & Patrice Plouvin", false, Color.WHITE);

        quitterBtn = new JButton(quitterAction);
        quitterBtn.setBorderPainted(false);
        quitterBtn.setFocusPainted(false);
        quitterBtn.setContentAreaFilled(false);
        quitterBtn.setForeground(Color.WHITE);
        setGridBag(1, 60, 0.5, 1, 2);
        infoPanel.add(quitterBtn, c);

        commencerBtn = new JButton(commencerAction);
        commencerBtn.setBorderPainted(false);
        commencerBtn.setFocusPainted(false);
        commencerBtn.setContentAreaFilled(false);
        commencerBtn.setForeground(Color.WHITE);
        setGridBag(1, 60, 0.5, 3, 2);
        infoPanel.add(commencerBtn, c);

        getContentPane().add(infoPanel);
    }

    public boolean getCommencer(){
        return commencer;
    }

    public TcpClient getTcp(){
        return tcp;
    }

    public String getMyName(){
        return myName;
    }

    public ResGameJoin getResGameJoin(){
        return resGameJoin;
    }

    @Override
    public void actionPerformed(ActionEvent e) {}

    public void scan(){
        System.out.println("Scan des serveurs");
        UdpBroadcastClient udp = new UdpBroadcastClient();
        List<String> servers = udp.scanConnection();
        if (servers.isEmpty()){
            System.err.println("Aucuns servers trouvé");
            publishMessage("Aucuns servers trouvé", true, Color.RED);
            return;
        }

        // On enleve l'image BOMBER
        infoPanel.remove(bomber);
        clear();

        // On cree un bouton pour chaque serveur lorsqu'on appui dessus on affiche le nom du bouton
        for (int i=0; i<servers.size(); i++){
            String server = servers.get(i);
            JButton btn = createBtn(server);
            btn.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    tcp = new TcpClient(server);
                    try {
                        tcp.tcpConnect();
                    } catch (IOException ex) {
                        publishMessage("Erreur lors de la connexion au serveur TCP ", true, Color.RED);
                        System.err.println("Erreur lors de la connexion au serveur TCP " + ex.getMessage());
                        return;
                    }
                    System.out.println("Connexion Réussie !");
                    addBtnAfterConnect();
                }
            });
            setGridBag(1, 60, 0.5, 1, i+2);
            
            infoPanel.add(btn, c);
        }

        // On affiche les boutons
        infoPanel.revalidate();
        infoPanel.repaint();
    }

    private void addBtnAfterConnect(){
        // On enleve les boutons
        clear();
        // ToolBar
        JToolBar toolBar = new JToolBar();
        // lister les maps
        JButton maps = createBtn("Lister les maps");
        maps.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clear();
                System.out.println("Envoie demande liste des maps");
                tcp.post(JsonConnection.getMapList());
                String maps = tcp.get();
                if (maps == null){
                    closeAll();
                    return;
                }
                ResMapList res = MapperRes.fromJson(maps, ResMapList.class);
                if (res != null && res.getStatut().equals("200")){
                    setGridBag(1, 20, 0.5, 1, 0);
                    publishMessage(res.getNbMapsList() + " map(s) trouvée(s) :", true, Color.GREEN);
                    int nbmap = 0;
                    int i = 1;
                    while (nbmap < res.getNbMapsList()){
                        
                        String map = res.getMaps().get(nbmap).getContent();
                        int x = res.getMaps().get(nbmap).getWidth();
                        int y = res.getMaps().get(nbmap).getHeight();
                        map = map.replaceAll(" ", "_ ");
                        String[] lignes = map.split("\n");
                        setGridBag(1, 20, 0.5, 1, (nbmap*x)+1);
                        JLabel label = new JLabel("Map " + (res.getMaps().get(nbmap).getId() +1), JLabel.LEFT);
                        label.setForeground(Color.BLUE);
                        infoPanel.add(label, c);
                        for (int j=0; j<lignes.length; j++){
                            setGridBag(x, 5, 1, 1, (nbmap*x)+j+2);
                            JLabel label2 = new JLabel(lignes[j]);
                            label2.setForeground(Color.WHITE);
                            infoPanel.add(label2, c);
                        }
                        nbmap++;
                        i = i+y+1;
                    }
                }else{
                    publishMessage("Erreur lors de la récupération des maps", true, Color.RED);
                }
                infoPanel.revalidate();
                infoPanel.repaint();
            }
        });

        JButton games = createBtn("Lister les games");
        games.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clear();
                System.out.println("Envoie demande liste des games");
                tcp.post(JsonConnection.getGameList());
                String gameslist = tcp.get();
                if (gameslist == null){
                    closeAll();
                    return;
                }
                ResGameList res = MapperRes.fromJson(gameslist, ResGameList.class);
                if (res != null && res.getStatut().equals("200")){
                    publishMessage(res.getGames().size() + " game(s) trouvées", true, Color.GREEN);
                    // TODO : afficher les games
                }else{
                    publishMessage("Erreur lors de la récupération des games", true, Color.RED);
                }
                infoPanel.revalidate();
                infoPanel.repaint();
            }
        });

        JButton create = createBtn("Créer une game");
        create.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clear();
                vueCreateGame();
                infoPanel.revalidate();
                infoPanel.repaint();
            }
        });

        toolBar.add(maps);
        toolBar.addSeparator();
        toolBar.add(games);
        toolBar.addSeparator();
        toolBar.add(create);
        toolBar.setBackground(new Color(28, 25, 71));
        toolBar.setFloatable(false);
        toolBar.setRollover(true);
        toolBar.setBorderPainted(false);

        // On ajoute la ToolBar
        getContentPane().add(toolBar, BorderLayout.NORTH);

        infoPanel.revalidate();
        infoPanel.repaint();
    }

    private JButton createBtn(String name){
        JButton btn = new JButton(name);
        btn.setBorderPainted(false);
        btn.setFocusPainted(false);
        btn.setContentAreaFilled(false);
        btn.setForeground(Color.WHITE);
        return btn;
    }

    private void setGridBag(int gridwidth, int ipady, double weightx, int gridx, int gridy){
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = gridwidth;
        c.ipady = ipady;
        c.weightx = weightx;
        c.gridx = gridx;
        c.gridy = gridy;
    }

    private void clear(){
        Component[] components = infoPanel.getComponents();
        for (Component component : components) {
            if (component instanceof JButton || component instanceof JLabel || component instanceof JTextField) {
                infoPanel.remove(component);
            }
        }
    }

    private void publishMessage(String message, boolean now, Color color) {
        if (infoJeu != null) {
            infoPanel.remove(infoJeu);
        }
        infoJeu = new JLabel(message, JLabel.CENTER);
        infoJeu.setForeground(color);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 5;
        c.ipady = 20;
        c.gridx = 0;
        c.gridy = 0;
        infoPanel.add(infoJeu, c);
        if (now) {
            infoPanel.revalidate();
            infoPanel.repaint();
        }
    }

    private void vueCreateGame(){
        JTextField nom = new JTextField(10);
        nom.setText("Nom de la game");
        JTextField id = new JTextField(10);
        id.setText("Id de la map");
        JButton submitButton = createBtn("Créer une game");
        submitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("Envoie demande creation de game");
                if (nom.getText().equals("Nom de la game") || id.getText().equals("Id de la map") || nom.getText().equals("") || id.getText().equals("")){
                    publishMessage("Veuiller remplir les champs", true, Color.RED);
                    return;
                }
                id.setText(String.valueOf(Integer.parseInt(id.getText())-1));
                Game game = new Game(nom.getText(), id.getText());
                tcp.post(JsonConnection.postGameCreate(game));
                String create = tcp.get();
                if (create == null){
                    closeAll();
                    return;
                }
                ResGameJoin res = MapperRes.fromJson(create, ResGameJoin.class);
                if (res != null && res.getStatut().equals("201")){
                    publishMessage(" creation reussi", true, Color.GREEN);
                    myName = "player" + res.getNbPlayers()+1;
                    resGameJoin = res;
                    commencer=true;
                }else{
                    publishMessage("Erreur lors de la creation de game", true, Color.RED);
                }
                infoPanel.revalidate();
                infoPanel.repaint();
            }
        });

        JLabel cg = new JLabel("Zone de création de partie", JLabel.CENTER);
        cg.setForeground(Color.BLUE);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 5;
        c.ipady = 20;
        c.gridx = 0;
        c.gridy = 2;
        infoPanel.add(cg, c);

        setGridBag(1, 20, 0.5, 1, 4);
        infoPanel.add(nom, c);
        setGridBag(1, 20, 0.5, 1, 5);
        infoPanel.add(id, c);
        setGridBag(1, 20, 0.5, 2, 4);
        infoPanel.add(submitButton, c);
    }

    private void closeAll(){
        JOptionPane.showMessageDialog(this,"La connection au serveur a été intérompu");
        try {
            tcp.closeSocket();
        } catch (Exception e) {
            System.out.println("Erreur lors de la fermeture du socket");
        } finally {
            int i=0;
            while (i < MAX_RETRY) {
                System.out.println("Tentative de reconnexion au serveur...");
                if (UdpBroadcastClient.retryconnection(tcp.getServer())){
                    try {
                        tcp.tcpConnect();
                        break;
                    } catch (Exception e) {}
                }
                i++;
                if (i < MAX_RETRY){
                    try {
                        Thread.sleep(10000);
                    } catch (Exception ex) {
                        System.out.println("Erreur lors de la reconnexion au serveur");
                    }
                }
            }
            if (i == MAX_RETRY) {
                JOptionPane.showMessageDialog(this,"Impossible de se reconnecter au serveur");
                System.exit(0);
            }
        }
    }
}

