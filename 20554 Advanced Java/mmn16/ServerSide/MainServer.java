package ServerSide;

import java.io.IOException;
import java.net.ServerSocket;


public class MainServer{

    public static void main(String[] args) {
        try {
            //Create new Server
            new Server(new ServerSocket(8888));

        } catch (IOException e) {
            System.err.println("Could not create server");
        }

    }
}
