<?php
	session_start();
	require_once 'dbconfig.php';

	if((isset($_POST['table_name'])) && (isset($_POST['id']))) {

		$id = $_POST['id'];
		$tableName = $_POST['table_name'];

		$sql = "DELETE FROM $tableName WHERE id=$id";

		try {
			$db_con->exec($sql);
			echo "Recipe deleted successfully";
		}
		catch(PDOException $e)
		{
			echo $sql . "<br>" . $e->getMessage();
		}
	}
?>