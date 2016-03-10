<?php
	session_start();
	require_once 'dbconfig.php';

	if(isset($_POST['table_name'])) {

		$tableName = $_POST['table_name'];

		// sql to create table for user's recipes
		$sql = "CREATE TABLE $tableName (
		id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
		recipe_name VARCHAR(16) NOT NULL,
		ingredient_1 VARCHAR(16) NOT NULL,
		amount_1 DECIMAL(2,1) NOT NULL,
		ingredient_2 VARCHAR(16) NOT NULL,
		amount_2 DECIMAL(2,1) NOT NULL,
		ingredient_3 VARCHAR(16) NOT NULL,
		amount_3 DECIMAL(2,1) NOT NULL,
		ingredient_4 VARCHAR(16) NOT NULL,
		amount_4 DECIMAL(2,1) NOT NULL,
		glass_type VARCHAR(16) NOT NULL
		)";

		try {
			$db_con->exec($sql);
			echo "Created user table successfully";
		}
		catch(PDOException $e)
		{
			echo $sql . "<br>" . $e->getMessage();
		}
	}
?>