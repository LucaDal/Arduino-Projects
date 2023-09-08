/*
  MySQL Connector/Arduino Example : basic select

  This example demonstrates how to issue a SELECT query with no parameters
  and use the data returned. For this, we use the Cursor class to execute
  the query and get the results.

  It demonstrates who methods for running queries. The first allows you to
  allocate memory for the cursor and later reclaim it, the second shows how
  to use a single instance of the cursor use throughout a sketch.

  NOTICE: You must download and install the World sample database to run
          this sketch unaltered. See http://dev.mysql.com/doc/index-other.html.

  CAUTION: Don't mix and match the examples. Use one or the other in your
           own sketch -- you'll get compilation errors at the least.

  INSTRUCTIONS FOR USE

  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Connect a USB cable to your Arduino
  4) Select the correct board and port
  5) Compile and upload the sketch to your Arduino
  6) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  Note: The MAC address can be anything so long as it is unique on your network.

  C
  reated by: Dr. Charles A. Bell
*/


#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include <ESP8266WiFi.h> 


IPAddress server_addr(52,29,239,198);
char user[] = "sql7291876";         
char password[] = "r7uCzJiuX8";   
char ssid[]="FASTWEB - D'Alessandro";    
char pass[]="manzoni2018";       // MySQL user login password

// Sample query
char query[] = "SELECT 'TIME_FORMAT(ora_a1, '%H')' FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query1[] = "SELECT TIME_FORMAT(ora_a1, '%i') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query2[] = "SELECT TIME_FORMAT(ora_s1, '%H') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query3[] = "SELECT TIME_FORMAT(ora_s1, '%i') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query4[] = "SELECT TIME_FORMAT(ora_a2, '%H') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query5[] = "SELECT TIME_FORMAT(ora_a2, '%i') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query6[] = "SELECT TIME_FORMAT(ora_s2, '%H') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query7[] = "SELECT TIME_FORMAT(ora_s2, '%i') FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";
char query8[] = "SELECT temperaturaImp FROM sql7291876.gestione_prodotti WHERE cod_prodotto = '8pklP'";

WiFiClient client;   
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;
MySQL_Cursor cur = MySQL_Cursor(&conn);

void setup() {
    Serial.begin(115200);
}

void loop() {
          Serial.print("ok");
          WiFi.begin(ssid, pass);
          while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
          }
          Serial.print("Connecting to SQL...  ");
         if (conn.connect(server_addr, 3306, user, password)){
            Serial.println("OK.");
              uno();
              due();
              tre();
              qua();
              cin();
              sei();
              set();
              ott();
              nov();
             Serial.println("connessione chiusa");
          } else{
          Serial.println("FAILED.");    
         }

}
void uno(){
  row_values *row = NULL;
  int head_count = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" ora = ");
  Serial.println(head_count);
  cur.close();
  delay(500);


  
}
void due(){
  row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query1);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" minuti = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}

void tre(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query2);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" ore = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void qua(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query3);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" min = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void cin(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query4);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" min = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void sei(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query5);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" min = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void set(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query6);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" min = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void ott(){
  row_values *row = NULL;
  int head_count2 = 0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query7);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" min = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
void nov(){
  row_values *row = NULL;
  double head_count2 = 0.0;
  delay(1000);
  Serial.println("");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn); 
  
  cur_mem->execute(query8);
  column_names *columns = cur_mem->get_columns();
  do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      head_count2 = atol(row->values[0]);
    }
  } while (row != NULL);
  delete cur_mem;

  // Show the result
  Serial.print(" temp = ");
  Serial.println(head_count2);
  cur.close();
  delay(500);
}
