<?php
        if((isset($_POST['control'])) && (isset($_POST['ing_1']) && (isset($_POST['ing_2']) && (isset($_POST['ing_3']) && (isset($_POST['ing_4']))) {

        $control = $_POST['control'];
        $ing_1 = $_POST['ing_1'];
        $ing_2 = $_POST['ing_2'];
        $ing_3 = $_POST['ing_3'];
        $ing_4 = $_POST['ing_4'];
        }
    echo system("echo \"$control\" > /dev/ttyAMA0 ");
    echo system("echo \"$ing_1\" > /dev/ttyAMA0 ");
    echo system("echo \"$ing_2\" > /dev/ttyAMA0 ");
    echo system("echo \"$ing_3\" > /dev/ttyAMA0 ");
    echo system("echo \"$ing_4\" > /dev/ttyAMA0 ");
?>