package main;
import java.io.IOException;
import java.net.ServerSocket;

import javax.swing.DefaultListModel;

import beans.Host;
import views.MainView;

public class Main extends Thread{
	public static MainView mainView= null;
	public static DefaultListModel<Host> listOfHosts= null;
	public static Host selectedHost= null;
	public static ServerSocket serverSock= null;
	
	public Main(int port){
		listOfHosts= new DefaultListModel<Host>();
		mainView= new MainView("Monitor");
		try {
			serverSock = new ServerSocket(port);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void run(){
		while(true){
			try {
				Host h;
				h = new Host(serverSock.accept());
				h.start();
//				h.startLoadPacket();
				listOfHosts.addElement(h);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				break;
			}
		}
		for(int i= 0; i<listOfHosts.size();i++){
			listOfHosts.get(i).disconnect();
		}
	}
	
	public static void main(String[] args) {
		new Main(9995).start();
		
	}

}
