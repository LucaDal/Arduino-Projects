<?php
    $device_name = $_GET['device_name'];
    $ora = $_GET['ora'];
    $min = $_GET['min'];
    $temp = $_GET['temp'];
    require_once 'connection.php';
    $sql = "INSERT INTO term_state (data_agg,ora_salvataggio,temp,cod_prodotto) VALUES (CURDATE(),'$ora:$min','$temp','$device_name');";
	$stmt = $db->prepare($sql);
	$stmt->execute();
    $db=null;
    #test:
    #http://dalessandroluca.altervista.org/Projects/sentFromTermostato.php?device_name=8pklP&ora=23&min=30&temp=20.3
?>