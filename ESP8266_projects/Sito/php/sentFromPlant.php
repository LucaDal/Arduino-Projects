<?php
    $device_name = $_GET['device_name'];
    $ora = $_GET['ora'];
    $min = $_GET['min'];
    $temp_aria = $_GET['temp_aria'];
    $temp_terreno = $_GET['temp_terreno'];
    $umid_aria = $_GET['umid_aria'];
    $umid_terreno = $_GET['umid_terreno'];
    require_once 'connection.php';
    $sql = "INSERT INTO plant_state (data_agg,ora_salvataggio,temp_aria,temp_terreno,umid_aria,umid_terreno,cod_prodotto) 
            VALUES (CURDATE(),'$ora:$min','$temp_aria','$temp_terreno','$umid_aria','$umid_terreno','$device_name');";
	$stmt = $db->prepare($sql);
	$stmt->execute();
    $db=null;
?>


http://dalessandroluca.altervista.org/Projects/sentFromPlant.php?device_name=oJd4K&ora=23&min=30&temp_aria=18&temp_terreno=20.3&umid_aria=70&umid_terreno=90