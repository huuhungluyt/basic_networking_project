package beans;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Arrays;

import javax.swing.JOptionPane;

import main.Main;
import models.PacketTableModel;
import models.VisitedIPTableModel;
import views.HostPanel;

public class Host extends Thread {
	public static final int BUFFER_SIZE = 65536;
//	private Queue<byte[]> rawPackets = null;
	private HostPanel hostPanel;
	private PacketTableModel packetTableModel;
	private VisitedIPTableModel visitedIPTableModel;
	private Socket socket = null;
	private DataInputStream dis = null;
	private DataOutputStream dos = null;
//	private boolean loadFlag= true;

	public Host(Socket soc) {
		socket = soc;
		try {
			dis = new DataInputStream(soc.getInputStream());
			dos = new DataOutputStream(soc.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		}
		packetTableModel = new PacketTableModel();
		visitedIPTableModel = new VisitedIPTableModel();
		hostPanel = new HostPanel(socket.getInetAddress().getHostName(), this);
//		rawPackets = new LinkedList<>();
	}

//	public void startLoadPacket() {
//		loadFlag= true;
//		new Thread() {
//			@Override
//			public void run() {
//				while (loadFlag) {
//					try {
//						byte[] temp = rawPackets.remove();
//						Packet p = new Packet(temp);
//						packetTableModel.addPacket(p);
//						visitedIPTableModel.addIP(p.getIp().getSourceIP());
//						visitedIPTableModel.addIP(p.getIp().getDestIP());
//					} catch (Exception ex) {
//					}
//				}
//			}
//		}.start();
//	}
	
//	public void stopLoadPacket(){
//		loadFlag= false;
//	}

	@Override
	public void run() {
		while (true) {
			byte[] buffer = new byte[BUFFER_SIZE];
			try {
//				int c = dis.read(buffer);
//				System.out.println("c ="+c);
				byte[] receive= Arrays.copyOf(buffer, c);

				if (c > 0) {
					String tempStr = new String(receive);
					String tempStrs[] = tempStr.split(":");
					if (tempStrs[0].equals("iface")) {
						System.out.println(tempStr);
						hostPanel.refreshIfaces(tempStrs[1].split(","));
					} else if (tempStrs[0].equals("lock")) {
						if (tempStrs[1].equals("ok")) {
							JOptionPane.showMessageDialog(Main.mainView,
									"Locked successfully !", "IP Locking",
									JOptionPane.INFORMATION_MESSAGE);
						} else {
							JOptionPane.showMessageDialog(Main.mainView,
									"IP already locked !", "IP Locking",
									JOptionPane.ERROR_MESSAGE);
						}
					}else if(tempStrs[0].equals("unlock")){
						if (tempStrs[1].equals("ok")) {
							JOptionPane.showMessageDialog(Main.mainView,
									"Unlocked successfully !", "IP Unlocking",
									JOptionPane.INFORMATION_MESSAGE);
						} else {
							JOptionPane.showMessageDialog(Main.mainView,
									"IP already unlocked !", "IP Unlocking",
									JOptionPane.ERROR_MESSAGE);
						}
					}
					else {
//						System.out.println(receive.length);
						try{
							Packet p = new Packet(receive);
							packetTableModel.addPacket(p);
							visitedIPTableModel.addIP(p.getIp().getSourceIP());
							visitedIPTableModel.addIP(p.getIp().getDestIP());
						}catch(Exception ex){
							continue;
						}
					}

				}

				else if (c < 0)
					throw new IOException();
			} catch (Exception e) {
				disconnect();
				e.printStackTrace();
				break;
			}
		}
	}

	public PacketTableModel getPacketTableModel() {
		return packetTableModel;
	}

	public VisitedIPTableModel getVisitedIPTableModel() {
		return visitedIPTableModel;
	}

	public HostPanel getHostPanel() {
		return hostPanel;
	}

	public void sendRequest(String sms) {
		try {
			dos.write(sms.getBytes());
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	public void disconnect() {
		try {
//			stopLoadPacket();
			dis.close();
			dos.close();
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		Main.listOfHosts.removeElement(this);
	}

	public String toString() {
		return socket.getInetAddress().getHostName();
	}
}
