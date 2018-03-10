package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class IPHeader extends Header{
	private byte[] rawData;
	private int version, headerLen, typeOfService, totalLen, id, flagD, flagM,
			fragOffset, timeToLive, protocol, checksum;

	private String sourceIP, destIP;

	public IPHeader(byte[] packet, int b) {
		version = (packet[b] >> 4) & 0x0f;
		headerLen = packet[b] & 0x0f;
		typeOfService = packet[b+1] & 0xff;
		totalLen = (((packet[b+2] & 0xff) << 8) | (packet[b+3] & 0xff)) & 0xffff;
		id = (((packet[b+4] & 0xff) << 8) | (packet[b+5] & 0xff)) & 0xffff;
		flagD = (packet[b+6] >> 6) & 0x01;
		flagM = (packet[b+6] >> 5) & 0x01;
		fragOffset = (packet[b+6] & 0x1f << 8) | packet[b+7];
		timeToLive = packet[b+8] & 0xff;
		protocol = packet[b+9] & 0xff;
		checksum = (((packet[b+10] & 0xff) << 8) | (packet[b+11] & 0xff)) & 0xffff;

		sourceIP = (packet[b+12] & 0xff) + "." + (packet[b+13] & 0xff) + "."
				+ (packet[b+14] & 0xff) + "." + (packet[b+15] & 0xff);
		destIP = (packet[b+16] & 0xff) + "." + (packet[b+17] & 0xff) + "."
				+ (packet[b+18] & 0xff) + "." + (packet[b+19] & 0xff);
		
		rawData= new byte[headerLen*4];
		for(int i= 0; i<headerLen*4; i++){
			rawData[i]= packet[b+i];
		}
	}
	
	public byte[] getRawData(){
		return rawData;
	}

	public int getVersion() {
		return version;
	}

	public int getHeaderLen() {
		return headerLen*4;
	}

	public int getTypeOfService() {
		return typeOfService;
	}

	public int getTotalLen() {
		return totalLen;
	}

	public int getId() {
		return id;
	}

	public int getFlagD() {
		return flagD;
	}

	public int getFlagM() {
		return flagM;
	}

	public int getFragOffset() {
		return fragOffset;
	}

	public int getTimeToLive() {
		return timeToLive;
	}

	public int getProtocol() {
		return protocol;
	}

	public int getChecksum() {
		return checksum;
	}

	public String getSourceIP() {
		return sourceIP;
	}
	
	public String getDestIP() {
		return destIP;
	}
	
//	public void print(){
//		System.out.println("[ IP Header ]");
//		System.out.println("\t|-Version          : " + version);
//		System.out.println("\t|-Header length    : " + headerLen + " DWORDS or "
//				+ headerLen * 4 + " BYTES");
//		System.out.println("\t|-Type of service  : " + typeOfService);
//		System.out.println("\t|-Total length     : " + totalLen + " BYTES");
//		System.out.println("\t|-Identification   : " + id);
//		System.out.println("\t|-Do not fragment  : " + flagD);
//		System.out.println("\t|-More fragment    : " + flagM);
//		System.out.println("\t|-Fragment offset  : " + fragOffset);
//		System.out.println("\t|-Time to live     : " + timeToLive);
//		System.out.println("\t|-Protocol         : " + protocol);
//		System.out.println("\t|-Checksum         : " + checksum);
//		System.out.println("\t|-Source IP        : " + sourceIP);
//		System.out.println("\t|-Destination IP   : " + destIP);
//		System.out.println("\n\t|-Raw data       : ");
//		Packet.printRawData(rawData);
//	}
	
	public void fill(JTextArea ta){
		ta.append("[ IP Header ]\n");
		ta.append("\t|-Version:\t\t" + version+"\n");
		ta.append("\t|-Header length:\t" + headerLen + " DWORDS or "
				+ headerLen * 4 + " BYTES\n");
		ta.append("\t|-Type of service:\t" + typeOfService+"\n");
		ta.append("\t|-Total length:\t\t" + totalLen + " BYTES\n");
		ta.append("\t|-Identification:\t" + id+"\n");
		ta.append("\t|-Do not fragment:\t" + flagD+"\n");
		ta.append("\t|-More fragment:\t" + flagM+"\n");
		ta.append("\t|-Fragment offset:\t" + fragOffset+"\n");
		ta.append("\t|-Time to live:\t\t" + timeToLive+"\n");
		ta.append("\t|-Protocol:\t\t" + protocol+"\n");
		ta.append("\t|-Checksum:\t\t" + checksum+"\n");
		ta.append("\t|-Source IP:\t\t" + sourceIP+"\n");
		ta.append("\t|-Destination IP:\t" + destIP+"\n");
//		ta.append("\n\t|-Raw data       : \n");
//		Packet.printRawData(rawData, ta);
	}

	@Override
	public void log(BufferedWriter bw) throws IOException {
		bw.write("[ IP Header ]\n");
		bw.write("\t|-Version:\t\t" + version+"\n");
		bw.write("\t|-Header length:\t" + headerLen + " DWORDS or "
				+ headerLen * 4 + " BYTES\n");
		bw.write("\t|-Type of service:\t" + typeOfService+"\n");
		bw.write("\t|-Total length:\t\t" + totalLen + " BYTES\n");
		bw.write("\t|-Identification:\t" + id+"\n");
		bw.write("\t|-Do not fragment:\t" + flagD+"\n");
		bw.write("\t|-More fragment:\t" + flagM+"\n");
		bw.write("\t|-Fragment offset:\t" + fragOffset+"\n");
		bw.write("\t|-Time to live:\t\t" + timeToLive+"\n");
		bw.write("\t|-Protocol:\t\t" + protocol+"\n");
		bw.write("\t|-Checksum:\t\t" + checksum+"\n");
		bw.write("\t|-Source IP:\t\t" + sourceIP+"\n");
		bw.write("\t|-Destination IP:\t" + destIP+"\n");
	}
}
