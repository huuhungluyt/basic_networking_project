package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class TCPHeader extends Header{
	private byte[] rawData;
	private int sourcePort, destPort, off, window, checksum, urgpoint;
	private int[] flags;
	private long seqNum, ackNum;

	public TCPHeader(byte[] packet, int b) {
		sourcePort = (((packet[b] & 0xff) << 8) | (packet[b + 1] & 0xff)) & 0xffff;
		destPort = (((packet[b + 2] & 0xff) << 8) | (packet[b + 3] & 0xff)) & 0xffff;
		flags = new int[8];
		off = (packet[b + 12] & 0xf0) >> 4;
		flags[0] = (packet[b + 13] & 0x80) >> 7;
		flags[1] = (packet[b + 13] & 0x40) >> 6;
		flags[2] = (packet[b + 13] & 0x20) >> 5;
		flags[3] = (packet[b + 13] & 0x10) >> 4;
		flags[4] = (packet[b + 13] & 0x08) >> 3;
		flags[5] = (packet[b + 13] & 0x04) >> 2;
		flags[6] = (packet[b + 13] & 0x02) >> 1;
		flags[7] = (packet[b + 13] & 0x01) >> 7;
		window = (((packet[b + 14] & 0xff) << 8) | (packet[b + 15] & 0xff)) & 0xffff;
		checksum = (((packet[b + 16] & 0xff) << 8) | (packet[b + 17] & 0xff)) & 0xffff;
		urgpoint = (((packet[b + 18] & 0xff) << 8) | (packet[b + 19] & 0xff)) & 0xffff;

		seqNum = (((packet[b + 4] & 0xff) << 24)
				| ((packet[b + 5] & 0xff) << 16)
				| ((packet[b + 6] & 0xff) << 8) | (packet[b + 7] & 0xff)) & 0xffffffffL;
		ackNum = (((packet[b + 8] & 0xff) << 24)
				| ((packet[b + 9] & 0xff) << 16)
				| ((packet[b + 10] & 0xff) << 8) | (packet[b + 11] & 0xff)) & 0xffffffffL;
		
		
		rawData= new byte[off*4];
		for(int i= 0; i<off*4; i++){
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

	public int getOff() {
		return off*4;
	}

	public int getWindow() {
		return window;
	}

	public int getChecksum() {
		return checksum;
	}

	public int getUrgpoint() {
		return urgpoint;
	}

	public long getSeqNum() {
		return seqNum;
	}

	public long getAckNum() {
		return ackNum;
	}
	
	public int getFlagCWR(){
		return flags[0];
	}
	
	public int getFlagECE(){
		return flags[1];
	}

	
	public int getFlagURG(){
		return flags[2];
	}

	
	public int getFlagACK(){
		return flags[3];
	}

	
	public int getFlagPSH(){
		return flags[4];
	}

	
	public int getFlagRST(){
		return flags[5];
	}

	
	public int getFlagSYN(){
		return flags[6];
	}

	public int getFlagFIn(){
		return flags[7];
	}


//	public void print(){
//		System.out.println("[ TCP Header ]");
//		System.out.println("\t|-Source port         : " + sourcePort);
//		System.out.println("\t|-Destination port    : " + destPort);
//		System.out.println("\t|-SEQ Number          : " + seqNum);
//		System.out.println("\t|-ACK Number          : " + ackNum);
//		System.out.println("\t|-Offset              : " + off + " DWORDS or "
//				+ off * 4 + " BYTES");
//		System.out.println("\t|-Flags               : ");
//		System.out.println("\t\t|-CWR    : " + flags[0]);
//		System.out.println("\t\t|-ECE    : " + flags[1]);
//		System.out.println("\t\t|-URG    : " + flags[2]);
//		System.out.println("\t\t|-ACK    : " + flags[3]);
//		System.out.println("\t\t|-PSH    : " + flags[4]);
//		System.out.println("\t\t|-RST    : " + flags[5]);
//		System.out.println("\t\t|-SYN    : " + flags[6]);
//		System.out.println("\t\t|-FIN    : " + flags[7]);
//		System.out.println("\t|-Window              : " + window);
//		System.out.println("\t|-Checksum            : " + checksum);
//		System.out.println("\t|-Urgent pointer      : " + urgpoint);
//		System.out.println("\n\t|-Raw data       : ");
//		Packet.printRawData(rawData);
//	}
	
	public void fill(JTextArea ta){
		ta.append("[ TCP Header ]\n");
		ta.append("\t|-Source port:\t\t" + sourcePort+"\n");
		ta.append("\t|-Destination port:\t" + destPort+"\n");
		ta.append("\t|-SEQ Number:\t" + seqNum+"\n");
		ta.append("\t|-ACK Number:\t" + ackNum+"\n");
		ta.append("\t|-Offset:\t\t" + off + " DWORDS or "
				+ off * 4 + " BYTES\n");
		ta.append("\t|-Flags:\n");
		ta.append("\t\t|-CWR:\t" + flags[0]+"\n");
		ta.append("\t\t|-ECE:\t" + flags[1]+"\n");
		ta.append("\t\t|-URG:\t" + flags[2]+"\n");
		ta.append("\t\t|-ACK:\t" + flags[3]+"\n");
		ta.append("\t\t|-PSH:\t" + flags[4]+"\n");
		ta.append("\t\t|-RST:\t" + flags[5]+"\n");
		ta.append("\t\t|-SYN:\t" + flags[6]+"\n");
		ta.append("\t\t|-FIN:\t" + flags[7]+"\n");
		ta.append("\t|-Window:\t\t" + window+"\n");
		ta.append("\t|-Checksum:\t\t" + checksum+"\n");
		ta.append("\t|-Urgent pointer:\t" + urgpoint+"\n");
//		ta.append("\n\t|-Raw data       : ");
//		Packet.printRawData(rawData, ta);
	}



	@Override
	public void log(BufferedWriter bw) throws IOException {
		bw.write("[ TCP Header ]\n");
		bw.write("\t|-Source port:\t\t" + sourcePort+"\n");
		bw.write("\t|-Destination port:\t" + destPort+"\n");
		bw.write("\t|-SEQ Number:\t" + seqNum+"\n");
		bw.write("\t|-ACK Number:\t" + ackNum+"\n");
		bw.write("\t|-Offset:\t\t" + off + " DWORDS or "
				+ off * 4 + " BYTES\n");
		bw.write("\t|-Flags:\n");
		bw.write("\t\t|-CWR:\t" + flags[0]+"\n");
		bw.write("\t\t|-ECE:\t" + flags[1]+"\n");
		bw.write("\t\t|-URG:\t" + flags[2]+"\n");
		bw.write("\t\t|-ACK:\t" + flags[3]+"\n");
		bw.write("\t\t|-PSH:\t" + flags[4]+"\n");
		bw.write("\t\t|-RST:\t" + flags[5]+"\n");
		bw.write("\t\t|-SYN:\t" + flags[6]+"\n");
		bw.write("\t\t|-FIN:\t" + flags[7]+"\n");
		bw.write("\t|-Window:\t\t" + window+"\n");
		bw.write("\t|-Checksum:\t\t" + checksum+"\n");
		bw.write("\t|-Urgent pointer:\t" + urgpoint+"\n");
	}
}
