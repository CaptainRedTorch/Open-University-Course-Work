package ClientSide;

import java.io.Serializable;

public class Message implements Serializable {


    public enum Type {
        SERVER,     //Message sent from server
        OTHER,      //Message from other user
        THIS,       //This user message
        JOIN_GROUP, //User joined the server
        LEAVE_GROUP,//User left the server
        USERID,     //Message containing UserID
        USER_LIST,  //Message containing active user list
        SERVER_NAME //Message containing server chat name
    }

    private String messageContent;  //The message itself
    private User user;              //User originator of message
    private Type type;              //Type of message

    /**
     * Constructor for message
     * @param message the string contents of the message
     * @param user    the user originating
     * @param type    the type
     */
    public Message(String message, User user, Type type) {
        this.user = user;
        this.type = type;
        messageContent = message;
    }

    /**
     * @return String content of the message
     */
    public String getText() {
        return messageContent;
    }

    /**
     * @return User of the message
     */
    public User getUser() {
        return user;
    }

    /**
     * @return Type of the message
     */
    public Type getType() {
        return type;
    }


    /**
     * @param type to set
     */
    public void setType(Type type) {
        this.type = type;
    }

    /**
     * @return String representation of the message formated as [username : content : type]
     */
    public String toString() {
        String toReturn = "";
        if (user != null) {
            toReturn += user.getUsername();
        }
        toReturn += " : " + messageContent + " : " + type.toString();
        return toReturn;
    }

}
