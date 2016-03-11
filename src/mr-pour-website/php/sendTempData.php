<?php
        if((isset($_POST['control']))) {

        $control = $_POST['control'];
        $temp = $_POST['temp'];
        }
    usleep(250000);
    echo system("echo \"$control\" > /dev/ttyAMA0 ");
    usleep(250000);
    echo system("echo \"$temp\" > /dev/ttyAMA0 ");
?>