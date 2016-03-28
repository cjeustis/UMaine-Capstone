<?php
session_start();

if(!isset($_SESSION['user_session']))
{
 header("Location: ../dashboard/dash.html");
}

include_once 'dbconfig.php';

$tableName = $_POST['table_name'];

$stmt = $db_con->prepare("SELECT * FROM $tableName");
$stmt->execute(array(":uid"=>$_SESSION['user_session']));

$results = $stmt->fetchAll(PDO::FETCH_ASSOC);

if (!$results) {
echo 'no recipes found';
	die('No recipes found');
}

// return as json
header('Content-Type: application/json');
echo json_encode($results);

?>
