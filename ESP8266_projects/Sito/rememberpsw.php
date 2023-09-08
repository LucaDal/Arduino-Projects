<?php session_start();?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Recupero</title>

    <link href="https://fonts.googleapis.com/css?family=Ubuntu&display=swap" rel="stylesheet">
    <link rel="stylesheet" type="text/css" href="Style.css">


  </head>
  <body>


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
      Spiacenti
    </div>
    <div class="paragrafo">
      <p>Momentanemaente non è possibile recuperare l'Email o la password, prova più tardi</p>
    </div>




  </body>
</html>
