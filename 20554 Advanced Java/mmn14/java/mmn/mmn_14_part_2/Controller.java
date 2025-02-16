package mmn.mmn_14_part_2;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.*;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

import java.util.*;

public class Controller {

    @FXML
    private HBox expandedPane;

    @FXML
    private GridPane calenderGrid;

    @FXML
    private DatePicker datePicker;  //used to pick a specific date

    @FXML
    private TextArea glance;    //used to display events at a glance(hover)

    @FXML
    private MenuButton monthPicker; //used to display and pick month

    @FXML
    private Label yearLabel;

    //manual hover and inline css is used to create common css effects that otherwise would be written in .css file in this class

    protected HashMap<Date, String> events = new HashMap<>();       //Map to save event for each date
    protected Calendar calendar = Calendar.getInstance();           //Current date chosen, initializes with current date

    private ToggleButton currentDateButton;                         //button which corresponds to the current day of month selected
    private Button currentMonthButton;                              //button which corresponds to the current month selected

    private Dialog<ButtonType> eventPopup = new Dialog<>();         //form popup for event input
    private TextArea eventArea = new TextArea();                    //text area in popup for event input

    private ToggleButton[][] gridArray = new ToggleButton[6][7];    //grid corresponding to the calendar displayed using buttons

    //List of button corresponding to the months
    private ArrayList<Button> monthsButtons = new ArrayList<>(Arrays.asList(new Button[]
    {
        new Button("Jan"),
        new Button("Feb"),
        new Button("Mar"),
        new Button("Apr"),
        new Button("May"),
        new Button("Jun"),
        new Button("Jul"),
        new Button("Aug"),
        new Button("Sep"),
        new Button("Oct"),
        new Button("Nov"),
        new Button("Dec")
    }))
    ;

    /**
     * set up event popup
     */
    private void setEventPopup(){
        eventPopup.setTitle("Event Manager");
        eventArea.setPrefSize(500,300);
        eventArea.setStyle("-fx-faint-focus-color: transparent; -fx-font-size: 16px");
        eventArea.setPromptText("Enter event information");
        VBox vbox = new VBox(eventArea);
        eventPopup.getDialogPane().setContent(vbox);
        eventPopup.getDialogPane().getButtonTypes().addAll(ButtonType.APPLY, ButtonType.CANCEL);
    }



    /**
     * runs first
     * setup initial values and display current date on calendar
     */
    @FXML
    void initialize() {

        Locale.setDefault(Locale.US);

        setEventPopup();

        //Set up the month buttons style and onAction
        for (Button button : monthsButtons) {
            button.setStyle(" -fx-background-radius: 50px; -fx-background-color:#DFE0E1FF; -fx-font-size: 15.5px;");
            button.setOnAction(event -> {
                onMonthPicked(event);
            });
        }

        initializeGridArray();

        //set the current day in month to not be null before loadDate() call
        currentDateButton = gridArray[calendar.get(Calendar.WEEK_OF_MONTH) - 1][calendar.get(Calendar.DAY_OF_WEEK)-1];
        //display and set the current month as selected
        currentMonthButton = monthsButtons.get(calendar.get(Calendar.MONTH));
        setCurrentMonthButton(currentMonthButton);

        //Load the current month and specific date onto the grid and corresponding display values
        loadDate();

    }

