<?php
	session_start();
	require_once 'dbconfig.php';

	if((isset($_POST['table_name'])) && (isset($_POST['recipe_name'])) && (isset($_POST['ing_1'])) && (isset
	($_POST['amt_1'])) && (isset($_POST['ing_2'])) && (isset($_POST['amt_2'])) && (isset($_POST['ing_3'])) && (isset($_POST['amt_3'])) && (isset($_POST['ing_4'])) && (isset($_POST['amt_4'])) &&
	(isset($_POST['glass_type']))) {

		$tableName = $_POST['table_name'];
		$recName = $_POST['recipe_name'];
		$ing1 = $_POST['ing_1'];
		$amt1 = $_POST['amt_1'];
		$ing2 = $_POST['ing_2'];
		$amt2 = $_POST['amt_2'];
		$ing3 = $_POST['ing_3'];
		$amt3 = $_POST['amt_3'];
		$ing4 = $_POST['ing_4'];
		$amt4 = $_POST['amt_4'];
		$glassType = $_POST['glass_type'];

		// sql to create table for user's recipes
		$sql = ("INSERT INTO $tableName(recipe_name,ingredient_1,amount_1,ingredient_2,amount_2,ingredient_3,amount_3,ingredient_4,amount_4,glass_type) VALUES('$recName','$ing1','$amt1','$ing2','$amt2','$ing3','$amt3','$ing4','$amt4','$glassType')");

		try {
			$db_con->exec($sql);
			echo "Recipe saved successfully";
		}
		catch(PDOException $e)
		{
			echo $sql . "<br>" . $e->getMessage();
		}
	}
?>