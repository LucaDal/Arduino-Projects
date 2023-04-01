<?php session_start();?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Aggiornamenti</title>
    <link href="https://fonts.googleapis.com/css?family=Ubuntu&display=swap" rel="stylesheet">
    <link rel="stylesheet" type="text/css" href="Style.css">


  </head>
  <body background-color="white">


   <header>

      <ul>
          <li><a href="index.php">Home</a></li>
          <li><a href="update.php">Aggiornamenti</a></li>
          <li><a href="contact.php">Contattaci</a></li>
          <?php if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
                    echo "<li style='float:right'> <a class='active' href='logout.php'>Logout</a></li>";
                    echo "<li style='float:right'> <a class='active' href='accesso.php'>Area personale</a></li>";
                }else {
                    echo  "<li style='float:right'><a class='active' href='login.php'>Login</a></li>";
            } ?>
      </ul>
    </header>

    <div class="title">
      Informazioni
    </div>

    <div class="page">

	  <div class='tit'>25 - 07 - 19 miglioramenti software e del sito</div>
	 <p>
        Software leggermente meno pesante, problema che portava il microprocessore a riempire la memoria  <br>
        causando continui crash. <br>
        Miglioramenti grafici e correzione di alcuni bug sul sito.
      </p>
     
      <div class='tit'>In realizzazione</div>
      <p>
        Qualsiasi funzione volete venga aggiunta riportatela in 'Contattaci'.
      </p>

      <div class='tit'>Funzionamento del modulo WiFi:</div>
      <p>
      Il modulo wifi, permette all'utente di gestire i timer di accensione e spegnimento e di controllare la temperatura attuale. <br>
      Nell'area personale, accessibile solo dopo aver eseguito l'accesso e aver inserito il codice prodotto, è stato aggiunto
      un grafico che riporterà la temperatura della giornata odierna.
      </p>
   </div>

  </body>
</html>
