<?php
	if((isset($_POST['motor']))) {

		motor = $_POST['motor'];
	}
    echo system("echo \"motor\" > /dev/ttyAMA0 ");
?>