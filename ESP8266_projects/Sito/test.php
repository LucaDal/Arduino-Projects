<?php 
   $data = json_decode(file_get_contents("php://test"), true);
   $_SESSION[$data["legendID"]] = $data["value"];
   echo "Hello, " . $data["legendID"] . "!";
   echo "value " . $data["value"];
?>