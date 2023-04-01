<?php session_start();?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Contatti</title>
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
    <style media="screen">
    input[type=text], select, textarea {
      width: 100%;
      padding: 12px;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
      margin-top: 6px;
      margin-bottom: 16px;
      resize: vertical;
      }

    input[type=submit] {
      background-color: red;
      color: white;
      padding: 12px 20px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }

    input[type=submit]:hover {
      background-color: #45a049;
    }

    </style>


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

    <?php
    if (isset($_GET['message'])) {
      $message = $_GET['message'];
      if ($message=='s' ){
        echo "<div id='mydiv'class='alertr'>
               Modulo inviato con successo</div>";
	   }
     if ($message=='e' ){
       echo "<div id='mydiv'class='alert'>
             <strong>Errore!</strong> Riprovare più tardi - oppure contattami qui: <a href='mailto:luca-dalessandro@virgilio.it'>luca-dalessandro@virgilio.it</a></div>";
    }

   }
    ?>

    <div class="title">
      Contattaci
    </div>


  <div class="container">
    <form action="invia.php" method="GET">
      <label for="fname">Nome</label>
      <input type="text" id="fname" name="nome" placeholder="Il tuo nome..">

      <label for="lname">Cognome</label>
      <input type="text" id="lname" name="cognome" placeholder="Il tuo cognome..">

      <label for="country">E-mail</label>
      <input type="text" id="mail" name="mail" placeholder="Inserire email per essere ricontattati..">

      <label for="subject">Testo</label>
      <textarea id="testo" name="testo" placeholder="Scrivi qualcosa.." style="height:200px"></textarea>

      <input type="submit" value="Invia">
    </form>
   </div>




  </body>
</html>
