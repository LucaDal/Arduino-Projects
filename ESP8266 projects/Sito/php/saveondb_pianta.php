<?php

        session_start();
        ob_start();
        //includo i file necessari a collegarmi al db con relativo script di accesso
        require_once 'connection.php';

        $sec_to_water = $_POST['sec_to_water'];
        $umid_to_water = $_POST['umid_to_water'];
        $ml_to_give = $_POST['ml_to_give'];
        $ora_accensione_luci = $_POST['ora_accensione_luci'];
        $email = $_SESSION['email'];

        $sql = "UPDATE gestione_plant
                JOIN prodotti P on P.cod_prodotto = gestione_plant.cod_prodotto
                SET sec_to_water = $sec_to_water, umid_to_water = $umid_to_water, ml_to_give = $ml_to_give, ora_accensione_luci = '$ora_accensione_luci'
                WHERE P.email = '$email';";
	$stmt = $db->prepare($sql);
	if($stmt->execute()){
        header("location:datiPianta.php?message=success");     
    }else{
        header("location:datiPianta.php?message=error");    
    }
?>
