<?php
	if((isset($_POST['status'])) && (isset($_POST['control']))) {

        $control = $_POST['control'];
		$status = $_POST['status'];
	}
    echo system("echo \"$control\" > /dev/ttyAMA0 ");
    echo system("echo \"$status\" > /dev/ttyAMA0 ");
?>