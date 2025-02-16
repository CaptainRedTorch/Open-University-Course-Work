module mmn.mmn_13_part_2 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens mmn.mmn_13_part_2 to javafx.fxml;
    exports mmn.mmn_13_part_2;
}