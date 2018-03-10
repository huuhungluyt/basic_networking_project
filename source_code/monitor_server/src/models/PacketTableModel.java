package models;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

import javax.swing.table.AbstractTableModel;

import beans.Packet;
import beans.Packet.PACKET_TYPE;
import beans.TCPHeader;
import beans.UDPHeader;

public class PacketTableModel extends AbstractTableModel {
	/**
	 */
	private static final long serialVersionUID = 1L;
	private static final int MAX_PACKETS = 256;
	private Vector<Packet> packetBox = null;
	private String[] columnNames = { "No.", "Sour MAC", "Dest MAC", "Sour IP",
			"Dest IP", "Sour port", "Dest port", "Protocol", "Length", "Info" };

	public PacketTableModel() {
		packetBox = new Vector<>();
	}

	public Vector<Packet> getPacketBox() {
		return packetBox;
	}

	@Override
	public String getColumnName(int i) {
		return columnNames[i];
	}

	@Override
	public int getColumnCount() {
		return columnNames.length;
	}

	@Override
	public int getRowCount() {
		return packetBox.size();
	}

	@Override
	public Object getValueAt(int arg0, int arg1) {
		Packet p = packetBox.get(arg0);
		switch (arg1) {
		case 0:
			return arg0;
		case 1:
			return p.getMac().getSourceMAC();
		case 2:
			return p.getMac().getDestMAC();
		case 3:
			return p.getIp().getSourceIP();
		case 4:
			return p.getIp().getDestIP();
		case 5:
			if (p.getType() == Packet.PACKET_TYPE.TCP)
				return ((TCPHeader) (p.getProtocolHeader())).getSourcePort();
			if (p.getType() == Packet.PACKET_TYPE.UDP)
				return ((UDPHeader) (p.getProtocolHeader())).getSourcePort();
			return null;
		case 6:
			if (p.getType() == Packet.PACKET_TYPE.TCP)
				return ((TCPHeader) (p.getProtocolHeader())).getDestPort();
			if (p.getType() == Packet.PACKET_TYPE.UDP)
				return ((UDPHeader) (p.getProtocolHeader())).getDestPort();
			return null;
		case 7:
			return p.getType();
		case 8:
			return p.getIp().getTotalLen();
		case 9:
			if (p.getType() == PACKET_TYPE.TCP) {
				return "[ACK]="
						+ ((TCPHeader) (p.getProtocolHeader())).getAckNum()
						+ "; [SEQ]="
						+ ((TCPHeader) (p.getProtocolHeader())).getSeqNum();
			}
		default:
			return null;
		}
	}

	@Override
	public boolean isCellEditable(int row, int column) {
		return false;
	}

	public void addPacket(Packet p) {
		packetBox.add(p);
		if (packetBox.size() >= MAX_PACKETS)
			packetBox.removeElementAt(0);
		this.fireTableDataChanged();
	}

	public void resetPacketBox() {
		packetBox.removeAllElements();
		this.fireTableDataChanged();
	}
	
	public void logPackets(File file){
		BufferedWriter writer= null;
		try {
			writer = new BufferedWriter( new FileWriter( file));
			for(int i= 0; i<packetBox.size(); i++){
				packetBox.get(i).log(writer);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				writer.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
