package views;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.net.InetAddress;
import java.net.UnknownHostException;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

import main.Main;
import main.Resources;
import beans.Host;
import beans.Packet;

public class HostPanel extends JPanel implements ActionListener, KeyListener{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private Host host;
	
	private JComboBox<String> cbIface = null;
	private JComboBox<String> cbFilter = null;
	private JButton btStart, btStop, btSave, btRefresh;
	private JTextField tfIPSearch;
	private JTextArea taData = null;
	private JTable tbPackets = null, tbVisitedIP = null;

	public HostPanel(String hostName, Host host) {
		//
		// Init components
		//
		String filters[] = { "ip", "icmp", "tcp", "udp" };
		this.host 				= host;
		cbIface					= new JComboBox<>();
		cbFilter				= new JComboBox<>(filters);
		btStart					= new JButton(Resources.icStart);
		btStop					= new JButton(Resources.icStop);
		btSave					= new JButton(Resources.icSave);
		btRefresh				= new JButton(Resources.icRefresh);
		tfIPSearch				= new JTextField(15);
		taData					= new JTextArea();
		tbVisitedIP				= new JTable(this.host.getVisitedIPTableModel());
		tbPackets				= new JTable(this.host.getPacketTableModel());
		taData.setEditable(false);
		JScrollPane scrData		= new JScrollPane(taData);
		JScrollPane scrHeader 	= new JScrollPane(tbPackets);
		JScrollPane scrIPTable 	= new JScrollPane(tbVisitedIP);
		
		tbVisitedIP.getTableHeader().setReorderingAllowed(false);
		tbVisitedIP.getTableHeader().setResizingAllowed(false);
		for(int i= 0; i<Resources.TB_VISITEDIP_WIDTHS.length; i++){
			tbVisitedIP.getColumnModel().getColumn(i).setPreferredWidth(Resources.TB_VISITEDIP_WIDTHS[i]);
		}
		
		tbPackets.getTableHeader().setReorderingAllowed(false);
		tbPackets.getTableHeader().setResizingAllowed(false);
		for(int i= 0; i<Resources.TB_PACKET_WIDTHS.length; i++){
			tbPackets.getColumnModel().getColumn(i).setPreferredWidth(Resources.TB_PACKET_WIDTHS[i]);
		}
		
		tbVisitedIP.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
//				int row = tbVisitedIP.rowAtPoint(new Point(e.getX(), e.getY()));
				int col = tbVisitedIP.columnAtPoint(new Point(e.getX(), e
						.getY()));

				if (col == 2) {
					int selectedRow= tbVisitedIP.getSelectedRow();
					if(selectedRow>-1){
					InetAddress ip= host.getVisitedIPTableModel().getPacketBox().get(selectedRow);
					new IPInfoView(Main.mainView, host, ip);
					}
				}
			}

			@Override
			public void mouseEntered(MouseEvent e) {}

