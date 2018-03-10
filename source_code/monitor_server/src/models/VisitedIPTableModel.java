package models;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Vector;

import javax.swing.table.AbstractTableModel;

public class VisitedIPTableModel extends AbstractTableModel{
	private static final long serialVersionUID = 1L;
	private Vector<InetAddress> visitedIPList = null;
	private String[] columnNames = {"No.", "IP address", "Host name"};
	
	public VisitedIPTableModel() {
		visitedIPList= new Vector<>();
	}
	
	public Vector<InetAddress> getPacketBox(){
		return visitedIPList;
	}
	
	@Override
	public String getColumnName(int i){
		return columnNames[i];
	}
	@Override
	public int getColumnCount() {
		return columnNames.length;
	}

	@Override
	public int getRowCount() {
		return visitedIPList.size();
	}

	@Override
	public Object getValueAt(int arg0, int arg1) {
		InetAddress ip = visitedIPList.get(arg0);
		switch (arg1) {
		case 0:
			return arg0;
		case 1:
			return ip.getHostAddress();
		case 2:
			return ip.getHostName();
		default:
			return null;
		}
	}
	
	@Override
	public boolean isCellEditable(int row, int column){
		return false;
	}
	
	public void addIP(String ipAddress){
		for(int i= 0; i<visitedIPList.size(); i++){
			if(ipAddress.equals(visitedIPList.get(i).getHostAddress())) return;
		}
		try {
			InetAddress ip= InetAddress.getByName(ipAddress);
			visitedIPList.add(ip);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		this.fireTableDataChanged();
	}
	
	public void resetVisitedIPList() {
		visitedIPList.removeAllElements();
		this.fireTableDataChanged();
	}
}
