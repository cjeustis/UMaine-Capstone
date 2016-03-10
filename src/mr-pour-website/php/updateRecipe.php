<?php
	session_start();
	require_once 'dbconfig.php';

//	if((isset($_POST['table_name'])) && (isset($_POST['recipe_name'])) && (isset($_POST['ing_1'])) && (isset($_POST['amt_1'])) && (isset($_POST['ing_2'])) && (isset($_POST['amt_2'])) && (isset($_POST['ing_3'])) && (isset($_POST['amt_3'])) && (isset($_POST['ing_4'])) && (isset($_POST['amt_4'])) && (isset($_POST['glass_type']))) {

		$id = $_POST['id'];
		$tableName = $_POST['table_name'];
		$recName = $_POST['recipe_name'];
		$ing1 = $_POST['ingredient_1'];
		$amt1 = $_POST['amount_1'];
		$ing2 = $_POST['ingredient_2'];
		$amt2 = $_POST['amount_2'];
		$ing3 = $_POST['ingredient_3'];
		$amt3 = $_POST['amount_3'];
		$ing4 = $_POST['ingredient_4'];
		$amt4 = $_POST['amount_4'];
		$glassType = $_POST['glass_type'];

		$sql = "UPDATE $tableName SET recipe_name='$recName', ingredient_1='$ing1', amount_1='$amt1', ingredient_2='$ing2', amount_2='$amt2', ingredient_3='$ing3', amount_3='$amt3', ingredient_4='$ing4', amount_4='$amt4', glass_type='$glassType' WHERE id='$id'";

		try {
			$db_con->exec($sql);
			echo "Recipe updated successfully";
		}
		catch(PDOException $e)
		{
			echo $sql . "<br>" . $e->getMessage();
		}
//	}
?>
