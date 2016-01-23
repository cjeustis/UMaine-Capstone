<?php
session_start();

if(!isset($_SESSION['user_session']))
{
 header("Location: ../html/main.html");
}

require_once 'dbconfig.php';

$stmt = $db_con->prepare("SELECT * FROM tbl_users WHERE user_id=:uid");
$stmt->execute(array(":uid"=>$_SESSION['user_session']));
$row=$stmt->fetch(PDO::FETCH_ASSOC);

header('Content-Type: application/json');

//echo json_encode($var)
echo json_encode($row);

?>
