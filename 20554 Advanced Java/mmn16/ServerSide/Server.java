package ServerSide;

import ClientSide.Message;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

public class Server {

    private final static String serverName = "Server Name";      //server name displayed to users
    public static final int PORT = 8888;                        //server port
    private ServerSocket serverSocket;
    private Map<String, UserHandler> users;                     //list of active users(connections)
    private List<Message> messages;                             //log of messages


    /**
     * Constructor for multi-client tcp chat room server
     * with message logging
     * @param serverSocket
     */
    public Server(ServerSocket serverSocket) {
        this.serverSocket = serverSocket;
        messages = new ArrayList<>();
        users = new HashMap<>();

        //accept new connections
        System.out.println("Server started");
        while (true) {
            try {
                acceptNewConnection(serverSocket.accept());
                System.out.println("DEBUG:New connection accepted");
            } catch (IOException e) {
                System.out.println("Error accepting new connection");
                throw new RuntimeException(e);
            }
        }
    }

    /**
     * Create new user thread to handle the new user
     *
     * @param clientSocket the new user accepted socket connection
     */
    public void acceptNewConnection(Socket clientSocket) {
        UserHandler userHandler = new UserHandler(clientSocket, users, messages);
        users.put(userHandler.userID, userHandler); //add to users list
        new Thread(userHandler).start();

    }

    //class to handle user connections and maintain input and output stream for each.
    public static class UserHandler implements Runnable {
        private Map<String, UserHandler> users; //active users list
        private List<Message> messages;         //messages log
        private String userID;
        private String username;
        private Socket clientSocket;
        private ObjectInputStream in;
        private ObjectOutputStream out;

        /**
         * Constructor of userHandler
         *
         * @param clientSocket
         * @param users        map of users
         * @param messages     list of messages
         */
        public UserHandler(Socket clientSocket, Map<String, UserHandler> users, List<Message> messages) {
            this.clientSocket = clientSocket;
            this.users = users;
            this.messages = messages;
            this.username = "missing";
            this.userID = UUID.randomUUID().toString();
            try {
                System.out.println("DEBUG:Creating out stream");
                this.out = new ObjectOutputStream(clientSocket.getOutputStream());
                System.out.println("DEBUG:Creating in steam");
                this.in = new ObjectInputStream(clientSocket.getInputStream());
            } catch (IOException e) {
                System.out.println("Error in UserHandler");
                e.printStackTrace();
            }
            //send user their id
            sendUserID();
            //send server name
            sendServerName();
        }

        /**
         * receive messages from client
         */
        @Override
        public void run() {
            try {
                //while user has an active connection
                while (!clientSocket.isClosed()) {
                    System.out.println("DEBUG:Attempting to read message");
                    Object obj = in.readObject();
                    if (obj instanceof Message) {
                        Message message = (Message) obj;
                        switch (message.getType()) {
                            //User sent message directed to chat
                            case THIS:
                                messages.add(message); //add to message log
                                broadcastMessage(message);
                                System.out.println("DEBUG:Message received from user: " + message);
                                break;
                            //User joined the chat
                            case JOIN_GROUP:
                                username = message.getUser().getUsername(); //update username on server side
                                sendUserList();     //send active users usernames to new user
                                sendChatMessage();  //send message log to new user
                                messages.add(message); //add to message log
                                broadcastMessage(message);
                                System.out.println("DEBUG:User joined group");
                                break;
                            //User left the chat
                            case LEAVE_GROUP:
                                messages.add(message); //add to message log
                                broadcastMessage(message);
                                System.out.println("DEBUG:User left group");
                                closeAll(); //close all socket connections
                                break;
                        }
                    }
                }
            } catch (IOException | ClassNotFoundException e) {
                System.out.println("Error in UserHandler receive");
                closeAll();
            } finally {
                try {
                    System.out.println("DEBUG:Closing connection");
                    closeAll();
                    users.remove(userID); //remove user from active user list when disconnected

                    //send updated user list after user disconnected from server
                    sendUserList();

                    System.out.println("DEBUG:User removed");
                } catch (Exception e) {
                    System.out.println("Error in UserHandler closing thread connection");
                }
            }
        }

        /**
         * Send user the username list of active users
         */
        public void sendUserList() {
            //create username list
            String userList = "";
            for (UserHandler user : users.values()) {
                userList += user.username + ", ";
            }
            if (userList.length() > 1)
                userList = userList.substring(0, userList.length() - 2); //remove ", " from end

            //send username list
            Message message = new Message(userList, null, Message.Type.USER_LIST);
            System.out.println("DEBUG:Sending user list");
            broadcastMessage(message);
        }

        /**
         * send chat name
         */
        public void sendServerName() {
            Message message = new Message(serverName, null, Message.Type.SERVER_NAME);
            System.out.println("DEBUG:Sending Server Name");
            sendMessage(message);
        }

        /**
         * send user id
         */
        public void sendUserID() {
            Message message = new Message(this.userID, null, Message.Type.USERID);
            System.out.println("DEBUG:Sending UserID");
            sendMessage(message);
        }

        /**
         * send chat message log
         */
        public void sendChatMessage() {
            System.out.println("DEBUG:Sending chatlog");
            for (Message message : messages) {
                sendMessage(message);
            }
        }

        /**
         * send message to user
         * @param message message to be sent to this user
         */
        public void sendMessage(Message message) {
            try {
                out.writeObject(message);
                System.out.println("DEBUG:Message broadcast sent");
                out.flush();
            } catch (IOException e) {
                System.out.println("Error in UserHandler send message");
            }
        }

        /**
         * message to be broadcast to all users
         * @param message message to be broadcast
         */
        public void broadcastMessage(Message message) {
            for (UserHandler user : users.values()) {
                user.sendMessage(message);
            }
        }

        /**
         * close all connections to server
         * close input, output, and socket of this user
         */
        private void closeAll() {
            try {
                in.close();
                out.close();
                clientSocket.close();
            } catch (IOException e) {
                System.out.println("Error closing connection");
                throw new RuntimeException(e);
            }
        }

    }
}
