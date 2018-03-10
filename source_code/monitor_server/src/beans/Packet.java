package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public class Packet {
	// public static void printRawData(byte[] packet) {
	// int i, j, len = packet.length;
	// for (i = 0; i < len; i++) {
	// if (i != 0 && i % 16 == 0) // if one line of hex printing is
	// // complete...
	// {
	// System.out.print("\t\t");
	// for (j = i - 16; j < i; j++) {
	// if (packet[j] >= 32 && packet[j] <= 128)
	// System.out.print((char) packet[j]); // if its a number
	// // or
	// // alphabet
	//
	// else
	// System.out.print("."); // otherwise print a dot
	// }
	// System.out.println();
	// }
	//
	// if (i % 16 == 0)
	// System.out.print("\t");
	// String hexDigit = Integer.toHexString(packet[i] & 0xff);
	// System.out.print(" " + ((hexDigit.length() == 1) ? "0" : "")
	// + hexDigit);
	//
	// if (i == len - 1) // print the last spaces
	// {
	// for (j = 0; j < 15 - i % 16; j++)
	// System.out.print("   "); // extra spaces
	//
	// System.out.print("\t\t");
	//
	// for (j = i - i % 16; j <= i; j++) {
	// if (packet[j] >= 32 && packet[j] <= 128)
	// System.out.print((char) packet[j]);
	// else
	// System.out.print(".");
	// }
	// System.out.println();
	// }
	// }
	// }

	public static void printRawData(byte[] packet, JTextArea ta) {
		ta.append("[Data Payload]\n");
		int i, len = packet.length;
		for (i = 0; i < len; i++) {
			if (i % 32 == 0) {
				ta.append("\n\t");
			}
			if (packet[i] >= 32 && packet[i] <= 128)
				ta.append(((char) packet[i]) + "");
			else
				ta.append(".");
			// if(i%32==0&&i!=0) ta.append("\n");
		}
	}

	public static int MAC_INDEX = 0;
	public static int IP_INDEX = 14;

	public enum PACKET_TYPE {
		OTHER, TCP, UDP, ICMP
	};

	private PACKET_TYPE type;

	private MACHeader mac;
	private IPHeader ip;

	private Header protocolHeader;

	private byte[] packetPayload;

	public Packet(byte[] packet) {
		mac = new MACHeader(packet, Packet.MAC_INDEX);
		ip = new IPHeader(packet, Packet.IP_INDEX);
		type = PACKET_TYPE.OTHER;
		int protoLen = 0;
		switch (ip.getProtocol()) {
		case 6:
			protocolHeader = new TCPHeader(packet, Packet.IP_INDEX
					+ ip.getHeaderLen());
			protoLen = ((TCPHeader) protocolHeader).getOff();
			this.type = PACKET_TYPE.TCP;
			break;

		case 17:
			protocolHeader = new UDPHeader(packet, Packet.IP_INDEX
					+ ip.getHeaderLen());
			protoLen = 8;
			this.type = PACKET_TYPE.UDP;
			break;

		case 1:
			protocolHeader = new ICMPHeader(packet, Packet.IP_INDEX
					+ ip.getHeaderLen());
			protoLen = 4;
			this.type = PACKET_TYPE.ICMP;
			break;
		}

		packetPayload = new byte[ip.getTotalLen() - ip.getHeaderLen()
				- protoLen];
		for (int i = 0; i < packetPayload.length; i++) {
			packetPayload[i] = packet[Packet.IP_INDEX + ip.getHeaderLen()
					+ protoLen + i];
		}
	}

	// public void print() {
	// System.out
	// .println("\n[---------------------------------PACKET---------------------------------]");
	// mac.print();
	// ip.print();
	// protocolHeader.print();
	// System.out.println("[Data Payload]");
	// Packet.printRawData(packetPayload);
	// System.out
	// .println("\n[-----------------------------------END----------------------------------]");
	// }

	public PACKET_TYPE getType() {
		return type;
	}

	public MACHeader getMac() {
		return mac;
	}

	public IPHeader getIp() {
		return ip;
	}

	public Header getProtocolHeader() {
		return protocolHeader;
	}

	public byte[] getPacketPayload() {
		return packetPayload;
	}

	public void setPacketPayload(byte[] packetPayload) {
		this.packetPayload = packetPayload;
	}

	public void fill(JTextArea ta) {
		ta.setText("");
		mac.fill(ta);
		ip.fill(ta);
		try {
			protocolHeader.fill(ta);
		} catch (Exception ex) {
		}
		
		Packet.printRawData(packetPayload, ta);
	}
	
	public void log(BufferedWriter bw) throws IOException{
		bw.write("\n------------------------------[ PACKET ]------------------------------\n");
		mac.log(bw);
		ip.log(bw);
		protocolHeader.log(bw);
		Packet.printRawData(packetPayload, bw);
	}
	
	public static void printRawData(byte[] packet, BufferedWriter bw) throws IOException{
		bw.write("[Data Payload]\n");
		int i, len = packet.length;
		for (i = 0; i < len; i++) {
			if (i % 32 == 0) {
				bw.write("\n\t");
			}
			if (packet[i] >= 32 && packet[i] <= 128)
				bw.write(((char) packet[i]) + "");
			else
				bw.write(".");
		}
	}

	@Override
	public String toString() {
		return null;
	}
}