    /**
     * set the calendar displayed and date selected
     */
    private void loadDate() {
        //set year and month titles
        yearLabel.setText(String.valueOf(calendar.get(Calendar.YEAR)));
        monthPicker.setText(calendar.getDisplayName(Calendar.MONTH,Calendar.LONG,Locale.US));

        //unselect old day of month displayed and update to current day of month
        unselectButton(currentDateButton);
        currentDateButton = gridArray[calendar.get(Calendar.WEEK_OF_MONTH) - 1][calendar.get(Calendar.DAY_OF_WEEK)-1];
        selectButton(currentDateButton);

        //temporary calendar set to current date
        Calendar thisMonth = Calendar.getInstance();
        thisMonth.set(calendar.get(Calendar.YEAR), calendar.get(Calendar.MONTH), calendar.get(Calendar.DAY_OF_MONTH));
        //get to the first of the month
        thisMonth.add(Calendar.DAY_OF_MONTH, -thisMonth.get(Calendar.DAY_OF_MONTH)+1);
        //get the day of the week the first of the month is on and subtract from date to get starting date for grid
        thisMonth.add(Calendar.DAY_OF_MONTH, -thisMonth.get(Calendar.DAY_OF_WEEK)+1);

        //set up grid buttons dates
        for (int i = 0; i < gridArray.length; i++) {
            for (int j = 0; j < gridArray[i].length; j++) {
                //disable every button to reset
                gridArray[i][j].setDisable(false);
                //make active every button that is of current displayed month
                if (thisMonth.get(Calendar.MONTH) != calendar.get(Calendar.MONTH)) {
                    gridArray[i][j].setDisable(true);
                }
                //set the dates for each button
                gridArray[i][j].setText(thisMonth.get(Calendar.DAY_OF_MONTH) + "");
                thisMonth.add(Calendar.DAY_OF_MONTH, 1); //iterate on calendar date
            }
        }
    }

    /**
     * Display day of month button as selected
     * @param toggleButton the button to set as selected
     */
    private void selectButton(ToggleButton toggleButton) {
        toggleButton.setSelected(true);
        toggleButton.setStyle("-fx-background-color: #2aa7e5; -fx-text-fill: white; -fx-background-radius: 150px");

    }

    /**
     * Display day of month button as unselected
     * @param toggleButton the button to set as unselected
     */
    private void unselectButton(ToggleButton toggleButton) {
        toggleButton.setSelected(false);
        toggleButton.setStyle("-fx-background-color: transparent; -fx-text-fill: black; -fx-background-radius: 150px");
    }

    /**
     * update and display the selected month button
     * @param button the new selected month button
     */
    private void setCurrentMonthButton(Button button) {
        //reset old
        currentMonthButton.setStyle(" -fx-background-radius: 50px; -fx-background-color:#DFE0E1FF; -fx-font-size: 15.5px;");
        //get new and set to pressed
        currentMonthButton = button;
        currentMonthButton.setStyle(" -fx-background-radius: 50px; -fx-background-color:#398ee8;-fx-text-fill: white; -fx-font-size: 15.5px;");

    }

    /**
     * set grid array instead of gridPane for day of month buttons
     * to make it possible to work with button indexes in grid
     */
    private void initializeGridArray() {
        Integer colIndex = 0;
        Integer rowIndex = 0;
        for (Node node : calenderGrid.getChildren()) {
            if (node instanceof ToggleButton) { //ignore week day names
                colIndex = GridPane.getColumnIndex(node);
                rowIndex = GridPane.getRowIndex(node);
                if (colIndex == null) {
                    gridArray[rowIndex - 1][0] = (ToggleButton) node;
                } else
                    gridArray[rowIndex - 1][colIndex] = (ToggleButton) node;
            }
        }
    }

    /**
     * on button clicked in calendar
     * open event popup and display as selected
     * @param event new calendar button clicked
     */
    @FXML
    void onDateClicked(ActionEvent event) {
        ToggleButton picked = (ToggleButton) event.getSource();

        //unselect old selected button and update to new
        unselectButton(currentDateButton);
        currentDateButton = picked;
        selectButton(picked);

        //no need to display datepicker value if datepicker didnt originate date
        datePicker.setValue(null);

        //update the calendar class to reflect the change
        calendar.set(Calendar.DAY_OF_MONTH, Integer.parseInt(currentDateButton.getText()));

        displayPopup();
    }

    /**
     * Displays the event popup form
     * and saves event onto event map
     */
    private void displayPopup(){

        //set event header to current date
        eventPopup.setHeaderText(calendar.getDisplayName(Calendar.DAY_OF_WEEK,Calendar.LONG,Locale.US) +", "
                +calendar.getDisplayName(Calendar.MONTH,Calendar.LONG,Locale.US)+ " "
                +calendar.get(Calendar.DAY_OF_MONTH)+", "
                +calendar.get(Calendar.YEAR));

        //load existing event to textArea
        eventArea.setText(events.get(calendar.getTime()));
        //display popup and wait for user response
        Optional<ButtonType> results = eventPopup.showAndWait();

        //user saved changes, update event map and at a glance text
        if (results.isPresent() && results.get() == ButtonType.APPLY) {
            events.put(calendar.getTime(), eventArea.getText());
            glance.setText(events.get(calendar.getTime()));
        }
        eventArea.clear(); //clear text for next event popup
    }

