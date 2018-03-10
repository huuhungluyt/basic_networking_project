package views;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.URI;
import java.net.URL;
import java.net.URLConnection;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.EmptyBorder;

import beans.Host;
import main.Resources;

public class IPInfoView extends JDialog implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Host host = null;
	private InetAddress ip = null;
//	private float lon, lat;
	private JTextArea taIPInfo;
	private JButton btAccess, btLock, btUnlock;

	public IPInfoView(JFrame parent, Host host, InetAddress ip) {
		super(parent, ip.getHostName());
		this.host = host;
		this.ip = ip;

		taIPInfo = new JTextArea();
		taIPInfo.setEditable(false);
		JScrollPane srcIPInfo = new JScrollPane(taIPInfo);
		taIPInfo.setText(getIPInfo(ip));
		btAccess = new JButton(Resources.icURL);
		btLock = new JButton(Resources.icLock);
		btUnlock = new JButton(Resources.icUnlock);

		setSize(Resources.IPINFOVIEW_W, Resources.IPINFOVIEW_H);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setLayout(new BorderLayout());
		setResizable(false);
		setLocationRelativeTo(parent);

		JPanel wrapper = new JPanel(new BorderLayout(Resources.PADDING,
				Resources.PADDING));
		wrapper.setBorder(new EmptyBorder(Resources.PADDING, Resources.PADDING,
				Resources.PADDING, Resources.PADDING));
		JPanel pnIPInfoLabel = new JPanel(new FlowLayout(FlowLayout.LEADING));
		pnIPInfoLabel.add(new JLabel("IP Address infomation:", Resources.icIP,
				JLabel.HORIZONTAL));
		wrapper.add(pnIPInfoLabel, BorderLayout.NORTH);
		wrapper.setPreferredSize(new Dimension(this.getWidth()
				- this.getHeight(), 0));
		wrapper.add(srcIPInfo, BorderLayout.CENTER);

		JPanel pnButton = new JPanel();
		pnButton.setLayout(new GridLayout(1, 3));
		pnButton.add(btAccess);
		pnButton.add(btLock);
		pnButton.add(btUnlock);
		wrapper.add(pnButton, BorderLayout.SOUTH);

		add(wrapper, BorderLayout.CENTER);

		setVisible(true);

		btAccess.addActionListener(this);
		btLock.addActionListener(this);
		btUnlock.addActionListener(this);
	}

	public String getIPInfo(InetAddress ip) {
		try {
			URL url = new URL("https://ipfind.co/?ip=" + ip.getHostAddress());
			URLConnection con = url.openConnection();
			InputStream in = con.getInputStream();
			String encoding = con.getContentEncoding();
			encoding = encoding == null ? "UTF-8" : encoding;
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			byte[] buf = new byte[8192];
			int len = 0;
			while ((len = in.read(buf)) != -1) {
				baos.write(buf, 0, len);
			}
			String body = new String(baos.toByteArray(), encoding);
			String attrs[] = body.split(",");
			String result = "";
			int i;
			for (i = 0; i < attrs.length; i++) {
				attrs[i] = attrs[i].trim().replaceAll("[\\s{\"\\]]", "");
				String temp[] = attrs[i].split(":");
				result += temp[0] + ":\t" + temp[1];
//				if (temp[0].equals("longitude"))
//					lon = Float.parseFloat(temp[1]);
//				if (temp[0].equals("latitude"))
//					lat = Float.parseFloat(temp[1]);
				if (temp[0].equals("languages"))
					break;
				result += "\n";
			}
			for (i++; i < attrs.length - 1; i++) {
				result += ", " + attrs[i];
			}
			return result;
		} catch (Exception ex) {
			ex.printStackTrace();
			return "[ERROR]: IP not found !";
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		Object click = e.getSource();
		if (click == btLock) {
			host.sendRequest("lock:" + ip.getHostAddress());
		} else if (click == btUnlock) {
			host.sendRequest("unlock:" + ip.getHostAddress());
		} else if(click == btAccess){
			try {
				URI link = new URI("http://" + ip.getHostName());
				if (Desktop.isDesktopSupported()) {
					Desktop.getDesktop().browse(link);
				}
			} catch (Exception e1) {
				e1.printStackTrace();
			}
		}
	}
}
