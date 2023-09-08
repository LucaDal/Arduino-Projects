<?php

        session_start();
        ob_start();
        //includo i file necessari a collegarmi al db con relativo script di accesso
        require_once 'connection.php';

        $oraa1 = $_POST['oraa1'];
        $oras1 = $_POST['oras1'];
        $oraa2 = $_POST['oraa2'];
        $oras2 = $_POST['oras2'];
        $temp  = $_POST['temp'];
        $email = $_SESSION['email'];

        $sql = "UPDATE gestione_term
                JOIN prodotti P on P.cod_prodotto = gestione_term.cod_prodotto
                SET ora_a1 = '$oraa1', ora_s1 = '$oras1', ora_a2 = '$oraa2', ora_s2 = '$oras2', temp = $temp
                WHERE P.email = '$email';";
	$stmt = $db->prepare($sql);
	if($stmt->execute()){
                header("location:datiTermostato.php?message=success");     
        }else{
                header("location:datiTermostato.php?message=error");    
        }


?>
