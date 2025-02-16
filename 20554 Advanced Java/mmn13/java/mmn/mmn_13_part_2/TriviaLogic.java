package mmn.mmn_13_part_2;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class TriviaLogic {

    Random rand = new Random();

    final int numberOfAnswers =4;   //number of possible answers for each question
    final int correctAnswer=10;     //score to add for correct answer
    final int wrongAnswer=-5;       //score to add for wrong answer

    private String triviaName;                                          //The name of the trivia
    private List<Question> questionsList = new ArrayList<Question>();   //The collection of questions
    private int score = 0;                                              //Current score

    /**
     * Constructor for trivia game,
     * the file format is as follows:
     * trivia name,
     * question,
     * correct answer,
     * options...
     * @param path The path to the trivia questions file
     * @throws FileNotFoundException file location is wrong or file doesn't exist
     */
    public TriviaLogic(String path) throws FileNotFoundException {
        Scanner sc = new Scanner(new File(path));
        String question="";
        String[] options = new String[numberOfAnswers];

        //Read trivia name
        //And check if file is empty
        if (sc.hasNextLine())
            triviaName = sc.nextLine();
        else
            throw new RuntimeException("File is empty");

        //read file and parse to object Question
        while (sc.hasNextLine()) {
            for (int i = -1; i < numberOfAnswers ; i++){
                if (i==-1)
                    question = sc.nextLine();
                else
                    options[i] = sc.nextLine();
            }
            Question newQ = new Question(question,options);
            questionsList.add(newQ);
        }
        sc.close();
    }

    /**
     * remove and return a random question from the list
     * @return a random question
     */
    public Question extractRandomQuestion(){
        Question question = questionsList.get(rand.nextInt(questionsList.size()));
        questionsList.remove(question);
        return question;
    }

    /**
     * set the score
     * @param score new score
     */
    public void setScore(int score){
        this.score = score;
    }

    /**
     * @return the current score
     */
    public int getScore(){
        return score;
    }

    /**
     * @return the trivia's name
     */
    public String getTriviaName(){
        return triviaName;
    }

    /**
     * @return the number of questions in the list
     */
    public int getNumberOfQuestions(){
        return questionsList.size();
    }


}
