package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class MACHeader extends Header{
	private byte[] rawData;
	private String destMAC;
	private String sourceMAC;
	private int etherType;
	
	public MACHeader(byte[]packet, int b){
		destMAC= "";
		sourceMAC= "";
		for(int i= 0; i<6; i++)
		{
			String temp= Integer.toHexString(packet[b+i]&0xff);
			destMAC+=((i==0)?"":":")+((temp.length()==1)?"0":"")+temp;
		}
		
		for(int i= 6; i<12; i++)
		{
			String temp= Integer.toHexString(packet[b+i]&0xff);
			sourceMAC+=((i==6)?"":":")+((temp.length()==1)?"0":"")+temp;
		}
		
		etherType = (((packet[b+12] & 0xff) << 8) | (packet[b+13] & 0xff)) & 0xffff;
		
		rawData= new byte[14];
		for(int i= 0; i<14; i++){
			rawData[i]= packet[b+i];
		}
	}
	
	
	
	public byte[] getRawData() {
		return rawData;
	}



	public String getDestMAC() {
		return destMAC;
	}



	public String getSourceMAC() {
		return sourceMAC;
	}



	public int getEtherType() {
		return etherType;
	}



//	public void print(){
//		System.out.println("[ MAC Header ]");
//		System.out.println("\t|-Destination MAC        : "+destMAC);
//		System.out.println("\t|-Source MAC             : "+sourceMAC);
//		System.out.println("\t|-Ether type             : "+etherType);
//		System.out.println("\n\t|-Raw data             : ");
//		Packet.printRawData(rawData);
//	}



	@Override
	public void fill(JTextArea ta) {
		ta.append("[ MAC Header ]\n");
		ta.append("\t|-Destination MAC:\t"+destMAC+"\n");
		ta.append("\t|-Source MAC:\t\t"+sourceMAC+"\n");
		ta.append("\t|-Ether type:\t\t"+etherType+"\n");
//		ta.append("\n\t|-Raw data             :\n");
//		Packet.printRawData(rawData, ta);
	}



	@Override
	public void log(BufferedWriter bw) throws IOException {
		// TODO Auto-generated method stub
		bw.write("[ MAC Header ]\n");
		bw.write("\t|-Destination MAC:\t"+destMAC+"\n");
		bw.write("\t|-Source MAC:\t\t"+sourceMAC+"\n");
		bw.write("\t|-Ether type:\t\t"+etherType+"\n");
	}
}
