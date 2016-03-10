<?php
	if((isset($_POST['length']))) {

		length = $_POST['length'];
	}
    echo system("echo \"length\" > /dev/ttyAMA0 ");
?>