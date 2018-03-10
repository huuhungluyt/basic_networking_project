package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class UDPHeader extends Header{
	private byte[] rawData;
	private int sourcePort, destPort, length, checksum;

	public UDPHeader(byte[] packet, int b) {
		sourcePort = (((packet[b] & 0xff) << 8) | (packet[b + 1] & 0xff)) & 0xffff;
		destPort = (((packet[b + 2] & 0xff) << 8) | (packet[b + 3] & 0xff)) & 0xffff;
		length = (((packet[b + 4] & 0xff) << 8) | (packet[b + 5] & 0xff)) & 0xffff;
		checksum = (((packet[b + 6] & 0xff) << 8) | (packet[b + 7] & 0xff)) & 0xffff;
		
		rawData= new byte[8];
		for(int i= 0; i<8; i++){
			rawData[i]= packet[b+i];
		}
	}
	
	public byte[] getRawData() {
		return rawData;
	}

	public int getSourcePort() {
		return sourcePort;
	}

	public int getDestPort() {
		return destPort;
	}
	public int getLength() {
		return length;
	}

	public int getChecksum() {
		return checksum;
	}

//	public void print(){
//		System.out.println("\n[ UDP Header ]");
//		System.out.println("\t|-Source port         : " + sourcePort);
//		System.out.println("\t|-Destination port    : " + destPort);
//		System.out
//				.println("\t|-Length              : " + length + " BYTES");
//		System.out.println("\t|-Checksum            : " + checksum);
//		System.out.println("\n\t|-Raw data       : ");
//		Packet.printRawData(rawData);
//	}
	
	public void fill(JTextArea ta){
		ta.append("[ UDP Header ]\n");
		ta.append("\t|-Source port:\t\t" + sourcePort+"\n");
		ta.append("\t|-Destination port:\t" + destPort+"\n");
		ta.append("\t|-Length:\t\t" + length + " BYTES\n");
		ta.append("\t|-Checksum:\t\t" + checksum+"\n");
//		ta.append("\n\t|-Raw data       :\n");
//		Packet.printRawData(rawData, ta);
	}

	@Override
	public void log(BufferedWriter bw) throws IOException {
		bw.write("[ UDP Header ]\n");
		bw.write("\t|-Source port:\t\t" + sourcePort+"\n");
		bw.write("\t|-Destination port:\t" + destPort+"\n");
		bw.write("\t|-Length:\t\t" + length + " BYTES\n");
		bw.write("\t|-Checksum:\t\t" + checksum+"\n");
	}
}
