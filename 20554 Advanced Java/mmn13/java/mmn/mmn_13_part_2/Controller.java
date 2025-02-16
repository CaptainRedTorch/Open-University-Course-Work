package mmn.mmn_13_part_2;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.effect.ColorAdjust;
import javafx.scene.input.MouseEvent;

import java.io.FileNotFoundException;
import java.util.*;

public class Controller {

    @FXML
    private Button answer1;
    @FXML
    private Button answer2;
    @FXML
    private Button answer3;
    @FXML
    private Button answer4;
    @FXML
    private Button nextBtn;
    @FXML
    private Label questionCount;
    @FXML
    private Label questionLabel;
    @FXML
    private Label quizName;
    @FXML
    private Label score;

    private Button correctBtn;          // The button with the right answer
    private Question currentQuestion;   // The current question displayed
    private int numberOfQuestions;      // The definitive number of question in the trivia
    private boolean waitingForNext;     // if an option button was chosen true, until next question then reset to false
    protected List<Button> buttons;
    TriviaLogic newGame;

    /**
     * Fetch question file for game
     * set initial values and question
     * @throws FileNotFoundException file couldn't be fetched
     */
    @FXML
    void initialize() throws FileNotFoundException {
        //reset
        waitingForNext=false;

        //create new game based on question file
        newGame = new TriviaLogic("src\\main\\java\\mmn\\mmn_13_part_2\\animalTrivia");

        //initialize text values
        score.setText("Score: "+newGame.getScore());
        quizName.setText(newGame.getTriviaName());
        numberOfQuestions = newGame.getNumberOfQuestions();
        //set up buttons in array list to be used to manage answer buttons
        buttons = new ArrayList<>(Arrays.asList(answer1, answer2, answer3, answer4)) ;
        nextQuestion();
    }

    /**
     * End game button triggers exit
     * open popup for confirmation
     */
    @FXML
    void exit(ActionEvent event){
        //setup popup for exit confirmation
        Alert confirmation = new Alert(Alert.AlertType.CONFIRMATION);
        confirmation.setTitle("End Game?");
        confirmation.setHeaderText("    Are you sure you want to exit?");
        confirmation.getButtonTypes().setAll(ButtonType.YES, ButtonType.CANCEL);
        //wait for user response
        Optional<ButtonType> confirmExit = confirmation.showAndWait();

        if (confirmExit.get() == ButtonType.YES) { //user wants to exit
            //open new popup with final score displayed
            confirmation.setTitle("End Game");
            confirmation.setHeaderText("    Final Score: "+newGame.getScore());
            confirmation.getButtonTypes().setAll(ButtonType.OK);
            confirmation.showAndWait();
            //exit everything
            System.exit(0);
        }

    }

    /**
     * @param button the button to check
     * @return returns true if button is one of the answer buttons, false otherwise
     */
    private boolean isAnswerButton(Button button){
        for (Button b : buttons) {
            if (b.equals(button))
                return true;
        }
        return false;
    }

    /**
     * hover color effect
     * @param event button
     */
    @FXML
    void hoverEnter(MouseEvent event) {
        Button source = (Button) event.getSource();
        if ((waitingForNext && isAnswerButton(source) )|| (!waitingForNext && source.equals(nextBtn)))
            return;
        source.setEffect(new ColorAdjust(0.2,0.04,0,0));
    }

    /**
     * return to normal after hover exit
     * @param event button
     */
    @FXML
    void hoverExit(MouseEvent event) {
        Button source = (Button) event.getSource();
        if (waitingForNext && isAnswerButton(source) )
            return;
        source.setEffect(new ColorAdjust(0,0,0,0));
    }

    /**
     * user pressed an answer button logic
     *
     * @param event
     */
    @FXML
    void tryAnswer(ActionEvent event) {

        //disable pressing answer buttons while waiting for a new question
        if (waitingForNext)
            return;
        waitingForNext=true;

        //get button pressed
        Button pressedButton = (Button) event.getSource();

        for (Button button : buttons) {
            button.setEffect(new ColorAdjust(0,-1,0,0));  //change every button to pink
        }
        correctBtn.setEffect(new ColorAdjust(-0.40,0,0,0));   //change the correct button to a different color

        //answer is correct
        if (pressedButton.equals(correctBtn)) {
            newGame.setScore(newGame.getScore()+ newGame.correctAnswer); //update score
        }
        else { //answer is wrong
            pressedButton.setEffect(new ColorAdjust(0.85,-0.2,0,0));  //darken the pressed wrong answer
            newGame.setScore(newGame.getScore()+ newGame.wrongAnswer);  //update score
        }
        score.setText("Score: "+newGame.getScore()); //display score
    }

    /**
     * load next question
     * if an answer is chosen and there are more questions to be asked
     * @param event next question button
     */
    @FXML
    void next(ActionEvent event) {
        if (!waitingForNext)
            //no answer has been picked yet
            return;
        if (!nextQuestion()){
            questionLabel.setText("Out of questions!");
            return;
        }
        //remove hover effect after press
        nextBtn.setEffect(new ColorAdjust(0,0,0,0));
        //cancel waiting because next button was pressed
        waitingForNext=false;
    }

    /**
     * attach answers to answer buttons randomly
     * and save correct answer in correctBtn
     */
    private void answersToButtons(){
        Collections.shuffle(buttons); //shuffle the buttons
        int index = 0;
        /*
         * Because the empty buttons are shuffled
         * we attach the answers in their original order to each button,
         * and mark the first button as the correct answer.
         */
        for (Button button : buttons) {
            if (index == 0)
                correctBtn = button;
            button.setText(currentQuestion.getOptions().get(index++));
            //reset tryAnswer color effect
            button.setEffect(new ColorAdjust(0,0,0,0));
        }
    }

    /**
     * setups next question
     * @return false if no questions left, otherwise returns true
     */
    private boolean nextQuestion(){
        //update display for number of question answered already
        questionCount.setText(numberOfQuestions-newGame.getNumberOfQuestions()+ " of " + numberOfQuestions);

        //check if exists another question
        if (newGame.getNumberOfQuestions()<=0)
            return false;

        //load new question
        currentQuestion = newGame.extractRandomQuestion();
        questionLabel.setText(currentQuestion.getQuestion());
        answersToButtons();
        return true;
    }

}