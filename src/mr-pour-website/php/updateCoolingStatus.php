<?php
	if((isset($_POST['status']))) {

		$status = $_POST['status'];
	}
    echo system("echo \"$status\" > /dev/ttyAMA0 ");
?>