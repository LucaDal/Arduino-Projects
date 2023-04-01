<?php session_start();?>
<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <meta charset="utf-8">
    <title>WaterPlan</title>
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
          <?php if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == true){
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
    ?>
    <?php
          $email = $_SESSION['email'];
          if ($_SESSION['logged_in'] != true){
          echo "<div class='page'><h1>Area riservata, accesso negato.</h1></div>";
            exit;
          }

         require_once 'connection.php';
         $sql = "SELECT * FROM(SELECT data_agg,TIME_FORMAT(PS.ora_salvataggio, '%H:%i') AS ora_salvataggio
                  FROM plant_state PS
                  join prodotti P on PS.cod_prodotto = P.cod_prodotto
                  WHERE P.email = '$email' ORDER BY data_agg DESC,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";

         $stmt = $db->prepare($sql);
         $stmt->execute();

         $sql2 = "SELECT * FROM(SELECT data_agg,ora_salvataggio,temp_aria
                  FROM plant_state PS
                  join prodotti P on PS.cod_prodotto = P.cod_prodotto
                  WHERE P.email = '$email' ORDER BY data_agg DESC,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC ;";

         $stmt2 = $db->prepare($sql2);
         $stmt2->execute();

         $sql3 = "SELECT * FROM(SELECT data_agg,ora_salvataggio,temp_terreno
              FROM plant_state PS
              join prodotti P on PS.cod_prodotto = P.cod_prodotto
              WHERE P.email = '$email' ORDER BY data_agg DESC,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";
        $stmt3 = $db->prepare($sql3);
        $stmt3->execute();

        $sql4 = "SELECT * FROM(SELECT data_agg,ora_salvataggio,umid_aria
        FROM plant_state PS
        join prodotti P on PS.cod_prodotto = P.cod_prodotto
        WHERE P.email = '$email' ORDER BY data_agg DESC,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";
        $stmt4 = $db->prepare($sql4);
        $stmt4->execute();

        $sql5 = "SELECT * FROM(SELECT data_agg,ora_salvataggio,umid_terreno
        FROM plant_state PS
        join prodotti P on PS.cod_prodotto = P.cod_prodotto
        WHERE P.email = '$email' ORDER BY data_agg DESC,ora_salvataggio DESC LIMIT 48) as r order by  r.data_agg,r.ora_salvataggio ASC;";
        $stmt5 = $db->prepare($sql5);
        $stmt5->execute();
        ?>
        
        <div class="chart"  style="width: 75%;">
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

                    var temp_aria = [<?php while($result = $stmt2->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['temp_aria'] .'",';} ?>];
                    var temp_terreno = [<?php while($result = $stmt3->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['temp_terreno'] .'",';} ?>];
                    var umid_aria = [<?php while($result = $stmt4->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['umid_aria'] .'",';} ?>];
                    var umid_terreno = [<?php while($result = $stmt5->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['umid_terreno'] .'",';} ?>];

                    var TIME = [<?php while($result = $stmt->fetch(PDO::FETCH_ASSOC)){ echo '"'.  $result['ora_salvataggio'] .'",';} ?>];
                    var config = {
                    type: 'line',
                    data: {
                        labels: TIME,
                        datasets: [{
                        label: "Temp Aria",
                        backgroundColor: window.chartColors.red,
                        borderColor: window.chartColors.red,
                        data: temp_aria,
                        fill: false,
                        }, {
                        label: "Temp Terreno",
                        fill: false,
                        backgroundColor: window.chartColors.orange,
                        borderColor: window.chartColors.orange,
                        data: temp_terreno,
                        },
                        {
                        label: "Umidità Aria",
                        fill: false,
                        backgroundColor: window.chartColors.green,
                        borderColor: window.chartColors.green,
                        data: umid_aria,
                        },
                        {
                        label: "Umidità Terreno",
                        fill: false,
                        backgroundColor: window.chartColors.blue,
                        borderColor: window.chartColors.blue,
                        data: umid_terreno,
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


        $sql1 = "SELECT umid_to_water, ml_to_give, sec_to_water, TIME_FORMAT(ora_accensione_luci, '%H:%i') as  ora_accensione_luci
                  FROM gestione_plant GP
                  join prodotti P on GP.cod_prodotto = P.cod_prodotto
                  WHERE P.email = '$email';";
        $stmt1 = $db->prepare($sql1);
        $stmt1->execute();
        $result1 = $stmt1->fetchAll(PDO::FETCH_ASSOC);
        $totale1 = $stmt1->rowCount();

        if(is_null($result1[0]['umid_to_water'])) {
          $umid_to_water = 90;
        } else {
          $umid_to_water = $result1[0]['umid_to_water'];
        }
        if(is_null($result1[0]['sec_to_water'])) {
          $sec_to_water = 0;
        }else {
          $sec_to_water = $result1[0]['sec_to_water'];
        }
        if(is_null($result1[0]['ml_to_give'])) {
          $ml_to_give = 0;
        } else {
          $ml_to_give = $result1[0]['ml_to_give'];
        }
        if(is_null($result1[0]['ora_accensione_luci'])) {
          $ora_accensione_luci = "Da impostare";
        } else {
          $ora_accensione_luci = $result1[0]['ora_accensione_luci'];
        }
        
          echo "
          <form name='F2'  action='saveondb_pianta.php' method='post'>
          <div class='acces'>Gestione dati</div>
              <table>
              <tr>
                <th></th><th>Umidità minima</th><th>Millilitri da versare</th><th>Millisecondi di ritardo</th><th>Accensione luci</th>
              </tr>
               <tr>
                <th>Dati</th>
                <td><input type='number' step='5' min='0' max='100' name='umid_to_water' value='$umid_to_water' required></td>
                <td><input type='number' step='any' min='0' max='99999' name='ml_to_give' value='$ml_to_give' required></td>
                <td><input type='number' step='any' min='0' max='99999' name='sec_to_water' value='$sec_to_water' required></td>
                <td><input type='text'value='$ora_accensione_luci'  name='ora_accensione_luci' required></td>
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