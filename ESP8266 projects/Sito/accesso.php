<?php session_start();?>

<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <meta charset="utf-8">
    <title>Registra prodotto</title>
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
          <?php if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
                    echo "<li style='float:right'><a class='active' href='logout.php'>Logout</a></li>";
                } else {
                    echo  "<li style='float:right'><a class='active' href='login.php'>Login</a></li>";
                }
         ?>
      </ul>
     </header>

    <?php
      if ($_SESSION['logged_in'] != true){
      echo "<div class='page'><h1>Area riservata, accesso negato.</h1></div>";
        exit;
      }

      if (isset($_GET['message'])) {
        $message = $_GET['message'];
        if ($message=='e' ){
          echo "<div id='mydiv' class='alert'>
          <strong>Attenzione!</strong>Codice errato o già in uso.</div>";
       }
      }

      require_once 'connection.php';
      
      $email = $_SESSION['email'];
      $sql = "SELECT cod_prodotto,tipologia
               FROM prodotti P
               join utenti U on P.email = U.email
               WHERE U.email = '$email';";
      $stmt = $db->prepare($sql);
      $stmt->execute();
      $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
      $totale = $stmt->rowCount();
      $db=null;
      if ($totale == 0 || (isset($_GET['message']) && $_GET['message'] == 'add')) {    //controllo se il prodotto non è stato registrsato
          echo "<div class='title'>Registra il tuo prodotto</div>";



          echo"    <form name='F1' id='login' action='controllo.php' method='post'>
               <fieldset id='inputs'>
               <input type='text' name='cod' placeholder='codice' autofocus required>
               </fieldset>
               <input type='submit' value='Registra'>
               </form>
               <div class='page'>Inserendo il codice potrai visualizzare e gestire in tempo reale i dati del tuo prodotto </div>

               ";
      }else if($totale == 1){//controllo che prodotto è e lo reindirizzo solo su quella pagina
            if($result[0]['tipologia'] == 'termostato'){
              header("location:datiTermostato.php");
            }else{
              header("location:datiPianta.php");
            }
      }else{
          header("location:menuProdotti.php");
      }


       ?>

  </body>
</html>