			@Override
			public void mouseExited(MouseEvent e) {}
		});
		
		tbPackets.getSelectionModel().addListSelectionListener(
				new ListSelectionListener() {

					@Override
					public void valueChanged(ListSelectionEvent e) {
						int selectedRow;
						if ((selectedRow = tbPackets.getSelectedRow()) > -1) {
							Packet p = host.getPacketTableModel()
									.getPacketBox().get(selectedRow);
							p.fill(taData);
						}
					}
				});
		
		
		setLayout(new BorderLayout());

		
		
		
		// HOST NAME PANEL
		JPanel pnTop = new JPanel();
		pnTop.add(new JLabel("[Host] " + hostName));
		add(pnTop, BorderLayout.NORTH);

		
		
		
		// CENTER
		JPanel pnCenter = new JPanel(new BorderLayout(Resources.PADDING,
				Resources.PADDING));
		
		
		

		// CENTER - Capture panel
		JPanel pnControl = new JPanel();
		pnControl.setLayout(new BorderLayout());
		
		JPanel pnIPSearch= new JPanel(new FlowLayout(FlowLayout.LEADING));
		pnIPSearch.setBorder(new TitledBorder("Search IP"));
		pnIPSearch.add(new JLabel("IP Address: ", Resources.icIPSearch, JLabel.LEADING));
		pnIPSearch.add(tfIPSearch);
		pnControl.add(pnIPSearch, BorderLayout.WEST);

		JPanel pnCapture = new JPanel(new FlowLayout(FlowLayout.RIGHT, 10, 10));
		pnCapture.setBorder(new TitledBorder("Capture packets"));
		pnCapture.add(new JLabel("Interface: ", Resources.icIface, JLabel.HORIZONTAL));
		pnCapture.add(cbIface);
		pnCapture.add(new JLabel("Filter: ", Resources.icFilter, JLabel.HORIZONTAL));
		pnCapture.add(cbFilter);
		pnCapture.add(btStart);
		pnCapture.add(btStop);
		pnCapture.add(btSave);
		pnCapture.add(btRefresh);
		pnControl.add(pnCapture, BorderLayout.CENTER);

		pnCenter.add(pnControl, BorderLayout.NORTH);

		
		
		
		//CENTER - Accession panel
		JPanel pnAccession = new JPanel(new BorderLayout(Resources.PADDING,
				Resources.PADDING));
		pnAccession.add(new JLabel("Visited IP address:", Resources.icIP, JLabel.LEFT), BorderLayout.NORTH);
		pnAccession.add(scrIPTable, BorderLayout.CENTER);
		
		pnCenter.add(pnAccession, BorderLayout.CENTER);

		
		
		
		//CENTER - Packets
		JPanel pnPacket = new JPanel(new BorderLayout(Resources.PADDING,
				Resources.PADDING));
		pnPacket.setPreferredSize(new Dimension(0,
				(int) (Resources.MAINVIEW_H * 0.4)));
		pnPacket.add(new JLabel("Packet header: ", Resources.icHeader, JLabel.LEFT),
				BorderLayout.NORTH);
		pnPacket.add(scrHeader, BorderLayout.CENTER);

		pnCenter.add(pnPacket, BorderLayout.SOUTH);

		
		
		
		
		//CENTER - Data detail
		JPanel pnData = new JPanel(new BorderLayout(Resources.PADDING,
				Resources.PADDING));
		pnData.setPreferredSize(new Dimension((int) (Resources.MAINVIEW_W * 0.35),
				0));
		pnData.add(new JLabel("Packet data: ", Resources.icData, JLabel.LEFT),
				BorderLayout.NORTH);
		pnData.add(scrData, BorderLayout.CENTER);
		
		pnCenter.add(pnData, BorderLayout.EAST);

		
		// END CENTER
		add(pnCenter, BorderLayout.CENTER);

		
		// Register event
		btRefresh.addActionListener(this);
		btStart.addActionListener(this);
		btStop.addActionListener(this);
		btSave.addActionListener(this);
		
		tfIPSearch.addKeyListener(this);
	}

	public void refreshIfaces(String[] ifaceNames) {
		cbIface.removeAllItems();
		for (int i = 0; i < ifaceNames.length; i++) {
			cbIface.addItem(ifaceNames[i]);
		}
	}

	@Override
	public void actionPerformed(ActionEvent ae) {
		Object click = ae.getSource();
		if (click == btRefresh) {
			
			host.sendRequest("stop");
			host.getPacketTableModel().resetPacketBox();
			host.getVisitedIPTableModel().resetVisitedIPList();
			host.sendRequest("iface");
			taData.setText("");
			btStop.setEnabled(false);
			btStart.setEnabled(true);
			btSave.setEnabled(false);
			
		} else if (click == btStop) {
			
			host.sendRequest("stop");
			btStop.setEnabled(false);
			btStart.setEnabled(true);
			btSave.setEnabled(true);
			
		} else if (click == btStart) {
			
			host.getPacketTableModel().resetPacketBox();
			int devIndex = cbIface.getSelectedIndex();
			String filterStr = cbFilter.getSelectedItem().toString();
			host.sendRequest("start:" + devIndex + ","+filterStr);
			btStop.setEnabled(true);
			btStart.setEnabled(false);
			btSave.setEnabled(false);
			
		}else if(click == btSave){
			JFileChooser c = new JFileChooser();
			c.setFileFilter((FileFilter)new FileNameExtensionFilter("TEXT FILES", "txt", "text"));
		      // Demonstrate "Open" dialog:
		      int rVal = c.showSaveDialog(Main.mainView);
		      if (rVal == JFileChooser.APPROVE_OPTION) {
		        File file= c.getSelectedFile();
		        host.getPacketTableModel().logPackets(file);
		      }
		      if (rVal == JFileChooser.CANCEL_OPTION) {

		      }
		}
	}

	@Override
	public void keyPressed(KeyEvent e) {
		if(e.getKeyCode()==KeyEvent.VK_ENTER){
			String temp= tfIPSearch.getText();
			if(temp.length()>0){
				InetAddress ip;
				try {
					ip = InetAddress.getByName(temp);
				} catch (UnknownHostException e1) {
					return;
				}
				new IPInfoView(Main.mainView, host, ip);
			}
		}
	}

	@Override
	public void keyReleased(KeyEvent e) {}

	@Override
	public void keyTyped(KeyEvent e) {}
}
