package ClientSide;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.scene.text.TextFlow;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.ConnectException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Optional;

public class Controller {

    @FXML
    private ListView<VBox> chatLog;
    @FXML
    private Label chatMembers;
    @FXML
    private Label chatName;
    @FXML
    private TextArea textBox;
    @FXML
    private ScrollPane scrollPane;

    public static final int MAX_MESSAGE_WIDTH = 650; //Max message width in pixels
    private User user;
    private String serverName;
    private String userName;
    private boolean validLogin = false; //user has entered info and pressed enter button in login screen

    public void initialize() {
        //display login page until user pressed the confirmation buttons
        System.out.println("DEBUG:Displaying login");
        enterChat();
        //user canceled joining chat
        if (!validLogin) {
            System.out.println("DEBUG:User canceled connection");
            System.exit(0);
        }
        //user wants to enter the chat server
        System.out.println("DEBUG:Attempting to initialize client controller");
        try {
            user = new User(new Socket(serverName, 8888), userName); //create new user
            user.receive(chatLog, user, chatMembers, chatName);   //start thread
            System.out.println("Connected");
        } catch (UnknownHostException | ConnectException e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText(null);
            if (e instanceof UnknownHostException) {
                alert.setContentText("Unknown host name: " + e.getMessage());
            }
            else
                alert.setContentText("Host isn't reachable, server is down");
            alert.showAndWait();
            System.out.println("Error connecting to server");
            System.exit(1);
        }
        catch (IOException e) {
            System.out.println("Error connecting to server");
            System.exit(1);
        }
        System.out.println("DEBUG:joining group");
        user.joinGroup();
        chatMembers.setText("");


    }

    public static void serverDisconnected(){
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText("Server is down");
        alert.showAndWait();
        Platform.exit();
    }

    /**
     * display server room info
     * all chat members and server name
     */
    @FXML
    void getInfo(){
        Dialog<ButtonType> dialog = new Dialog<>();
        dialog.setTitle("Info");
        dialog.setHeaderText("chat room created for assignment 16 for advanced java course");
        dialog.setContentText("Chat name: " + chatName.getText()+"\nMembers: " + chatMembers.getText());
        dialog.getDialogPane().getButtonTypes().add(ButtonType.OK);
        dialog.showAndWait();

    }

    /**
     * exit group button onAction
     * Opens Dialog confirmation before exit
     */
    @FXML
    void exitChat() {
        Dialog<ButtonType> dialog = new Dialog<>();
        dialog.setTitle("Exit");
        dialog.setContentText("Are you sure you want to leave the chat?");
        dialog.getDialogPane().getButtonTypes().addAll(ButtonType.YES, ButtonType.NO);
        Optional<ButtonType> result = dialog.showAndWait();
        if (result.get() == ButtonType.YES) {
            closeUser();
            Platform.exit();
        }

    }

    /**
     * send leave group message to server
     * and close all connections
     */
    public void closeUser() {
        if (user != null) {
            user.leaveGroup();
            user.closeAll();
        }
    }

    /**
     * create and handle Login window
     */
    private void enterChat() {
        //create stage
        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL); //block main application window from showing while this window is open
        stage.setTitle("Enter chat server");

        Label serverLabel = new Label("Server Name");
        TextField getServerName = new TextField();
        getServerName.setPromptText("server name, i.e localhost");

        Label nameLabel = new Label("Username");
        TextField getUsername = new TextField();
        getUsername.setPromptText("username, i.e Mike.k");

        Button enterButton = new Button("Enter");
        Button cancelButton = new Button("Cancel");

        //user wants to enter
        enterButton.setOnAction(e -> {
            serverName = getServerName.getText();
            userName = getUsername.getText();
            validLogin = true;
            stage.close();
        });

        //user cancels
        cancelButton.setOnAction(e -> {
            stage.close();
        });

