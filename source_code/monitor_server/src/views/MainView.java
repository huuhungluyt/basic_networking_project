package views;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import beans.Host;
import main.Main;
import main.Resources;

public class MainView extends JFrame implements ActionListener, ListSelectionListener{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JList<Host> jlListOfHosts;
	private JPanel pnMain;
	
	public MainView(String title){
		super(title);
		setSize(Resources.MAINVIEW_W, Resources.MAINVIEW_H);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setLocationRelativeTo(null);;
		setLayout(new GridLayout(1,1));
		
		JPanel wrapper= new JPanel();
		wrapper.setBorder(new EmptyBorder(Resources.PADDING, Resources.PADDING, Resources.PADDING, Resources.PADDING));
		wrapper.setLayout(new BorderLayout(5,5));
		wrapper.add(listHostPanel(), BorderLayout.WEST);
		
		
		pnMain= new JPanel(){
			/**
			 * 
			 */
			private static final long serialVersionUID = 1L;

			@Override
			public void paintComponent(Graphics g){
				super.paintComponent(g);
				BufferedImage image= null;
				try {
					image = ImageIO.read(new File("res/logo.png"));
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				g.drawImage(image, (this.getWidth() - image.getWidth()) / 2,
						(this.getHeight() - image.getHeight()) / 2, null);
			}
		};
		pnMain.setLayout(new BorderLayout());
		wrapper.add(pnMain, BorderLayout.CENTER);
		
		add(wrapper, BorderLayout.CENTER);
		
		setVisible(true);
	}
	
	
	private JPanel listHostPanel(){
		JPanel pn= new JPanel();
		pn.setLayout(new BorderLayout());
		pn.setPreferredSize(new Dimension((int)(this.getWidth()*0.15), 0));
		JPanel pnTitle= new JPanel();
		pnTitle.setLayout(new BoxLayout(pnTitle, BoxLayout.Y_AXIS));
		
		pnTitle.add(new JLabel("List of hosts:", Resources.icHost, JLabel.HORIZONTAL));
		pn.add(pnTitle, BorderLayout.NORTH);
		
		jlListOfHosts= new JList<>(Main.listOfHosts);
		jlListOfHosts.addListSelectionListener(this);
		JScrollPane scrList= new JScrollPane(jlListOfHosts);
		pn.add(scrList);
		return pn;
	}

	@Override
	public void actionPerformed(ActionEvent ae) {
		
	}


	@Override
	public void valueChanged(ListSelectionEvent e) {
		Main.selectedHost = (Host) jlListOfHosts.getSelectedValue();
		pnMain.removeAll();
		if (Main.selectedHost != null) {
			pnMain.add(Main.selectedHost.getHostPanel(), BorderLayout.CENTER);
		}
		pnMain.repaint();
		this.setVisible(true);
	}
}
