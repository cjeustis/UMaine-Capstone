<?php
        if((isset($_POST['control']))) {

        $control = $_POST['control'];
        $ing1 = $_POST['amount_1'];
        $ing2 = $_POST['amount_2'];
        $ing3 = $_POST['amount_3'];
        $ing4 = $_POST['amount_4'];
        }
    usleep(250000);
    echo system("echo \"$control\" > /dev/ttyAMA0 ");
    usleep(250000);
    echo system("echo \"$ing1\" > /dev/ttyAMA0 ");
    usleep(250000);
    echo system("echo \"$ing2\" > /dev/ttyAMA0 ");
    usleep(250000);
    echo system("echo \"$ing3\" > /dev/ttyAMA0 ");
    usleep(250000);
    echo system("echo \"$ing4\" > /dev/ttyAMA0 ");
?>