package ClientSide;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class Main extends Application {

    @Override
    public void start(Stage stage) throws IOException{
        System.out.println("DEBUG:Attempting to open chatFX");
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("/ClientSide/ChatFX.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("chat");
        stage.setScene(scene);

        //comment next line to remove css style from application
        scene.getStylesheets().add(getClass().getResource("/ClientSide/style.css").toExternalForm());
        stage.show();

        System.out.println("DEBUG:Showing chatFX");

        //configure exit stage(window) button to disconnect user
        Controller controller = fxmlLoader.getController();
        stage.setOnCloseRequest(event -> {
            controller.closeUser();
            System.out.println("DEBUG:Closing chatFX");
        });

    }


    public static void main(String[] args) {
        launch(args);
    }
}
