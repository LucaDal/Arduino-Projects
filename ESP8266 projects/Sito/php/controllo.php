<?php
   session_start();
   ob_start();
   require_once 'connection.php';
   $cod = $_POST['cod'];
   $sql = "SELECT * FROM prodotti WHERE cod_prodotto = :cod AND email IS NULL";
  	$stmt = $db->prepare($sql);
  	$params = array(
   ':cod' => $cod,
   );
   var_dump($params);	//verifico che il codice sia davvero esistente e poi lo associo all'utente
  	$stmt->execute($params);
   $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
  	$totale = $stmt->rowCount();

   if ($totale > 0 ){
      $email = $_SESSION['email'];
      if($result[0]['tipologia'] == 'termostato'){
         $sql1 = " INSERT INTO gestione_term(cod_prodotto) VALUES ('$cod'); "; 
      }else if($result[0]['tipologia'] == 'plant'){
         $sql1 = " INSERT INTO gestione_plant(cod_prodotto) VALUES ('$cod'); "; 
      }
      $stmt = $db->prepare($sql1);
      $stmt->execute();          
      $sql2 = " UPDATE prodotti SET email = '$email' WHERE cod_prodotto = '$cod'; ";
      $stmt2 = $db->prepare($sql2);
      $stmt2->execute();
      header("location:accesso.php?message=r");
   }else {
      //Username e password errati, redirect alla pagina di login con errore
       header("location:accesso.php?message=e");
   }

?>
