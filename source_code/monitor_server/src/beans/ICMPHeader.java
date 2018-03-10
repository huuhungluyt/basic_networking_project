package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class ICMPHeader extends Header{
	private byte[] rawData;
	int type, code, checksum;

	public ICMPHeader(byte[] packet, int b) {
		type = packet[b] & 0xff;
		code = packet[b + 1] & 0xff;
		checksum = (((packet[b + 2] & 0xff) << 8) | (packet[b + 3] & 0xff)) & 0xffff;
		
		rawData= new byte[4];
		for(int i= 0; i<4; i++){
			rawData[i]= packet[b+i];
		}	
	}
	
	public byte[] getRawData() {
		return rawData;
	}

	public int getType() {
		return type;
	}

	public int getCode() {
		return code;
	}

	public int getChecksum() {
		return checksum;
	}

//	@Override
//	public void print(){
//		System.out.println("[ ICMP Header ]");
//		System.out.print("\t|-Type         : " + type);
//		if(type==11) System.out.println(" (TTL expired)");
//		else if(type==0) System.out.println(" (ICMP Echo Reply)");
//		System.out.println("\t|-Code       : "+code);
//		System.out.println("\t|-Checksum   : "+checksum);
//		System.out.println("\n\t|-Raw data       : ");
//		Packet.printRawData(rawData);
//	}
	
	@Override
	public void log(BufferedWriter bw) throws IOException{
		bw.write("[ ICMP Header ]\n");
		bw.write("\t|-Type:\t" + type);
		if(type==11) bw.write(" (TTL expired)\n");
		else if(type==0) bw.write(" (ICMP Echo Reply)\n");
		bw.write("\t|-Code:\t"+code+"\n");
		bw.write("\t|-Checksum:\t"+checksum+"\n");
	}
	
	@Override
	public void fill(JTextArea ta){
		ta.append("[ ICMP Header ]\n");
		ta.append("\t|-Type:\t" + type);
		if(type==11) ta.append(" (TTL expired)\n");
		else if(type==0) ta.append(" (ICMP Echo Reply)\n");
		ta.append("\t|-Code:\t"+code+"\n");
		ta.append("\t|-Checksum:\t"+checksum+"\n");
//		ta.append("\n\t|-Raw data       : \n");
//		Packet.printRawData(rawData, ta);
	}
}
