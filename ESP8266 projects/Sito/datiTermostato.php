<?php session_start();?>
<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <!--<META HTTP-EQUIV="refresh" CONTENT="300;datiPianta.php"> riavvio pagina ogni 5 minuti-->
    <meta charset="utf-8">
    <title>Termostato</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>
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
            <li><a href="menuProdotti.php">Menu prodotti</a></li>
            <?php 
            if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
                    echo "<li style='float:right'><a class='active' href='logout.php'>Logout</a></li>";
                } else {
                    echo  "<li style='float:right'><a class='active' href='login.php'>Login</a></li>";
                }
            ?>
        </ul>
     </header>
    
     <?php
    if (isset($_GET['message'])) {
      $message = $_GET['message'];
      if ($message=='success' ){
        echo "<div id='mydiv'class='alertr'>
              Dati Aggiornati</div>";
	    }
      if ($message=='error' ){
       echo "<div id='mydiv'class='alert'>
             <strong>Attenzione!</strong> Parametri errati</div>";
      }
    }
          $email = $_SESSION['email'];
          if ($_SESSION['logged_in'] != true){
          echo "<div class='page'><h1>Area riservata, accesso negato.</h1></div>";
            exit;
          }

         require_once 'connection.php';
         $sql = "SELECT * FROM(SELECT data_agg,TIME_FORMAT(PS.ora_salvataggio, '%H:%i') AS ora_salvataggio
                  FROM term_state PS
                  join prodotti P on PS.cod_prodotto = P.cod_prodotto
                  WHERE P.email = '$email' ORDER BY data_agg,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";

         $stmt = $db->prepare($sql);
         $stmt->execute();

         $sql2 = "SELECT * FROM(SELECT data_agg,ora_salvataggio,temp
                  FROM term_state PS
                  join prodotti P on PS.cod_prodotto = P.cod_prodotto
                  WHERE P.email = '$email' ORDER BY data_agg,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";

         $stmt2 = $db->prepare($sql2);
         $stmt2->execute();
        ?>
        
        <div class="chart" style="width: 80%;">
            <canvas id="canvas"></canvas>
        </div>
        <script>
                   window.chartColors = {
                    red: 'rgb(255, 99, 132)',
                    orange: 'rgb(255, 159, 64)',
                    yellow: 'rgb(255, 205, 86)',
                    green: 'rgb(75, 192, 192)',
                    blue: 'rgb(54, 162, 235)',
                    purple: 'rgb(153, 102, 255)',
                    grey: 'rgb(231,233,237)'
                    };

                    var temp = [<?php while($result = $stmt2->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['temp'] .'",';} ?>];
                    var TIME = [<?php while($result = $stmt->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['ora_salvataggio'] .'",';} ?>];
                    var config = {
                    type: 'line',
                    data: {
                        labels: TIME,
                        datasets: [{
                        label: "Temp Aria",
                        backgroundColor: window.chartColors.red,
                        borderColor: window.chartColors.red,
                        data: temp,
                        fill: false,
                        }]
                    },
                    options: {
                        responsive: true,
                        title:{
                        display:true,
                        text:'Dati box'
                        },
                        tooltips: {
                            mode: 'index',
                            intersect: false,
                        },
                        hover: {
                            mode: 'nearest',
                            intersect: true
                        },
                        scales: {
                        xAxes: [{
                            display: true,
                            scaleLabel: {
                            display: true,
                            labelString: 'Ora'
                            }
                        }],
                        yAxes: [{
                            display: true,
                            scaleLabel: {
                            display: true,
                            },
                        }]
                        }
                    }
                    };

            var ctx = document.getElementById("canvas").getContext("2d");
            var myLine = new Chart(ctx, config);
        </script>

        <?php


        $sql1 = "SELECT TIME_FORMAT(ora_a1, '%H:%i') as  ora_a1,TIME_FORMAT(ora_s1, '%H:%i')as  ora_s1, TIME_FORMAT(ora_a2, '%H:%i')as  ora_a2, TIME_FORMAT(ora_s2, '%H:%i')as  ora_s2, temp
                  FROM gestione_term GT
                  join prodotti P on P.cod_prodotto = GT.cod_prodotto
                  WHERE P.email = '$email';";

        $stmt1 = $db->prepare($sql1);
        $stmt1->execute();
        $result1 = $stmt1->fetchAll(PDO::FETCH_ASSOC);
        $totale1 = $stmt1->rowCount();

        if(is_null($result1[0]['ora_a1'])) {
            $oraa1 = "Da impostare";
        } else {
            $oraa1 = $result1[0]['ora_a1'];
        }
        if(is_null($result1[0]['ora_s1'])) {
            $oras1 = "Da impostare";
        } else {
            $oras1 = $result1[0]['ora_s1'];
        }
        if(is_null($result1[0]['ora_a2'])) {
            $oraa2 = "Da impostare";
        } else {
            $oraa2 = $result1[0]['ora_a2'];
        }
        if(is_null($result1[0]['ora_s2'])) {
            $oras2 = "Da impostare";
        } else {
            $oras2 = $result1[0]['ora_s2'];
        }
        if(is_null($result1[0]['temp'])) {
            $temp = "Da impostare";
        } else {
            $temp = $result1[0]['temp'];
        }
          echo "

              <form name='F2'  action='saveondb_term.php' method='post'>
              <div class='acces'>Gestisci i tuoi dati</div>
                  <table>
                  <tr>
                    <th></th><th>Ora accensasione</th><th>Spegnimento</th><th>Temperatura attuale impostata</th>
                  </tr>
                   <tr>
                    <th>Presa 1</th><td><input type='text' value='$oraa1' name='oraa1' required></td>
                    <td><input type='text'value='$oras1'  name='oras1' required></td>
                    <td><input type='number' step='any' name='temp' value='$temp'required></td>
                   </tr>
                   <tr>
                    <th>Presa 2</th><td><input type='text'value='$oraa2'  name='oraa2' required></td>
                    <td><input type='text' value='$oras2' name='oras2' required> </td>
                    <td>Usare il punto per il decimale</td>
                   </tr>
                  </table>
                  <div class='centr'>
                   <input type='submit' id='inp' value='Salva'>
                  </div>
              </form>
          ";

    ?>
    <br><br>
  </body>
</html>
