<?php
    try {
    $hostname = "localhost";
    $dbname = "my_dalessandroluca";
    $user = "dalessandroluca";
    $pass = "";
    $db = new PDO ("mysql:host=$hostname;dbname=$dbname", $user, $pass);
    }  catch (PDOException $e) {
       echo "Errore: " . $e->getMessage();
       die();
    }
?>
