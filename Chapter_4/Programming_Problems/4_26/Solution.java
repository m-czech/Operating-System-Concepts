import java.net.*;
import java.io.*;

class RequestHandler implements Runnable {
    Socket socket;
    RequestHandler(Socket socket) {
        this.socket = socket;
    }
    public void run() {
        try {
            PrintWriter pout = new PrintWriter(socket.getOutputStream(), true);
            pout.println(new java.util.Date().toString());
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

public class Solution
{
    public static void main(String[] args) {
        try {
            ServerSocket sock = new ServerSocket(6013);
            while (true) {
                Socket client = sock.accept();
                Thread requestHandler = new Thread(new RequestHandler(client));
                requestHandler.start();
            }
        }
        catch (Exception ignored) {}
    }
}