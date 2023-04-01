/*
  MySQL Connector/Arduino Example : complex select

  This example demonstrates how to issue a SELECT query with parameters that
  we provide from code. Thus, it demonstrates how to build query parameters
  dynamically.

  Notice also the sketch demonstrates how to read columns and rows from
  the result set. Study this example until you are familiar with how to
  do this before writing your own sketch to read and consume query results.

  NOTICE: You must download and install the World sample database to run
          this sketch unaltered. See http://dev.mysql.com/doc/index-other.html.

  INSTRUCTIONS FOR USE

  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Connect a USB cable to your Arduino
  4) Select the correct board and port
  5) Compile and upload the sketch to your Arduino
  6) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell
*/
#include <ESP8266WiFi.h> 
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


IPAddress server_addr(52,29,239,198);
char user[] = "sql7291876";         
char password[] = "r7uCzJiuX8";   
char ssid[]="FASTWEB - D'Alessandro";    
char pass[]="manzoni2018";           // MySQL user login password
String data[4];
// Sample query
//
// Notice the "%lu" - that's a placeholder for the parameter we will
// supply. See sprintf() documentation for more formatting specifier
// options
const PROGMEM char QUERY_POP[] = "SELECT ora_a1,ora_s1,ora_a2,ora_s2,temperaturaImp FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query[128];                //ora_a1,ora_s1,ora_a2,ora_s2,temperaturaImp
WiFiClient client;   
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;
MySQL_Cursor cur = MySQL_Cursor(&conn);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
          while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
          }
    Serial.print("Connecting to SQL...  ");
         if (conn.connect(server_addr, 3306, user, password)){
    delay(1000);
    Serial.println("ok.");
  }
  else
    Serial.println("Connection failed.");
}


void loop() {
  

  Serial.println("> Running SELECT with dynamically supplied parameter");

  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  sprintf(query, QUERY_POP, 9000000);
  // Execute the query
  cur_mem->execute(query);
  // Fetch the columns and print them
  column_names *cols = cur_mem->get_columns();
  for (int f = 0; f < cols->num_fields; f++) {
    Serial.print(cols->fields[f]->name);
    if (f < cols->num_fields-1) {
      Serial.print(',');
    }
  }
  Serial.println();
  // Read the rows and print them
  row_values *row = NULL;
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      for (int f = 0; f < cols->num_fields; f++) {
        Serial.print(row->values[f]);     
        //Serial.print(data[f]);
        if (f < cols->num_fields-1) {
          Serial.print(',');
        }
      }
      Serial.println();
    }
  } while (row != NULL);
  // Deleting the cursor also frees up memory used
  delete cur_mem;
// 
//  int ora=data[1].toInt();
//  Serial.print(ora);
  delay(10000);
}
