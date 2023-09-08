<?php

       $device_name = $_GET['device_name'];
       require_once 'connection.php';

        $sql = "SELECT 
                TIME_FORMAT(ora_a1, '%H') as ora_a1,
                TIME_FORMAT(ora_a1, '%i') as min_a1,
                TIME_FORMAT(ora_s1, '%H') as ora_s1,
                TIME_FORMAT(ora_s1, '%i') as min_s1,
                TIME_FORMAT(ora_a2, '%H') as ora_a2,
                TIME_FORMAT(ora_a2, '%i') as min_a2,
                TIME_FORMAT(ora_s2, '%H') as ora_s2,
                TIME_FORMAT(ora_s2, '%i') as min_s2,
                temperaturaImp
                FROM gestione_term WHERE cod_prodotto = '$device_name'";
		    $stmt = $db->prepare($sql);
		    $stmt->execute();
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
	 	    $totale = $stmt->rowCount();
         if ($totale > 0) {
                echo $result['ora_a1'].";";
                echo $result['min_a1'].";";
                echo $result['ora_s1'].";";
                echo $result['min_s1'].";";
                echo $result['ora_a2'].";";
                echo $result['min_a2'].";";
                echo $result['ora_s2'].";";
                echo $result['min_s2'].";";
                echo $result['temperaturaImp'];
                $db=null; //chiudo connessione con database

          }else {
                  //Username e password errati, redirect alla pagina di login con errore
                   echo "Error:" . $db->error;
                   error_reporting(E_ALL);
         }
?>
