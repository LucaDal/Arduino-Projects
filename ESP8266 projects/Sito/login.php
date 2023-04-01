<?php session_start();?>
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Login</title>
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
                    echo "<li style='float:right'> <a class='active' href='logout.php'>Logout</a></li>";
                    echo "<li > <a class='active' href='accesso.php'>Area personale</a></li>";
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
              <strong>Successo!</strong> Registrazione effettuata</div>";
	   }
     if ($message=='e' ){
       echo "<div id='mydiv'class='alert'>
             <strong>Attenzione!</strong> E-mail o password errati</div>";
    }

   }
    ?>

    <div class="title">
      Accedi
    </div>

    <form name="F1" id="login" action="loginc.php" onsubmit="return controlla();"method="post">

        <fieldset id="inputs">
           <input type="email" name="mail" placeholder="E-mail" autofocus required>
           <input type="password" name="psw" placeholder="Password" autofocus required>
        </fieldset>
        <a>Se non ricordi l'Email o password</a> <a href="rememberpsw.php">Clicca qui</a></p> <br>
        <input type="submit" value="Entra">
        <input type="button" name="" onclick="location.href='signup.php'" value="Registrati">
    </form>



  </body>
</html>
