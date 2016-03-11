<?php
        if((isset($_POST['control']))) {

        $control = $_POST['control'];
        $temp = $_POST['temp'];
        }
    echo system("echo \"$control\" > /dev/ttyAMA0 ");
    echo system("echo \"$temp\" > /dev/ttyAMA0 ");
?>