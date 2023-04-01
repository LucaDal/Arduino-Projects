<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Registrati</title>
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
  <body background-image="file/sand.jpg">

    <header>
      <ul>
          <li><a href="index.php">Home</a></li>
          <li><a href="update.php">Aggiornamenti</a></li>
          <li><a href="contact.php">Contattaci</a></li>
          <li style="float:right"><a class="active" href="login.php">Login</a></li>
      </ul>
    </header>

    <?php
    if (isset($_GET['message'])) {
      $message = $_GET['message'];
      if ($message=='e' ){
        echo "<div class='alert'>
              <strong>Attenzione!</strong> E-mail o password errati</div>";
       }
       if ($message=='r' ){
         echo "<div class='alertr'>
         <strong>Successo!</strong> Registrazione effettuata</div>";
        }
        if ($message=='u' ){
          echo "<div class='alert'>
          <strong>Attenzione!</strong> e-mail già in uso.</div>";
         }
     }
    ?>
    <div class="title">
      Registrati
    </div>

    <form name="F1" id="login" action="signupr.php" onsubmit="return controlla();"method="post">

        <fieldset id="inputs">
          <input type="text" name="nome" placeholder="Nome" autofocus required>
          <input type="text" name="cognome" placeholder="Cognome" autofocus required>
          <input type="email" name="mail" placeholder="E-mail" autofocus required>
          <input type="password" name="psw" placeholder="Password" autofocus required>
        </fieldset>
          <input type="submit" value="Registrati">
          <input type="button" onclick="location.href='login.php'" value="Entra">
    </form>
  </body>
</html>
