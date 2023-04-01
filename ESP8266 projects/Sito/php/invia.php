<?php
        session_start();
        ob_start();

        //inizio la sessione
        //includo i file necessari a collegarmi al db con relativo script di accesso
        //require 'connection.php';

        $nome= $_GET['nome'];
        $cognome= $_GET['cognome'];
        $mail= $_GET['mail'];
        $testo= $_GET['testo'];
        $miam = "luca-dalessandro@virgilio.it";

          $message = "
                    <html>
                        <head>
                            <title>Modulo di richiesta</title>
                        </head>
                        <body>
                            $nome $cognome, chiede: <br>
                            <p>$testo</p><br><br>
                            <p>per rispondere scrivere a:<br> $mail</p>
                        </body>
                    </html>
                ";
        $headers[] = 'MIME-Version: 1.0';
        $headers[] = 'Content-type: text/html; charset=utf-8';
              mail($miam, 'Richiesta', $message, implode("\r\n", $headers));
              header("location:contact.php?message=s");
    ?>
