<?php

	require_once 'dbconfig.php';

	if($_POST) {
		$user_name = $_POST['user_name'];
		$user_email = $_POST['user_email'];
		$user_password = $_POST['password'];
		$joining_date =date('Y-m-d H:i:s');

		try {
			$stmt = $db_con->prepare("SELECT * FROM tbl_users WHERE user_email=:email");
			$stmt->execute(array(":email"=>$user_email));
			$count = $stmt->rowCount();


			if($count==0) {

				$stmt = $db_con->prepare("SELECT * FROM tbl_users WHERE user_name=:userName");
				$stmt->execute(array(":userName"=>$user_name));
				$count = $stmt->rowCount();

				if ($count == 0) {
					$stmt = $db_con->prepare("INSERT INTO tbl_users(user_name,user_email,user_password,joining_date) VALUES(:uname, :email, :pass, :jdate)");
					$stmt->bindParam(":uname",$user_name);
					$stmt->bindParam(":email",$user_email);
					$stmt->bindParam(":pass",$user_password);
					$stmt->bindParam(":jdate",$joining_date);

					if($stmt->execute()) {
						echo "registered";
					}
					else {
						echo "Query could not execute !";
					}
				}
				else {
					echo "2"; //  not available
				}
			}
			else {
				echo "1";
			}
		}
		catch(PDOException $e) {
	   		echo $e->getMessage();
		}
	}

?>