package mmn.mmn12;

import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.ArcType;
import javafx.scene.control.Label;


public class Controller {

    GraphicsContext gc; //canvas graphic component
    GameLogic newGame;  //the Game of Life instance
    double sizeMulti;   //size multiplier for graphics
    final double canvasSize = 900.0;    //height and width size of the canvas
    final int bunnySize = 50;           //easter-egg: bunny displays only for tiles that are 50x50

    @FXML
    protected Slider slider;

    @FXML
    private Label sliderLabel;

    @FXML
    private Label genCountID;

    @FXML
    private Canvas canvas1;

    //runs on start
    public void initialize() {
        int boardSize;
        boardSize = (int)slider.getValue();
        sizeMulti =(canvasSize/boardSize);

        newGame = new GameLogic(boardSize,true);
        canvas1.setHeight(newGame.getBoardSize()*sizeMulti);
        canvas1.setWidth(newGame.getBoardSize()*sizeMulti);

        slider.setStyle(
                "-fx-base: #2a2929; " +
                "-fx-focus-color:#b0b0b0; " +
                "-fx-faint-focus-color: transparent;");

        gc = canvas1.getGraphicsContext2D();
        drawGrid();
        drawGame();
        genCountID.setText(""+newGame.getGenCount());
    }

    /**
     * Draws the grid based on the size multiplier and board size relative to window size
     */
    private void drawGrid() {

        gc.setFill(Color.LIGHTGRAY);
        gc.fillRect(0, 0, canvas1.getWidth(), canvas1.getHeight());

        gc.setStroke(Color.SILVER);

        gc.setLineWidth(1);
        for (int i = 0; i <= newGame.getBoardSize(); i++){
            gc.strokeLine(0,i*sizeMulti,canvas1.getHeight(),i*sizeMulti);
            gc.strokeLine(i*sizeMulti,canvas1.getHeight(),i*sizeMulti,0);
        }
    }

    /**
     * Draws the tiles on the grid
     */
    private void drawGame() {
        for (int i = 0; i < newGame.getBoardSize(); i++) {
            for (int j = 0; j < newGame.getBoardSize(); j++) {
                if (newGame.getBoardTile(i,j)) {
                    if (sizeMulti == bunnySize) //easter-egg: only at a specific size would the bunny display
                        drawBunny(i*sizeMulti, j*sizeMulti);
                    else    //normal black squares
                        drawSquare(i*sizeMulti, j*sizeMulti);
                }
            }
        }
    }

    /**
     * Draws the life tiles at x,y
     * size is based on size multiplier
     * @param x coordinates
     * @param y coordinates
     */
    private void drawSquare(double x ,double y) {
        gc.setFill(Color.BLACK);
        gc.fillRect(x,y,sizeMulti,sizeMulti);
    }

    /**
     * Draws the life tiles as bunnies sized 50x50
     * does not display right for any other size, tiles are 50x50 when board size is 18
     * @param x coordinates
     * @param y coordinates
     */
    private void drawBunny(double x, double y) {
        //  Easter-egg bunny
        gc.setFill(Color.rgb(220,129,129));
        gc.fillRect(x+1,y+1,sizeMulti-1,sizeMulti-1);
        gc.setLineWidth(2);

        // Draw bunny ears
        gc.setFill(Color.DIMGRAY);
        gc.fillOval(x+13 , y+7, 7, 23);  // Left ear
        gc.fillOval(x + 30, y+7, 7, 23); // Right ear
        gc.setStroke(Color.GAINSBORO);
        gc.strokeOval(x+13 , y+7, 7, 23);
        gc.strokeOval(x + 30, y+7, 7, 23);

        // Draw bunny face
        gc.setFill(Color.GAINSBORO);
        gc.fillOval(x + 9, y + 13, 31, 26);    // Face
        gc.fillOval(x+7, y + 21, 23, 23);   // Left Cheeks
        gc.fillOval(x + 20, y + 21, 23, 23);    // Right Cheeks
        gc.setFill(Color.LIGHTCORAL);
        gc.fillOval(x + 9, y + 29, 7, 5);   // Left blush
        gc.fillOval(x + 34, y + 29, 7, 5);  // Right blush

        // Draw bunny eyes
        gc.setFill(Color.BLACK);
        gc.fillOval(x + 13, y + 20, 7, 7);   // Left eye
        gc.fillOval(x + 30, y + 20, 7, 7);   // Right eye
        gc.setStroke(Color.WHITE);
        gc.strokeArc(x+15,y+22,3,2,-180,-50, ArcType.OPEN); // Left sparkle
        gc.strokeArc(x+32,y+22,3,2,-180,-80, ArcType.OPEN); // Right sparkle

        // Draw bunny nose
        gc.setFill(Color.DIMGRAY);
        gc.fillOval(x + 21, y + 30, 8, 5);   // Nose
        gc.fillRect(x+24,y+34,2,6);// Nose line

        // Draw bunny mouth
        gc.setFill(Color.DIMGRAY);
        gc.fillRoundRect(x+20,y+38,10,2,2,2); // Upper line


    }

    /**
     * Calculates the next Game of Life generation
     * and draws it onto the board
     */
    @FXML
    public void NextGeneration() {
        newGame.addGenCount();
        genCountID.setText(""+newGame.getGenCount());
        newGame.nextGen();
        gc.clearRect(0, 0, canvas1.getWidth(), canvas1.getHeight());
        drawGrid();
        drawGame();
    }

    /**
     * Updates the slider label position and value to match the thumb of the slider
     */
    @FXML
    public void ShowSliderLabel(){
        sliderLabel.setOpacity(1);
        slider.applyCss();
        slider.addEventHandler(MouseEvent.MOUSE_DRAGGED, (MouseEvent event) -> {
            sliderLabel.setTranslateX(slider.lookup(".thumb").getLayoutX());
            sliderLabel.setText((int)slider.getValue()+"");
        });
        sliderLabel.setTranslateX(slider.lookup(".thumb").getLayoutX());
        sliderLabel.setText((int)slider.getValue()+"");
    }

    /**
     * Hides the slider label
     */
    @FXML
    public void HideSliderLabel(){
        sliderLabel.setOpacity(0);
    }

}
