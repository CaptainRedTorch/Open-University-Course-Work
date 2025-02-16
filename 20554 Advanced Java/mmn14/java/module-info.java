module mmn.mmn_14_part_2 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;
    requires annotations;


    opens mmn.mmn_14_part_2 to javafx.fxml;
    exports mmn.mmn_14_part_2;
}