        HBox buttons = new HBox(enterButton, cancelButton);
        buttons.setStyle("-fx-padding:20 0 0 0 ; -fx-spacing: 10px; -fx-alignment: bottom-right; -fx-fill-height: true");

        VBox layout = new VBox(10, serverLabel, getServerName, nameLabel, getUsername, buttons);
        layout.setStyle("-fx-padding: 30px; -fx-alignment: top-left; -fx-spacing: 10px; -fx-fill-weight: true");

        Scene scene = new Scene(layout, 300, 230);
        stage.setScene(scene);
        stage.showAndWait();
    }

    /**
     * set the chatName label to message content
     *
     * @param chatName label to set
     * @param message  message content
     */
    public static void updateServerName(Label chatName, Message message) {
        chatName.setText(message.getText());
    }

    /**
     * set the chatMembers label to message content
     *
     * @param chatMembers label to set
     * @param message     message content
     */
    public static void updateUserList(Label chatMembers, Message message) {
        chatMembers.setText(message.getText());
    }

    /**
     * add message to chatLog
     *
     * @param chatLog chatLog component
     * @param message Message to add
     * @param user    user who received the message
     */
    public static void getMessage(ListView<VBox> chatLog, Message message, User user) {
        VBox container = createMessageBox(chatLog, message, user);
        chatLog.getItems().add(container);
        chatLog.scrollTo(container);

    }

    /**
     * send button OnAction
     * formulates message and sends to server
     */
    @FXML
    void sendMessage() {
        if (textBox.getText().isEmpty()) {
            return;
        }
        Message newMessage = new Message(textBox.getText(), user, Message.Type.THIS);
        user.sendMessage(newMessage);
        textBox.clear();
    }

    /**
     * create message display component
     *
     * @param chatLog chatLog to contain the message and bind width to
     * @param message message object
     * @param user    user receiving the message
     * @return The message component
     */
    private static VBox createMessageBox(ListView<VBox> chatLog, Message message, User user) {
        //message type based on receiving user and message originator and message type
        Message.Type type;
        //Server messages
        if (message.getUser() == null || message.getType() == Message.Type.JOIN_GROUP || message.getType() == Message.Type.LEAVE_GROUP)
            type = Message.Type.SERVER;
        //This user messages
        else if (message.getUser().equals(user))
            type = Message.Type.THIS;
        //other Users messages
        else
            type = Message.Type.OTHER;

        //create component
        Text messageContent = new Text(message.getText());
        TextFlow messageFlow = new TextFlow();
        messageFlow.getStyleClass().add("user-label");

        //set look of text message
        if (type == Message.Type.OTHER) {
            Text username = new Text(message.getUser().getUsername() + "\n");
            username.setFill(message.getUser().getColor());
            messageFlow.getChildren().add(username);

        } else if (type == Message.Type.THIS) {
            messageFlow.getStyleClass().add("this-user-label");

        } else { //Server messages
            messageContent.setFill(Color.DARKSLATEGRAY);
            messageFlow.setTextAlignment(TextAlignment.CENTER);
            messageFlow.getStyleClass().add("server-label");
        }

        messageFlow.getChildren().add(messageContent);
        messageFlow.setMaxWidth(MAX_MESSAGE_WIDTH);
        messageFlow.prefWidthProperty().bind(chatLog.widthProperty().multiply(0.4)); //bind width

        VBox messageBox = new VBox(messageFlow);
        messageBox.setFillWidth(false);
        //set message direction based on type
        switch (type) {
            case SERVER:
                messageBox.setAlignment(Pos.CENTER);
                break;
            case THIS:
                messageBox.setAlignment(Pos.CENTER_RIGHT);
                break;
            case OTHER:
                messageBox.setAlignment(Pos.CENTER_LEFT);
                break;
            default:
                messageBox.setDisable(true);
                messageContent.setText("Server Error");
                break;
        }
        return messageBox;
    }


}
