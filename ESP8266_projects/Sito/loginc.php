<?php

        session_start();
        ob_start();
        //includo i file necessari a collegarmi al db con relativo script di accesso
       require_once 'connection.php';

        $mail = $_POST['mail'];
        $psw = SHA1($_POST['psw']);

        $sql = "SELECT * FROM utenti WHERE email = :mail AND password = :psw";
	$stmt = $db->prepare($sql);
	$params = array(':mail' => $mail,':psw' => $psw,);
 	var_dump($params);		//evita sql injection
	$stmt->execute($params);
        $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
	$totale = $stmt->rowCount();

         if ($totale > 0 ){//utente rientrato
                $_SESSION['email'] = $mail;
                $_SESSION['logged_in'] = true;
                setcookie("logged_in", "true", time() + 604800, "/", "dalessandroluca.altervista.org", 1); //will last a week
                $db=null; //chiudo connessione con database
                header("location:accesso.php");
          }else {
                  //Username e password errati, redirect alla pagina di login con errore
                   $db=null;
                   header("location:login.php?message=e");
         }
?>
