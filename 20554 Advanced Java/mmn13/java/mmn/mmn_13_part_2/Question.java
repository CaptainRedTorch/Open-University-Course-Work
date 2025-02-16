package mmn.mmn_13_part_2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Question
{
    private String question;    //The question
    private String[] options;   //The possible answers

    /**
     * constructor for a trivia question
     * @param question the question itself
     * @param options the possible answers
     */
    public Question(String question, String[] options) {
        this.question = question;
        this.options = Arrays.copyOf(options, options.length);
    }

    /**
     * @return the question
     */
    public String getQuestion() {
        return question;
    }

    /**
     * @return a copy of the options as a List
     */
    public List<String> getOptions() {
        return new ArrayList<>(Arrays.asList(options));
    }

}
