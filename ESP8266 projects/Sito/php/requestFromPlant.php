<?php

       $device_name = $_GET['device_name'];
       require_once 'connection.php';

        $sql = "SELECT 
                sec_to_water,
                umid_to_water,
                ml_to_give,
                TIME_FORMAT(ora_accensione_luci, '%H') as ora_a,
                TIME_FORMAT(ora_accensione_luci, '%i') as min_a
                FROM gestione_plant WHERE cod_prodotto = '$device_name'";
	$stmt = $db->prepare($sql);
	$stmt->execute();
        $result = $stmt->fetch(PDO::FETCH_ASSOC);
	 	    $totale = $stmt->rowCount();
         if ($totale > 0) {
                echo $result['umid_to_water'].";";
                echo $result['ml_to_give'].";";
                echo $result['sec_to_water'].";";
                echo $result['ora_a'].";";
                echo $result['min_a'].";";
                $db=null; //chiudo connessione con database

          }else {
                  //Username e password errati, redirect alla pagina di login con errore
                echo "Error:" . $db->error;
                error_reporting(E_ALL);
         }
?>
