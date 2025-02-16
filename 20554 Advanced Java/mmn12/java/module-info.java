module mmn.mmn12 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens mmn.mmn12 to javafx.fxml;
    exports mmn.mmn12;
}