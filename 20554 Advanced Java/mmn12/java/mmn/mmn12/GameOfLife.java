package mmn.mmn12;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Objects;

public class GameOfLife extends Application {

    private Controller controller;
    final int screenWidth = 1200;
    final int screenHeight = 1000;

    /**
     * loads the fxml, scene and stage
     * also handles the 'press D' event
     * @param stage the primary stage for this application, onto which
     * the application scene can be set.
     * Applications may create other stages, if needed, but they will not be
     * primary stages.
     * @throws IOException
     */
    @Override
    public void start(Stage stage) throws IOException {

        //loads and runs the fxml file
        FXMLLoader fxmlLoader = new FXMLLoader(GameOfLife.class.getResource("gameoflife.fxml"));
        Scene scene = new Scene(fxmlLoader.load(),screenWidth,screenHeight);
        stage.setTitle("Game Of Life");
        stage.setScene(scene);
        stage.show();
        controller = fxmlLoader.getController();

        //Event handler for pressing D to speed throw generations
        stage.addEventHandler(KeyEvent.KEY_PRESSED, (KeyEvent event) -> {
            if (Objects.requireNonNull(event.getCode()) == KeyCode.D) {
                controller.NextGeneration();
            }
        });
    }

    public static void main(String[] args) {
        launch();
    }
}