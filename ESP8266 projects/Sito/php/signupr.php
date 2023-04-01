<?php
        session_start();
        ob_start();

        //inizio la sessione
        //includo i file necessari a collegarmi al db con relativo script di accesso
        //require 'connection.php';
        require_once 'connection.php';

        $nome= $_POST['nome'];
        $cognome= $_POST['cognome'];
        $mail= $_POST['mail'];
        $psw= SHA1($_POST['psw']);

        
        $sql2 = "SELECT * FROM utenti WHERE email = :mail";
		$stmt2 = $db->prepare($sql2);
		$params2 = array(':mail' => $mail,);

        //evita sql injection
		$stmt2->execute($params2);
        $result2 = $stmt2->fetchAll(PDO::FETCH_ASSOC);
	 	$totale2 = $stmt2->rowCount();

         if ($totale2 > 0 ){//email già registrata
             header("location:signup.php?message=u");
         }
        else{

            $sql = "INSERT INTO utenti(nome, cognome, email,password) VALUES(:nome,:cognome,:mail,:psw)";
      		$stmt = $db->prepare($sql);
      		$params = array(
               ':nome' => $nome,
               ':cognome' => $cognome,
               ':mail' => $mail,
               ':psw' => $psw,
        		);
       	    var_dump($params);		//evita sql injection
            if ($stmt->execute($params)) {
                $db=null;
                  $message = "
                            <html>
                                <head>
                                    <title>Benvenuto</title>
                                </head>
                                <body>
                                    <h1>Benvenuto sul sito $nome </h1>
                                    <p>La registrazione è stata effettuata con successo.</p><br>
                                </body>
                            </html>
                        ";
                $headers[] = 'MIME-Version: 1.0';
                $headers[] = 'Content-type: text/html; charset=utf-8';
                mail($mail, 'Benvenuto sul sito', $message, implode("\r\n", $headers));
                header("location:login.php?message=s");
            }else{

                $db=null; //chiudo connessione con database
                echo "Errore nella connessione, riprova più tardi";

           }
        }
    ?>
    
