package beans;

import java.io.BufferedWriter;
import java.io.IOException;

import javax.swing.JTextArea;

public abstract class Header {
	protected byte[] rawData;
	
	public Header(){
		
	}
	
//	public abstract void print();
	public abstract void fill(JTextArea ta);
	
	public abstract void log(BufferedWriter bw) throws IOException;
}
