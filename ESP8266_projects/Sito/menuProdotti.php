<?php session_start();?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Home</title>
    <script type="text/javascript">
      function hideMyDiv(){
        document.getElementById("mydiv").style.display="none";
        }
        window.onload=function(){
        setTimeout(hideMyDiv,4000);
      }
    </script>
    <link href="https://fonts.googleapis.com/css?family=Ubuntu&display=swap" rel="stylesheet">
    <link rel="stylesheet" type="text/css" href="Style.css">
  </head>
  <body>
   <header>
      <ul>
          <li><a href="index.php">Home</a></li>
          <li><a href="update.php">Aggiornamenti</a></li>
          <li><a href="contact.php">Contattaci</a></li>
          <li><a href="accesso.php?message=add">Aggiungi Prodotto</a></li>
          <?php if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
                    echo "<li style='float:right'> <a class='active' href='logout.php'>Logout</a></li>";
                    echo "<li style='float:right'> <a class='active' href='accesso.php'>Area personale</a></li>";
                }else {
                    echo  "<li style='float:right'><a class='active' href='login.php'>Login</a></li>";
            } ?>
      </ul>
    </header>
            
    <div class="cen">
        <p><a href="datiTermostato.php">Gestione Termostato</a></p>
        <p><a href="datiPianta.php">Gestione Pianta</a></p>
    </div>

  </body>
</html>