    /**
     * load new month onto calendar
     * @param event month button pressed
     */
    @FXML
    void onMonthPicked(ActionEvent event) {
        //set the new month
        calendar.set(Calendar.MONTH , monthsButtons.indexOf((Button)event.getSource()));
        //make the date the first day of the month
        calendar.set(Calendar.DAY_OF_MONTH,calendar.getMinimum(Calendar.DAY_OF_MONTH));

        //update month title
        monthPicker.setText(calendar.getDisplayName(Calendar.MONTH,Calendar.LONG,Locale.US));
        loadDate();

        //don't select any button if month changed
        unselectButton(currentDateButton);
        setCurrentMonthButton((Button) event.getSource());
    }

    /**
     * calendar button hover
     * display at a glance for that date
     * @param event toggle button
     */
    @FXML
    void dateHoverEntered(MouseEvent event) {
        ToggleButton thisEvent = ((ToggleButton)event.getSource());
        //temporary calendar to save current date
        Calendar thisDate = Calendar.getInstance();
        thisDate.set(calendar.get(Calendar.YEAR), calendar.get(Calendar.MONTH), calendar.get(Calendar.DAY_OF_MONTH));
        //set calendar to hovered date
        calendar.set(Calendar.DAY_OF_MONTH, Integer.parseInt(thisEvent.getText()));

        //event in event map corresponding to the hovered date exists, then display it
        if (events.get(calendar.getTime()) != null) {
            glance.setText(events.get(calendar.getTime()));
        }
        //load back current date to main calendar
        calendar.set(Calendar.DAY_OF_MONTH, thisDate.get(Calendar.DAY_OF_MONTH));

        //hover color effect
        if (!thisEvent.equals(currentDateButton)) {
            thisEvent.setStyle("-fx-background-color: #ececec; -fx-background-radius: 150px");
        }

    }

    /**
     * calendar button hover exit
     * cancel out hover enter
     * @param event
     */
    @FXML
    void dateHoverExist(MouseEvent event) {
        ToggleButton thisEvent = ((ToggleButton)event.getSource());
        //cancel at a glance
        glance.setText("");
        //cancel hover color effect
        if (!thisEvent.equals(currentDateButton)) {
            thisEvent.setStyle(" -fx-background-color: transparent; -fx-background-radius: 150px");
        }
    }

    /**
     * on date picked using datePicker
     * load new date picked
     * @param event datePicker changed
     */
    @FXML
    void onDatePicked(ActionEvent event) {
        //don't activate when date changes to null
        if (datePicker.getValue() != null) {
            //set the calendar to the date picked
            calendar.set(datePicker.getValue().getYear(), datePicker.getValue().getMonthValue() - 1, datePicker.getValue().getDayOfMonth());
            loadDate();
            setCurrentMonthButton(monthsButtons.get(datePicker.getValue().getMonthValue()-1));
            displayPopup();
        }
    }

    /**
     * Display month buttons dynamically when hovering above month+year title area
     */
    @FXML
    void monthHoverEntered(MouseEvent event) {
        expandedPane.getChildren().addAll(monthsButtons);
    }

    /**
     * Hide month buttons when nit hovering above month+year title area
     */
    @FXML
    void monthHoverExist(MouseEvent event) {
        expandedPane.getChildren().clear();
    }

    /**
     * Increase/Decrease year
     * @param event button
     */
    @FXML
    void increaseYear(ActionEvent event) {
        int increase = 0;
        increase = ((Button)event.getSource()).getId().equals("increaseYear") ? 1 : -1;

        //update to new year
        calendar.set(Calendar.YEAR, calendar.get(Calendar.YEAR) + increase);
        //set day of month to the first day of the month
        calendar.set(Calendar.DAY_OF_MONTH, calendar.getMinimum(Calendar.DAY_OF_MONTH));
        loadDate();
        //dont display date button when year changes
        unselectButton(currentDateButton);

    }
}
